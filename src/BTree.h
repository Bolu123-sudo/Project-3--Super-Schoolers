#ifndef PROJECT_3_SUPER_SCHOOLERS_BTREE_H
#define PROJECT_3_SUPER_SCHOOLERS_BTREE_H

#include "School.h"
#include <vector>

const int DEGREE = 3;

class BTreeNode {
public:
    bool leaf;
    std::vector<School> keys;
    std::vector<BTreeNode*> children;

    BTreeNode(bool isLeaf);
    void traverse(std::vector<School>& out);
    void insertNonFull(const School& s);
    void splitChild(int i, BTreeNode* y);
};

class BTree {
private:
    BTreeNode* root;

public:
    BTree();
    void insert(const School& s);
    std::vector<School> getAll();
};

#endif //PROJECT_3_SUPER_SCHOOLERS_BTREE_H