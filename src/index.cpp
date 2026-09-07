#include "index.h"
#include <fstream>
#include <sstream>
#include <set>

Index::Index() : m(create_multimap()), filesIndexed(0), tokenizer(create_tokenizer())
{
}

Index::~Index()
{
    delete m;
    delete tokenizer;
}

int Index::build_index(const std::string& path)
{
    fileNames = get_filenames(path);
    std::string token;
    // Index each file
    for(const std::string& p : fileNames) {
        add_doc(p);
        filesIndexed++;
    }
	return filesIndexed;
}

void Index::add_doc(const std::string& doc_file)
{
    // Open and read the entire file into a string
    std::string token;
    std::ifstream file(doc_file);
    if (!file.is_open()) {
        return;
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    
    std::string fileContent = buffer.str();
    // Tokenize the file contents and insert each token and filename into the multimap
    tokenizer->tokenize(fileContent);
    while(tokenizer->next(token)) {
        m->put(token, doc_file);
    }
}

std::vector<std::string> Index::query(const std::vector<std::string>& terms) const
{
    if (terms.empty()) return {};
    
    // Start with all documents matching the first term
    std::set<std::string> result;
    MultimapBase::IteratorBase* it = m->get(terms[0]);
    std::string val;
    while (it->next(val)) {
        result.insert(val);
    }
    delete it;
    
    for (size_t i = 1; i < terms.size(); i++) {
        std::set<std::string> termDocs;
        it = m->get(terms[i]);
        while (it->next(val)) {
            termDocs.insert(val);
        }
        delete it;
        
        std::set<std::string>::iterator iter = result.begin();
        while (iter != result.end()) {
            if (termDocs.find(*iter) == termDocs.end())
                iter = result.erase(iter);
            else
                ++iter;
        }
    }
    
    return std::vector<std::string>(result.begin(), result.end());
}
