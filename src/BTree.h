#ifndef PROJECT_3_SUPER_SCHOOLERS_BTREE_H
#define PROJECT_3_SUPER_SCHOOLERS_BTREE_H

#include "School.h"
#include <vector>

const int DEGREE = 3;

// A node in the B-Tree
class BTreeNode {
public:
    bool leaf;
    std::vector<School> keys;
    std::vector<BTreeNode*> children;

    // Constructor- create a node
    BTreeNode(bool isLeaf);
    void traverse(std::vector<School>& out);
    // Insert a new key
    void insertNonFull(const School& s);
    void splitChild(int i, BTreeNode* y);
};

class BTree {
private:
    BTreeNode* root;

public:
    BTree();
    // Insert a new School record into the B-Tree
    void insert(const School& s);

    // Retrieved all School records in sorted order.
    std::vector<School> getAll();
};

#endif //PROJECT_3_SUPER_SCHOOLERS_BTREE_H