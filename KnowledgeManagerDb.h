#ifndef KNOWLEDGEMANAGERDB_H
#define KNOWLEDGEMANAGERDB_H
#include "sqlite3.h"
#include "../../hnswlib/hnswlib.h"
#include <string>
#include <vector>


class KnowledgeManagerDb {
public:
    KnowledgeManagerDb(const char* dbPath);
    ~KnowledgeManagerDb();


    bool insertRecord(int id, const std::string& data, const std::vector<float>& embeddings);
    std::string selectRecord(int id, int k);
    std::string lookupRecord(int id);

private:
    sqlite3* db;
    hnswlib::L2Space* space;
    hnswlib::HierarchicalNSW<float>* alg_hnsw;
    std::string hnswIndexPath;


    
};


#endif // KNOWLEDGEMANAGERDB_H
