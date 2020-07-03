#ifndef FIBBO_GEN_H
#define FIBBO_GEN_H

#include <iostream>
#include <vector>

using namespace std;

class FibboGen {

    public:
        FibboGen(int start, int order);
        ~FibboGen();
        
        int getStart();
        void setStart(int start);
        int getTarget();
        void setTarget(int target);

        bool solve();
        void clear();
        bool isSolved();
        int countSolutions();

        void printMoves();

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
        vector<string> moves;
        bool solved;

        void setDirections();
        int calculateSize(int order) const;
        void resetBoard();

        bool backtrack();
        int findPos(bool* ptr);
        bool checkSolved();
        string boardString(int position);

};

#endif