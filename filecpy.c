#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#define BUF_SIZE 512

int main(int argc, char *argv[])
{
  char buf [BUF_SIZE];
  
  if(argc != 3)
    printf("Error. Need two arguments.\n");
  else{
    /*open input file*/
    int inputf;
    inputf = open(argv[1], O_RDONLY);
    if(inputf == -1){
      printf("Error. %s\n", strerror(errno));
      return 1;
    }
    int outputf;
    outputf = open(argv[2], O_WRONLY | O_CREAT | O_EXCL, 0644);
    
    if(outputf == -1){
      printf("Error. %s\n", strerror(errno));
      return 1;
    }
    else{
	printf("Success opening input file: %s \n", argv[1]);
	printf("Success opening output file: %s \n", argv[2]);
	
	/*read input file and write to output*/
	int size_in;
       	while((size_in = read(inputf, buf, BUF_SIZE)) != 0)
	  { 
	    write(outputf, buf, size_in);
	  }
    }

    /*close input and output file*/
    close(inputf);
    close(outputf);

    /*completion message*/
    printf("Successfully copied %s to %s.\n", argv[1], argv[2]);

    return 0;
  }
}
