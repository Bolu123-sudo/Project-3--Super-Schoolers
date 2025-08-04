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
    BTree() : root(nullptr) {}
    void insert(const School& s);
    std::vector<School> getAll();
};

// ===== Node Implementation =====

BTreeNode::BTreeNode(bool isLeaf) {
    leaf = isLeaf;
}

void BTreeNode::traverse(std::vector<School>& out) {
    int i;
    for (i = 0; i < keys.size(); i++) {
        if (!leaf) children[i]->traverse(out);
        out.push_back(keys[i]);
    }
    if (!leaf) children[i]->traverse(out);
}

void BTreeNode::insertNonFull(const School& s) {
    int i = keys.size() - 1;
    keys.push_back(s); // expand size

    while (i >= 0 && s.name < keys[i].name) {
        keys[i + 1] = keys[i];
        i--;
    }
    keys[i + 1] = s;
}

void BTreeNode::splitChild(int i, BTreeNode* y) {
    BTreeNode* z = new BTreeNode(y->leaf);
    for (int j = 0; j < DEGREE - 1; j++)
        z->keys.push_back(y->keys[j + DEGREE]);

    if (!y->leaf) {
        for (int j = 0; j < DEGREE; j++)
            z->children.push_back(y->children[j + DEGREE]);
    }

    y->keys.resize(DEGREE - 1);
    y->children.resize(y->leaf ? 0 : DEGREE);

    children.insert(children.begin() + i + 1, z);
    keys.insert(keys.begin() + i, y->keys[DEGREE - 1]);
}

// ===== Tree Implementation =====

void BTree::insert(const School& s) {
    if (!root) {
        root = new BTreeNode(true);
        root->keys.push_back(s);
    } else {
        if (root->keys.size() == 2 * DEGREE - 1) {
            BTreeNode* newRoot = new BTreeNode(false);
            newRoot->children.push_back(root);
            newRoot->splitChild(0, root);

            int i = (s.name > newRoot->keys[0].name) ? 1 : 0;
            newRoot->children[i]->insertNonFull(s);
            root = newRoot;
        } else {
            root->insertNonFull(s);
        }
    }
}

std::vector<School> BTree::getAll() {
    std::vector<School> res;
    if (root) root->traverse(res);
    return res;
}



#endif //PROJECT_3_SUPER_SCHOOLERS_BTREE_H
