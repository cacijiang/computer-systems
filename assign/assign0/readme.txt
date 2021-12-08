File: readme.txt
Assignment: assign0
Author: <Liu Jiang>
Pronouns: <she/her/hers>
Current Timezone: <Pacific Standard Time>
----------------------

0. Tell us about yourself!  What do you do for fun?  Tell a quick anecdote about something that you feel makes you unique; a talent, experience, anything.
I enjoy playing table tennis. I followed professional training on table tennis for 9 years before.


1. Please initial below to confirm your course enrollment.

[x] I have completed the Honor Code quiz on the assign0 page, and I have read and understood the CS107 collaboration policy at https://cs107.stanford.edu/collaboration.html. By adding my initials, I affirm my intention to abide by this policy.

INTRUDER DETECTION ACTIVITY
2a) The username of the intruder is "mattv". Firstly I use "cd" to go to "home" directory which contains all the users. Secondly, to find the intruder, I performed this bulk of commands: for d in *; do if ! grep -q $d ../users.list; then echo $d; fi; done. The idea here is to iterate all users in "home" directory and check whether it's contained in "users.list". So, I used a for-loop to iterate all user directories, which is "for d in *; do ...; done". Here, "d" is the variable to denote the directory names inside "home". Inside the for-loop, I used a conditional statement, to print the intruder name if that name cannot be found in the user list. The command structure for conditional statement is: if ...; then ..; fi. Inside this conditional statement, in order to find whether a file contains a string, I used "grep -q", where "-q" is to suppress the output of "grep". To print the intruder name, I used "echo" command.

2b) The file is .bash_history, and it contains a record of commands that intruder has issued during his/her session. Under the intruder's home directory, there should a "work" directory, which has two sub-directories, "project" and "server". The intruder frequenly edited and run C programs in "project" and "server". The intruder also tried to find the port inside "config.h" under "server" and might modify it. The intruder changed the password in "project/etc" and copy "etc" folder in his or her USB. Finally, the intruder run the program by pretending to be another user (usually the root user). To get the answer, I used "cd" to go to the intruder's home directory. Then I used "ls -a" to list all the files or directories including hidden files. Then, "cat .bash_history" is used to print the content of ".bash_history".

2c) The intruder used "sudo dmesg", "sudo emacs /etc/passwd", "sudo cp -r /etc /mnt/usb/etc", and "sudo su". To get the answer, I used "cd" to go to the intruder's home directory. Then my command is: grep "sudo" .bash_history, to search all the lines in ".bash_history" containing "sudo". 

