#include <iostream>
#include "TreeNode.h"

using namespace std;

TreeNode::TreeNode(int data, int step) {
    this->data = data;
    this->step = step;
    this->parent = NULL;
    this->isIgnore = false;
}

TreeNode::~TreeNode() {
}

bool TreeNode::ignore() {
    if (isIgnore) {
        return true;
    } else {
        isIgnore = true;
        return false;
    }
}

int TreeNode::getData() {
    return data;
}

int TreeNode::getStep() {
    return step;
}

int TreeNode::getNumChildren() {
    int num = 0;
    for (int i = 0; i < children.size(); i++) {
        if (children[i] != NULL) {
            num++;
        }
    }
    return num;
}

void TreeNode::addChild(TreeNode* child) {
    children.push_back(child);
}

TreeNode* TreeNode::getParent() {
    return parent;
}

void TreeNode::setParent(TreeNode* parent) {
    this->parent = parent;
}

TreeNode* TreeNode::getChildByData(int data) {
    vector<TreeNode*>::iterator it = children.begin();
    for (; it != children.end(); it++) {
        if ((*it)->getData() == data) {
            return *it;
        }
    }
    return NULL;
}

TreeNode* TreeNode::getChildByIndex(int index) {
    return ( index >= 0 && index < children.size() ) ? children[index] : NULL;
}

vector<TreeNode*>& TreeNode::getAllChildren() {
    return children;
}

vector<TreeNode*>::iterator TreeNode::getChildrenBegin() {
    return children.begin();
}

vector<TreeNode*>::iterator TreeNode::getChildrenEnd() {
    return children.end();
}

//! NOTE: Very, very dangerous. Does not delete node in
// question, just removes pointer. You better know what
// you're doing...
void TreeNode::deleteChild(TreeNode* node) {
    for (int i = 0; i < children.size(); i++) {
        if (children[i] == node) {
            children[i] = NULL;
        }
    }
}

void TreeNode::removeNullChildren() {
    vector<TreeNode*> newVec;
    for (int i = 0; i < children.size(); i++) {
        if (children[i] != NULL) {
            newVec.push_back(children[i]);
        }
    }
    children.clear();
    children.insert(children.end(), newVec.begin(), newVec.end());
}

// int main() {
//     TreeNode head(1, 0);
//     head.addChild(new TreeNode(2, 1));
//     head.addChild(new TreeNode(3, 1));
//     head.addChild(new TreeNode(4, 1));

//     vector<TreeNode*>::iterator it = head.getChildrenBegin();
//     for (; it != head.getChildrenEnd(); it++) {
//         (*it)->addChild(new TreeNode(0, 2));
//         (*it)->addChild(new TreeNode(1, 2));
//         (*it)->addChild(new TreeNode(2, 2));
//         (*it)->addChild(new TreeNode(3, 2));
//     }

//     cout << head.getData() << endl;
//     it = head.getChildrenBegin();
//     for (; it != head.getChildrenEnd(); it++) {
//         cout << (*it)->getData() << " ";
//     }
//     cout << endl;

//     it = head.getChildrenBegin();
//     vector<TreeNode*>::iterator childIt;
//     for (; it != head.getChildrenEnd(); it++) {
//         childIt = (*it)->getChildrenBegin();
//         for (; childIt != (*it)->getChildrenEnd(); childIt++) {
//             cout << (*childIt)->getData() << " ";
//         }
//     }
//     cout << endl;
// }