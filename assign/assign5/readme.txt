File: readme.txt
Author: Liu (Caci) Jiang
----------------------

ATM
---
1a)

Defect description: In the withdraw function, the amount parameter is an unsigned integer.
When the program substracts the amount from current balance which is a signed integer of
that account, they are converted to unsigned integers as well as their subtraction result.
So, once the result is a negative value, as an unsigned integer, it can be really positively
big and pass the conditional check. As a result, the user still can withdraw cash.
-------------------

Test case explanation (put test case in custom_tests!): My test is "samples/atm 108 cacikong".
I try to withdraw $108 from my account whose initial balance should be $107. This operation
leaves my balance to be $-1, but I still can withdraw cash successfully.
-------------------------------------------------------

Recommended fix: Change line 117 of atm.c to "if ((int) cust->balance - amount >= MIN_BALANCE) {",
to cast the unsigned subtraction result to a signed integer.
----------------

1b)

Defect description: strcmp can modify the value stored by register %eax, which would
maintain the return value for function find_account. If the input username unmatches with all
the account names and is lexicographically greater than the last account name in the database,
strcmp will modify the value in %eax to be a positive value, as username at the first argument
of strcmp is greater than the last account name at the second argument. In that case, the
program will not go to authorization check, and %eax's value will not be modified in the rest
of function and finally returned. As a result, someone else's account can get withdrawn.
-------------------

Test case explanation (put test case in custom_tests!): My test is "samples/atm 40 zzz". "zzz"
is not a valid account name, so it will not match any account in the database. At the last
iteration indicated by i (%rbx) equals to 253, "zzz" still cannot match with the last account.
However, strcmp will assign a positive value to %eax, which is the comparison result of "zzz"
and last account name. That result in this case is 17, so instead, account #13 will be
withdrawn by $40.
-------------------------------------------------------

Recommended fix: Add a line after line 75, as a final return statement "return NOT_AUTHORIZED;"
for function find_account. So when the invalid username unmatches any account in the database,
even though it is lexicographically greater than the last account, the return value will be
negative anyway, since our added return statement will maintain %eax's value to be negative at
last for this case.
----------------

1c)

Defect description: Under the gdb mode, it is found that the stack allocates 0x40 for the variables
in read_secret_passcode, with 8 more bytes for pushed %rbx. However, if the account name as the
filename here is too long, it can run out of allocated stack space here and even overwrite the next
instruction address for function lookup_by_number, which is supposed to be 0x400be7. Normally, after
read_secret_passcode is returned, lookup_by_number will go through a passcode check with the passcode
file. However, if the account name is very long, 0x400be7 can overwritten as 0x400c0c, which directly
jump to the return statement.
-------------------

Test case explanation (put test case in custom_tests!): My test is
"samples/atm 300 `perl -e 'print "a"x31,"\x10\x0c\x40"'` 0". Firstly find how many bytes are below
the address 0x400be7 in the stack before we finish the call of read_secret_passcode. To find the
attack test, the account name along with the bank data directory and code variable should use up
that stack space and even overwrite the address 0x400be7 to 0x400c0c. In the test case, last "0" is
just a random passcode. "\x10\x0c\x40" is to use up code variable space and overwrite the address
pushed in the stack. "a"x31 is to use up 31 bytes stack space, where 31 = 0x40 + 8 (pushed %rbx) -
41 (length of band data directory).
-------------------------------------------------------

Recommended fix: Before initializing the path array in function read_secret_passcode, we can get
the string length of BANK_DATA_DIR and input filename at first and then declare the path array
having the sufficient length to hold these two strings, to avoid overwriting other information
in the stack.

----------------

Bomb
----
2a) Under gdb mode, I set a breakpoint at the address of function explode_bomb, i.e. b *0x40160c.
So, every time gdb stops at this breakpoint, I will know the bomb is going to explode. Before gdb
continues on this breakpoint, I will kill the program.

2b) 40199a:	48 89 c6             	mov    %rax,%rsi
    40199d:	bf f0 2b 40 00       	mov    $0x402bf0,%edi
    4019a2:	e8 59 f6 ff ff       	callq  401000 <strcmp@plt>

So, indicated by the instruction at 0x40199d, %edi is assigned with the string stored at address
0x402bf0, which is "Debugging is anticipated with distaste, performed with reluctance, and bragged
about forever." %edi will be 2nd argument of strcmp, and 1st line of input stored at %rax will be 
1st argument of strcmp. When %rax and %edi stores equal string, the program will jump and skip the
explode_bomb function. Since %edi is a pointer to characters which consumes 8 bytes, we only need 
32-bit register instead of full 64-bit register (%rdi).

2c) Right before jle, there is "xor %ebx, %ebp", where %ebx stores the integer before the space in
the 2nd line of input and %ebp stores the integer after the space in the input line. XOR can set
carry and overflow flags to zero. Here, jle will jump and skip the right following explode_bomb when
zero flag is on, or sign flag is not equal to overflow flag. For my defused input, XOR sets the
overflow flag to 0, while the sign flag is on since the XOR result is negative. So, jle will be true
and perform jump.

2d) The while loop's logic of winky function can be represented as following pseudocode:
    counter = 6
    while (true) {
        if (strcmp(word, input_word + 1) == 0) {
            exit the while loop
        }
        counter -= strspn(word, first_char_of_input_word)
        set word to be the next word in the wordlist
    }
    return counter

Function winky will read 64 words from the wordlist, from line 67301 to line 67364. To exit the
while loop, the substring of the input word starting from the 2nd character must match one of the 64
words. But before that, the counter variable, initially to be 6, which will be returned by function
winky to level_3 must be 0 at the end. counter can be reduced on the condition that current word is
not matched with the substring AND the initial part of current word contains the first character of
input word. I calculated the occurrence of the first character for the 64 words and found 's' has an
occurrence of 6, although there can be other characters as the first character having occurrence
greater or equal than 6 as well. In my case, the first character of my input word is s, followed by
"murre" which is the word right after the 6-th word starting as 's'.

2e) 0x8(%rsp) is a double pointer of char, because it is used as the 2nd argument of strtoul. By
refering to the manual of stroul, 0x8(%rsp) should be char **endptr. Since the first argument is
the 4-th input line string, it has 8 bytes as a char *. That is why the endptr is pointed to 8 bytes
after the string we would like to read and convert using strtoul. When the strtoul reads any non-
digit character (which is a white space in my case) or EOF, strtoul will store the first invalid
character to endptr. For example, my 4-th input line is:
"0x10001 0x20000 0x10000 0xffff 0x7cc5 0x100 0x8 0x1", which is the first argument of strtoul. After
calling strtoul, 0x8(%rsp) will become " 0x20000 0x10000 0xffff 0x7cc5 0x100 0x8 0x1", for the ease
of reading next hex value in the string.

So, in the while loop, each time, we will read a hex value and store it to an integer array. At each
iteration, 0x8(%rsp) will be set to the substring where there is a white space before next hex value
until it reaches the end of the input line string. The while loop must go through at least 8
iterations at most 19 iterations.

2f) mycmp is used as function pointer in level_4 to serve as the compare function, which compares the
hex values of our array. It makes use of a function called <jzpfqvy> to compute the return value given
the void pointers to the two signed integers. Essentially, <jzpfqvy> computes how many bits we need to
left shift to make an integer negative, although it assigns special value for zero case. So, essentially
mycmp compares the number of significant bits of the two integers in a reverse order, which means that
it will make the integer having larger number of significant bits appear first. In function check_sorted
Any two consecutive integers cannot be the same, otherwise it will also trigger the bomb explosion. 
Overall, starting from the 2nd element of our input array, the value as well as the number of significant
bits of the integers should decrease.

Later, mycmp is also used in find1 and find2 to perform binary search. It will find the pointer to a 
signed integer in the array if exists, which has the same number of significant bits as the one pointed
by the key, although the values of these two integers do not need to be the same.

