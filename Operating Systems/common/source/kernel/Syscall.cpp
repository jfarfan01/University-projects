#include "offsets.h"
#include "Syscall.h"
#include "syscall-definitions.h"
#include "Terminal.h"
#include "debug_bochs.h"
#include "VfsSyscall.h"
#include "ProcessRegistry.h"
#include "File.h"
#include "Scheduler.h"
#include "UserProcess.h"
#include "UserThread.h"
#include "ArchThreads.h"

size_t Syscall::syscallException(size_t syscall_number, size_t arg1, size_t arg2, size_t arg3, size_t arg4, size_t arg5)
{
  size_t return_value = 0;
  if ((syscall_number != sc_sched_yield) && (syscall_number != sc_outline)) // no debug print because these might occur very often
  {
    debug(SYSCALL, "Syscall %zd called with arguments %zd(=%zx) %zd(=%zx) %zd(=%zx) %zd(=%zx) %zd(=%zx)\n",
          syscall_number, arg1, arg1, arg2, arg2, arg3, arg3, arg4, arg4, arg5, arg5);
  }

  if(static_cast<UserThread*>(currentThread)->getCancelRequest())
  {
    debug(THREAD,"Thread going to be cancelled\n");
    pthread_exit((void*)PTHREAD_CANCELLED);
  }

  switch (syscall_number)
  {
    case sc_sched_yield:
      Scheduler::instance()->yield();
      break;
    case sc_createprocess:
      return_value = createprocess(arg1, arg2);
      break;
    case sc_exit:
      exit(arg1);
      break;
    case sc_write:
      return_value = write(arg1, arg2, arg3);
      break;
    case sc_read:
      return_value = read(arg1, arg2, arg3);
      break;
    case sc_open:
      return_value = open(arg1, arg2);
      break;
    case sc_close:
      return_value = close(arg1);
      break;
    case sc_outline:
      outline(arg1, arg2);
      break;
    case sc_trace:
      trace();
      break;
    case sc_pseudols:
      pseudols((const char*) arg1, (char*) arg2, arg3);
      break;
    case sc_pthread_create:
      return_value = spawnThread(arg1, arg2, arg3);
      break;
    case sc_pthread_cancel:
      return_value = cancel_thread(arg1);
      break;
    case sc_pthread_exit:
      pthread_exit((void*)arg1);
      break;
    case sc_pthread_join:
      return_value = joinThread(arg1, (void**)arg2);
      break;
    case sc_pthread_setcancelstate:
      return_value = setCancelState(arg1, (int*)arg2);
      break;
    case sc_pthread_setcanceltype:
      return_value = setCancelType(arg1, (int*)arg2);
      break;
    default:
      return_value = -1;
      kprintf("Syscall::syscallException: Unimplemented Syscall Number %zd\n", syscall_number);
  }
  return return_value;
}

size_t Syscall::spawnThread(size_t wrapper, size_t routine, size_t args)
{
  UserProcess* process = currentThread->getParentProcess();
  if(process)
  {
    size_t result = process->spawnThread(wrapper, routine, args);
    return result;
  } else
  {
    debug(USERPROCESS, "No valid Userprocess instance");
    assert(false);
  }
  return 0;
}

size_t Syscall::joinThread(size_t tid, void** value_ptr)
{
  debug(SYSCALL, "Syscall::joinThread called with TID %zu\n", tid);
  UserProcess* process = currentThread->getParentProcess();
  UserThread* the_current_thread = static_cast<UserThread*>(currentThread);
  UserThread* target = nullptr;
  
  process->threads_lock_.acquire();
  for(auto thread : process->processThreads)
  {
    if(thread->getTID() == tid)
    {
      target = static_cast<UserThread*>(thread);
      target->setToBeJoined(true);
      target->setJoiningThread(the_current_thread);
      break;
    }
  }
  process->threads_lock_.release();

  the_current_thread->join_mutex_.acquire();
  while(!the_current_thread->getTerminationCondSignal())
  {
    the_current_thread->join_condition_.wait();
  }
  the_current_thread->join_mutex_.release();
  
  if(value_ptr)
  {
    *value_ptr = the_current_thread->getExitValue();
  }

  the_current_thread->setTerminationCondSignal(false);
  return 0;
}

void Syscall::pthread_exit(void* value_ptr)
{
  debug(SYSCALL, "Syscall::pthread_exit called with value %p\n", value_ptr);
  //TODO clean up stuff
  UserThread* the_current_thread = static_cast<UserThread*>(currentThread);
  if(the_current_thread->getToBeJoined())
  {
    UserThread* joining_thread = the_current_thread->getJoiningThread();
    joining_thread->setExitValue(value_ptr);
    joining_thread->join_mutex_.acquire();
    joining_thread->setTerminationCondSignal(true);
    joining_thread->join_condition_.signal();
    joining_thread->join_mutex_.release();
  }
  currentThread->kill();
}

size_t Syscall::cancel_thread(size_t target_tid)
{
  debug(THREAD,"cancel thread called\n");
  UserProcess * process = currentThread-> getParentProcess();

  process->threads_lock_.acquire();
  for (Thread* thread: process->processThreads)
  {

    if (static_cast<UserThread*>(thread)->getTID() == target_tid)
    {
      static_cast<UserThread*>(thread)->requestCancel();
      process->threads_lock_.release();
      return 0;
    }
  }
  process->threads_lock_.release();

  return -1;
}

size_t Syscall::setCancelState(size_t state, int* oldstate)
{
  if(state != PTHREAD_CANCEL_ENABLE && state != PTHREAD_CANCEL_DISABLE)
  {
    return -1; //invalid state argument
  }
  if(oldstate)
  {
    ArchThreads::atomic_set(*oldstate, currentThread->getCancelState());
  }
  currentThread->setCancelState(state);
  
  return 0;
}

size_t Syscall::setCancelType(size_t type, int* oldtype)
{
  if(type != PTHREAD_CANCEL_DEFERRED && type != PTHREAD_CANCEL_ASYNCHRONOUS)
  {
    return -1; //invalid type argument
  }
  if(oldtype)
  {
    ArchThreads::atomic_set(*oldtype, currentThread->getCancelType());
  }
  currentThread->setCancelType(type);

  return 0;
}

void Syscall::pseudols(const char *pathname, char *buffer, size_t size)
{
  if(buffer && ((size_t)buffer >= USER_BREAK || (size_t)buffer + size > USER_BREAK))
    return;
  if((size_t)pathname >= USER_BREAK)
    return;
  VfsSyscall::readdir(pathname, buffer, size);
}

void Syscall::exit(size_t exit_code)
{
  debug(SYSCALL, "Syscall::EXIT: called, exit_code: %zd\n", exit_code);
  UserProcess* process = currentThread->getParentProcess();
  process->threads_lock_.acquire();
  for (Thread* thread: process->processThreads)
  {
    thread->setExitCalledFlag(true);
  }
  process->threads_lock_.release();
  currentThread->kill();
  assert(false && "This should never happen");
}

size_t Syscall::write(size_t fd, pointer buffer, size_t size)
{
  //WARNING: this might fail if Kernel PageFaults are not handled
  if ((buffer >= USER_BREAK) || (buffer + size > USER_BREAK))
  {
    return -1U;
  }

  size_t num_written = 0;

  if (fd == fd_stdout) //stdout
  {
    debug(SYSCALL, "Syscall::write: %.*s\n", (int)size, (char*) buffer);
    kprintf("%.*s", (int)size, (char*) buffer);
    num_written = size;
  }
  else
  {
    num_written = VfsSyscall::write(fd, (char*) buffer, size);
  }
  return num_written;
}

size_t Syscall::read(size_t fd, pointer buffer, size_t count)
{
  if ((buffer >= USER_BREAK) || (buffer + count > USER_BREAK))
  {
    return -1U;
  }

  size_t num_read = 0;

  if (fd == fd_stdin)
  {
    //this doesn't! terminate a string with \0, gotta do that yourself
    num_read = currentThread->getTerminal()->readLine((char*) buffer, count);
    debug(SYSCALL, "Syscall::read: %.*s\n", (int)num_read, (char*) buffer);
  }
  else
  {
    num_read = VfsSyscall::read(fd, (char*) buffer, count);
  }
  return num_read;
}

size_t Syscall::close(size_t fd)
{
  return VfsSyscall::close(fd);
}

size_t Syscall::open(size_t path, size_t flags)
{
  if (path >= USER_BREAK)
  {
    return -1U;
  }
  return VfsSyscall::open((char*) path, flags);
}

void Syscall::outline(size_t port, pointer text)
{
  //WARNING: this might fail if Kernel PageFaults are not handled
  if (text >= USER_BREAK)
  {
    return;
  }
  if (port == 0xe9) // debug port
  {
    writeLine2Bochs((const char*) text);
  }
}

size_t Syscall::createprocess(size_t path, size_t sleep)
{
  // THIS METHOD IS FOR TESTING PURPOSES ONLY AND NOT MULTITHREADING SAFE!
  // AVOID USING IT AS SOON AS YOU HAVE AN ALTERNATIVE!

  // parameter check begin
  if (path >= USER_BREAK)
  {
    return -1U;
  }

  debug(SYSCALL, "Syscall::createprocess: path:%s sleep:%zd\n", (char*) path, sleep);
  ssize_t fd = VfsSyscall::open((const char*) path, O_RDONLY);
  if (fd == -1)
  {
    return -1U;
  }
  VfsSyscall::close(fd);
  // parameter check end

  size_t process_count = ProcessRegistry::instance()->processCount();
  ProcessRegistry::instance()->createProcess((const char*) path);
  if (sleep)
  {
    while (ProcessRegistry::instance()->processCount() > process_count) // please note that this will fail ;)
    {
      Scheduler::instance()->yield();
    }
  }
  return 0;
}

void Syscall::trace()
{
  currentThread->printBacktrace();
}
