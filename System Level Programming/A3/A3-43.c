#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int global_initialized_int = 10;
int global_uninitialized_int;


int main() {
  static int static_int;
  int stack_int = 10;
  int *heap_int =  malloc(20);
  int shmid;
  key_t key = 1234;


  int *shared_int = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  

  printf("Function address: %p\n", main);
  printf("Global uninitialized int address: %p\n", &global_uninitialized_int);
  printf("Global initialized int address: %p\n", &global_initialized_int);
  printf("Static int address: %p\n", &static_int);
  printf("Stack int address: %p\n", &stack_int);
  printf("Heap int address: %p\n", heap_int);
  printf("Shared int address: %p\n", &shared_int);
  printf("PID: %d\n", getpid());
  sleep(80);

  return 0;
}
