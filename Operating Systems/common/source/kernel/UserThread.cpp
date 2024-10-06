#include "ProcessRegistry.h"
#include "UserThread.h"
#include "kprintf.h"
#include "Console.h"
#include "Loader.h"
#include "VfsSyscall.h"
#include "File.h"
#include "PageManager.h"
#include "ArchThreads.h"
#include "offsets.h"
#include "Scheduler.h"

UserThread::UserThread(ustl::string filename, FileSystemInfo *fs_info, Loader* loader, UserProcess* parentProcess) :
    Thread(fs_info, filename, Thread::USER_THREAD), join_mutex_("JoinLock"), join_condition_(&join_mutex_, "JoinCondition")
    ,fd_(VfsSyscall::open(filename, O_RDONLY)), cancel_request(false), exit_value_(0)
{
  // cancel_request = false;
  loader_ = loader;
  parentProcess_ = parentProcess;
  //debug(SEPARATION, "Starting creation of UserThread!\n");
  
  ArchThreads::createUserRegisters(user_registers_, loader_->getEntryFunction(),
                                   (void*) (USER_BREAK - sizeof(pointer)),
                                    getKernelStackStartPointer());
                                   
  ArchThreads::setAddressSpace(this, loader_->arch_memory_);

  //debug(USERTHREAD, "Finished starting up UserThread!\n");
}

//TODO change this, at the moment this is just copied from thread
UserThread::~UserThread()
{

  //debug(THREAD, "parentprocess is %p\n", parentProcess_);
  //debug(THREAD, "parentprocess processthreads size is %ld\n", parentProcess_->processThreads.size());

  int i = 0;
  for(auto thread : parentProcess_->processThreads)
  {
    if (thread == this)
    {
      //debug(THREAD, "erasing %d from parentProcess vector\n", i);
      parentProcess_->processThreads.erase(parentProcess_->processThreads.begin() + i, parentProcess_->processThreads.begin() + i + 1);
      //debug(THREAD, " parentProcess vector size is  %ld \n", parentProcess_->processThreads.size());
      break;
    }
    i++;
  }

  if(parentProcess_->processThreads.size() == 0 )
  {
    debug(THREAD, "parentProcess will be deleted\n");
    delete parentProcess_;
  }
  debug(THREAD, "~Thread: done (%s)\n", name_.c_str());
}

// TODO implement, at the moment this is just to stop the error
void UserThread::Run()
{

}

void UserThread::setJoiningThread(UserThread* thread)
{
  joining_thread_ = thread;
}

UserThread* UserThread::getJoiningThread()
{
  return joining_thread_;
}

void UserThread::setToBeJoined(bool val)
{
  to_be_joined_ = val;
}

bool UserThread::getToBeJoined()
{
  return to_be_joined_;
}

bool UserThread::getTerminationCondSignal()
{
  return termination_cond_signal_;
}

void UserThread::setTerminationCondSignal(bool val)
{
  termination_cond_signal_ = val;
}

void UserThread::setExitValue(void* value)
{
  exit_value_ = value;
}

void* UserThread::getExitValue()
{
  return exit_value_;
}

void UserThread::requestCancel()
{
  // debug(THREAD, "cancel request state: %d \n",cancel_request);
  cancel_request = true;
  // debug(THREAD, "cancel request state: %d \n",cancel_request);
}