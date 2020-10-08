#ifndef FIBBO_GEN_H
#define FIBBO_GEN_H

#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include "TreeNode.h"

using namespace std;

class FibboGen {

    public:
        FibboGen(int order);
        ~FibboGen();

    private:
        bool* board;

        bool** dirOne;
        bool** dirTwo;
        bool** dirThree;

        int order;
        int size;
        int start;
        int target;
        vector< unordered_set<int>* > boardNodes;

        void setDirections();
        int calculateSize(int order) const;
        void setBoard(int enc);
        int encodeCurrentBoard();
        vector<int>* getChildren(unordered_set<int>* candidates, int node);

};

#endif