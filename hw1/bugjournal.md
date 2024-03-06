# Bug 1

## A) How is your program acting differently than you expect it to?
- I am getting a segmentation fault in line 155 of the HashTable_Insert function, 
which is supposed to assign a payload value in a node to a new value.

## B) Brainstorm a few possible causes of the bug
- I think the_payload->value access a memory location that is not allowed.
- the_payload variable may to passed into the SearchKey function incorrectly
- SearchKey function may be accepting the_payload variable incorrectly

## C) How you fixed the bug and why the fix was necessary
- I changed the HTKeyValue_t input of the SearchKey function as a double pointer
of type HTKeyValue_t instead of a single pointer. Basically is changed SearchKey(LLIterator \*lli, HTKey_t key, HTKeyValue_t\* keyvalue) to SearchKey(LLIterator \*lli, HTKey_t key, HTKeyValue_t\*\* keyvalue). This is because, I was passing in the address of a pointer. Thus, a double pointer is need to store the address of the pointer.


# Bug 2

## A) How is your program acting differently than you expect it to?
- I am getting "true" as the return value for HTIterator_IsValid(it) in line 252 of 
test_hashtable.cc, whereas it is supposed to return "false".

## B) Brainstorm a few possible causes of the bug
- The logic for HTIterator_Next function is wrong, maybe my interpretation of the
functionality of this function is incorrect.
- The HashTable_Insert may have bugs, resulting in a seemingly incorrect number of
key value pairs inserted into the hashtable.
- The HTIterator_IsValid function might be doing wrong evaluations in its if 
statement in checking if the iterator it it pointing at is a valid element

## C) How you fixed the bug and why the fix was necessary
- The HTIterator_IsValid function was indeed doing the wrong evaluation. The if
statement contained the HTIterator_Next() function that was to check if the next node
was valid. However, it would also increment the node that its pointing to. Basically I changed  if (!HTIterator_IsValid(iter) || !LLIterator_IsValid(iter->bucket_it)) || !HTIterator_Next(iter) to  if (iter->ht->num_elements == 0 || !LLIterator_IsValid(iter->bucket_it)) .Thus, the fix was to remove the HTIterator_Next() function.


# Bug 3

## A) How is your program acting differently than you expect it to?
- I got a segmentation fault at line 299 of the LinkedList_Slice function. Where the function is supposed to pop off the last node of the input linked list.

## B) Brainstorm a few possible causes of the bug
- The assignment of the payload_ptr may be done incorrectly, it may have accessed assigned the payload value to the memory of payload_ptr instead of the payload_ptr itself.
- I might need to evaluate the case when there are zero elements in the linked list.
- I might be deferencing a NULL value somewhere when I am deferenceing nodes.

## C) How you fixed the bug and why the fix was necessary
- I was missing the case when there are zero elements in the linked list. I had to do this because empty linked list can be passed in. If that's the case, then my code would be dereferencing pointers becuase it is trying to access nodes that are just NULL. Basically I added an additional if statement, if (list->num_elements == 0) that would return false if evaluated true.
