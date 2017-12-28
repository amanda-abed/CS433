#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define N_THREADS 2

int total_hits = 0; /*total points in the circle*/
pthread_mutex_t mutex;

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

  for(i = 0; i < npoints; i++)
    {
      x = random_double() * 2.0 - 1.0;
      y = random_double() * 2.0 - 1.0;

      if(sqrt(x*x + y*y) < 1.0)
        ++hit_count;
    }
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_lock(&mutex);
    total_hits += hit_count;
    pthread_mutex_unlock(&mutex);

    pthread_exit(0);
}

int main(int argc, char *argv[])
{
  pthread_t tid[N_THREADS];
  int npoints = atoi(argv[1]);

  if(argc != 2){
    printf("Error. Need one argument.\n");
    return -1;
  }

  if(npoints <= 0){
    printf("Error. Only enter positive numbers.\n");
    return 1;
  }

  int i;
  for(i = 0; i < N_THREADS; i++)
    pthread_create(&tid[i], NULL, runner, (void*)argv[1]);
  for(i = 0; i < N_THREADS; i++)
    pthread_join(tid[i], NULL);
  
  double pi;
  pi = (double) 4 * total_hits / (N_THREADS * npoints);
  printf("Pi: %f\n", pi);

  /*printf("Total Number of Points: %d\nNumber of Points in Circle: %d\n", npoints, total_hits / N_THREADS);*/

  return 0;
}
