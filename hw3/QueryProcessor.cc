/*
 * Copyright ©2023 Justin Hsia.  All rights reserved.  Permission is
 * hereby granted to students registered for University of Washington
 * CSE 333 for use solely during Winter Quarter 2023 for purposes of
 * the course.  No other use, copying, distribution, or modification
 * is permitted without prior written consent. Copyrights for
 * third-party components of this work must be honored.  Instructors
 * interested in reusing these course materials should contact the
 * author.
 */

#include "./QueryProcessor.h"

#include <iostream>
#include <algorithm>
#include <list>
#include <string>
#include <vector>

extern "C" {
  #include "./libhw1/CSE333.h"
}

using std::list;
using std::sort;
using std::string;
using std::vector;

namespace hw3 {

// This helper function accepts two DocIDElementHeader lists. It finds elements
// from these lists that contains the same docid and return a list of these
// docids with the sum of their occurances extracted from both lists.
static list<DocIDElementHeader> CombineResults
       (const list<DocIDElementHeader>&, const list<DocIDElementHeader>&);

QueryProcessor::QueryProcessor(const list<string>& index_list, bool validate) {
  // Stash away a copy of the index list.
  index_list_ = index_list;
  array_len_ = index_list_.size();
  Verify333(array_len_ > 0);

  // Create the arrays of DocTableReader*'s. and IndexTableReader*'s.
  dtr_array_ = new DocTableReader* [array_len_];
  itr_array_ = new IndexTableReader* [array_len_];

  // Populate the arrays with heap-allocated DocTableReader and
  // IndexTableReader object instances.
  list<string>::const_iterator idx_iterator = index_list_.begin();
  for (int i = 0; i < array_len_; i++) {
    FileIndexReader fir(*idx_iterator, validate);
    dtr_array_[i] = fir.NewDocTableReader();
    itr_array_[i] = fir.NewIndexTableReader();
    idx_iterator++;
  }
}

QueryProcessor::~QueryProcessor() {
  // Delete the heap-allocated DocTableReader and IndexTableReader
  // object instances.
  Verify333(dtr_array_ != nullptr);
  Verify333(itr_array_ != nullptr);
  for (int i = 0; i < array_len_; i++) {
    delete dtr_array_[i];
    delete itr_array_[i];
  }

  // Delete the arrays of DocTableReader*'s and IndexTableReader*'s.
  delete[] dtr_array_;
  delete[] itr_array_;
  dtr_array_ = nullptr;
  itr_array_ = nullptr;
}

// This structure is used to store a index-file-specific query result.
typedef struct {
  DocID_t doc_id;  // The document ID within the index file.
  int     rank;    // The rank of the result so far.
} IdxQueryResult;

vector<QueryProcessor::QueryResult>
QueryProcessor::ProcessQuery(const vector<string>& query) const {
  Verify333(query.size() > 0);

  // STEP 1.
  // (the only step in this file)
  vector<QueryProcessor::QueryResult> final_result;
  for (int i = 0; i < array_len_; i++) {
    IndexTableReader* itr = itr_array_[i];

    DocIDTableReader* didtr_0 = itr->LookupWord(query[0]);
    if (didtr_0 == NULL) {
      delete(didtr_0);
      continue;
    }

    list<DocIDElementHeader> result = didtr_0->GetDocIDList();

    for (size_t j = 1; j < query.size(); j++) {
      DocIDTableReader* didtr_tmp = itr->LookupWord(query[j]);
      if (didtr_tmp == NULL) {
        result.clear();
        delete(didtr_tmp);
        break;
      }

      list<DocIDElementHeader> result_tmp = didtr_tmp->GetDocIDList();
      result = CombineResults(result, result_tmp);

      delete(didtr_tmp);
    }

    if (result.size() == 0) {
      delete(didtr_0);
      continue;
    } else {
      DocTableReader* dtr = dtr_array_[i];
      for (auto const& element : result) {
        DocID_t doc_id = element.doc_id;
        int num_positions = element.num_positions;
        QueryProcessor::QueryResult qres;

        Verify333(dtr->LookupDocID(doc_id, &qres.document_name));
        qres.rank = num_positions;

        final_result.push_back(qres);
      }
    }

    delete(didtr_0);
  }


  // Sort the final results.
  sort(final_result.begin(), final_result.end());
  return final_result;
}

static list<DocIDElementHeader> CombineResults
       (const list<DocIDElementHeader> &list1,
       const list<DocIDElementHeader> &list2) {
  list<DocIDElementHeader> final_list;

  for (auto const& element1 : list1) {
    DocID_t doc_id = element1.doc_id;
    int num_positions = element1.num_positions;
    bool exists = false;

    for (auto const& element2 : list2) {
      if (element2.doc_id == doc_id) {
        exists = true;
        num_positions += element2.num_positions;
        break;
      }
    }

    if (exists)
      final_list.push_back(DocIDElementHeader{doc_id, num_positions});
  }

  return final_list;
}

}  // namespace hw3
