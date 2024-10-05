#include "definitions.h"

Student* second_chance_students[SECOND_CHANCE_STUDENTS_CAPACITY];

vector free_desks;

bool agStart = false;

/*
 * STUDENT TODO BEGIN:
 *  locking-related global variables may be declared here
 */
pthread_mutex_t agStart_Mutex;

pthread_mutex_t second_chance_Mutex;
sem_t  second_chance_Semaphore;

sem_t free_desks_Semaphore;

pthread_cond_t agStart_Cond_Var = PTHREAD_COND_INITIALIZER;
pthread_mutex_t free_desks_Mutex;



// ------------------------------------------------------------------------
/* STUDENT TODO:
 *  make sure : 
    ~ shared resources are locked correctly
    ~ every second chance student gets interviewed
 */
void takeTheSecondChance(Student* student) 
{
  int i = 0;

  printf("Student %zd waits for the second chance interview ...\n", student->id);

  sem_wait(&second_chance_Semaphore);
  pthread_mutex_lock(&second_chance_Mutex);
  while (second_chance_students[i] != NULL) 
  {
    i++;
  }

  second_chance_students[i] = student;
  
  pthread_mutex_unlock(&second_chance_Mutex);


  secondChanceInterview();
  pthread_mutex_lock(&second_chance_Mutex);


  second_chance_students[i] = NULL;
  sem_post(&second_chance_Semaphore);
  pthread_mutex_unlock(&second_chance_Mutex);


  printf("Student %zd finished the second chance interview ...\n", student->id);
}

// ------------------------------------------------------------------------
/* STUDENT TODO:
 *  make sure : 
    ~ shared resources are locked correctly
    ~ the student gets notified about entered points
 */
void enterPoints(Student* student)
{
  student->points = getPoints();

  printf("Student %zd received %zd points\n", student->id, student->points);
  sem_post(&student->personal_Semaphore);//points entered
}

// ------------------------------------------------------------------------
/* STUDENT TODO:
 *  make sure : 
    ~ shared resources are locked correctly
 */
Desk* findFreeDesk()  // used by tutors
{
  vector_iterator it = vector_begin(&free_desks);
  vector_iterator it_end = vector_end(&free_desks);

  Desk* desk = NULL;

  for (; it != it_end; ++it) 
  {
    Desk* current_desk = (Desk*)*it;
    if (current_desk->tutor == NULL)
    {
      desk = current_desk;
      break;
    } 
  }
  return desk;
}

// ------------------------------------------------------------------------
/* STUDENT TODO:
 *  make sure : 
    ~ shared resources are locked correctly
 */
vector_iterator findActiveDesk()  // used by students
{
  pthread_mutex_lock(&free_desks_Mutex);
  vector_iterator it = vector_begin(&free_desks);
  vector_iterator it_end = vector_end(&free_desks);
  vector_iterator desk_it = NULL;
  for (; it != it_end; ++it) 
  {
    Desk* current_desk = (Desk*)*it;
    if (current_desk->active)
    {
      desk_it = it;
      break;
    }
  }
  // pthread_mutex_unlock(&free_desks_Mutex);

  return desk_it;
}

// ------------------------------------------------------------------------
/* STUDENT TODO:
 *  make sure : 
    ~ all shared resources are locked correctly
    ~ students are notified once all tutors are ready and ags can start
 */
void checkIfInterviewsCanStart()
{
  bool start = true;
  vector_iterator it = vector_begin(&free_desks);
  vector_iterator it_end = vector_end(&free_desks);

  for (; it != it_end; ++it) 
  {
     Desk* current_desk = (Desk*)*it;
     start &= current_desk->active;

     if(!start)
       break;
  }
  pthread_mutex_unlock(&free_desks_Mutex);

  // ----- interviews can start -----
  if(start)
  {
    pthread_mutex_lock(&agStart_Mutex);
    printf("AGs START!\n");
    assert(!agStart && "This looks like a problem\n"); // try using asserts for debugging ;)
    agStart = true;
    pthread_cond_broadcast(&agStart_Cond_Var);
    pthread_mutex_unlock(&agStart_Mutex);
  }
}

// ------------------------------------------------------------------------
/* STUDENT TODO:
 *  make sure : 
    ~ shared resources are locked correctly
    ~ tutor always waits for the next student
 */
void slpAGTutor(Tutor* tutor)
{
  printf("Tutor %zd tries to find a free desk\n", tutor->id);

  pthread_mutex_lock(&free_desks_Mutex);

  Desk* desk = findFreeDesk();

  if(!desk || desk->tutor)
  {
    printf("Tutor %zd is confused, can't find a free desk\n", tutor->id);
  }

  desk->tutor = tutor;

  desk->active = true;


  printf("Tutor %zd found desk %zd\n", tutor->id, desk->id);

  checkIfInterviewsCanStart();

  while(1)
  {
    printf("Tutor %zd waits for the next student\n", tutor->id);
    sem_wait(&tutor->personal_Semaphore);//student found desk

    if(tutor->interviews_done)
    {
      printf("No more students left - Tutor %zd can go home\n", tutor->id);
      break;
    }

    //desk is now a private resource between this tutor and the student that he's interviewing,
    // because this tutor's private sem only got incrd by the student
    if(!desk->student)
    {
      printf("Tutor %zd has gone crazy, works although no student arrived\n", tutor->id);
    }

    checkStudentID();

    printf("Tutor %zd interviews the student %zd\n", tutor->id, desk->student->id);

    interviewTheStudent();  

    enterPoints(desk->student);
    
    desk->student = NULL;

    pthread_mutex_lock(&free_desks_Mutex);
    vector_push_back(&free_desks, desk);

    
    //important: nothing may be locked when enteringh this point
    if(isBreakNeeded())
    {

      desk->active = false;
      pthread_mutex_unlock(&free_desks_Mutex);

      printf("Tutor %zd needs a short break\n", tutor->id);
      coffeeBreak();

      pthread_mutex_lock(&free_desks_Mutex);
      desk->active = true;
    }

    pthread_mutex_unlock(&free_desks_Mutex);

    sem_post(&free_desks_Semaphore);
  }
  sem_destroy(&tutor->personal_Semaphore);
}

// ------------------------------------------------------------------------
/* STUDENT TODO:
 *  make sure : 
    ~ shared resources are locked correctly
    ~ students wait for ags to start (they do not look for a free tutor beforehand)
    ~ students do not leave before checking points status
 */
void slpAGStudent(Student* student)
{
  pthread_mutex_lock(&agStart_Mutex);
  while (!agStart)
  {
    pthread_cond_wait(&agStart_Cond_Var, &agStart_Mutex);
  }

  if(!agStart)
  {
    printf("Student %zd tries to get interviewed although AGs haven't started yet\n", student->id);
  }

  pthread_mutex_unlock(&agStart_Mutex);

  printf("Student %zd is looking for a tutor\n", student->id);
  //cond_val if free_desk-> size 0 wait.
  sem_wait(&free_desks_Semaphore);
  vector_iterator desk_it = findActiveDesk();
  Desk* desk = *desk_it;
  

  if(desk->student != NULL || !desk->active)
  {
    printf("Student %zd is confused, can't find a free desk\n", student->id);
  }

  desk->student = student;
  // pthread_cond_signal(&desk->tutor->next_student_Cond_Var);//commented this out

  vector_erase(&free_desks, desk_it);
  pthread_mutex_unlock(&free_desks_Mutex);
  sem_post(&desk->tutor->personal_Semaphore);//student found desk

  showStudentID();
  sem_wait(&student->personal_Semaphore);//points entered

  if(student->points == NO_ENTRY)
  {
    printf("Student %zd didn't receive any points :|\n", student->id);
  }
  else
  {
    printf("Student %zd received the points\n", student->id);
  }


  if(student->points < POSITIVE_POINTS)
  {
    printf("Student %zd must take the second chance\n", student->id);    
    takeTheSecondChance(student);
  }
  // sem_wait(&student->personal_Semaphore);//tutor checks if there's more work //commented this outs

  printf("Student %zd is done and can go home\n", student->id);

  sem_destroy(&student->personal_Semaphore);
  printf("student %zd exiting after destroy", student->id);
}

// ------------------------------------------------------------------------
/* STUDENT TODO:
 *  make sure : 
    ~ synchronization primitives are initialized correctly
 */
void createInterviewDesks(ssize_t num_desks)
{
  vector_init(&free_desks);

  for (ssize_t i = 0; i < num_desks; i++) 
  {
    Desk* desk = malloc(sizeof(Desk));

    if(!desk)
    {
      free(desk);
      fprintf(stderr, "Could not allocate memory!\n");
      exit(ERROR);
    }
    
    desk->id = i;
    desk->tutor = NULL;
    desk->student = NULL;
    
    vector_push_back(&free_desks, desk);
    sem_post(&free_desks_Semaphore);
  }
}

// ------------------------------------------------------------------------
/* STUDENT TODO:
//  *  make sure : 
    ~ synchronization primitives are initialized correctly
    ~ shared resources are locked correctly
    ~ tutors go home once all students have been interviewed
 */
int main(int argc, char* argv[]) 
{
  srand(time(NULL));
  pthread_mutex_init(&agStart_Mutex, 0);
  pthread_mutex_init(&free_desks_Mutex, 0);
  pthread_mutex_init(&second_chance_Mutex, 0);
  sem_init(&free_desks_Semaphore,0,0);
  sem_init(&second_chance_Semaphore, 0, SECOND_CHANCE_STUDENTS_CAPACITY);

  ssize_t num_tutors;
  ssize_t num_students;

  handleParameters(argc, argv, &num_tutors, &num_students);
  
  Tutor* tutors = malloc(num_tutors * sizeof(Tutor));
  for(size_t i = 0; i < num_tutors; i++)
  {
    sem_init(&tutors[i].personal_Semaphore,0,0);
  }
  Student* students = malloc(num_students * sizeof(Student));
  for(size_t i = 0; i < num_students; i++)
  {
    sem_init(&students[i].personal_Semaphore,0,0);
  }
  
  if (!tutors || !students) 
  {
    free(tutors);
    free(students);
    fprintf(stderr, "Could not allocate memory!\n");
    exit(ERROR);
  }
  
  createInterviewDesks(num_tutors);
  createStudents(students, num_students, (void*(*)(void*))slpAGStudent);
  createTutors(tutors, num_tutors, (void*(*)(void*))slpAGTutor); 
    
  for (ssize_t i = 0; i < num_students; i++) 
  {
    pthread_join(students[i].student, NULL);
  }
  // all students are now completely done.
  for (ssize_t i = 0; i < num_tutors; i++) 
  {
    tutors[i].interviews_done = true;
    sem_post(&tutors[i].personal_Semaphore);
    // printf("signaled that bitch\n");
    pthread_join(tutors[i].tutor, NULL);
  }

  printf("SLP AG DONE!\n");
  pthread_mutex_destroy(&agStart_Mutex);
  pthread_mutex_destroy(&free_desks_Mutex);
  pthread_mutex_destroy(&second_chance_Mutex);
  pthread_cond_destroy(&agStart_Cond_Var);
  sem_destroy(&free_desks_Semaphore);
  sem_destroy(&second_chance_Semaphore);
 
  freeResources(tutors, students, &free_desks);

  return 0;
}