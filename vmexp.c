#include <stdio.h>
#include <stdlib.h>

int main(int argc, char*argv[])
{
  if(argc != 2)
    {
      printf("Invalid number of arguments.\n");
      return 1;
    }
  
  unsigned int addr;
  unsigned int page;
  unsigned int offset;
  int size = 4096;

  addr = atoi(argv[1]);
  page = addr / size;
  offset = addr % size;

  printf("The address %u contains:\n", addr);
  printf("page number = %u\n", page);
  printf("offset = %u\n", offset);

  return 0;
}
