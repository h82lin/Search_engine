# Bug 1

## A) How is your program acting differently than you expect it to?
- In WriteIndex.cc, the program behaves unexpectedly when the WriteHashTable function is called with an empty chain in the bucket. It still attempts to write a record for the empty bucket, leading to unexpected behavior.

## B) Brainstorm a few possible causes of the bug
- The conditional statement that checks for an empty chain in the bucket may be incorrect or missing.
- There might be an issue with how the program handles empty chains or bucket records in the WriteHashTable function.
- It's possible that the bug is caused by a variable or pointer mismanagement related to the bucket chain.

## C) How you fixed the bug and why the fix was necessary
- To fix the bug, I added a conditional check within the WriteHashTable function to handle the case where the bucket's chain is empty. The fix involved updating the code logic to skip writing a record when the chain is empty, preventing unexpected behavior. This fix was necessary to ensure that the program correctly handles empty chains and avoids unnecessary operations for empty buckets.


# Bug 2

## A) How is your program acting differently than you expect it to?
- In WriteIndex.cc, the variable record_pos of the WriteHashTable function is not updated after writing the BucketRecord, leading to incorrect byte offsets when writing subsequent buckets.

## B) Brainstorm a few possible causes of the bug
- The code responsible for updating the record_pos variable might be missing or incorrectly implemented.
- There could be a logical error in the code that prevents the correct update of record_pos.
- The code calling the WriteHashTable function might be passing incorrect arguments, causing the record_pos variable to not update correctly.

## C) How you fixed the bug and why the fix was necessary
- To fix the bug, I added the necessary code to update the record_pos variable after writing the BucketRecord. This was done why updating record_pos by the number of bytes written by WriteHTBucket on every loop. The fix was necessary to prevent incorrect byte offsets.


# Bug 3

## A) How is your program acting differently than you expect it to?
- In WriteIndex.cc, an error value is returned when calling the WriteHeader function from the WriteIndex function.

## B) Brainstorm a few possible causes of the bug
- The method I used to implement the checksum calculation could be incorrect.
- I should probably using static_cast to cast &nextbyte instead of reinterpret_cast when casting the &nextbyte for fread.
- The datatype of nextbyte could be incorrect.

## C) How you fixed the bug and why the fix was necessary
- I fixed the bug by changing the datatype of nextbyte. Initially the datatype of nextbyte was int. I changed it to uint8_t. By changing the datatype to uint8_t, it ensures that 1 byte of data is read in each fread. This will allow the FoldByteIntoCRC function to perform the desired result since it accepts data in lengths of 1 byte.
