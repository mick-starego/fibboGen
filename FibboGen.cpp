#include <iostream>
#include <set>
#include <iomanip>
#include <string>
#include <stack>
#include <fstream>
#include <sstream>
#include <vector>
#include <bitset>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <math.h>

#include "FibboGen.h"
#include "TreeNode.h"
#include "timer.h"

using namespace std;

FibboGen::FibboGen(int order) {

    this->start = -1;
    this->target = -1;
    this->order = order;

    size = calculateSize(order);

    board = new bool[size];

    for (int i = 0; i < size; i++) {
        board[i] = true;
    }

    setDirections();

    for (int i = 0; i < size - 1; i++) {
        boardNodes.push_back(new unordered_set<int>());
    }
}

FibboGen::~FibboGen() {
    delete[] dirOne;
    delete[] dirTwo;
    delete[] dirThree;
    delete[] board;

    for (int i = 0; i < size - 1; i++) {
        delete boardNodes[i];
    }
}

void FibboGen::setDirections() {
    dirOne = new bool*[size + order - 5];
    dirTwo = new bool*[size + order - 5];
    dirThree = new bool*[size + order - 5];
    
    int nextIndex = 0;
    vector<vector<bool*>> allVecs;
    for (int i = 1; i <= order; i++) {
        vector<bool*> vecN;
        for (int j = 0; j < i; j++) {
            vecN.push_back(&board[nextIndex]);
            nextIndex++;
        }
        allVecs.push_back(vecN);
    }

    // dirOne
    vector<vector<bool*>>::iterator vecIt = allVecs.begin();
    int next = 0;
    for (; vecIt != allVecs.end(); vecIt++) {
        if (vecIt->size() < 3) continue;

        for (int i = 0; i < vecIt->size(); i++) {
            dirOne[next] = (*vecIt)[i];
            next++;
        }
        dirOne[next] = NULL;
        next++;
    }

    // dirTwo
    int colsLeft = order;
    next = 0;
    while (colsLeft > 2) {
        for (vecIt = allVecs.begin(); vecIt != allVecs.end(); vecIt++) {
            if ((order - colsLeft) < vecIt->size()) {
                dirTwo[next] = (*vecIt)[order - colsLeft];
                next++;
            }
        }
        dirTwo[next] = NULL;
        next++;
        colsLeft--;
    }

    // dirThree
    colsLeft = order;
    next = 0;
    while (colsLeft > 2) {
        for (vecIt = allVecs.begin(); vecIt != allVecs.end(); vecIt++) {
            if ((order - colsLeft) < vecIt->size()) {
                dirThree[next] = (*vecIt)[vecIt->size() - 1 - order + colsLeft];
                next++;
            }
        }
        dirThree[next] = NULL;
        next++;
        colsLeft--;
    }

    for (int i = 0; i < size; i++){
        cout << board[i] << " ";
    }
    cout << endl;

}

int FibboGen::calculateSize(int order) const {
    int sum = 0;
    for (int i = 1; i <= order; i++) {
        sum += i;
    }
    return sum;
}

void FibboGen::setBoard(int enc) {
    for (int i = size - 1; i >= 0; i--) {
        board[i] = enc % 2;
        enc /= 2;
    }
}

int FibboGen::getNodeCountAtLayer(int level) {
    if (level >= 0 && level < boardNodes.size()) {
        return boardNodes[level]->size();
    } else {
        return -1;
    }
}

int FibboGen::encodeCurrentBoard() {
    int enc = 0;
    for (int i = 0; i < size; i++) {
        enc *= 2;
        if (board[i] == 1) enc++;
    }
    return enc;
}

vector<int>* FibboGen::getChildren(unordered_set<int>* candidates, int node) {
    int currentBoard = encodeCurrentBoard();
    vector<int>* result = new vector<int>();
    setBoard(node);

    bool* (*dir) = NULL;

    for (int d = 0; d < 3; d++) {

        if (d == 0) {
            dir = dirOne;
        } else if (d == 1) {
            dir = dirTwo;
        } else {
            dir = dirThree;
        }
        
        for (int i = 0; i < (size - 2); i++) {
            if (dir[i] == NULL || dir[i+1] == NULL || dir[i+2] == NULL) continue;

            if (*(dir[i]) == 0 && *(dir[i+1]) == 0 && *(dir[i+2]) == 1) {
                *(dir[i]) = 1;
                *(dir[i+1]) = 1;
                *(dir[i+2]) = 0;

                result->push_back(encodeCurrentBoard());
            
                *(dir[i]) = 0;
                *(dir[i+1]) = 0;
                *(dir[i+2]) = 1;
                
            } else if (*(dir[i]) == 1 && *(dir[i+1]) == 0 && *(dir[i+2]) == 0) {
                *(dir[i]) = 0;
                *(dir[i+1]) = 1;
                *(dir[i+2]) = 1;
                
                result->push_back(encodeCurrentBoard());
                
                *(dir[i]) = 1;
                *(dir[i+1]) = 0;
                *(dir[i+2]) = 0;
            }
        }
    }
    setBoard(currentBoard);
    return result;
}

int main(int argc, char** argv) {
    
    return 0;
}