Introduction
The Knowledge Manager is an integral component of your AI generative model, facilitating the storage and retrieval of information. This tool employs both SQLite for text data and hnswlib for vector embeddings. This document offers an insightful explanation of the Knowledge Manager's functionality, especially the integration with hnswlib and SQLite.

1. Initialization
KnowledgeManagerDb knowledgeManager("my_database.db");
Upon initialization, the Knowledge Manager establishes connections with the underlying databases, SQLite and hnswlib, by providing the path to the SQLite database.

2. Inserting Records
When inserting a record, the insertRecord function is employed. This function takes three main parameters: ID, input text data, and vector embeddings. The ID serves as a unique identifier for the record, and both the text data and embeddings are associated with this ID.

Example:
int id = 1;
std::string inputText = "User input text";
std::vector<float> embeddings = LlamaGetEmbeddings(inputText); // Provided by llama.cpp
knowledgeManager.insertRecord(id, inputText, embeddings);

The insertRecord function performs the following tasks:

SQLite Database:
Prepares an SQL statement to insert the ID and text data into the SQLite database.
Executes the statement, storing the text data associated with the provided ID.

hnswlib Index:
Utilizes the addPoint function to add the embeddings and their associated ID to the hnswlib index.
After adding embeddings to the hnswlib index, the serialized index is saved using the saveIndex function. This function takes a string parameter representing the path to the file where the serialized HNSW index will be stored.

Example:
std::string hnswPath = "hnsw.bin";
knowledgeManager.saveHnswIndex(hnswPath);

3. Retrieving Records
To retrieve records, the selectRecord function is used. This function requires an ID as input and returns a vector of strings containing the text data associated with the k-nearest neighbors of the input vector embeddings.

Example:
int id = 1;
std::vector<std::string> retrievedData = knowledgeManager.selectRecord(id);

The selectRecord function performs the following tasks:

hnswlib Index Loading:
Loads the pre-saved hnswlib index using the loadIndex function.
Nearest Neighbor Search:
Uses the loaded index to perform a nearest neighbor search (searchKnn) for the given embeddings.
Retrieves the IDs of the k-nearest neighbors.

SQLite Data Retrieval:
Executes an SQL statement to select text data from the SQLite database where the ID matches those of the k-nearest neighbors.
Retrieves the text data associated with the retrieved IDs.

4. Workflow Overview
The typical workflow involves the following steps:

Inserting Data:

Text Data: Provided by the user.
Embeddings: Generated by Llama.
ID: Assigned to the record.

Storage:

SQLite Database:
Text data is stored with its corresponding ID.

hnswlib Index:
Embeddings are added to the index along with their associated IDs.

hnswlib Index Serialization:
Save the serialized hnswlib index using the saveHnswIndex function.

Retrieval:

hnswlib Index Loading:
Load the pre-saved hnswlib index.

Nearest Neighbor Search:
Use the index to find k-nearest neighbors for the given embeddings.

SQLite Data Retrieval:
Retrieve text data associated with the nearest neighbors' IDs.

Output:

Final String: Concatenate the retrieved text data.


Conclusion
The Knowledge Manager streamlines the process of managing information in an AI generative model. It leverages SQLite for efficient text data storage and hnswlib for vector embeddings, providing a powerful solution for storing, retrieving, and utilizing knowledge in the model. The integration with Llama further enhances the model's capability by providing embeddings for user-input text.
