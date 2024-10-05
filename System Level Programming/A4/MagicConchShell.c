#include "MagicConchShell.h"

char* messages[] = {                          //     motivated answer        unmotivated answer
        "Do I love programming?\n",           //0    "Oh yes!\n"     (12)    "Maybe.\n"      (9)
        "Should I drink some beer?\n",        //1    "Maybe.\n"      (9)     "Definitely!\n" (11)
        "Will I pass SLP?\n",                 //2    "Definitely!\n" (11)    "Maybe.\n"      (9)
        "Should I use locking?\n",            //3    "Definitely!\n" (11)    "Definitely!\n" (11)
        "Should I take a break?\n",           //4    "No.\n"         (10)    "Oh yes!\n"     (12)
        "Should I copy code?\n",              //5    "No.\n"         (10)    "No.\n"         (10)
        "bye\n",                              //6
        "motivation\n",                       //7
        "unmotivation\n",                     //8
        "Maybe.\n",                           //9
        "No.\n",                              //10
        "Definitely!\n",                      //11
        "Oh yes!\n",                          //12
        "I do not know.\n"                    //13
};

// TODO Student START
// you can add your own global variables here if needed
// (do not use the same names in User and MagicConchShell because of testing reasons)

// TODO Student END

/*-----------------------------------------------------------------
 *  Main routine that loops and forwards requests and eventually
 *  makes it public to the User. stops when "bye" is received.
 *  Initialize everything needed in here.
 *
 *  @param argc number of arguments given
 *  @param argv char* that points to the argument
 *
 *  @return predefined value
 **/
int startMagicConchShell()
{
  initSharedMemoriesMagicConchShell();
  checkSHMMagicConchShell();
  initMmapingsMagicConchShell();
  checkMMAPMagicConchShell();

  printf("[MAGIC CONCH SHELL] Ask me anything!\n");
  do
  {
    // TODO Student START
    sem_post(&mmaps.mapped_region_locks_->user_can_strcpy);
    sem_wait(&mmaps.mapped_region_locks_->user_wrote_request);
    // printf("%d\n", mmaps.mapped_region_state_->motivated);
    pthread_mutex_lock(&mmaps.mapped_region_locks_->reponse_mutex);
    strcpy(mmaps.mapped_region_response_->message, dispatcher(mmaps.mapped_region_request_));
    pthread_mutex_unlock(&mmaps.mapped_region_locks_->reponse_mutex);

    sem_post(&mmaps.mapped_region_locks_->conch_sent_reponse);
    
    // TODO Student END

  } while (strcmp(mmaps.mapped_region_request_->message, "bye\n") != 0);

  closeMmapingsMagicConchShell();
  checkCleanup();

  return 0;

}
/*-----------------------------------------------------------------
 * initializes the MagicConchShells shared objects and resizes them, make 
 * sure to only give the permissions the MagicConchShell needs.
 * but please use MODERW, for compatibility with the testsystem
 *
 * @param none
 *
 * @return none
 */
void initSharedMemoriesMagicConchShell()
{
  // TODO Student START
  fds.fd_shm_request_ = shm_open(SHM_NAME_REQUEST, FLAGS_SHM_READONLY, MODERW);
  ftruncate(fds.fd_shm_request_, sizeof(shmrequest));

  fds.fd_shm_response_ = shm_open(SHM_NAME_RESPONSE, FLAGS_SHM_READWRITE, MODERW);
  ftruncate(fds.fd_shm_response_, sizeof(shmresponse));

  fds.fd_shm_state_ = shm_open(SHM_NAME_STATE, FLAGS_SHM_READONLY, MODERW);
  ftruncate(fds.fd_shm_state_, sizeof(shmstate));

  fds.fd_shm_locks_ = shm_open(SHM_NAME_LOCKS, FLAGS_SHM_READWRITE, MODERW);
  ftruncate(fds.fd_shm_locks_, sizeof(shmlocks));

  // TODO Student END
}
/*-----------------------------------------------------------------
 * maps the shared objects to the virtual memory space of the MagicConchShell
 * don't do anything else in this function
 *
 * @param none
 *
 * @return none
 */
void initMmapingsMagicConchShell()
{
  if (checkProgressMagicConchShell())
  {
    return;
  }

  // TODO Student START
  mmaps.mapped_region_request_ = (shmrequest*) mmap(NULL, sizeof(shmrequest), PROT_READ, MAP_SHARED,fds.fd_shm_request_,0);
  mmaps.mapped_region_response_ = (shmresponse*) mmap(NULL, sizeof(shmresponse), PROT_READ|PROT_WRITE, MAP_SHARED, fds.fd_shm_response_, 0);
  mmaps.mapped_region_state_ =(shmstate*) mmap(NULL, sizeof(shmstate), PROT_READ, MAP_SHARED, fds.fd_shm_state_, 0);
  mmaps.mapped_region_locks_ = (shmlocks*) mmap(NULL, sizeof(shmlocks), PROT_READ|PROT_WRITE, MAP_SHARED, fds.fd_shm_locks_, 0);

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
void closeMmapingsMagicConchShell()
{
  if (checkProgressMagicConchShell())
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

/*-----------------------------------------------------------------
 * finds correct response to incoming requests
 *
 * @param shmrequest the pointer to the request object
 *
 * @return char* the answer to the request
 */
char *dispatcher(shmrequest *request)
{
  // TODO Student START
  // respond to the incoming questions
  if(mmaps.mapped_region_state_->motivated == 1)
  {
    if(!strcmp(messages[0], mmaps.mapped_region_request_->message))
    {
      return messages[12];
    }

    if(!strcmp(messages[1], mmaps.mapped_region_request_->message))
    {
      return messages[9];
    }

    if(!strcmp(messages[2], mmaps.mapped_region_request_->message ))
    {
      return messages[11];
    }

    if(!strcmp(messages[3], mmaps.mapped_region_request_->message))
    {
      return messages[11];
    }

    if(!strcmp(messages[4], mmaps.mapped_region_request_->message))
    {
      return messages[10];
    }

    if(!strcmp(messages[5], mmaps.mapped_region_request_->message))
    {
      return messages[10];
    }
  }


  if(mmaps.mapped_region_state_->motivated == 0)
  {
    if(!strcmp(messages[0], mmaps.mapped_region_request_->message))
    {
      return messages[9];
    }

    if(!strcmp(messages[1], mmaps.mapped_region_request_->message))
    {
      return messages[11];
    }

    if(!strcmp(messages[2], mmaps.mapped_region_request_->message))
    {
      return messages[9];
    }

    if(!strcmp(messages[3], mmaps.mapped_region_request_->message))
    {
      return messages[11];
    }

    if(!strcmp(messages[4], mmaps.mapped_region_request_->message))
    {
      return messages[12];
    }

    if(!strcmp(messages[5], mmaps.mapped_region_request_->message))
    {
      return messages[10];
    }
  }

  if(!strcmp(messages[6], mmaps.mapped_region_request_->message))
  {
    return messages[6];
  }
  

  // TODO Student END
  return messages[13];
}


