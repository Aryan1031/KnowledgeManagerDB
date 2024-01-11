#include "KnowledgeManagerDb.h"
#include <iostream>
#include <stdexcept>

int main() {

  KnowledgeManagerDb db("test.db"); // dbpath
  
  // Insert records
  std::string record0 = "Hello world";
  std::vector<float> embeddings0 = {0.1, 0.2, 0.3};
  int id0 = 0;
  db.insertRecord(id0, record0, embeddings0);

  std::cout << "Inserted records:\n";
  printRecords(db);

  // Query
  int queryId = 1;
  int k = 5;

  try {
    std::string records = db.selectRecord(queryId, k);
    
    std::cout << "\nLookup for ID " << queryId << " returned:\n";
    std::cout << records << "\n";

  } catch(std::exception& e) {
    std::cerr << "Error: " << e.what() << "\n";
  }

  return 0;
}

// Print records 
void printRecords(KnowledgeManagerDb& db) {
    int numRecords=1; //initialized for now, can be used as a variable to track the number of records inserted to sql database
  for(int id = 0; id < numRecords; id++) {
    try {
      std::string record = db.lookupRecord(id);
      std::cout << id << ": " << record << "\n";
    } catch(std::exception& e) {
      std::cerr << "Error retrieving record " << id << ": " << e.what() << "\n";
    }
  }
}