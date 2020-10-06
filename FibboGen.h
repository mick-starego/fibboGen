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

        void generate(int mode);
        void readTreeAndGetRandSoln(int order);
        
        int getStart();
        void setStart(int start);
        int getTarget();
        void setTarget(int target);
        int getSize();
        int getStep();
        int getNodeCount();
        TreeNode* getParentNode();
        unordered_set<TreeNode*>& getAllNodes();
        int getNodeCountAtLayer(int level);

        void clear();
        bool isSolved();
        void generateSolutionTree();
        void prune();
        void getParentsTest();

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
        int parentCount; // remove!
        int count; // remove!
        TreeNode* parentNode;
        TreeNode* head;
        unordered_set<TreeNode*> allPointers;

        vector<string> moves;
        vector< unordered_set<int>* > boardNodes;
        vector<TreeNode*> path;

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
        vector<TreeNode*>* getParents(unordered_map<int, TreeNode*>* candidates, TreeNode* node);
        vector<TreeNode*>* getChildren(unordered_map<int, TreeNode*>* candidates, TreeNode* node);
        void removeNullChildren(TreeNode* node);
        void deleteDeadNodes(unordered_set<TreeNode*>* stagedForDelete);
        void markNodesToKeep();

        void populateAllPointers(TreeNode* node);
        void destroyTree();

        void writeBinaryFile();

};

#endif