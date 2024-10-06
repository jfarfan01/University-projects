#pragma once

#include "Thread.h"
#include "UserProcess.h"

class UserThread : public Thread
{
  public:
    /**
     * Constructor
     * @param minixfs_filename filename of the file in minixfs to execute
     * @param fs_info filesysteminfo-object to be used
     * @param terminal_number the terminal to run in (default 0)
     *
     */
    UserThread(ustl::string minixfs_filename, FileSystemInfo *fs_info, Loader* loader = 0, UserProcess* userProcess = 0);

    ~UserThread();

    virtual void Run();

    void requestCancel();

    bool getCancelRequest() {
      return cancel_request;}

    int getCancelEnabled();

    void setCancelEnabled(int val);

    Mutex join_mutex_;

    Condition join_condition_;

    void setJoiningThread(UserThread* thread);

    UserThread* getJoiningThread();

    void setToBeJoined(bool val);

    bool getToBeJoined();

    bool getTerminationCondSignal();

    void setTerminationCondSignal(bool val);

    void* getExitValue();

    void setExitValue(void* value);

  private:
    int32 fd_;

    int cancel_request;

    UserThread* joining_thread_;

    bool to_be_joined_;

    bool termination_cond_signal_;

    void* exit_value_;
};

