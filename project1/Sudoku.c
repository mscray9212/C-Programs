/*
 * Author: Michael P. Scray
 * Assignment: Sudoku.c
 * Due date: 06/24/2015
 * Class: CSc 4320
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUMBER_OF_THREADS 		15 // we use 15 threads in this project
#define PUZZLE_SIZE 		    9  // it is a 9x9 puzzle
#define true 					1  // set bool value for true
#define false					0  // set bool value for false

void *column_worker(void *param); 	  /* thread that checks columns */
void *row_worker(void *param); 		  /* thread that checks rows */
void *subfield_worker(void *param);   /* thread that checks subfields */

/* example puzzle */

// this is a sample table, the values will be replaced by those read from the input file, except -1
int puzzle[PUZZLE_SIZE+1][PUZZLE_SIZE+1] = {
			{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
			{-1,5,3,4,6,7,8,9,1,2},
			{-1,6,7,2,1,9,5,3,4,8},
			{-1,1,9,8,3,4,2,5,6,7},
			{-1,8,5,9,7,6,1,4,2,3},
			{-1,4,2,6,8,5,3,7,9,1},
			{-1,7,1,3,9,2,4,8,5,6},
			{-1,9,6,1,5,3,7,2,8,4},
			{-1,2,8,7,4,1,9,6,3,5},
			{-1,3,4,5,2,8,6,1,7,9}
};

int status_map[NUMBER_OF_THREADS] = {0};

/* data structure for passing data to threads */
typedef struct
{
	int thread_id;
	int x;
	int y;
} parameters;

int main(int argc, char *argv[])
{
	//variable to reloop the program
	char con[1];
  	// get a puzzle from a txt file, which needs to be input from command line
	char filename[100];
	printf("Please enter your filename: ");
	scanf("%s",filename);
	//read user input file
  	FILE *myFile;
  	myFile = fopen(filename, "r");
  	//initialize variables i & j to use in for loop iterations
  	int i,j;
  	//process counter
  	int counter = 0;
  	//set to false if the file isn't a Sudoku
  	int truth = true;

  	if (myFile == NULL)
  	{
    	printf("Error Reading File\n");
    	exit (0);
  	}
  
  	for (i = 1; i < PUZZLE_SIZE + 1; i++)
  	{
		for (j =1; j < PUZZLE_SIZE + 1; j++)
		{
			fscanf(myFile, "%d,", &puzzle[i][j] );
		}

  	}

	pthread_t workers[NUMBER_OF_THREADS];
	parameters * data;

	//create the 3 threads for row check 
	//incremented by 3 to to produce 3
	for (i=1; i<PUZZLE_SIZE; i+=3) {
		//malloc reallocates so that nothing is overwritten
	    data=(parameters *)malloc(sizeof(parameters));
	    data->thread_id=counter;
	    data->x=1; 
	    data->y=i;
	    //create the thread
	    pthread_create(&workers[data->thread_id], 0, row_worker, data);
	    //increase the program counter
	    counter++;
	}
  
	//create the 3 threads for column check
	//incremented by 3 to produce 3
	for (i=1; i<PUZZLE_SIZE; i+=3) { 
		//malloc reallocates so that nothing is overwritten   
	    data=(parameters *)malloc(sizeof(parameters));
	    //load data for each thread through for loop iterations
	    data->thread_id=counter;
	    data->x=i; 
	    data->y=1;
	    //create the thread
	    pthread_create(&workers[data->thread_id], 0, column_worker, data);
	 	//increase the program counter
	 	counter++;
	}
  
	//create the 9 threads for the 9 subfields
	//variable i is to determine row, j is to determine column
	//incremented by 3 to produce 3
	for (i=1; i<PUZZLE_SIZE; i+=3) {
	    for (j=1; j<PUZZLE_SIZE; j+=3) {
	    	//malloc reallocates so that nothing is overwritten
		    data=(parameters *)malloc(sizeof(parameters));
		    //load data for each thread through for loop iterations
		   	data->thread_id=counter;
		   	data->x=i;
		   	data->y=j;
		   	//create the thread
		   	pthread_create(&workers[data->thread_id], 0, subfield_worker, data);
		   	//increase the program counter
		   	++counter;
	  	}
	}

	//get result
	for (i=0; i<NUMBER_OF_THREADS; i++){
    	pthread_join(workers[i], NULL);
  	}

	//compare result with flag status_map
	for (i=0; i<NUMBER_OF_THREADS; i++) {
    	if (status_map[i] == 0) { 
      		truth=false;
    	}
  	}
  
 	// print out results
 	if (truth==true) {
  		printf("File: %s is a Sudoku puzzle!\n", filename);
  	}
 	else {
  		printf("File: %s is not in Sudoku format.\n", filename);
  	}
  	// loop to rerun the program if the user isn't finished.

	return 0;
}


void *row_worker(void *params){
	parameters* p=(parameters *)params;

	//initialize variables i & j to use in for loop iterations
  	int i, j;
  	//initialize digits array
  	int digits[PUZZLE_SIZE + 1]={0};
  	//flag to determine if there is an error in analyzing digits array
  	int flag=1;

  	//loop to get rows
  	for ((i=p->y); (i<p->y+3); i++) {
    	for (j=1; j<PUZZLE_SIZE+1; j++) {
      		//increment digits array
      		digits[puzzle[j][i]]=digits[puzzle[j][i]]+1;
    	}    
  	}

	//ensure that the above loop executed correctly
	for (i=1; i<=PUZZLE_SIZE; i++) {
    	if (digits[i]!=3) {
      	flag=0;
      	break;
    	}
  	}
  	//update status_map
	if (flag) {
    	status_map[p->thread_id]=1;
  	}
  	//exit thread
  	pthread_exit(0);
}


void *column_worker(void *params){
	parameters* p = (parameters *)params;

	//initialize variables i & j to use in for loop iterations
  	int i, j; 
  	//initialize digits array
  	int digits[PUZZLE_SIZE+1]={0};
  	//flag to determine if there is an error in analyzing digits array
  	int flag=1;

  	//loop to get columns
	for (i=p->x; (i<p->x+3); i++) {
  		for (j=1; j<PUZZLE_SIZE+1; j++) {
    		//increment digits array
      		digits[puzzle[i][j]] = digits[puzzle[i][j]] + 1;
  		}
	}
  

	//ensure that the above loop executed correctly
  	for (i = 1; i <= PUZZLE_SIZE; i++) {
    	if (digits[i] != 3) {
      		flag = 0;
      		break;
    	}
  	}
  	//update status_map
  	if (flag) {
    	status_map[p->thread_id] = 1;
 	}
 	//exit thread
  	pthread_exit(0);
}


void *subfield_worker(void *params){
	parameters* p = (parameters *)params;

	//initialize variables i & j to use in for loop iterations
	int i, j;
	//initialize digits array
 	int digits[PUZZLE_SIZE + 1] = {0};
 	//flag to determine if there is an error in analyzing digits array
  	int flag =1;

  	//loop to get subgroups
 	for (i=p->x; (i<p->x+3); i++) {  	
    	for (j=p->y; (j<p->y+3); j++) {
      		digits[puzzle[i][j]] = 1;
  		}
 	}

 	//ensure that the above loop executed correctly
	for (i=1; i<PUZZLE_SIZE+1; i++) {
    	if (digits[i] == 0) {
   			flag = 0;
   			break;
  		}
 	}
 	//update status_map
 	if (flag) {
  		status_map[p->thread_id] = 1;
  	}
 	//exit thread
  	pthread_exit(0);
}