#ifndef RBTREE_H
#define RBTREE_H

#define black 0
#define red 1

#include <iostream>
#include <iomanip>

using namespace std;

template <class K, class V>
class rbTree {
private:
    struct Node {
        K key;
        V value;
        bool color; // black is 0 and red is 1
        Node* left;
        Node* right;
        Node* parent;
        Node(const K& newKey, const V& newValue) : key(newKey), value(newValue), color(red), left(NULL), right(NULL), parent(NULL) {} // default constructor that allocates a new node with color red
    };
    Node* root;
    Node* nil;
    size_t _size;

    void rotateLeft(Node* t);
    void rotateRight(Node* t);
    void insertFixup(Node* newNode);
    Node* getNode(const K& key) const;
    void replace(Node*& u, Node*& v);
    void deleteFixup(Node* target);

    void clear(Node* subRoot) {
        if (subRoot == nil)
            return;
        clear(subRoot->left);
        clear(subRoot->right);
        delete subRoot;
    }

    Node* copy(const Node* subroot) {
        if (subroot == nil)
            return nil;
        Node* newNode = new Node(subroot->key, subroot->value);
        newNode->left = copy(subroot->left);
        newNode->right = copy(subroot->right);
        return newNode;
    }

public:
    void insert(const K & key, const V & value);
    V find(const K& key) const;
    void remove(const K & key);
    Node* getMinNode(Node* root) const;
    Node* getMaxNode(Node* root) const;

    /* Default constructor that allocates an empty tree. */
    rbTree() {
        nil = new Node(0, 0);
        nil->color = black;
        root = nil;
        _size = 0;
    }

    /* Copy copnstructor. */
    rbTree(const rbTree& other) {
        nil = new Node(0, 0);
        nil->color = black;
        root = copy(other.root);
        root->parent = nil;
        _size = other.size;
    }

    /* Assignment operator overload. */
    const rbTree& operator=(const rbTree& rhs) {
        if (this != &rhs) {
            clear(root);
            root = copy(rhs.root);
            _size = rhs.size;
        }
        return *this;
    }

    void clear() {
        clear(root);
        delete nil;
    }

    /* Default destructor. */
    ~rbTree() {
        clear();
        root = NULL;
        nil = NULL;
    }

    Node* getRoot() const {
        return root;
    }

    size_t size() const {
        return _size;
    }

    /* Prints the tree in horizontal layout. */
    void print(Node* p, int indent) {
        if (p != NULL) {
            if (p->right)
                print(p->right, indent+4);
            if (indent)
                cout << setw(indent) << ' ';
            if (p->right)
                cout << " /\n" << setw(indent) << ' ';
            if (p->key == 0)
                cout << "nil" << "\n ";
            else if (p->color == red)
                cout << "\033[0;31m" << p->key << "\033[0m" << "\n ";
            else
                cout << p->key << "\n ";
            if (p->left) {
                cout << setw(indent) << ' ' << " \\\n";
                print(p->left, indent+4);
            }
        }
    }

};

#include "rbTree.cpp"
#endif