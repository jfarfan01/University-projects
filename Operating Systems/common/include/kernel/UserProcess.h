#pragma once

#include "Thread.h"
#include "uvector.h"
#include "offsets.h"

class UserProcess
{
  public:
    /**
     * Constructor
     * @param minixfs_filename filename of the file in minixfs to execute
     * @param fs_info filesysteminfo-object to be used
     * @param terminal_number the terminal to run in (default 0)
     *
     */
    UserProcess(ustl::string minixfs_filename, FileSystemInfo *fs_info, uint32 terminal_number = 0);

    virtual ~UserProcess();

    virtual void Run(); // not used
    
    // void kill();

    Mutex threads_lock_;

    Loader* loader_;

    uint64 current_stack_top_;

    size_t spawnThread(size_t wrapper, size_t routine, size_t args);

    ustl::vector<Thread*> processThreads;

    bool capableOfStarting() {
      return capableOfStarting_;
    }

  private:
    size_t pid_;

    int32 fd_;

    Terminal* my_terminal_;

    bool capableOfStarting_;

    ustl::string filename_;
  protected: 
    FileSystemInfo* working_dir_;
};

