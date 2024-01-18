#ifndef knowledgemanagerdb_h

#define knowledgemanagerdb_h
#include "./dependency_folder/sqlite.gz/sqlite-autoconf-3400100/sqlite3.h"
#include "./dependency_folder/hnswlib/hnswlib/hnswlib.h"
#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>

class knowledgemanagerdb {
public:
    knowledgemanagerdb(const char* dbpath);
    ~knowledgemanagerdb();

    bool insertrecord(int id, const std::string& data, const std::vector<float>& embeddings);
    std::string lookuprecord(int id);
    std::string selectrecord(int id, int k);

private:
    sqlite3* db;
    hnswlib::L2Space* space;
    hnswlib::HierarchicalNSW<float>* alg_hnsw;
    std::string hnswindexpath;
};

#endif // knowledgemanagerdb_h
