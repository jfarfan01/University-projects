#include "ProcessRegistry.h"
#include "UserProcess.h"
#include "kprintf.h"
#include "Console.h"
#include "Loader.h"
#include "VfsSyscall.h"
#include "File.h"
#include "PageManager.h"
#include "ArchThreads.h"
#include "offsets.h"
#include "Scheduler.h"

#include "UserThread.h"


UserProcess::UserProcess(ustl::string filename, FileSystemInfo *fs_info, uint32 terminal_number) :  
  threads_lock_("ThreadsLock") ,fd_(VfsSyscall::open(filename, O_RDONLY)),filename_(filename), working_dir_(fs_info)
{
  //debug(SEPARATION, "Thread ctor, this is %p, fs_info ptr: %p, filename: %s\n", this, working_dir_, (char*) filename);
  ProcessRegistry::instance()->processStart(); //should also be called if you fork a process
  current_stack_top_ = USER_BREAK - PAGE_SIZE;

  if (fd_ >= 0)
    loader_ = new Loader(fd_);

  if (!loader_ || !loader_->loadExecutableAndInitProcess())
  {
    debug(USERPROCESS, "Error: loading %s failed!\n", filename.c_str());
    // kill();
    capableOfStarting_ = false;
    return;
  }

  capableOfStarting_ = true;

  size_t page_for_stack = PageManager::instance()->allocPPN();
  bool vpn_mapped = loader_->arch_memory_.mapPage(USER_BREAK / PAGE_SIZE - 1, page_for_stack, 1);
  assert(vpn_mapped && "Virtual page for stack was already mapped - this should never happen");
  current_stack_top_ -= PAGE_SIZE;

  debug(USERPROCESS, "ctor: Done loading %s\n", filename.c_str());

  // TODO create first UserThread
  this->processThreads.push_back(new UserThread(filename, fs_info, loader_, this));

  if (main_console->getTerminal(terminal_number)) {
    processThreads.front()->setTerminal(main_console->getTerminal(terminal_number));
  }
}

UserProcess::~UserProcess()
{
  assert(Scheduler::instance()->isCurrentlyCleaningUp());
  delete loader_;
  loader_ = 0;

  if (fd_ > 0)
    VfsSyscall::close(fd_);

  delete working_dir_;
  working_dir_ = 0;

  ProcessRegistry::instance()->processExit();
}

size_t UserProcess::spawnThread(size_t wrapper, size_t routine, size_t args)
{
  UserThread* thread = new UserThread(filename_, working_dir_, loader_, this);
  thread->user_registers_->rip = wrapper;
  thread->user_registers_->rdi = routine;
  thread->user_registers_->rsi = args;

  size_t stack_size = PAGE_SIZE * 8;
  size_t num_of_pages = stack_size / PAGE_SIZE;

  size_t new_stack_top = current_stack_top_;
  current_stack_top_ -= (stack_size + PAGE_SIZE); // Add one PAGE_SIZE for a guard page
  size_t new_stack_bottom = current_stack_top_ + PAGE_SIZE; // Adjust the bottom to account for the guard page

  thread->user_registers_->rsp = new_stack_top;

  ustl::vector<size_t> physical_pages;
  for (size_t i = 0; i < num_of_pages; i++)
  {
    size_t ppn = PageManager::instance()->allocPPN(PAGE_SIZE);
    physical_pages.push_back(ppn);
  }

  for (size_t i = 0; i < num_of_pages; i++)
  {
    size_t virtual_page = (new_stack_bottom + (i * PAGE_SIZE)) / PAGE_SIZE;
    size_t physical_page = physical_pages[i];
    bool success = loader_->arch_memory_.mapPage(virtual_page, physical_page, 1);
    //debug(THREAD, "Mapping virtual page %lx to phys page %lx\n", virtual_page, physical_page);
    assert(success && "Failed to map user stack page");
  }

  this->processThreads.push_back(thread);
  Scheduler::instance()->addNewThread(thread);
  debug(THREAD, "Thread ID: %zu | Stack Start: %lx | Stack End: %lx\n", thread->getTID(), new_stack_top, new_stack_bottom);
  return thread->getTID();
}

void UserProcess::Run()
{
  debug(USERPROCESS, "Run: Fail-safe kernel panic - you probably have forgotten to set switch_to_userspace_ = 1\n");
  assert(false);
}

