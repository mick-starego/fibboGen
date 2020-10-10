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

        int getSize();
        void setStart(int pos);
        void stepDown(float load, int* matrix);
        void takeFirstStep(float load, int* matrix);
        unordered_set<__int128>* getBottomLayer();
        unordered_set<__int128>* getNextLayer();
        int* getBottomLayerMatrix(int numOnes);

    private:
        bool* board;

        bool** dirOne;
        bool** dirTwo;
        bool** dirThree;

        int order;
        int size;
        int start;
        int target;
        vector< unordered_set<__int128>* > boardNodes;


        void setDirections();
        int calculateSize(int order) const;
        void setBoard(__int128 enc);
        __int128 encodeCurrentBoard();
        int getNodeCountAtLayer(int level);
        int getScore(__int128 enc, int* matrix);
        unordered_set<__int128>* getChildren(__int128 node);

};

#endif