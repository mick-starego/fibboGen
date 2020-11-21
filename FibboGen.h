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
        __int128 getEncodedBoard();
        void stepDown(float load, int* matrix);
        void takeFirstStep(float load, int* matrix);
        unordered_set<__int128>* getBottomLayer();
        unordered_set<__int128>* getNextLayer();
        int* getBottomLayerMatrix(int numOnes);
        int* consolidateMatrix(int* matrix, int pF);

        vector<__int128>* getFirstHalfOfSoln(__int128 enc);

        string getBoardString(__int128 current, __int128 next);

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
        vector<string> moves;


        void setDirections();
        int calculateSize(int order) const;
        void setBoard(__int128 enc);
        __int128 encodeCurrentBoard();
        int getNodeCountAtLayer(int level);
        int getScore(__int128 enc, int* matrix);
        unordered_set<__int128>* getChildren(__int128 node);
        unordered_set<__int128>* getParents(__int128 node);

        vector<string>* constructBoardString(int to, int from1, int from2, int rightPad);

};

#endif