#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_LEN 16

void pluto(int num) {
    printf("\n\n ++++ Welcome to pluto! (... how in the world did I get here?)\n", num + 207);
    exit(0);
}

void greet() {
    char buf[BUF_LEN];

    printf("What is your name? ");
    if (gets(buf)) {
           printf("Hello, %s! Buffer has space for %zu chars, your name is length %zu.\n",
                  buf, sizeof(buf), strlen(buf));
           if (strlen(buf) > sizeof(buf)) {
                printf("You have overrun the buffer by %d chars.\n",
                       strlen(buf) - sizeof(buf));
           }
    }
}

int main(int argc, char *argv[]) {
    printf("You've got 5 tries to blow this popsicle stand. Go!\n");
    for (int i = 0; i < 5; i++) {
        greet();
    }
    return 0;
}
