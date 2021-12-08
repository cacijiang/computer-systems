File: readme.txt
Author: Caci Jiang
----------------------

implicit
--------
Design Decisions: 
For the implicit allocator, generally I decided to manipulate pointers 
based on headers instead of blocks, such as traversal. Because for headers, I can easily
know the block information such as block size and status. But, if the pointers manipulation
is based on blocks and I would like to know block information, I still need to go back to 
its header and get the stored information there.

For <mymalloc>, I decided to implement first-fit strategy in order to acommodate the 
allocation request from the clients. First-fit is fast, because the traversal is terminated
once it finds an appropriate free block. Although it will sacrifice the utilization,
especially when the requests are comlicated, I will fragement the block if possible to
create smaller partial free block, and I hope it could make up a bit in the utilization.
For best-fit search, I think it will impede the throughput a lot when there is a huge
number of requests.

For <myrealloc>, since it is not required to perform in-place re-allocation, I just
allocate a new block for the client's use and move data around. I could make use of
<myfree> and <myrelloc> to reduce the duplication of code.
--------

Overall Performance:
The average utilization of implicit allocator tested over all the scripts in the samples
folder is 71%, above 50%.

The real time of calling this program is around 6.95 seconds and reduced to 4.5 seconds 
after -o2 compilation, tested on all the scripts in samples folder. Average instructions 
count per call of <mymalloc> is 49 and reduced to 37 after -o2 comilation, tested on pattern
-repeat.script. Average instructions count per call of <myrealloc> is 26 and reduced to 25 
after -o2 compilation, tested on pattern-realloc.script. Average instructions count per 
request is 4,100 and reduced to 728 after -o2 compilation, tested on pattern-mixed.script. 
-o2 compilation optimizes significantly in terms of time and instructions count.
--------

explicit
--------
Design Decisions:
For struct, since a doubly linked list is needed, the list node is designed to denote the
header of free blocks, through which we can go to previous or next headers of free blocks.
Since I need to check the list often in different functions, I desined a head for the list.
This also determines my strategy when I would like to add new header node. If I do not have
enough information to add a new header node in place, I will add that node at the beginning
of the linked list. It can reduce a lot of efforts made in maintaining a list in the heap
address order. If I can add new node in place, which means I am replacing a node in the list,
adding it in place can restore the order based on location a bit.

For <mymalloc>, I re-use the idea of first-fit strategy from the implicit allocator. But this
time, I traverse over the linked list of header nodes of free blocks instead of all blocks.
Also, I fragment the block when possible to improve the utilization.

For <myfree>, if there is immediate right free block to coalesce, I will perform coalescing
and update the block information in place.

For <myrealloc>, I consider two cases, one for shrinking request and one for expanding
request. Obviously in-place re-allocation can be performed if it is a shrinking request. For
expanding request, I will check whether I can absorb right neighbors at first, after
confirming that, and then I continue absorbing neighbors. If I keep checking and absorbing
neighbors at the same time, it can be a bit messy.
--------

Overall Performance:
The average utilization of explicit allocator tested over all the scripts in the samples
folder is 89%, much higher than the implicit allocator's utilization, thanks to coalescing
and in place re-allocation.

The real time of calling this program is around 5.3 seconds and reduced to 4.0 seconds 
after -o2 compilation, tested on all the scripts in samples folder. Average instructions 
count per call of <mymalloc> is 50 and reduced to 28 after -o2 comilation, tested on pattern
-repeat.script. Average instructions count per call of <myrealloc> is 40 and reduced to 33
after -o2 compilation, tested on pattern-realloc.script. Average instructions count per 
request is 237 and reduced to 100 after -o2 compilation, tested on pattern-mixed.script. 
-o2 compilation optimizes significantly in terms of time and instructions count again.
Compared with implicit allocator, even though the lines of explicit allocator is greater than
those of implicit allocator, the instructions count of explicit allocator is optimized a lot.
-------

Anecdote: While I was writing explicit allocator, suddenly a vim swp warning appeared, and I
accidentally pressed [R]ecover. Then, the code of explicit.c went back to the state which was
a few hours ago. I had to re-type a lot of things!

Tell us about your quarter in CS107!
-----------------------------------
Even though lots of assignments made me suffer, I think the course experience is really great!


