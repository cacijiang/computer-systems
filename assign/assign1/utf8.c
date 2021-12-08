#include "samples/prototypes.h"
#include <error.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>


/* Params: (unsigned short) code_point, (unsigned char) utf8_bytes[]
 * Return: (int) bytes written on utf8_bytes
 * This function considers three different ranges of code_points to encode
 * and fills out utf8_bytes byte by byte starting from index 0. For writing
 * bytes 2 and 3, provde a placeholder byte with leading bits and add the
 * corresponding significant bits of code point then.
 * */
int to_utf8(unsigned short code_point, unsigned char utf8_bytes[]) {
    if(code_point <= 0x007f) {
    	utf8_bytes[0] = code_point;
	    return 1;
    } else if(code_point <= 0x07ff) {
    	utf8_bytes[0] = 0xc0 | (code_point >> 6);
	    utf8_bytes[1] = 0x80 | (code_point & 0x3f);
	    return 2;
    }
    utf8_bytes[0] = 0xe0 | (code_point >> 12);
    utf8_bytes[1] = 0x80 | ((code_point >> 6) & 0x3f);
    utf8_bytes[2] = 0x80 | (code_point & 0x3f);
    return 3;
}




// ------- DO NOT EDIT ANY CODE BELOW THIS LINE (but do add comments!)  -------

void print_utf8(unsigned short code_point, unsigned char utf8_bytes[], int utf8_bytes_length) {
    int i;
    printf("U+%04X   UTF-8 Hex:", code_point);
    for (i = 0; i < utf8_bytes_length; i++) {
       printf(" %02x", utf8_bytes[i]);
    }
    for ( ; i < 4; i++) {
        printf("   ");
    }
    utf8_bytes[utf8_bytes_length] = '\0';
    printf("Character: %s  \n", utf8_bytes);  
}

unsigned short convert_arg(const char *str, unsigned short low, unsigned short high) {
    char *end;
    long n = strtol(str, &end, 0);
    if (*end != '\0') {
        error(1, 0, "Invalid code point '%s'", str);
    }
    if (n < low || n > high) {
        error(1, 0, "Code point %s not in range [%u, %u]", str, low, high);
    }
    return n;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        error(1, 0, "Missing argument. Please supply one or more unicode code points in decimal or hex.");
    }
    
    for (int i = 1; i < argc; i++) {
        unsigned short code_point = convert_arg(argv[i], 0, USHRT_MAX);
        unsigned char utf8_bytes[4];
        int num_utf8_bytes = to_utf8(code_point, utf8_bytes);
        print_utf8(code_point, utf8_bytes, num_utf8_bytes);  
    }
    return 0;
}
