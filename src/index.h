#ifndef INDEX_H
#define INDEX_H

#include "provided.h"
#include <string>
#include <vector>

class Index : public IndexBase
{
public:
    Index();
    virtual ~Index();
    // Index every file in the given directory and return the number of files indexed
    virtual int build_index(const std::string& path);
    // Add a single document file to the index
    virtual void add_doc(const std::string& doc_file);
    // Return documents containing all of the given terms
    virtual std::vector<std::string> query(const std::vector<std::string>& terms) const;

private:
    MultimapBase* m;
    int filesIndexed;
    std::vector<std::string> fileNames;
    TokenizerBase* tokenizer;
};

#endif // INDEX_H
