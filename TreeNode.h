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
        TreeNode* getChildByIndex(int index);
        TreeNode* getChildByData(int data);
        vector<TreeNode*>& getAllChildren();
        vector<TreeNode*>::iterator getChildrenBegin();
        vector<TreeNode*>::iterator getChildrenEnd();
        int getNumChildren();
        void deleteChild(TreeNode* node);
        void removeNullChildren();

        void addParent(TreeNode* parent);
        TreeNode* getParentByIndex(int index);
        vector<TreeNode*>::iterator getParentsBegin();
        vector<TreeNode*>::iterator getParentsEnd();

        bool ignore();

    private:
        int data;
        vector<TreeNode*> children;
        vector<TreeNode*> parents;
        int step;
        bool isIgnore;
};

#endif