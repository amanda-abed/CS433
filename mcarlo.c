#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

int total_hits = 0; /*total points in the circle*/

double random_double()
{
  return random()/((double)RAND_MAX + 1);
}

void *runner(void* param)
{
  int hit_count = 0;
  int i;
  double x, y;
  
  char* points = (char*)param;
  int npoints = atoi(points);

  srand(time(NULL));

  for(i = 0; i < npoints; i++)
    {
      x = random_double() * 2.0 - 1.0;
      y = random_double() * 2.0 - 1.0;

      if(sqrt(x*x + y*y) < 1.0)
        ++hit_count;
    }
  total_hits += hit_count;

  pthread_exit(0);
}

int main(int argc, char *argv[])
{
  pthread_t tid;

  if(argc != 2){
    printf("Error. Need one argument.\n");
    return 1;
  }

  int npoints = atoi(argv[1]);
  if(npoints <= 0){
    printf("Error. Only enter positive numbers.\n");
    return 1;
  }

  pthread_create(&tid, NULL, runner, (void*)argv[1]);
  pthread_join(tid, NULL);
  
  double pi;
  pi = (double) 4 * total_hits / npoints;
  printf("Pi: %f\n", pi);

  return 0;
}
