#include <stdlib.h>
#include <stdio.h>

#define MAXPAGES 40

int frames, temp; // frames = # of frames, temp = # of pages
int data[MAXPAGES]; //array of pages
int fn[MAXPAGES]; //holds array of number of frames
int pg[MAXPAGES]; //for LRU
int next[MAXPAGES]; //for optimal

void FIFO();
void LRU();
void OPTIMAL();

int main(int argc, char*argv[])
{
  if(argc != 3)
    {
      printf("Error. <frames> <page reference string>\n");
      printf("Page reference string must only include commas in between page numbers, no spaces.\n");
      return 1;
    }

  frames = atoi(argv[1]);
  int i = 0;

  while(argv[2][i] != '\0')
    {
      if(argv[2][i] == ',')
	i++;
      data[temp] = atoi(&argv[2][i]);
      if(data[temp] >= 0 && data[temp] <= 9)
	{
	  i++;
	  temp++;
	}
      else
	{
	  printf("Error. Page number must be between 0-9.\n");
	  return 1;
	}
    }
  FIFO();
  LRU();
  OPTIMAL();
  return 0;
}

void FIFO()
{
  int fault = 0, count;
  int i, j; //for loop variables

  i = 0;
  while(i < frames)
    {
      fn[i] = -1;
      i++;
    }
  i = 0;
  while(i < temp)
    {
      count = 0;
      j = 0;
      while(j < frames)
	{
	  if(fn[j] == data[i])
	    {
	      count++;
	      fault--;
	    }
	  j++;
	}
      fault++;
      if(fault <= frames && count == 0){
	fn[i]= data[i];
      }
      else if(count == 0){
	fn[(fault-1)%frames] = data[i];
      }
      i++;
    }
  printf("FIFO Page Faults: %d\n", fault);
}

void LRU()
{
  int a, b, count, fault = 0; //a,b are flag vars
  int i, j, k, t; //for loop variable

  i = 0;
  while(i < frames)
    {
      fn[i] = -1;
      i++;
    }

  i = 0;
  while(i < temp)
    {
      a = 0;
      b = 0;
      j = 0;
      while(j < frames)
	{
	  if(fn[j] == data[i])
	    {
	      a = 1;
	      b = 1;
	      break;
	    }
	  j++;
	}
      if(a == 0)
	{
	  j = 0;
	  while(j < frames)
	    {
	      if(fn[j] == -1)
		{
		  fault++;
		  fn[j] = data[i];
		  b = 1;
		  break;
		}
	      j++;
	    }
	}

      if(b == 0)
	{
	  j = 0;
	  while(j < frames)
	    {
	      pg[j] = 0;
	      j++;
	    }
	  j = 0;
	  while(j < frames)
	    {
	      k = 1;
	      for(t = i - 1; k <= frames-1; t--) //read from the end of list
 		{
		  if(data[t] == fn[j])
		    {
		      pg[j] = 1;
		    }
		  k++;
		}
	      j++;
	    }
	  j = 0;
	  while(j < frames)
	    {
	      if(pg[j] == 0)
		{
		  count = j;
		}
	      j++;
	    }
	  fault++;
	  fn[count] = data[i];
	}
      i++;
    }
  printf("LRU Page Faults: %d\n", fault);
}

void OPTIMAL()
{
  int a, b, var, fault = 0, hit, count, top; //a,b are flag vars
  int i, j, t; //for loop variables

  i = 0;
  while(i < frames)
    {
      fn[i] = -1;
      i++;
    }

  hit = 0;
  i = 0;
  while(i < temp)
    {
      a = 0;
      b = 0;
      j = 0;
      while(j < frames)
	{
	  if(fn[j] == data[i])
	    {
	      a = 1;
	      b = 1;
	      break;
	    }
	  j++;
	}
      if(a == 0)
	{
	  j = 0;
	  while(j < frames)
	    {
	      if(fn[j] == -1)
		{
		  fault++;
		  fn[j] = data[i];
		  b = 1;
		  break;
		}
	      j++;
	    }
	}
      if(b == 0)
	{
	  j = 0;
	  while(j < frames)
	    {
	      next[j] = 0;
	      j++;
	    }
	  j = 0;
	  while(j < frames)
	    {
	      next[j] = 0;
	      for(t = i + 1; t < temp; t++) //read ahead of the list
		{
		  if(fn[j] == data[t])
		    {
		      next[j] = t - i;
		      break;
		    }
		}
	      j++;
	    }
	  j = 0;
	  while(j < frames)
	    {
	      if(next[j] == 0)
		{
		  count = j;
		  hit = 1;
		}
	      j++;
	    }
	  if(hit == 0)
	    {
	      top = next[0];
	      count = 0;
	      j = 1;
	      while(j < frames + 1)
		{
		  if(top < next[j])
		    {
		      top = next[j];
		      count = j;
		    }
		  j++;
		}
	    }
	  fault++;
	  fn[count] = data[i];
	}
      i++;
    }
  printf("Optimal Page Faults: %d\n", fault);
}
