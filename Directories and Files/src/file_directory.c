/**
 ============================================================================
 Name        : Question 1
 Author      : Giuseppe Akbari
 Version     : 1
 Copyright   : Your copyright notice
 Description : This will deal with directories and files
 ============================================================================
 */
//these are standard C libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_STRING 1024

//this is for the mkdir function to create a directory
#include <sys/stat.h>

//this is for errors
#include <errno.h>

//this is for listing all subdirectories and files in a
//given directory
#include <dirent.h>

//this is for removing directories
#include <unistd.h>

/**
 * @param dir_name
 * @param mode
 * This function will create a directory with the given directory name
 * and will allow access with the give access passed in through mode
 */
void create_directory(const char *dir_name, mode_t mode) {
	//This will create a directory and check if it was successful
	if (mkdir(dir_name, mode) == -1) {
		//Thses are error messages to the user if something has gone wrong
		printf("Value of errno: %d\n", errno);
		perror("Error message");
	} else {
		printf("Directory created successfully\n");
	}
	return;
}
/**
 * @param file_name
 * @param content
 * This function will create a file and write the give content into the file
 */
void create_write_file(const char *file_name, const char *content) { //this is working
	FILE *file;
	file = fopen(file_name, "w");
	//Here we will check if the file was successfully opened and if not
	//will send error messages
	if (file == NULL) {
		printf("Value of errno: %d\n", errno);
		perror("Error message");
		return;
	}
	//here will print the content passed in by the user and close the file
	fprintf(file, content);
	fclose(file);
	printf("Success writing to file\n");
	return;
}
/**
 * @param file_name
 * this function will read through from
 * /proc/meminfo and write the contents into an output file
 * with the name passed in
 */
void read_proc_mem_info_to_file(const char *file_name) { //this work
	//opening and checking if there are any errors when opening the output file
	FILE *file_output = fopen(file_name, "w");
	if (file_output == NULL) {
		printf("Value of errno: %d\n", errno);
		perror("Error message");
		return;
	}
	//opening and checking for any errors for /proc/meminfo
	FILE *file_mem = fopen("/proc/meminfo", "r");
	if (file_mem == NULL) {
		printf("Value of errno: %d\n", errno);
		perror("Error message");
		fclose(file_output);
		return;
	}
	//reserve space in a string to write so we can write to a file
	char data[1000];
	//loop through the file, read the content and write it into the
	//output file
	while (fgets(data, 1000, file_mem) != NULL) {
		fprintf(file_output, data);
	}

	//closing the files
	fclose(file_output);
	fclose(file_mem);

	return;
}

/**
 * @param start_dir
 * This function will recursively go through a directory and list all
 * of the files, directories and their sub directories
 */
void directory_listing(const char *start_dir) {	//this works
	DIR *directory;	//for opening directory
	struct dirent *entry;	//type representing a directory stream
	directory = opendir(start_dir);	//open directory
	char full_name[1024];	//this will make space for a path name to print
	if (directory == NULL) {//here we will check if the directory opened successfully if not send an error and leave
		printf("Value of errno: %d\n", errno);
		perror("Error message");
		return;
	}
	while ((entry = readdir(directory)) != NULL) {
		//Here we will skip over the parent and current directory
		if (strcmp(entry->d_name, ".") == 0
				|| strcmp(entry->d_name, "..") == 0) {
			continue;
		}
		//This will build the name for the current file/directory
		//format 'Parent Directory'/'Current Directory/File'
		strcpy(full_name, start_dir);
		strcat(full_name, "/");
		strcat(full_name, entry->d_name);

		//The if statements will check if it is a file or directory
		//File/Directory: file/directory name
		//if it is a directory it will print the path name then it will call the function again and continue the process until there is no more sub directories
		if (entry->d_type == DT_REG) {		//then it is a file
			printf("File: %s\n", full_name);
		} else if (entry->d_type == DT_DIR) {		//then it is a directory
			printf("Directory: %s\n", full_name);
			directory_listing(full_name);
		} else {		//other
			printf("Other: %s\n", full_name);
		}

	}
	//checks if there are any errors when closing the directory
	if (closedir(directory) == -1) {
		perror("Error closing directory\n");
	}
	return;
}

/**
 * @param dir_name
 * This function will remove all of the content inside of a directory
 * including its sub directories
 * To enter this function a user must respond to a y/n confirming this is what they want
 */
void remove_directory(const char *dir_name) {	//this still does not work
	char full_name[1024];	//this is for building the path name
	DIR *dir;	//for opening a directory
	struct dirent *entry;	//a type representing a directory stream
	dir = opendir(dir_name);	// opens directory
	if (dir == NULL) {	//checks if the directory was successfully opened
		printf("Value of errno: %d\n", errno);
		perror("Error message");
		return;
	}
	//while there is still files and sub directories in the current directories continue
	while ((entry = readdir(dir)) != NULL) {
		if (strcmp(entry->d_name, ".") == 0
				|| strcmp(entry->d_name, "..") == 0) {//this will skip over naming the current and parent directories
			continue;
		}
		//this will build the path name in the form of:
		// 'Parent Directory'/'Current Directory/File'
		strcpy(full_name, dir_name);
		strcat(full_name, "/");
		strcat(full_name, entry->d_name);
		//this will check if it is a directory or file
		//if file remove else go into the directory and remove from their files and sub directories first
		if (entry->d_type == DT_REG) { //then file
			if (remove(full_name) != 0) {
				printf("Value of errno: %d\n", errno);
				perror("Error message");
			}
		} else if (entry->d_type == DT_DIR) { //then subdirectory
			if (rmdir(full_name) != 0) {
				printf("Value of errno: %d\n", errno);
				perror("Error message");
			} else {
				remove_directory(full_name);
			}
		}
	}
	//remove the current directory and check if any error has occurred
	if (rmdir(dir_name) != 0) {
		printf("Value of errno: %d\n", errno);
		perror("Error message");
	}

	return;
}
/***
 * This function will simply print a menu of option to chose from
 */
void menu() {
	printf("-----Menu-----\n");
	printf("0 -> to see the menu again\n");
	printf("1 -> to create a directory with a name of your choice\n");
	printf("2 -> to create and write content into a file\n");
	printf(
			"3 -> to Extract and save key information about system’s memory from /proc/meminfo to a file.\n");
	printf(
			"4 -> Recursively lists all files and subdirectories in a given directory and its subdirectories.to \n");
	printf(
			"5 -> to Removes a given directory and its subdirectories from the system.\n");
	printf("99 -> to leave\n");
	return;
}

/**
 * @return
 * will simply clear the input buffer for when using scanf
 */
void clear_input() {
	int c;
	while ((c = getchar()) != '\n' && c != EOF)
		;
	return;
}

/**
 * @return
 * look for more ways to check for specific errors
 * |
 * -> in the testing for user input
 * |
 * -> when making/deleting files/directories
 *
 */
int main() {
	int input; //will be for choosing menu options
	mode_t mode; //this will be for access mode when creating directories
	char name[MAX_STRING]; //will be for making file/directory names
	char content[MAX_STRING]; //this will be for writing content into a file
	char yn; //will be to confirm to delete a given directory
	menu();
	while (1) {
		printf("Enter your choice: \n");
		scanf("%d", &input);
		clear_input();
		if (input == 0) {
			menu();
		} else if (input == 1) {
			printf("Enter directory name:\n");
			fgets(name, MAX_STRING, stdin);
			name[strcspn(name, "\n")] = '\0';
			printf("Enter mode:\n");
			scanf("%o", &mode);
			clear_input();
			create_directory(name, mode);
		} else if (input == 2) {
			printf("Enter file name: \n");
			fgets(name, MAX_STRING, stdin);
			name[strcspn(name, "\n")] = '\0';
			create_write_file(name, "Operating systems is Fun !!!");
		} else if (input == 3) {
			printf("Enter output file name: \n");
			fgets(name, MAX_STRING, stdin);
			name[strcspn(name, "\n")] = '\0';
			read_proc_mem_info_to_file(name);
		} else if (input == 4) {
			printf("Enter directory name: \n");
			fgets(name, MAX_STRING, stdin);
			name[strcspn(name, "\n")] = '\0';
			directory_listing(name);
		} else if (input == 5) {
			printf(
					"Warning: you are about to delete the directory and all its contents recursively. Proceed? (y/n): \n");

			scanf(" %c", &yn);
			clear_input();
			if (yn == 'y' || yn == 'Y') {
				printf("Enter directory to delete: \n");
				fgets(name, MAX_STRING, stdin);
				name[strcspn(name, "\n")] = '\0';
				remove_directory(name);
			}

		} else if (input == 99) {
			printf("exiting...\n");
			break;
		} else {
			printf("please enter a number from the menu\n");
		}

	}
	return 0;
}

