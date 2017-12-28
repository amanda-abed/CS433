#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#define BUF_MAX 64000

sem_t empty, full;
pthread_mutex_t mutex;
unsigned char *buf; /*shared buffer*/
int ntimes = 0; /*stores ntimes*/
int blocks = 0; /*shared memory blocks*/

static unsigned short checksum(unsigned short *ptr, int nbytes)
{
  int sum;
  unsigned short exbyte;
  unsigned short sumck;

  sum = 0;
  while (nbytes > 1 ) 
    {
      sum += *ptr++;
      nbytes -= 2;
    }
  if (nbytes == 1)
    {
      exbyte = 0;
      *((unsigned char *) &exbyte) = *(unsigned char *)ptr;
      sum += exbyte;
    }
  sum = (sum >> 16) + (sum & 0xffff);
  sum += (sum >> 16);
  sumck = ~sum;

  return sumck;		   
}

void *producer(void *param)
{
  int i, h, block = 0;
  unsigned short cksum;
  srand(time(NULL));

  for(i = 1; i <= ntimes; i++)
    {
      sem_wait(&empty);
      pthread_mutex_lock(&mutex);
      for(h = 0; h < 30; h++)
        {
          buf[h + (32 * block)] = (unsigned char)(rand() % 255);
        }
      cksum = checksum((unsigned short *)&buf[0 + (32 * block)], 32);
      memcpy((void *)&buf[30 + (32 * block)], (void *)&cksum, 2);
      printf("Checksum (Prod) Run %d: 0x%hx\n", i, cksum);//checking cksum generated
      block = (block + 1) % blocks; /*move to next block*/
      pthread_mutex_unlock(&mutex);
      sem_post(&full);
    }
  pthread_exit(0);
}

void *consumer(void *param)
{
  int i, h, block = 0;
  unsigned short cksum1, cksum_buf;
 
  for(i = 1; i <= ntimes; i++)
    {
      sem_wait(&full);
      pthread_mutex_lock(&mutex);
      memcpy((void *)&cksum_buf, (void *)&buf[30 + (32 * block)], 2);
      cksum1 = checksum((unsigned short *)&buf[0 + (32 * block)], 30);
      // printf("Checksum (Con) Run %d: 0x%hx\n",i , cksum_buf);      
      if(cksum1 != cksum_buf)
	{
	  printf("Run %d. Checksum 0x%hx does not match. Expected checksum: 0x%hx\n",i, cksum1, cksum_buf);
	  exit(-1);
	}
      else{
	printf("Checksum (Con) Run %d: 0x%hx\n", i, cksum1);
	printf("Same checksum.\n");}
      block = (block + 1)%blocks; /*move to next block*/
      pthread_mutex_unlock(&mutex);
      sem_post(&empty);
    }
  pthread_exit(0);
}

int main(int argc, char *argv[])
{
  /*checks number of arguments*/
  if(argc != 3)
    {
      printf("Error. Need two arguments: <memsize> <ntimes>.\n");
      return 1;
    }

  /*checks memsize if it's valid*/
  int buffer = atoi(argv[1]);
  if(buffer % 32 != 0)
    {
      printf("Error. <memsize> must be a multiple of 32.\n");
      return 1;
    }
  else if(buffer > BUF_MAX)
    {
      printf("Error. <memsize> must not exceed 64,000.\n");
      return 1;
    }

  /*checks ntimes*/
  blocks = buffer / 32;
  ntimes = atoi(argv[2]);
  if(ntimes > blocks || ntimes <= 0)
    {
      printf("Error. <ntimes> is invalid.\n");
      return 1;
    }

  buf = (unsigned char *)malloc(buffer);

  sem_init(&empty, 0, blocks);
  sem_init(&full, 0, 0);

  pthread_mutex_init(&mutex, NULL);
  pthread_t p_id, c_id;

  pthread_create(&p_id, NULL, &producer, NULL);
  pthread_create(&c_id, NULL, &consumer, NULL);
  pthread_join(p_id, NULL);
  pthread_join(c_id, NULL);

  pthread_mutex_destroy(&mutex);
  sem_destroy(&empty);
  sem_destroy(&full);

  return 0;
}
