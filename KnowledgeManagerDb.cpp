#include "KnowledgeManagerDb.h"
#include <iostream>
#include <random>
#include <stdexcept>

knowledgemanagerdb::knowledgemanagerdb(const char* dbpath) {
    sqlite3_open(dbpath, &db);

    int dim = 16;
    space = new hnswlib::L2Space(dim);
    alg_hnsw = new hnswlib::HierarchicalNSW<float>(space, 1, 16, 200);

    if (!alg_hnsw) {
        std::cout << "NULL PTR1" << "\n";
    }

    hnswindexpath = "hnsw.bin";
}

knowledgemanagerdb::~knowledgemanagerdb() {
    delete alg_hnsw;
    delete space;
    sqlite3_close(db);
}

bool knowledgemanagerdb::insertrecord(int id, const std::string& data, const std::vector<float>& embeddings) {
    const char* sql = "insert into my_table (id, data) values (?, ?)";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "failed to prepare insert statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_int(stmt, 1, id);
    sqlite3_bind_text(stmt, 2, data.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "insert failed: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);

    if (alg_hnsw == nullptr) {
        std::cout << "NULL PTR1" << "\n";
        return false;
    }

    alg_hnsw->addPoint(embeddings.data(), id);
    alg_hnsw->saveIndex(hnswindexpath);

    return true;
}

std::string knowledgemanagerdb::lookuprecord(int id) {
    if (!alg_hnsw) {
        std::cout << "NULL PTR" << "\n";
        return "";
    }

    const char* sql = "select data from my_table where id = ?";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "failed to prepare select statement: " << sqlite3_errmsg(db) << std::endl;
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

std::string knowledgemanagerdb::selectrecord(int id, int k) {
    if (!alg_hnsw) {
        std::cout << "NULL PTR2" << "\n";
        return "";
    }

    try {
        alg_hnsw->loadIndex(hnswindexpath, space, 1);
    }
    catch (const std::exception& e) {
        std::cout << "NULL PTR3" << "\n";
        // Handle error
    }

    float query[16];
    for (int i = 0; i < 16; i++) {
        query[i] = i;
    }

    std::priority_queue<std::pair<float, hnswlib::labeltype>> results = alg_hnsw->searchKnn(query, k);

    std::vector<int> ids;
    while (!results.empty()) {
        ids.push_back(results.top().second);
        results.pop();
    }

    std::string records;
    for (int neighborid : ids) {
        std::string record = lookuprecord(neighborid);
        records += record + "\n";
    }

    return records;
}
