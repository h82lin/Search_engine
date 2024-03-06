/*
 * Copyright ©2023 Chris Thachuk.  All rights reserved.  Permission is
 * hereby granted to students registered for University of Washington
 * CSE 333 for use solely during Spring Quarter 2023 for purposes of
 * the course.  No other use, copying, distribution, or modification
 * is permitted without prior written consent. Copyrights for
 * third-party components of this work must be honored.  Instructors
 * interested in reusing these course materials should contact the
 * author.
 */

// Feature test macro for strtok_r (c.f., Linux Programming Interface p. 63)
#define _XOPEN_SOURCE 600

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#include "libhw1/CSE333.h"
#include "./CrawlFileTree.h"
#include "./DocTable.h"
#include "./MemIndex.h"

//////////////////////////////////////////////////////////////////////////////
// Helper function declarations, constants, etc
static void Usage(void);
// Splits the input query by the " " delimiter to get an array of strings,
// where each string is a word inputted by the user.
//
// Arguments:
// - query: User inputted query
// - word_list: An output parameter which is an array of strings. Each array
//   element is a word from the query.
//
// Returns:
// - the number of words contained in the user inputted query.
static int ProcessQueries(char* query, char **word_list);

// Prints the document name and rank of all payloads in a LinkedList. This
// LinkedList is populated with payloads of SearchResult data.
//
// Arguments:
// - ll: LinkedList with SearchResult data
// - dt: The DocTable used to map doc_id to document name.
static void printSearchResults(LinkedList* ll, DocTable* dt);


//////////////////////////////////////////////////////////////////////////////
// Main
int main(int argc, char** argv) {
  if (argc != 2) {
    Usage();
  }

  // Implement searchshell!  We're giving you very few hints
  // on how to do it, so you'll need to figure out an appropriate
  // decomposition into functions as well as implementing the
  // functions.  There are several major tasks you need to build:
  //
  //  - Crawl from a directory provided by argv[1] to produce and index
  //  - Prompt the user for a query and read the query f./rom stdin, in a loop
  //  - Split a query into words (check out strtok_r)
  //  - Process a query against the index and print out the results
  //
  // When searchshell detects end-of-file on stdin (cntrl-D from the
  // keyboard), searchshell should free all dynamically allocated
  // memory and any other allocated resources and then exit.
  //
  // Note that you should make sure the fomatting of your
  // searchshell output exactly matches our solution binaries
  // to get full points on this part.
  int res;
  DocTable* dt;
  MemIndex* idx;
  char* directory = argv[1];


  printf("Indexing '%s'\n", directory);
  res = CrawlFileTree(directory,
                      &dt,
                      &idx);
  if (!res) {
    return EXIT_FAILURE;
  }
  while (1) {
    LinkedList* ll_res;
    char buffer[1024];
    char* query = NULL;
    int word_count;
    char **word_list = (char**)malloc(1024 * sizeof(char*));
    if (word_list == NULL) {
      return EXIT_FAILURE;
    }
    printf("%s\n", "enter query:");
    if (fgets(buffer, 1024, stdin) == NULL) {
      printf("shutting down...");
      free(word_list);
      break;
    }
    query = strdup(buffer);
    word_count = ProcessQueries(query, word_list);

    ll_res = MemIndex_Search(idx, word_list, word_count);
    printSearchResults(ll_res, dt);
  }

  return EXIT_SUCCESS;
}


//////////////////////////////////////////////////////////////////////////////
// Helper function definitions

static void Usage(void) {
  fprintf(stderr, "Usage: ./searchshell <docroot>\n");
  fprintf(stderr,
          "where <docroot> is an absolute or relative " \
          "path to a directory to build an index under.\n");
  exit(EXIT_FAILURE);
}

static int ProcessQueries(char* query, char **word_list) {
  int word_count = 0;
  char* token = NULL;
  char* rest_of_str = NULL;

  token  = strtok_r(query, " ", &rest_of_str);
  while (token != NULL) {
    int i = 0;
    while (token[i] != '\0') {
      token[i] = tolower(token[i]);
      i++;
    }
    word_list[word_count] = token;
    word_count++;
    token = strtok_r(rest_of_str, " ", &rest_of_str);
  }

  char *lp = strchr(word_list[word_count - 1], '\n');
  if (lp) {
    *lp = '\0';
  }

  return word_count;
}

static void printSearchResults(LinkedList* ll, DocTable* dt) {
  SearchResult* sr;
  if (ll == NULL) {
    return;
  }
  int ll_size = LinkedList_NumElements(ll);
  LLIterator* ll_iter = LLIterator_Allocate(ll);
  char* doc_name;
  for (int i=0; i<ll_size; i++) {
    LLIterator_Get(ll_iter, (LLPayload_t*) &sr);
    doc_name = DocTable_GetDocName(dt, sr->doc_id);
    printf("%s (%d)\n", doc_name, sr->rank);
    LLIterator_Next(ll_iter);
  }
}
