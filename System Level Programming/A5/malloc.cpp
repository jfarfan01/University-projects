// This should have all the imports you need. Please don't import iostream.
#include <pthread.h>
#include "memory.h"
#include <new>
#include "string.h"

namespace snp
{
// TODO Student

typedef struct  Chunk
{
  size_t size;
  bool free;
  Chunk* next;
  Chunk* prev;

} Chunk;

Chunk* head = NULL;
Chunk* tail = NULL;
size_t free_count = 0;
pthread_mutex_t malloc_mutex = PTHREAD_MUTEX_INITIALIZER;

void *Memory::malloc(size_t size)
{
  pthread_mutex_lock(&malloc_mutex);
  (void) size;
  Chunk* chunk;

  if(head == NULL)
  {
    chunk = (Chunk*) sbrk(size + sizeof(Chunk));
    if(chunk == (void*) -1)
    {
      return NULL;
    }
    head = chunk;
    tail = chunk;
    chunk ->size = size;
    chunk ->free = false;
    chunk ->next = NULL;
    chunk ->prev = NULL;
    pthread_mutex_unlock(&malloc_mutex);
    return chunk + 1;
  }

  else
  {
    //search for free block
    Chunk* temp = head;
    while(temp != NULL)
    {
      if(temp ->free == true && temp ->size >= size)
      {
        temp ->free = false;
        pthread_mutex_unlock(&malloc_mutex);
        return temp + 1;
      }

      temp = temp->next;
    }
    //suitable block not found
    chunk = (Chunk*) sbrk(size + sizeof(Chunk));
    if(chunk == (void*) -1)
    {
      return NULL;
    }
    chunk ->size = size;
    chunk ->free = false;
    chunk ->next = NULL;
    chunk ->prev = tail;
    tail ->next = chunk;
    tail = chunk;
    pthread_mutex_unlock(&malloc_mutex);
    return chunk + 1;
  }
  pthread_mutex_unlock(&malloc_mutex);
  return 0;  
}

void Memory::free(void *ptr)
{
  pthread_mutex_lock(&malloc_mutex);
  free_count++;
  if (ptr == NULL)
  {
    pthread_mutex_unlock(&malloc_mutex);
    return;
  }


  Chunk* temp = head;
  // Chunk* prev = NULL;
  Chunk* to_free = (Chunk*)ptr -1;
  int flag = 0;

  if(to_free ->free)
  {
    exit(-1);
  }
  
  while(temp != NULL)
  {
    if(to_free == temp)
    {
      flag = 1;
      break;
    }
    // prev = temp;
    temp = temp->next;
    
  }

  if(flag == 0)
  {
    pthread_mutex_unlock(&malloc_mutex);
    return;
  }

  (void) ptr;
  to_free ->free = true;
  
  //TODO merge contigious which are free
  if (to_free == tail)
  {
    if(to_free == head)
    {
      head = NULL;
      tail = NULL;
      brk(to_free);
      pthread_mutex_unlock(&malloc_mutex);
      return;
    }

    temp = to_free;
    while (temp != nullptr)
    {
      if(!temp ->free)
      {
        Chunk * to_break = temp ->next;
        temp -> next = NULL;
        tail = temp;
        brk(to_break);
        break;
      }

      if(temp == head && temp->free)
      {
        head = NULL;
        tail = NULL;
        temp -> next = NULL;
        brk(temp);
        break;
      }

      temp = temp->prev;

    }
    // void * brk =  sbrk(0); 
  }

  else
  {
    pthread_mutex_unlock(&malloc_mutex);
    return;
  }
  pthread_mutex_unlock(&malloc_mutex);

  //
}

void *Memory::calloc(size_t num_memb, size_t size_each)
{
  pthread_mutex_lock(&malloc_mutex);
  (void) num_memb;
  (void) size_each;
  size_t total_size = num_memb *size_each;

  Chunk* to_return = (Chunk*) malloc(total_size);
  memset(to_return, 0, sizeof(to_return));

  pthread_mutex_unlock(&malloc_mutex);
  return to_return;
}

size_t Memory::free_called_count() noexcept
{
  return free_count;
}


size_t Memory::free_block_info(int type) noexcept
{
  (void) type;
  Chunk* temp = head;
  while(temp != NULL)
  {
    if(temp ->free == true)
    {
      if(type == 0)
      {
        size_t address = (size_t)(temp) + sizeof(Chunk);
        // size_t * ptr =(size_t*) address;
        return  address;
      }
      else
      {
        return temp ->size;
      }

    }

    temp = temp->next;
  }

  return 0;
}








// TODO Student End
}
