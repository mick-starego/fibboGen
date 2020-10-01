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

        bool ignore();
        void markToKeep();
        void unMarkToKeep();
        bool isMarkedToKeep();

    private:
        int data;
        int step;
        bool isIgnore;
        bool isMarked;
};

#endif