# Bug 1

## A) How is your program acting differently than you expect it to?
- I am getting an error at line 271 in FileParser.c. This line is supposed to check whether there is a hash collision.

## B) Brainstorm a few possible causes of the bug
- The words that are stored in the WordPositions struct has been stored in the wrong format.
- Maybe I need to pass in the address of wp to kv.value instead of just the wp.
- The word string may not have been copied to newstr correctly.

## C) How you fixed the bug and why the fix was necessary
- I fixed the bug by increasing the number of bytes malloced to newstr. Then, copying the word to newstr, including the '\0' character. I did not pass in the '\0' before, resulting in the wrong copying of word to newstr. This was nessesarry because a string needs to have a '\0' at the end in order for it to identify as a string.


# Bug 2

## A) How is your program acting differently than you expect it to?
- There is an error in line 193 of CrawlFileTree.c. It is supposed to recursively loop through all the files and subfiles of a directory and mark each file as either a directory or not a directory.

## B) Brainstorm a few possible causes of the bug
- I might have understood the code wrong. Maybe I shouldn't be doing recursive calls to HandleDir here.
- Maybe what I should be doing here is just to mark each file as a directory or not a directory.
- I am putting the i integer in the wrong place. Integer i should be incremented inside of (S_ISREG(st.st_mode)) instead.

## C) How you fixed the bug and why the fix was necessary
- After asking the TA, it turned out the the code given to us already takes care of recursively iterating through each file and subfile in the directory. All we had to do was to mark whether each file is a directory or not. This was necessary because it gives useful information about the files so that the recursive iteration can handle files correctly.


# Bug 3

## A) How is your program acting differently than you expect it to?
- I am getting a segmentation fault in line 183 of MemIndex.c. This is wheere MemIndex_Search looks up the first query word.

## B) Brainstorm a few possible causes of the bug
- I may have passed in variables with the wrong datatype into HashTable_Find.
- Maybe HashTable_Find is not able to find the key in the hashtable.
- The key that was a result of the FNVHash64 hash function did not have the right datatype.

## C) How you fixed the bug and why the fix was necessary
- I fixed this error by adding an if statement to check if a key value with the inputed key to HashTable_Find could be found in the hash table. If not, return NULL. This was necessary because it would prevent having pointers such as wp access space that has not been allocated.
