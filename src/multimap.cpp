#include "multimap.h"
#include <string>

Multimap::Iterator::Iterator(std::vector<std::string>* vec) {
    current_vector = vec;
    index = 0;
}

bool Multimap::Iterator::next(std::string& value)
{
    // If no vector or we've gone past the end, nothing left
    if(current_vector == nullptr || index >= current_vector->size()) {
        return false;
    }
    value = (*current_vector)[index];
    index++;
	return true;
}


Multimap::Multimap() : root(nullptr), m_size(0)
{
}


Multimap::~Multimap()
{
    deleteHelper(root);
}
// Post-order traversal to delete every node in the BST
void Multimap::deleteHelper(Node *p) {
    if(p == nullptr) {
        return;
    }
    deleteHelper(p->m_left);
    deleteHelper(p->m_right);
    delete p;
}

void Multimap::put(const std::string& key, const std::string& value)
{
    // Empty tree: create root
    if(root == nullptr) {
        root = new Node(key);
        root->values.push_back(value);
        m_size++;
        return;
    }
    
    // Traverse the BST to find the key or the insertion point
    Node* current = root;
    Node* trail = current;
    while(current != nullptr) {
        trail = current;
        if(current->m_key == key) {
            for (const std::string& v : current->values) {
                if (v == value) return;  // duplicate pair, do nothing
            }
            current->values.push_back(value);
            m_size++;
            return;
        }
        if(key < current->m_key) {
            current = current->m_left;
        } else {
            current = current->m_right;
        }
    }
    // Key not found
    if(key < trail->m_key) {
        trail->m_left = new Node(key);
        trail->m_left->values.push_back(value);
        m_size++;
    } else {
        trail->m_right = new Node(key);
        trail->m_right->values.push_back(value);
        m_size++;
    }
}

MultimapBase::IteratorBase* Multimap::get(const std::string& key) const
{
    // Traverse the BST looking for the key
    if(root == nullptr) return new Iterator(nullptr);
    Node* current = root;
    while(current != nullptr) {
        if(current->m_key == key) {
            return new Iterator(&(current->values));
        }
        if(key < current->m_key) {
            current = current->m_left;
        } else {
            current = current->m_right;
        }
    }
    // Key not found
    return new Iterator(nullptr);
}

bool Multimap::empty() const
{
	return root==nullptr;
}

int Multimap::size() const
{
	return m_size;
}
