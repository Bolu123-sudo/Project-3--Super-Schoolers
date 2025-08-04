

#include "BTree.h"

// ===== Node / Tree constructors =====

BTree::BTree() : root(nullptr) {}

BTreeNode::BTreeNode(bool isLeaf) {
    leaf = isLeaf;
}

// ===== Traversal =====

void BTreeNode::traverse(std::vector<School>& out) {
    int i;
    for (i = 0; i < keys.size(); i++) {
        if (!leaf) children[i]->traverse(out);
        out.push_back(keys[i]);
    }
    if (!leaf) children[i]->traverse(out);
}

// ===== Insertion helpers =====

void BTreeNode::insertNonFull(const School& s) {
    int i = keys.size() - 1;
    keys.push_back(s); // expand size

    while (i >= 0 && s.name < keys[i].name) {
        keys[i + 1] = keys[i];
        i--;
    }
    keys[i + 1] = s;
}

// Split child y of this node at index i.

void BTreeNode::splitChild(int i, BTreeNode* y) {
    BTreeNode* z = new BTreeNode(y->leaf);
    // save median before resizing
    School median = y->keys[DEGREE - 1];

    // move last DEGREE-1 keys from y into z
    for (int j = 0; j < DEGREE - 1; j++)
        z->keys.push_back(y->keys[j + DEGREE]);

    if (!y->leaf) {
        for (int j = 0; j < DEGREE; j++)
            z->children.push_back(y->children[j + DEGREE]);
    }

    // shrink y to first DEGREE-1 keys and appropriate children
    y->keys.resize(DEGREE - 1);
    if (!y->leaf)
        y->children.resize(DEGREE);

    // insert new child and median into current node
    children.insert(children.begin() + i + 1, z);
    keys.insert(keys.begin() + i, median);
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

            int idx = (s.name > newRoot->keys[0].name) ? 1 : 0;
            newRoot->children[idx]->insertNonFull(s);
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
