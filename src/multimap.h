#ifndef MULTIMAP_H
#define MULTIMAP_H

#include "provided.h"
#include <string>
#include <vector>

class Multimap : public MultimapBase
{
public:
    // Iterator that goes through all values in a specific key
    class Iterator : public MultimapBase::IteratorBase
    {
    public:
        // Construct iterator over the given value list
        Iterator(std::vector<std::string>* vec);
        // Set value to the next item and advance
        virtual bool next(std::string& value);

    private:
        // pointer to a node's value list
        std::vector<std::string>* current_vector;
        int index;
    };

    Multimap();
    virtual ~Multimap();
    // Insert key/value pair
    virtual void put(const std::string& key, const std::string& value);
    virtual MultimapBase::IteratorBase* get(const std::string& key) const;
    virtual bool empty() const;
    virtual int size() const;

private:
    // BST node
    struct Node {
        std::string m_key;
        Node* m_left;
        Node* m_right;
        std::vector<std::string> values;
        Node(std::string s) {
            m_key = s;
            m_left = nullptr;
            m_right = nullptr;
        };
    };
    Node* root;
    int m_size;
    
    void deleteHelper(Node* p);
};

#endif // MULTIMAP_H
