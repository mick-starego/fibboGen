#include <iostream>
#include "TreeNode.h"

using namespace std;

TreeNode::TreeNode(int data, int step) {
    this->data = data;
    this->step = step;
    this->isIgnore = false;
    this->isMarked = false;
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

void TreeNode::markToKeep() {
    isMarked = true;
}

void TreeNode::unMarkToKeep() {
    isMarked = false;
}

bool TreeNode::isMarkedToKeep() {
    return isMarked;
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