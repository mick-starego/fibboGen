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
        FibboGen(int start, int order);
        ~FibboGen();
        
        int getStart();
        void setStart(int start);
        int getTarget();
        void setTarget(int target);
        int getStep();
        TreeNode* getParentNode();

        void clear();
        bool isSolved();
        void generateSolutionTree();
        void prune();

        void printMoves();
        void printBoardStateCounts();
        TreeNode* getNodeByData(TreeNode* node, int data);
        string constructBoardString(int enc);

    private:
        bool* board;

        bool** dirOne;
        bool** dirTwo;
        bool** dirThree;

        int order;
        int size;
        int start;
        int target;
        int solutions;
        bool solved;
        int step;
        TreeNode* parentNode;
        TreeNode* head;
        unordered_set<TreeNode*> allPointers;

        vector<string> moves;
        vector< unordered_set<int>* > boardStates;
        vector< unordered_map<int, TreeNode*>* > boardNodes;
        vector<TreeNode*> path;

        int pruneHelperCount;

        void setDirections();
        int calculateSize(int order) const;
        void resetBoard();
        void setBoard(int enc);

        void backtrack(int step);
        int findPos(bool* ptr);
        bool checkSolved();
        string boardString(int position);
        int encodeCurrentBoard();
        void pruneHelper(TreeNode* node, unordered_set<TreeNode*>* stagedForDelete);
        void removeNullChildren(TreeNode* node);
        void deleteDeadNodes(unordered_set<TreeNode*>* stagedForDelete);

        void populateAllPointers(TreeNode* node);
        void destroyTree();

};

#endif