#include "User.h"

/*-----------------------------------------------------------------
 *  Main routine that loops and makes requests and eventually
 *  makes it public to the server. Stops when the operation equals
 *  zero. Initialize everything needed in here. All checks in this
 *  function must not be relocated.
 *
 *  @param argc number of arguments given
 *  @param argv char* that points to the argument
 *
 *  @return predefined value
 **/
int startUser()
{
    char buffer[REQUEST_MAX_LENGTH];
    initSharedMemoriesUser();
    checkSHMUser();
    initMmapingsUser();
    checkMMAPUser();
    initLocks();
    initProcess();
    checkMagicConchShell();

    // TODO Student START
    // init additional variables here if needed
    mmaps.mapped_region_state_->motivated = 1;

    // TODO Student END

    do
    {
      // TODO Student START
      // process commands and values, synchronization,
      // assigning/receiving to/from shared memory
      // use the provided function getCommand()
      // break the loop if no command (EOF) is received or the command is 'bye'
      // (exit still has to be handled by the magic conch shell)

      // mmaps.mapped_region_request_->message = "suuup\n";
      // strcpy(mmaps.mapped_region_request_->message, "suuup\n");
      // printf("%s",mmaps.mapped_region_request_->message);


      if (!getCommand(buffer))
      {
        break;
      }
      sem_wait(&mmaps.mapped_region_locks_->user_can_strcpy);

      pthread_mutex_lock(&mmaps.mapped_region_locks_->reponse_mutex);
      strcpy(mmaps.mapped_region_request_->message, buffer);
      pthread_mutex_unlock(&mmaps.mapped_region_locks_->reponse_mutex);

      if(!strcmp("motivation\n",buffer))
      {
        mmaps.mapped_region_state_->motivated = 1;
      }

      if(!strcmp("unmotivation\n",buffer))
      {
        mmaps.mapped_region_state_->motivated = 0;
      }

      sem_post(&mmaps.mapped_region_locks_->user_wrote_request);
      sem_wait(&mmaps.mapped_region_locks_->conch_sent_reponse);

      // TODO Student END

      checkResults();

    } while (strcmp(buffer, "bye\n") != 0);

    closeMmapingsUser();
    checkCleanup();

  return 0;
}

/*-----------------------------------------------------------------
 *  This function starts and initializes the magic conch shell, the process does not
 *  run at this point. Be sure everything is defined and initialized.
 *  Use the predefined variable process_id for the pid of the newly created
 *  process.
 *
 *  @param none
` *
 *  @return none
 **/
void initProcess()
{
  if(checkSetup()) 
  {
    return;
  }

  // TODO Student START
  // start the MagicConchShell process and load the right executable
  process_id = fork();

  if (process_id == 0)
  {
    char * args [1] = { " MagicConshShell" };
    char * args2 [1] = {NULL}; 
    execve("./MagicConchShell",args, args2);
  }

  //TODO Student END
}

/*-----------------------------------------------------------------
 * initializes your shared objects on the User and resizes them
 * make sure you only give the permissions the User needs, use 
 * the provided defines (in util.h)
 * but please use MODERW, for compatibility with the testsystem
 *
 * @param none
 *
 * @return none
 */
void initSharedMemoriesUser()
{
    // TODO Student START
    fds.fd_shm_request_ = shm_open(SHM_NAME_REQUEST, FLAGS_SHM_READWRITE, MODERW);
    ftruncate(fds.fd_shm_request_,sizeof(shmrequest));

    fds.fd_shm_response_ = shm_open(SHM_NAME_RESPONSE, FLAGS_SHM_READONLY,MODERW);
    ftruncate(fds.fd_shm_response_, sizeof(shmresponse));

    fds.fd_shm_state_ = shm_open(SHM_NAME_STATE, FLAGS_SHM_READWRITE, MODERW);
    ftruncate(fds.fd_shm_state_, sizeof(shmstate));

    fds.fd_shm_locks_ = shm_open(SHM_NAME_LOCKS, FLAGS_SHM_READWRITE, MODERW);
    ftruncate(fds.fd_shm_locks_, sizeof(shmlocks));

    // TODO Student END
}
/*-----------------------------------------------------------------
 * maps the shared objects to the virtual memory space of the User
 * don't do anything else in this function
 *
 * @param none
 *
 * @return none
 */
void initMmapingsUser()
{
  if (checkProgressUser())
  {
    return;
  }

  // TODO Student START
  mmaps.mapped_region_request_ = (shmrequest*) mmap(NULL, sizeof(shmrequest), PROT_READ| PROT_WRITE, MAP_SHARED,fds.fd_shm_request_,0);
  mmaps.mapped_region_response_ =(shmresponse*) mmap(NULL, sizeof(shmresponse), PROT_READ, MAP_SHARED, fds.fd_shm_response_, 0);
  mmaps.mapped_region_state_ = (shmstate*)mmap(NULL, sizeof(shmstate), PROT_READ| PROT_WRITE, MAP_SHARED, fds.fd_shm_state_, 0);
  mmaps.mapped_region_locks_ = (shmlocks*)mmap(NULL, sizeof(shmlocks), PROT_READ| PROT_WRITE, MAP_SHARED, fds.fd_shm_locks_, 0);

  // TODO Student END
}
/*-----------------------------------------------------------------
 * initializes the locks of the shared object
 *
 * @param none
 *
 * @return none
 */
void initLocks()
{
  // TODO Student START
  sem_init(&mmaps.mapped_region_locks_->user_wrote_request,1 ,0);
  sem_init(&mmaps.mapped_region_locks_->conch_sent_reponse,1 ,0);
  sem_init(&mmaps.mapped_region_locks_->user_can_strcpy,1 ,0);

  pthread_mutexattr_t attr;
  pthread_mutexattr_init(&attr);
  pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
  pthread_mutex_init(&mmaps.mapped_region_locks_->reponse_mutex, &attr);

  // TODO Student END
}
/*-----------------------------------------------------------------
 * removes all mappings and shared objects as seen from the MagicConchShell
 * this part is an essential function for closing this application
 * accordingly without leaving artifacts on your system!
 *
 * @param none
 *
 * @return none
 */
void closeMmapingsUser()
{
  if(checkProgress()) 
  {
    return;
  }

  // TODO Student START
  munmap(mmaps.mapped_region_request_, sizeof(shmrequest));
  close(fds.fd_shm_request_);
  shm_unlink(SHM_NAME_REQUEST);

  munmap(mmaps.mapped_region_response_, sizeof(shmresponse));
  close(fds.fd_shm_response_);
  shm_unlink(SHM_NAME_RESPONSE);

  munmap(mmaps.mapped_region_state_, sizeof(shmstate));
  close(fds.fd_shm_state_);
  shm_unlink(SHM_NAME_STATE);

  munmap(mmaps.mapped_region_locks_, sizeof(shmlocks));
  close(fds.fd_shm_locks_);
  shm_unlink(SHM_NAME_LOCKS);

  // TODO Student END
}
