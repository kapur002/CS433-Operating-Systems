/*Programmer: Parth Kapur
  Assignment 2: Collatz.c
  Program Objective:
  Write a program using the Fork System call that will do n/2 if n is even
  and 3 x n + 1 if n is odd. Use the fork() system call to generate this
  sequence in the child process.
 */
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>

int collatz(int read_user);

int main(int argc, char* argv[])
{
  // In the event that the user does not enter the proper number of arguements
  // we must have some error checking. Thus, if the command to run does not
  // have 2 arguments, the program will not run.
  if (argc!=2)
  {
    printf("Please re-run the program.\n ./collatz <start number>\n");
    return 1;
  }
  
  // The atoi function will convert a string argument into an integer
  // As it is reading the first integer from the command line, this 
  // is necessary to do.
  int read_user=atoi(argv[1]);

  // Error checking - the user should always enter in an integer that
  // is greater than or equal to 0.
  if (read_user <= 0){
    printf("User Input \'%s\'.\n",argv[1]);
    printf("Please enter a positive number.\n");
    return 1;
  }
  else
  {
    //Calling the collatz function
    collatz(read_user);
  }
  return 0;
}

// Collatz function definition
int collatz(int read_user)
{
  // First we will create the child process via fork system call.
  pid_t pid = fork();
  if (read_user == 1)
  {
    printf("%d\n", read_user);
    return read_user; 
  }
  
  if (pid<0)
  {
    printf("Impossible to Fork a Child", strerror(errno));
    return -1;
  }

  else if (pid == 0)
  {
    printf("SEQUENCE:\n%d\n", read_user);
    pid_t child_pid = getpid();
    while (read_user !=1)
    {
      read_user=read_user % 2 ? 3 * read_user + 1: read_user/2;
      printf("Child %d\n", read_user);
    }
    printf("\nENDING");
    return EXIT_SUCCESS;
  }
  else
  {
    wait(NULL);
    printf("PARENT PROCESS!\n");
  }   
    printf("PARENT PROCESS NOW ENDING\n");
    return EXIT_SUCCESS;
}
