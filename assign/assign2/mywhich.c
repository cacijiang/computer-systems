/* CS 107 assign2
 * Code by Caci Jiang
 *
 * This program mimics the operation of "which", which searches for a command
 * by name and reports its full path for its executable file.
 *
 * If there is no additional command-line arguments, the program will print all
 * the paths in PATH environmental variable line by line. If there are additional
 * command-line arguments as executables, the program prints out the full path for
 * each executable.
 */
#include "samples/prototypes.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/* Function: main
 * ----------------------------
 * This function takes environmental variables and user's optional executable(s) as
 * input, and as a main function, it returns 0 successfully to exit the program. It
 * allows the user to change the environmental path to MYPATH, then the function will
 * search executable only in MYPATH. Otherwise, the default search path is all the paths
 * in environmental variable PATH. If no executable is specified, the function will print
 * out all the environmental paths. If there is executable specified, it will search
 * all possible environmental paths until finds the right executable path and then print
 * the full path. If nothing matched for specified executable, the function prints nothing.
 */
int main(int argc, char *argv[], const char *envp[]) {
    const char *searchpath = get_env_value(envp, "MYPATH");
    // Set the search path as default environmental variable PATH if MYPATH is not valid.
    if (searchpath == NULL) {
        searchpath = get_env_value(envp, "PATH");
    }
    
    // Print out all paths in environmental variable PATH.
    if (argc == 1) {
        char dir[PATH_MAX]; // PATH_MAX is the system's maximum length of a full path
        const char *remaining = searchpath;

	    // User scan_token to get each path split by ":".
        printf("Directories in search path:\n");
        while (scan_token(&remaining, ":", dir, sizeof(dir))) {
            printf("%s\n", dir);
        }
    }

    // Find full path for the executable input by user.
    if(argc > 1) {
	    // Iterate over each executable to find the corresponding path
    	for(int i = 1; i < argc; i++) {
	        char dir[PATH_MAX];
	        const char *remaining = searchpath;

	        while(scan_token(&remaining, ":", dir, sizeof(dir))) {
		        char fullpath[PATH_MAX];
		        strcpy(fullpath, dir);
		        // Concatenate the full path by adding the executable file name
	    	    strcat(fullpath, "/");
		        strcat(fullpath, argv[i]);
		        // Check whether the full path is valid under both R_OK and X_OK modes
		        if(!access(fullpath, R_OK | X_OK)) {
		            printf("%s\n", fullpath);
		            break; // exit the while-loop once a valid path is found
		        }
	        }
	    }
    }
    
    return 0;
}
