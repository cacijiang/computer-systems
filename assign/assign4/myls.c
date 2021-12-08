/* CS 107 assign4
 * Code by Caci Jiang
 *
 * This program is designed to have similar operations of ls, which is to print the contents
 * in a directory line by line. It supports two command-line flags. One is "-a" to include
 * hidden files. Another is "-z" to print the list sorted by type (directories ahead of
 * non-directories), and same types will be sorted by name. If no command-line flags, the
 * program prints contents without hidden files, which are sorted by name.
 */

#include <dirent.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <error.h>

enum { SORT_BY_NAME, SORT_BY_TYPE };
enum { EXCLUDE_DOT, INCLUDE_DOT };

/* On the myth filesystem, the only file type information that is accurate is
 * directory/not-directory used here. Other type info in struct dirent is 
 * not reliable.
 */

typedef int (*filter_fn) (const struct dirent *);
typedef int (*cmp_fn) (const struct dirent **, const struct dirent **);

/* This function accepts dirent structure pinter as input. It returns true if the dirent is a
 * directory and false if it is a non-directory.
 */
bool is_dir(const struct dirent *dirptr) {
    return dirptr->d_type == DT_DIR;
}

/* This is a filter function which accepts a dirent pointer. When there is no "-a" flag when
 * calling the program, this function will return 1 if the input is not a hiddent file and 0
 * for hidden file.
 */
int filter_dot(const struct dirent *p) {
  return (int) p->d_name[0] != '.';
}

/* This is a compare function which accepts two double pointers to dirent structures to sort
 * dirent names in alphabetical order.
 */
int compare_name(const struct dirent **p, const struct dirent **q) {
    return strcmp((*p)->d_name, (*q)->d_name);
}

/* This is a compare function which accepts two double pointers to dirent structures to sort
 * the dirents according to their type (directories ahead of non-directories). Entries of the 
 * same type are sorted by name.
 */
int compare_type(const struct dirent **p, const struct dirent **q) {
    bool is_p_dir = is_dir(*p);
    bool is_q_dir = is_dir(*q);
    if (is_p_dir == is_q_dir) {
        return compare_name(p, q);
    } else {
        return (int) is_q_dir - (int) is_p_dir;
    }
}

/* This function accepts the pointer to the directory path, filter type and the sorting order
 * type as parameters. Firstly it will check whether the directory path is valid. If it is
 * valid, then it will choose the corresponding filter and compare functions. Next, scandir
 * is used to store the contents into a heap-allocated array, sorted by the compare function.
 * Lastly, the dirent names are printed line by line, and all heap-allocated memoeries are
 * freed.
 */
void ls(const char *dirpath, int filter, int order) {
    // Test whether the directory path is accessible
    if (access(dirpath, R_OK | F_OK)) {
        error(0, 0, "cannot access %s\n", dirpath);
        return;
    }
    struct dirent **namelist;
    filter_fn my_filter = (filter == INCLUDE_DOT) ? NULL : filter_dot;
    cmp_fn my_compare = (order == SORT_BY_NAME) ? compare_name : compare_type;
    int n = scandir(dirpath, &namelist, my_filter, my_compare);
    if(n < 0) {
        perror("scandir");
    } else {
        for(int i = 0; i < n; i++) {
            // Add a trailing slash if it is a directory
            is_dir(namelist[i]) ? printf("%s/\n", namelist[i]->d_name) : printf("%s\n", namelist[i]->d_name);
            free(namelist[i]);
        }
        free(namelist);
    }
}

/* This is the main functions of the program. The user can input the directory path and
 * optional command-line flags which are "a" and "z". "a" is to determine the filter function
 * whether to include the hidden files, and "z" is to determine the compare function, whether
 * it is sorted by directory names or types. Then it calls ls function to sort and print the
 * directory names.
 */
int main(int argc, char *argv[]) {
    int order = SORT_BY_NAME;
    int filter = EXCLUDE_DOT;

    int opt = getopt(argc, argv, "az");
    while (opt != -1) {
        if (opt == 'a') {
            filter = INCLUDE_DOT;
        } else if (opt == 'z') {
            order = SORT_BY_TYPE;
        } else {
            return 1;
        }

        opt = getopt(argc, argv, "az");
    }
    
    if (optind < argc - 1) {
        for (int i = optind; i < argc; i++) {
            printf("%s:\n", argv[i]);
            ls(argv[i], filter, order);
            printf("\n");
        }
    } else {
        ls(optind == argc - 1 ? argv[optind] : ".", filter, order);
    }
    
    return 0;
}
