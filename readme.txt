VIRTUAL MEMORY MANAGEMENT SIMULATOR Ethan Russell

This virtual memory management simulator is given read and write operations with virtual memory addresses via a file provided as an argument.  These virtual memory addresses are similar to the ones in a user program on an operating system.  It is up to the operating system to translate virtual addresses to physical memory even when there are multiple processes trying to use the same virtual addresses.  Furthermore, it is possible that the user program is trying to use more memory than the physical memory we have access to, so we need to know when to swap the memory out to disk.  VMM uses four different policies for deciding which bits of memory get swapped out to disk when these page faults happen: 
Random - choose any old frame.
FIFO - take the frame we accesed first
LRU - use the frame not used recently
CLOCK - look for a frame that has not been accesed.  If that's not possible, clear the reference bit for the pages we've looked through and use one of those if need be

VMM defaults to random page replacement, 4 frames, 8 pages, and a block size of 1024.  You can change any of these characteristics with these flags:
-f <number of frames>
-p <number of pages>
-h <block size>
-r <replacement policy> ("random", "lru", "fifo", or "clock")

Transcript:
xinu@xinu-develop-end:~/hwk8$ make clean
rm -f *.o *~ vmm*
xinu@xinu-develop-end:~/hwk8$ make
gcc -Wall hwk8.c paging.c vector.c -lm -o vmm
xinu@xinu-develop-end:~/hwk8$ ./vmm -r fifo -f 4 -p 8 in1.txt
Page Replacement Policy: FIFO
opened in1.txt
BEGIN WRITE
        Virtual Address: 0x00000000 (Page 0 + Offset 0)
        Page Fault: Page 0
        Next free frame: 0
        Physical Address: 0x00000000
END WRITE
BEGIN READ
        Virtual Address: 0x00000400 (Page 1 + Offset 0)
        Page Fault: Page 1
        Next free frame: 1
        Physical Address: 0x00000400
END READ
BEGIN WRITE
        Virtual Address: 0x00000800 (Page 2 + Offset 0)
        Page Fault: Page 2
        Next free frame: 2
        Physical Address: 0x00000800
END WRITE
BEGIN READ
        Virtual Address: 0x00000C00 (Page 3 + Offset 0)
        Page Fault: Page 3
        Next free frame: 3
        Physical Address: 0x00000C00
END READ
BEGIN WRITE
        Virtual Address: 0x00001000 (Page 4 + Offset 0)
        Page Fault: Page 4
        Page replacement: evicted page 0.  Writeout: 1
        Physical Address: 0x00000000
END WRITE
BEGIN READ
        Virtual Address: 0x00001400 (Page 5 + Offset 0)
        Page Fault: Page 5
        Page replacement: evicted page 1.  Writeout: 0
        Physical Address: 0x00000400
END READ
BEGIN WRITE
        Virtual Address: 0x00001800 (Page 6 + Offset 0)
        Page Fault: Page 6
        Page replacement: evicted page 2.  Writeout: 1
        Physical Address: 0x00000800
END WRITE
BEGIN READ
        Virtual Address: 0x00001C00 (Page 7 + Offset 0)
        Page Fault: Page 7
        Page replacement: evicted page 3.  Writeout: 0
        Physical Address: 0x00000C00
END READ
BEGIN WRITE
        Virtual Address: 0x00002000 (Page 8 + Offset 0)
        Segmentation Fault.  8 >= 8.
END WRITE
BEGIN READ
        Virtual Address: 0x00002400 (Page 9 + Offset 0)
        Segmentation Fault.  9 >= 8.
END READ
BEGIN WRITE
        Virtual Address: 0x00002800 (Page 10 + Offset 0)
        Segmentation Fault.  10 >= 8.
END WRITE
BEGIN READ
        Virtual Address: 0x00002C00 (Page 11 + Offset 0)
        Segmentation Fault.  11 >= 8.
END READ
BEGIN WRITE
        Virtual Address: 0x00003000 (Page 12 + Offset 0)
        Segmentation Fault.  12 >= 8.
END WRITE
BEGIN READ
        Virtual Address: 0x00003400 (Page 13 + Offset 0)
        Segmentation Fault.  13 >= 8.
END READ
BEGIN WRITE
        Virtual Address: 0x00003800 (Page 14 + Offset 0)
        Segmentation Fault.  14 >= 8.
END WRITE
BEGIN READ
        Virtual Address: 0x00003C00 (Page 15 + Offset 0)
        Segmentation Fault.  15 >= 8.
END READ
Frame   | F     Page
0       | 0     4
1       | 0     5
2       | 0     6
3       | 0     7
Page    | V     M       R       Frame
0       | 0     1       1       -
1       | 0     0       1       -
2       | 0     1       1       -
3       | 0     0       1       -
4       | 1     1       1       0
5       | 1     0       1       1
6       | 1     1       1       2
7       | 1     0       1       3
Paging statistics: 16 operations (50% read, 50% write), page faults: 8 (50%), replacements: 4 (25%)