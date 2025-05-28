/**
 * -------------------------------------
 * @file  file_copy.c
 * file description
 * -------------------------------------
 * @author Giuseppe Akbari, Giuseppemuia2005@gmail.com
 *
 * @version 2025-05-25
 *
 * -------------------------------------
 */
#include <stdio.h>
#include <stdlib.h>
//this is for creat(), open()
#include <fcntl.h>
//this gives me access to: write(), read(), access(), close() and _exit()
#include <unistd.h>
//this is for creat(),
#include <sys/stat.h>

//this is for catching and printing error messages
#include <errno.h>
//this will be a constant, which will be the size of what I am reading from one file to another
#define BUF_SIZE 1024

int main(int argc, char *argv[]) {
	//first checks if the right amount of parameters were passed in
	if (argc != 3) { //if not exit
		printf("Insufficient parameters passed."); //
		_exit(0);
	}
	//opens the file which will return -1 if there was an error
	int input_file = open(argv[1], O_RDONLY);
	if (input_file == -1) {
		printf("Value of errno: %d\n", errno);
		perror("Error message");
		_exit(0);
	}
	//first we will check if the file already exits before trying to create a new one
	if (access(argv[2], F_OK) == 0) {	//this checks if the file already exists
		//access will return 0 if the file exists and now we need to delete it
		int removed = remove(argv[2]);		//deletes the file
		if (removed == -1) {		//checks if error when removing file
			printf("Value of errno: %d\n", errno);
			perror("Error message");
			_exit(0);
		}
	}
	//creates the output file with read, executable and write permissions
	//int output_file = creat(argv[2], S_IRUSR | S_IWUSR | S_IXUSR | S_IRWXO);
	int output_file = open(argv[2], O_WRONLY|O_CREAT|O_TRUNC, 0666);

	if (output_file == -1) {		//checks if error creating file
		//error creating file
		printf("Value of errno: %d\n", errno);
		perror("Error message");
		_exit(0);
	}
	char buf[BUF_SIZE];	//this is for copying and writing contents from input to output files

	int wri;	//for return value of the write function
	//read will return the number of bytes read
	//0 will indicate the end of file
	ssize_t numRead = read(input_file, buf, BUF_SIZE - 1);
	if (numRead == -1) {	//check for error when reading file
		printf("Value of errno: %d\n", errno);
		perror("Error message");
		_exit(0);
	}
	while (numRead > 0) {//if numRead is less than or equal to 0 then either end of file or error
		buf[numRead] = '\0';	//set the end of the line to the null terminator
		wri = write(output_file, buf, BUF_SIZE);	//writes to the output file
		if (wri == -1) {	//if wri is -1 then error writing to file
			printf("Value of errno: %d\n", errno);
			perror("Error message");
			_exit(0);
		}
		numRead = read(input_file, buf, BUF_SIZE - 1);
		if (numRead == -1) {	//if numRead is -1 then error reading from file
			printf("Value of errno: %d\n", errno);
			perror("Error message");
			_exit(0);
		}
	}
	if (close(input_file) == -1) {	//checks if error in closing input file
		printf("Value of errno: %d\n", errno);
		perror("Error message");
		if (close(output_file) == -1) {	//then checks if there is an error closing output file
			printf("Value of errno: %d\n", errno);
			perror("Error message");
			_exit(0);
		}
		_exit(0);
	}
	if (close(output_file) == -1) {	//check if there is an error closing output file
		printf("Value of errno: %d\n", errno);
		perror("Error message");
		_exit(0);
	}
	printf("The contents of file '%s' has been successfully copied into '%s' file\n",argv[1] ,argv[2] );
	return 0;
}

















