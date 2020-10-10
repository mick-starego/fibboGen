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
#include <map>

#include "FibboGen.h"
#include "TreeNode.h"
#include "timer.h"

using namespace std;

float getRand();

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
        boardNodes.push_back(new unordered_set<__int128>());
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

int FibboGen::getSize() {
    return size;
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

void FibboGen::setStart(int pos) {
    for (int i = 0; i < size; i++) {
        if (i == pos) {
            board[i] = 0;
        } else {
            board[i] = 1;
        }
    }
}

void FibboGen::setBoard(__int128 enc) {
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

__int128 FibboGen::encodeCurrentBoard() {
    __int128 enc = 0;
    for (int i = 0; i < size; i++) {
        enc *= 2;
        if (board[i] == 1) enc++;
    }
    return enc;
}

unordered_set<__int128>* FibboGen::getChildren(__int128 node) {
    __int128 currentBoard = encodeCurrentBoard();
    unordered_set<__int128>* result = new unordered_set<__int128>();
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

            if (*(dir[i]) == 1 && *(dir[i+1]) == 1 && *(dir[i+2]) == 0) {
                *(dir[i]) = 0;
                *(dir[i+1]) = 0;
                *(dir[i+2]) = 1;
                
                result->insert(encodeCurrentBoard());

                *(dir[i]) = 1;
                *(dir[i+1]) = 1;
                *(dir[i+2]) = 0;
                
            } else if (*(dir[i]) == 0 && *(dir[i+1]) == 1 && *(dir[i+2]) == 1) {
                *(dir[i]) = 1;
                *(dir[i+1]) = 0;
                *(dir[i+2]) = 0;
                
                result->insert(encodeCurrentBoard());
                
                *(dir[i]) = 0;
                *(dir[i+1]) = 1;
                *(dir[i+2]) = 1;
            }
        }
    }
    setBoard(currentBoard);
    return result;
}

unordered_set<__int128>* FibboGen::getBottomLayer() {
    int bottom = 0;
    for (int i = size - 2; i >= 0; i--) {
        if (boardNodes[i]->size() > 0) {
            bottom = i;
            break;
        }
    }
    return boardNodes[bottom];
}

unordered_set<__int128>* FibboGen::getNextLayer() {
    int bottom = 0;
    for (int i = size - 2; i >= 0; i--) {
        if (boardNodes[i]->size() > 0) {
            bottom = i;
            break;
        }
    }
    if (bottom != size - 2) {
        return boardNodes[bottom + 1];
    } else {
        return NULL;
    }
}

int FibboGen::getScore(__int128 enc, int* matrix) {
    __int128 temp = encodeCurrentBoard();
    setBoard(enc);

    int score = 0;
    for (int i = 0; i < size; i++) {
        if (board[i] == 1) {
            score += matrix[i];
        }
    }

    setBoard(temp);
    return score;
}

void FibboGen::stepDown(float load, int* matrix) {

    unordered_set<__int128>* bottom = getBottomLayer();
    unordered_set<__int128>* next = getNextLayer();
    unordered_set<__int128>* children = NULL;
    unordered_set<__int128>::iterator childIt;
    multimap<int, __int128> scoreMap;

    int numNodes = load * bottom->size();

    unordered_set<__int128>::iterator it = bottom->begin();
    for (; it != bottom->end(); it++) {
        children = getChildren(*it);
        for (childIt = children->begin(); childIt != children->end(); childIt++) {
            scoreMap.insert({getScore(*childIt, matrix), *childIt});
        }
        delete children;
        children = NULL;
    }
    auto mapIt = scoreMap.cbegin();
    int numAdded = 0;
    for (; mapIt != scoreMap.cend(); mapIt++) {
        if (next->find(mapIt->second) == next->end()) {
            next->insert(mapIt->second);
            numAdded++;
        }

        if (numAdded >= numNodes) {
            break;
        }
    }
}

void FibboGen::takeFirstStep(float load, int* matrix) {

    unordered_set<__int128>* next = getBottomLayer();
    unordered_set<__int128>* children = NULL;
    unordered_set<__int128>::iterator childIt;
    multimap<int, __int128> scoreMap;

    children = getChildren(encodeCurrentBoard());
    for (childIt = children->begin(); childIt != children->end(); childIt++) {
        scoreMap.insert({getScore(*childIt, matrix), *childIt});
    }
    delete children;

    auto mapIt = scoreMap.cbegin();
    int numAdded = 0;
    for (; mapIt != scoreMap.cend(); mapIt++) {
        next->insert(mapIt->second);
        numAdded++;

        if (numAdded >= ceil(load)) {
            break;
        }
    }
}


int* FibboGen::getBottomLayerMatrix(int numOnes) {
    int* rawMatrix = new int[size] {0};
    unordered_set<__int128>* bottom = getBottomLayer();

    if (bottom->size() == 0) {
        for (int i = 0; i < size; i++) {
            rawMatrix[i] += board[i];
        }
    }

    unordered_set<__int128>::iterator it = bottom->begin();
    for (; it != bottom->end(); it++) {
        __int128 enc = *it;
        for (int i = size - 1; i >= 0; i--) {
            if (enc % 2 == 1) {
                rawMatrix[i]++;
            }
            enc /= 2;
        }
    }

    multimap<int, __int128> scoresAndIndices;
    for (int i = 0; i < size; i++) {
        scoresAndIndices.insert({rawMatrix[i] ,i});
    }
    delete[] rawMatrix;

    int* matrix = new int[size] {0};
    int numAdded = 0;
    auto mapIt = scoresAndIndices.crbegin();
    for (; mapIt != scoresAndIndices.crend(); mapIt++) {
        matrix[mapIt->second]++;
        numAdded++;

        if (numAdded >= numOnes) {
            break;
        }
    }
    return matrix;
}

int getStepsA(int size){
    return size - floor((float) size / 2) - 1;
}

int getStepsB(int size) {
    return size - ceil((float) size / 2) - 1;
}

__int128 reverse(__int128 a, __int128 size) {
    __int128 b = 0;
    for (int i = 0; i < size; i++) {
        b *= 2;
        b += a % 2;
        a /= 2;
    }
    return b;
}

__int128 getComplement(__int128 a, __int128 size) {
    __int128 b = 0;
    for (int i = 0; i < size; i++) {
        b *= 2;
        if (a % 2 == 0) {
            b++;
        }
        a /= 2;
    }
    return reverse(b, size);
}

bool doesComplementExist(unordered_set<__int128>* a, unordered_set<__int128>* b, int size) {
    cout << "Searching for complements... " << a->size() << " " << b->size() << endl;
    unordered_set<__int128>::iterator itA = a->begin();

    for (; itA != a->end(); itA++) {
        if (b->find(getComplement(*itA, size)) != b->end()) {
            cout << "FOUND!!!" << endl;
            // cout << *itA << endl << getComplement(*itA, size) << endl;
        }
    }
    return false;
}

bool getSoln(int order, int start, int target, float load1, int boundary1, float load2, int boundary2, float load3) {

    FibboGen genA(order);
    FibboGen genB(order);

    int size = genA.getSize();

    // Calculate how many steps for A and B to take
    int stepsA = getStepsA(size);
    int stepsB = getStepsB(size);

    genA.setStart(start);
    genB.setStart(target);

    int numSteps = 0;
    int stepsATaken = 0;
    int stepsBTaken = 0;
    int* matrix = genB.getBottomLayerMatrix(size - 1);

    while (numSteps < stepsA || numSteps < stepsB) {
        numSteps++;

        if (numSteps <= stepsA) {
            cout << endl << "Step A" << numSteps << endl;
            stepsATaken++;

            cout << "Matrix: [";
            for (int i = 0; i < size; i++) {
                cout << " " << matrix[i];
            }
            cout << " ]" << endl;

            if (numSteps == 1) {
                genA.takeFirstStep(load1, matrix);
            } else if (numSteps < boundary1) {
                genA.stepDown(load1, matrix);
            } else if (numSteps < boundary2) {
                genA.stepDown(load2, matrix);
            } else {
                genA.stepDown(load3, matrix);
            }

            delete[] matrix;
            matrix = genA.getBottomLayerMatrix(size - stepsATaken - 1);
        }
        
        if (numSteps <= stepsB) {
            cout << endl << "Step B" << numSteps << endl;
            stepsBTaken++;

            cout << "Matrix: [";
            for (int i = 0; i < size; i++) {
                cout << " " << matrix[i];
            }
            cout << " ]" << endl;

            if (numSteps == 1) {
                genB.takeFirstStep(load1, matrix);
            } else if (numSteps < boundary1) {
                genB.stepDown(load1, matrix);
            } else if (numSteps < boundary2) {
                genB.stepDown(load2, matrix);
            } else {
                genB.stepDown(load3, matrix);
            }

            delete[] matrix;
            matrix = genB.getBottomLayerMatrix(size - stepsBTaken - 1);
        }
    }

    cout << endl << "Steps taken by A: " << stepsATaken << endl;
    cout << "Steps taken by B: " << stepsBTaken << endl;

    delete[] matrix;

    return doesComplementExist(genA.getBottomLayer(), genB.getBottomLayer(), size);
}

int main(int argc, char** argv) {
    srand(time(NULL));
    string load1;
    string boundary1;
    string load2;
    string boundary2;
    string load3;

    cout << "Load 1: ";
    cin >> load1;
    cout << "Boundary 1: ";
    cin >> boundary1;
    cout << "Load 2: ";
    cin >> load2;
    cout << "Boundary 2: ";
    cin >> boundary2;
    cout << "Load 2: ";
    cin >> load3;

    getSoln(stoi(argv[1]), stoi(argv[2]), stoi(argv[3]), stof(load1), stoi(boundary1), stof(load2), stoi(boundary2), stof(load3));
    return 0;
}