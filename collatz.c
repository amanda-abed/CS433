#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
  pid_t pid;
  int num;

  if(argc != 2)
    printf("Error. Need one argument.\n");
  
  else{
    num = atoi(argv[1]);
    pid = fork();
    
    if(pid < 0){
      printf("Error");
      return 1;
    }
    else if(pid == 0){
      if(num <= 0){
	printf("Error. Enter a positive number\n");
	return 1;
      }
      else{
	while(num != 1){
	  printf("%d, ", num);
	  if(num%2 == 0)
	    num = num/2;
	  else
	    num = (num*3)+1;
	}
	printf("%d\n", num);
      }
    }
    else
      wait();
  }
  return 0;
}
