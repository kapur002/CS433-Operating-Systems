/* Programmer: Parth Kapur
   Professor: Tom Springer
   Class: CS433 Operating Systems
   Assignment: Monte Carlo Simulation
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

// these are the total points in the circle
int totHits = 0; 

double random_double()
{
  return random()/((double)RAND_MAX + 1);
}

void *runner(void* param)
{
  int hitCount = 0;
  int i;
  double x, y;
  
  char* points = (char*)param;
  int numpoints = atoi(points);

  srand(time(NULL));

  for(i = 0; i < numpoints; i++)
    {
      x = random_double() * 2.0 - 1.0;
      y = random_double() * 2.0 - 1.0;

      if(sqrt(x*x + y*y) < 1.0)
        ++hitCount;
    }
  totHits += hitCount;

  pthread_exit(0);
}

int main(int argc, char *argv[])
{
  pthread_t tid;

  // If there are not two arguments entered into the command line, there will be an error.
  if(argc != 2){
    printf("ERROR: Need one additional argument.\n");
    return 1;
  }

  // If the input for the keyboard for the number is not positive, then there will be an error informing the user.
  int numpoints = atoi(argv[1]);
  if(numpoints <= 0){
    printf("ERROR: Enter only positive integers.\n");
    return 1;
  }

  // create and join thread
  pthread_create(&tid, NULL, runner, (void*)argv[1]);
  pthread_join(tid, NULL);
  
  double pi;
  pi = (double) 4 * totHits / numpoints;
  printf("Pi is: %f\n", pi);

  return 0;
}
