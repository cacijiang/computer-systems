# A simple makefile for building a program composed of C source files.
# Refer to CS107 guide to Make for background info on makefiles

PROGRAMS = trials copy isort

all:: $(PROGRAMS)

CC = gcc
# no-stack-protector because char buf used for tmp
CFLAGS = -Og -g3 -std=gnu99 -Wall $$warnflags $$nofortify
export nofortify = -fno-builtin -fno-stack-protector -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=0
export warnflags = -Wtype-limits -Wpointer-arith -Wlogical-op -Wshadow -Winit-self -fno-diagnostics-show-option
LDFLAGS =
LDLIBS =

$(PROGRAMS): %:%.c
	$(CC) $(CFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

trials: LDLIBS += -lm

clean::
	rm -f $(PROGRAMS) *.o callgrind.out.*

.PHONY: clean all 
