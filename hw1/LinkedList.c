/*
 * Copyright ©2023 Chris Thachuk.  All rights reserved.  Permission is
 * hereby granted to students registered for University of Washington
 * CSE 333 for use solely during Winter Quarter 2023 for purposes of
 * the course.  No other use, copying, distribution, or modification
 * is permitted without prior written consent. Copyrights for
 * third-party components of this work must be honored.  Instructors
 * interested in reusing these course materials should contact the
 * author.
 */

#include <stdio.h>
#include <stdlib.h>

#include "CSE333.h"
#include "LinkedList.h"
#include "LinkedList_priv.h"


///////////////////////////////////////////////////////////////////////////////
// LinkedList implementation.

LinkedList* LinkedList_Allocate(void) {
  // Allocate the linked list record.
  LinkedList *ll = (LinkedList *) malloc(sizeof(LinkedList));
  Verify333(ll != NULL);

  // STEP 1: initialize the newly allocated record structure.
  ll->num_elements = 0;
  ll->head = NULL;
  ll->tail = NULL;
  // Return our newly minted linked list.
  return ll;
}

void LinkedList_Free(LinkedList *list,
                     LLPayloadFreeFnPtr payload_free_function) {
  Verify333(list != NULL);
  Verify333(payload_free_function != NULL);

  // STEP 2: sweep through the list and free all of the nodes' payloads
  // (using the payload_free_function supplied as an argument) and
  // the nodes themselves.
  LinkedListNode* lln_head = list->head;
  LinkedListNode* traverse = lln_head;
  LinkedListNode* temp;
  while (traverse != NULL) {  // Traverse the linkedlist
    payload_free_function(traverse->payload);
    temp = traverse;
    traverse = traverse->next;
    free(temp);
  }

  free(list);
}

int LinkedList_NumElements(LinkedList *list) {
  Verify333(list != NULL);
  return list->num_elements;
}

void LinkedList_Push(LinkedList *list, LLPayload_t payload) {
  Verify333(list != NULL);

  // Allocate space for the new node.
  LinkedListNode *ln = (LinkedListNode *) malloc(sizeof(LinkedListNode));
  Verify333(ln != NULL);

  // Set the payload
  ln->payload = payload;

  if (list->num_elements == 0) {
    // Degenerate case; list is currently empty
    Verify333(list->head == NULL);
    Verify333(list->tail == NULL);
    ln->next = ln->prev = NULL;
    list->head = list->tail = ln;
    list->num_elements = 1;
  } else {
    // STEP 3: typical case; list has >=1 elements
    ln->payload = payload;
    ln->next =  list->head;
    ln->prev = NULL;
    list->head->prev = ln;
    list->head = ln;
    list->num_elements += 1;
  }
}

bool LinkedList_Pop(LinkedList *list, LLPayload_t *payload_ptr) {
  Verify333(payload_ptr != NULL);
  Verify333(list != NULL);

  // STEP 4: implement LinkedList_Pop.  Make sure you test for
  // and empty list and fail.  If the list is non-empty, there
  // are two cases to consider: (a) a list with a single element in it
  // and (b) the general case of a list with >=2 elements in it.
  // Be sure to call free() to deallocate the memory that was
  // previously allocated by LinkedList_Push().
  if (list->num_elements == 0) {  // Case when the linkedlist is empty
    return false;
  } else if (list->num_elements == 1) {
    *payload_ptr = list->head->payload;  // Assign the payload_ptr
    free(list->head);
    list->head = NULL;
    list->tail = NULL;
    list->num_elements = 0;
  } else {
    *payload_ptr = list->head->payload;  // Assign the payload_ptr
    LinkedListNode* temp = list->head;
    list->head = list->head->next;
    list->head->prev = NULL;
    free(temp);
    temp = NULL;
    list->num_elements -= 1;
  }
  return true;
}

void LinkedList_Append(LinkedList *list, LLPayload_t payload) {
  Verify333(list != NULL);

  // STEP 5: implement LinkedList_Append.  It's kind of like
  // LinkedList_Push, but obviously you need to add to the end
  // instead of the beginning.

  // Malloc new node to append to the linked list
  LinkedListNode *new_node = (LinkedListNode *) malloc(sizeof(LinkedListNode));
  new_node->payload = payload;
  new_node->next = NULL;

  if (list->head == NULL) {  // Case when the linkedlist is empty
    list->head = new_node;
    list->head->prev = NULL;
  } else {
    LinkedListNode *tail_node = list->tail;
    tail_node->next = new_node;
    new_node->prev = tail_node;
  }
  list->tail = new_node;
  list->num_elements += 1;
}

void LinkedList_Sort(LinkedList *list, bool ascending,
                     LLPayloadComparatorFnPtr comparator_function) {
  Verify333(list != NULL);
  if (list->num_elements < 2) {
    // No sorting needed.
    return;
  }

  // We'll implement bubblesort! Nnice and easy, and nice and slow :)
  int swapped;
  do {
    LinkedListNode *curnode;

    swapped = 0;
    curnode = list->head;
    while (curnode->next != NULL) {
      int compare_result = comparator_function(curnode->payload,
                                               curnode->next->payload);
      if (ascending) {
        compare_result *= -1;
      }
      if (compare_result < 0) {
        // Bubble-swap the payloads.
        LLPayload_t tmp;
        tmp = curnode->payload;
        curnode->payload = curnode->next->payload;
        curnode->next->payload = tmp;
        swapped = 1;
      }
      curnode = curnode->next;
    }
  } while (swapped);
}


///////////////////////////////////////////////////////////////////////////////
// LLIterator implementation.

LLIterator* LLIterator_Allocate(LinkedList *list) {
  Verify333(list != NULL);

  // OK, let's manufacture an iterator.
  LLIterator *li = (LLIterator *) malloc(sizeof(LLIterator));
  Verify333(li != NULL);

  // Set up the iterator.
  li->list = list;
  li->node = list->head;

  return li;
}

void LLIterator_Free(LLIterator *iter) {
  Verify333(iter != NULL);
  free(iter);
}

bool LLIterator_IsValid(LLIterator *iter) {
  Verify333(iter != NULL);
  Verify333(iter->list != NULL);

  return (iter->node != NULL);
}

bool LLIterator_Next(LLIterator *iter) {
  Verify333(iter != NULL);
  Verify333(iter->list != NULL);
  Verify333(iter->node != NULL);

  // STEP 6: try to advance iterator to the next node and return true if
  // you succeed, false otherwise
  // Note that if the iterator is already at the last node,
  // you should move the iterator past the end of the list

  // Case when the iterator is pointing to the last node of the linked list
  if (iter->node->next == NULL) {
    iter->node = iter->node->next;
    return false;

  } else {
    iter->node = iter->node->next;
    return true;
  }
}

void LLIterator_Get(LLIterator *iter, LLPayload_t *payload) {
  Verify333(iter != NULL);
  Verify333(iter->list != NULL);
  Verify333(iter->node != NULL);

  *payload = iter->node->payload;
}

bool LLIterator_Remove(LLIterator *iter,
                       LLPayloadFreeFnPtr payload_free_function) {
  Verify333(iter != NULL);
  Verify333(iter->list != NULL);
  Verify333(iter->node != NULL);

  // STEP 7: implement LLIterator_Remove.  This is the most
  // complex function you'll build.  There are several cases
  // to consider:
  // - degenerate case: the list becomes empty after deleting.
  // - degenerate case: iter points at head
  // - degenerate case: iter points at tail
  // - fully general case: iter points in the middle of a list,
  //                       and you have to "splice".
  //
  // Be sure to call the payload_free_function to free the payload
  // the iterator is pointing to, and also free any LinkedList
  // data structure element as appropriate.


  // Cases that consider when there is only one node in the linked list,
  // when the iterator points to the head of the linked list, when the
  // iterator points to the tail of the linked list, and when the
  // iterator points to the middle of the linked list.
  // LinkedList_Pop is used to remove the first node of a linked list
  // and LinkedList_Slice is used to remove the last node of a linked list
  if (iter->list->head == iter->node && iter->list->num_elements == 1) {
    LLPayload_t the_payload;
    iter->node = NULL;
    LinkedList_Pop(iter->list, &the_payload);
    payload_free_function(the_payload);
    return false;
  } else if (iter->node == iter->list->head) {
    LLPayload_t the_payload;
    LinkedList_Pop(iter->list, &the_payload);
    payload_free_function(the_payload);
    iter->node = iter->list->head;
  } else if (iter->node == iter->list->tail) {
    LLPayload_t the_payload;
    iter->node = iter->node->prev;
    LinkedList_Slice(iter->list, &the_payload);
    payload_free_function(the_payload);
  } else {
    LinkedListNode* prev_node = iter->node->prev;
    LinkedListNode* next_node = iter->node->next;
    prev_node->next = next_node;
    next_node->prev = prev_node;
    payload_free_function(iter->node->payload);
    free(iter->node);
    iter->node = next_node;
    iter->list->num_elements -= 1;
  }

  return true;
}


///////////////////////////////////////////////////////////////////////////////
// Helper functions

bool LinkedList_Slice(LinkedList *list, LLPayload_t *payload_ptr) {
  Verify333(payload_ptr != NULL);
  Verify333(list != NULL);

  // STEP 8: implement LinkedList_Slice.
  if (list->num_elements == 0) {
    return false;

  } else if (list->num_elements == 1) {  // Case when the linked list is empty
    *payload_ptr = list->tail->payload;
    free(list->tail);
    list->head = NULL;
    list->tail = NULL;
    list->num_elements = 0;

  } else {
    *payload_ptr = list->tail->payload;
    LinkedListNode* temp = list->tail;
    list->tail = list->tail->prev;
    list->tail->next = NULL;
    free(temp);
    list->num_elements -= 1;
  }
  return true;
}

void LLIterator_Rewind(LLIterator *iter) {
  iter->node = iter->list->head;
}
