File: readme.txt
Author: YOUR NAME HERE
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

