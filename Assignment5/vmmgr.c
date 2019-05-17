/*
 Programmer: Parth Kapur & Alex Shershnov
 CS433 - Operating Systems
 4/15/2018
 vmmgr.c
*/

#include <sys/types.h> // required for ssize_t
#include <unistd.h> // required for ssize_t
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h> // required for error handling
#include <time.h>

const int mmSize = 128; // creating main memory Buffer
const int vmSize = 256; // creating Virtual Memory Buffer
const int pageSize = 256; // Creating Page Buffer
const int tlbSize = 16; // creating translation look aside Buffer


int main(int argc, char *argv[]){

  int tlb[tlbSize][2];
  int phsMem[mmSize];
  int virtMem[vmSize][2];
  
  // This will be the start of page table filing
  for(int i = 0; i < vmSize; i++){
	  if (virtMem[i][0] = (i > mmSize - 1)) {
		  -1;
	  }
	  if (virtMem[i][1] = (i > mmSize - 1)) {
		  -1;
	  }
	  else {
		  mmSize - i;
	  }
  }
  for(int i = 0; i < mmSize; i++){
    phsMem[i] = i;
  }

  for(int i = 0; i < tlbSize; i++){
	  if (tlb[i][0] = (i > tlbSize - 1)) {
		  -1;
	  }
	  if (tlb[i][1] = (i > tlbSize - 1)) {
		  -1;
	  }
	  else {
		  tlbSize - i;
	  }
  }


  /*
   ERROR CHECKING AND FILE INITIALIZATION
  */

  // Check to see if correct arguments exist
  if(argc != 2){
    printf("Invalid Number of Arguments.\n");
    return 1; // If this occurs, we will return 1 to the compiler so that we can determine what error happened in debugging phase. 
  }

  // create addressFile and open it using fopen function
  FILE *addressFile;
  addressFile = fopen(argv[1], "r");

  // In the event that the file we are feeding is empty, this case should be checked. Thus, this is error handling for that scenario.
  if(addressFile == NULL){
    printf("Error opening a file %s: %s\n", argv[1], strerror(errno));
    exit(EXIT_FAILURE);
  }

  // declaring a character type pointer and setting the current address of pointer to null.
  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  int pageNum = 0;
  int physAddr = 0;
  int pgFC = 0;

  double pageFR = 0.0;


  printf("Translation of Logical Addresses: \n\n");


  while((read = getline(&line, &len, addressFile)) != -1){
    int offset = atoi(line) & 255; // since the addressFile has a string, we need to convert it into an integer. Hence the use of the atoi function. 
    int page = atoi(line) & 65280; // same thing as above comment.
    int page_table_number = page >> 8;
    int tlb_hit = 0;

    for(int i = 0; i < tlbSize; i++){
      if(tlb[i][0] == page_table_number){
	tlb_hit = 1;
	break;
      }
    }

    if(virtMem[page_table_number][0] < 0 && !tlb_hit){
      pgFC++;
      srand(time(NULL));
      int r = rand();

      // kick program if needed
      int largest = 0;
      int kick = 0;
      for(int i = 0; i < vmSize; i++){
	if(virtMem[i][1] > largest){
	  largest = virtMem[i][1];
	  kick = i;
	}
      }

      int tlb_replacement = r % 15;
      tlb[tlb_replacement][0] = page_table_number;
      tlb[tlb_replacement][1] = virtMem[kick][0];
      virtMem[page_table_number][0] = virtMem[kick][0];
      virtMem[page_table_number][1] = 0;
      virtMem[kick][0] = -1;
      virtMem[kick][1] = 0;
    }

	// Virtual Address Print Format
    if (page < 100) {
      printf("Virtual Address: %d     \t", page);
    }
    else if (page < 1000) {
      printf("Virtual Address: %d     \t", page);
    }
    else {
      printf("Virtual Address: %d     \t", page);
    }


	// This algorithm will calc the physical address.
    physAddr = (phsMem[virtMem[page_table_number][0]] * pageSize) + offset;

	// physAddr is an integer, so %d will take this as the next argument and print it as an integer
    printf("Physical Address: %d\n", physAddr);

	// increment pageNum.
    pageNum++;

    for(int i = 0; i < vmSize; i++){
      virtMem[i][1]++;
    }
  }

  // This algorithm will calculate the percentage of pageFaults and then use a printf statement to display it.
  // Since we have 1000 addresses, the number of PageFaults divided by the total number of addresses (1000), multiplied by 10
  // gets us our percentage.
  pageFR = (double) pgFC / 1000 * 100;

  // We will now display the percentage that we have calculated.
  printf("\nPage Fault Rate: %.2f%% \n", pageFR);

  // free will deallocate the memory that the pointer(line) currently takes.
  free(line);

  // fclose will close the stream (open file), which in this case is addressFile.
  fclose(addressFile);

  exit(EXIT_SUCCESS);

}
