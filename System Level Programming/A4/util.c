#include "util.h"

/*-----------------------------------------------------------------
 * reads from stdin and processes the string entered for an easier
 * handling of commands.
 *
 * @param char pointer to the buffer that will be processed
 *
 * @return int true/false when successfully read from the stdin
 */
int getCommand(char *buffer) {
  memset(buffer, 0, REQUEST_MAX_LENGTH * sizeof(char));
  if (fgets(buffer, REQUEST_MAX_LENGTH * sizeof(char), stdin) != NULL) {
    return 1;
  }
  else
    return 0;
}

/*-----------------------------------------------------------------
 * predefined function for testing. This is just a wrapper for testing
 * the specific results. You can extend, modify or remove our provided
 * output if you want.
 *
 * @param none
 *
 * @return none
 */
void checkResults()
{
  printf("[MAGIC CONCH SHELL] %s\n", mmaps.mapped_region_response_->message);
}

/*-----------------------------------------------------------------
 * wrapper function for testing the MagicConchShell setup.
 *
 * @param none
 *
 * @return none
 */
void checkMagicConchShell() 
{
  if (process_id == -1)
    exit(-3);
}

/*-----------------------------------------------------------------
 * checks the read- and write capabilities of the shared objects
 * initialized before.
 * You can extend our provided tests here if you want.
 *
 * @param none
 *
 * @return none
 */
void checkSHMrw(int fd) 
{
  if (fd == -1) 
  {
    exit(-1);
  }
}

/*-----------------------------------------------------------------
 * checks the read-only capabilities of the shared objects
 * initialized before.
 * You can extend our provided tests here if you want.
 *
 * @param none
 *
 * @return none
 */
void checkSHMro(int fd) 
{
  if (fd == -1) 
  {
    exit(-1);
  }
}

/*-----------------------------------------------------------------
 * wrapper function for testing the shared objects
 *
 * @param none
 *
 * @return none
 */
void checkSHMUser() 
{
  // use checkSHMro and checkSHMrw here if you want
}

void checkSHMMagicConchShell() 
{
  // use checkSHMro and checkSHMrw here if you want
}

/*-----------------------------------------------------------------
 * predefined function for testing.
 * wrapper function for testing the mmap capabilities.
 * You can extend our provided tests here if you want.
 *
 * @param none
 *
 * @return none
 */
void checkMMAPMagicConchShell() 
{
  if (mmaps.mapped_region_response_ == NULL || mmaps.mapped_region_request_ == NULL || 
      mmaps.mapped_region_locks_ == NULL || mmaps.mapped_region_state_ == NULL) 
  {
    exit(-1);
  }
}

/*-----------------------------------------------------------------
 * predefined function for testing.
 * wrapper function for testing the mmap capabilities.
 * You can extend our provided tests here if you want.
 *
 * @param none
 *
 * @return none
 */
void checkMMAPUser() 
{
  if (mmaps.mapped_region_response_ == NULL || mmaps.mapped_region_request_ == NULL || 
      mmaps.mapped_region_locks_ == NULL || mmaps.mapped_region_state_ == NULL) 
  {
    exit(-1);
  }
}

/*-----------------------------------------------------------------
 * predefined function for testing. Checks if everything was cleaned
 * up properly in both processes. Add your own tests here if you want.
 *
 * @param none
 *
 * @return none
 */
void checkCleanup() 
{
}

/*-----------------------------------------------------------------
 * predefined function for testing.
 *
 * @param none
 *
 * @return none
 */
int checkSetup() 
{
  return 0;
}

/*-----------------------------------------------------------------
 * predefined function for testing.
 *
 * @param none
 *
 * @return none
 */
int checkProgress()
{
  return 0;
}

/*-----------------------------------------------------------------
 * predefined function for testing.
 *
 * @param none
 *
 * @return none
 */
int checkProgressMagicConchShell()
{
  return 0;
}

/*-----------------------------------------------------------------
 * predefined function for testing.
 *
 * @param none
 *
 * @return none
 */
int checkProgressUser()
{
  return 0;
}

/*-----------------------------------------------------------------
 * programs main function. Here for testing reasons.
 *
 * @param none
 *
 * @return none
 */
int main()
{
  #ifdef START
  return START();
  #endif
}

