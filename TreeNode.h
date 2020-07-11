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

        void addChild(TreeNode* child);
        
        TreeNode* getParent();
        void setParent(TreeNode*);
        
        TreeNode* getChildByIndex(int index);
        TreeNode* getChildByData(int data);
        vector<TreeNode*>& getAllChildren();
        vector<TreeNode*>::iterator getChildrenBegin();
        vector<TreeNode*>::iterator getChildrenEnd();
        int getNumChildren();
        void deleteChild(TreeNode* node);
        void removeNullChildren();

        bool ignore();

    private:
        int data;
        vector<TreeNode*> children;
        int step;
        TreeNode* parent;
        bool isIgnore;
};

#endif