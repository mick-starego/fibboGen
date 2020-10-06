#ifndef TREE_NODE_H
#define TREE_NODE_H

#include <iostream>
#include <vector>

using namespace std;

class TreeNode {
    public:
        TreeNode(int data, int step);
        ~TreeNode();

        int getData();
        int getStep();
        
        void addParent(TreeNode* child);
        TreeNode* getParentByIndex(int index);
        TreeNode* getParentByData(int data);
        vector<TreeNode*>::iterator getParentsBegin();
        vector<TreeNode*>::iterator getParentsEnd();
        int getNumParents();

        bool ignore();
        void markToKeep();
        void unMarkToKeep();
        bool isMarkedToKeep();

    private:
        int data;
        vector<TreeNode*> parents;
        int step;
        bool isIgnore;
        bool isMarked;
};

#endif