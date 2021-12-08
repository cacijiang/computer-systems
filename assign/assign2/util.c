/* CS 107 assign2
 * Code by Caci Jiang
 *
 * This program provides two helper functions, get_env_value and scan_token to mimic
 * the operations of "printenv" and "strtok", which will supply myprintenv.c and
 * tokenize.c. But for the operation of "strtok", this program will avoid the desgin
 * flaws of "strtok" by not modifying the original string.
 */

#include "samples/prototypes.h"
#include <string.h>

/* This function will take the environmental variables and optional user-specified
 * key as input, and then return the value of that environmental variable name of key.
 * If key is not specified or not found, the function will NULL. Otherwise, this function
 * search all the environmental variables and try to find whether the variable name
 * matches the key. If matched, return that environmental value.
 * */
const char *get_env_value(const char *envp[], const char *key) {
    int key_len = strlen(key);
    for(int i = 0; envp[i] != NULL; i++) {
    	int varname_len = strcspn(envp[i], "=");
	    // Make sure the strncmp will not be operated on garbage value at first and then
	    // compare variable name and key.
	    if(varname_len == key_len && strncmp(envp[i], key, varname_len) == 0) {
	        return envp[i] + varname_len + 1;
	    }
    }
    return NULL;
}

/* This function takes a pointer to string, a char array of delimiters, a buffer array to
 * write, and the length of the buffer array. This function aims to be called in a loop
 * until it finds all the tokens split by delimiters in the input string. At each round,
 * if no token is found, the The function will return false. If token is found, the function
 * will write it to buffer array and finally return true.
 */
bool scan_token(const char **p_input, const char *delimiters, char buf[], size_t buflen) {
    // Trim leading delimiters.
    *p_input += strspn(*p_input, delimiters);
    size_t token_len = strcspn(*p_input, delimiters);
    // There's no token can be added
    if(token_len == 0) return false;
    if(token_len < buflen) {
	    // Directly copy the token into buffer array
    	strncpy(buf, *p_input, token_len);
	    buf[token_len] = '\0';
	    *p_input += token_len;
    } else {
	    // Trim the exceeding part if the token is longer than the buffer array
    	strncpy(buf, *p_input, buflen - 1);
	    buf[buflen - 1] = '\0';
	    *p_input += (buflen - 1);
    }
    return true;
}
