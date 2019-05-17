/* 
   Programmer: Parth Kapur
   Date: 2/6/2018
   Assignment: Programming Assignment 1
   Professor: Tom Springer

   Program Purpose: Familiarize oneself with the standard system calls
   and how to use them in a program.
   
 */

#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

// argc means command line. Argv means index of arguments.
int main(int argc, char*argv[]){
  char buf[256]; // Buffer to read files. This is needed since we are writing in C.


  //The user needs to give 3 arguments
  //If the user does not, it will error.
  if(argc != 3){
    printf("Please re-run the program\n./filecpy <input file> <output file>\n");
    return 1;
  }

  // argv[1] is our first arguement. This is <inputfile>.
  // We will then call open fucntion to read.
  // The function open returns an integer, which we store into filedirectory.
  int input_fd = open(argv[1],O_RDONLY);

  // errno is a global integer.
  // If the function open returns a -1, this is an error and we must 
  // address this.
  if(input_fd == -1){
    printf("Error opening file for reading.\n");
    puts(strerror(errno));   // This function will handle the error for reading file.  
    return 1;
  }
  
  // Now we will create our second file directory, which stores our second
  // argument. 
  int output_fd = open(argv[2],O_CREAT | O_WRONLY | O_EXCL, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if(output_fd == -1){
    printf("Error creating file for writing.\n");
    puts(strerror(errno));
    return 1;
  }


  // This will create an infinite loop to read 256 bytes.
  // Alternatively, keep in mind we could also use a while loop.
  for(;;){
    ssize_t bytes_read = read(input_fd, buf, 256); 
    if(bytes_read == -1){
      printf("Error reading from disk.\n");
      puts(strerror(errno));
      return 1;
    } 
    // When you reach the end of the file, the loop will break.
    if(bytes_read == 0){
      break; 
    }
    
    ssize_t bytes_write = write(output_fd, buf, bytes_read);
    if(bytes_write == -1){
      printf("Error writing to file.");
      puts(strerror(errno));
      return 1;
    }
  }
  
  int fileclose1 = close(input_fd);
  if(fileclose1 == -1){
    printf("Error in closing input file.\n");
    puts(strerror(errno));
  }

  int fileclose2 = close(output_fd);  
  if(fileclose2 == -1){
    printf("Error in closing output file\n");
    puts(strerror(errno));
  }
  return 0;
}
