#include "KnowledgeManagerDb.h"
#include <iostream>
#include <random>


KnowledgeManagerDb::KnowledgeManagerDb(const char* dbPath) {
    // Initialize SQLite DB
    sqlite3_open(dbPath, &db);


    // Initialize hnswlib
    int dim = 16;
    space = new hnswlib::L2Space(dim);
    alg_hnsw = new hnswlib::HierarchicalNSW<float>(space, 1, 16, 200);
    hnswIndexPath = "hnsw.bin";
}


KnowledgeManagerDb::~KnowledgeManagerDb() {
    // Cleanup
    delete alg_hnsw;
    delete space;
    sqlite3_close(db);
}


bool KnowledgeManagerDb::insertRecord(int id, const std::string& data) {
    // Insert text into SQLite
    // ...existing SQLite code


    // Generate random embeddings
    std::mt19937 rng;
    rng.seed(47);
    std::uniform_real_distribution<> distrib;
    float embeddings[16]; //dim
    for (int i = 0; i < 16; i++) {
        embeddings[i] = distrib(rng);
    }


    // Add embeddings to alg_hnsw
    alg_hnsw->addPoint(embeddings, id);


    // Save index
    alg_hnsw->saveIndex(hnswIndexPath);
    return true;
}


std::string KnowledgeManagerDb::lookupRecord(int id) {
    const char* sql = "SELECT data FROM my_table WHERE id = ?";
    sqlite3_stmt* stmt;


    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare select statement: " << sqlite3_errmsg(db) << std::endl;
        return "";
    }


    sqlite3_bind_int(stmt, 1, id);


    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char* text = sqlite3_column_text(stmt, 0);
        if (text) {
            std::string result(reinterpret_cast<const char*>(text));
            sqlite3_finalize(stmt);
            return result;
        }
    }


    sqlite3_finalize(stmt);
    return "";
}


std::string KnowledgeManagerDb::selectRecord(int id, int k) {
    // Load index
    alg_hnsw->loadIndex(hnswIndexPath);


    // Dummy query point
    float query[16];
    for (int i = 0; i < 16; i++) {
        query[i] = i;
    }


    // Search for nearest neighbors
    std::priority_queue<std::pair<float, hnswlib::labeltype>> results = alg_hnsw->searchKnn(query, k);


    // Get nearest neighbor IDs
    std::vector<int> ids;
    while (!results.empty()) {
        ids.push_back(results.top().second);
        results.pop();
    }


    // Look up strings for IDs in SQLite DB
    std::string records;
    for (int neighborId : ids) {
        std::string record = lookupRecord(neighborId);
        records += record + " "; //newline ch
    }


    return records;
}
