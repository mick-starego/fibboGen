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
#include <string.h>

#include "FibboGen.h"
#include "TreeNode.h"
#include "timer.h"
#include "Hypers.h"

using namespace std;

float getRand() {
    return rand() / (float) RAND_MAX;
}

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
    dirOne = new bool*[size + order - 2];
    dirTwo = new bool*[size + order - 2];
    dirThree = new bool*[size + order - 2];
    
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
        if (vecIt->size() < 2) continue;

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
    while (colsLeft > 1) {
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
    while (colsLeft > 1) {
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

__int128 FibboGen::getEncodedBoard() {
    return encodeCurrentBoard();
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

unordered_set<__int128>* FibboGen::getParents(__int128 node) {
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
        
        for (int i = 0; i < (size + order - 2); i++) {
            if (dir[i] == NULL || dir[i+1] == NULL || dir[i+2] == NULL) continue;

            if (*(dir[i]) == 0 && *(dir[i+1]) == 0 && *(dir[i+2]) == 1) {
                *(dir[i]) = 1;
                *(dir[i+1]) = 1;
                *(dir[i+2]) = 0;
                
                result->insert(encodeCurrentBoard());

                *(dir[i]) = 0;
                *(dir[i+1]) = 0;
                *(dir[i+2]) = 1;
                
            } else if (*(dir[i]) == 1 && *(dir[i+1]) == 0 && *(dir[i+2]) == 0) {
                *(dir[i]) = 0;
                *(dir[i+1]) = 1;
                *(dir[i+2]) = 1;
                
                result->insert(encodeCurrentBoard());
                
                *(dir[i]) = 1;
                *(dir[i+1]) = 0;
                *(dir[i+2]) = 0;
            }
        }
    }
    setBoard(currentBoard);
    return result;
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
        
        for (int i = 0; i < (size + order - 2); i++) {
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
        } else if (matrix[i] == 0) {
            score += 1;
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
    // int totalScore = 0;
    for (; mapIt != scoreMap.cend(); mapIt++) {
        if (next->find(mapIt->second) == next->end()) {
            next->insert(mapIt->second);
            // totalScore += mapIt->first;
            numAdded++;
        }

        if (numAdded >= numNodes) {
            break;
        }
    }
    // for (int i = 0; i < size; i++) {
    //     int c = scoreMap.count(i);
    //     if (c != 0) {
    //         cout << i << ": " << c << endl;
    //     }
    // }
    // cout << "Num Added: " << numAdded;
    // cout << "Average score (lower is better): " << totalScore / numAdded << endl;
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
    int pruneFactor = 7; // MAY WANT TO PASS THIS IN

    if (size - numOnes + 3 > pruneFactor) {
        pruneFactor = 0;
    }

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

        if (numAdded >= numOnes + pruneFactor) {
            break;
        }
    }
    return consolidateMatrix(matrix, pruneFactor);
}

int* FibboGen::consolidateMatrix(int* matrix, int pF) {

    // Initialize neighbor matrix to -1
    int* M_n = new int[size] {0};
    for (int i = 0; i < size; i++) {
        M_n[i] = -1;
    }

    // Load matrix into board
    __int128 hold = getEncodedBoard();
    __int128 enc = 0;
    for (int i = 0; i < size; i++) {
        enc *= 2;
        if (matrix[i] == 1) enc++;
    }
    setBoard(enc);

    // Calculate num neighbors
    bool* (*dir) = NULL;

    for (int d = 0; d < 3; d++) {

        if (d == 0) {
            dir = dirOne;
        } else if (d == 1) {
            dir = dirTwo;
        } else {
            dir = dirThree;
        }
        
        for (int i = 0; i < (size + order - 2); i++) {
            if (dir[i] != NULL && *(dir[i]) == 1) {
                int boardIndex = findBoardIndex(dir[i]);
                if (M_n[boardIndex] == -1) {
                    M_n[boardIndex] = 0;
                } 
                if (i < (size + order - 1) && dir[i+1] != NULL && *(dir[i+1]) == 1) {
                    M_n[boardIndex]++;
                }
                if (i > 0 && dir[i-1] != NULL && *(dir[i-1]) == 1) {
                    M_n[boardIndex]++;
                }
            } 
        }
    }

    // Convert M_n to multimap
    multimap<int, __int128> numNeighbors;
    for (int i = 0; i < size; i++) {
        if (M_n[i] != -1) {
            numNeighbors.insert({M_n[i] ,i});
        }
    }
    delete[] M_n;

    // Remove lowest scores from matrix
    int numAdded = 0;
    auto mapIt = numNeighbors.cbegin();
    for (; mapIt != numNeighbors.cend(); mapIt++) {
        matrix[mapIt->second] = 0;
        numAdded++;

        if (numAdded >= pF) {
            break;
        }
    }

    setBoard(hold);
    return matrix;
}

int FibboGen::findBoardIndex(bool* p) {
    for (int i = 0; i < size; i++) {
        if (p == &(board[i])) {
            return i;
        }
    }
    return -1;
}

// Will segfault if boardNodes is not populated
vector<__int128>* FibboGen::getFirstHalfOfSoln(__int128 enc) {
    int bottom = 0;
    for (int i = size - 2; i >= 0; i--) {
        if (boardNodes[i]->size() > 0) {
            bottom = i;
            break;
        }
    }

    if (boardNodes[bottom]->find(enc) == boardNodes[bottom]->end()) {
        cout << "This board is not valid" << endl;
        return NULL;
    }

    vector<__int128>* solnVec = new vector<__int128>();
    solnVec->push_back(enc);
    __int128 previous = enc;
    unordered_set<__int128>::iterator it;

    for (int i = bottom; i > 0; i--) {
        unordered_set<__int128>* parents = getParents(previous);
        unordered_set<__int128> invalidParents;

        for (it = parents->begin(); it != parents->end(); it++) {
            if (boardNodes[i - 1]->find(*it) == boardNodes[i - 1]->end()) {
                invalidParents.insert(*it);
            }
        }

        for (it = invalidParents.begin(); it != invalidParents.end(); it++) {
            parents->erase(*it);
        }

        int index = getRand() * parents->size();
        int numSkipped = 0;
        for (it = parents->begin(); it != parents->end(); it++) {
            if (numSkipped == index) {
                solnVec->push_back(*it);
                previous = *it;
            }
            numSkipped++;
        }

        delete parents;
    }

    return solnVec;
}

string FibboGen::getBoardString(__int128 current, __int128 next) {
    __int128 hold = encodeCurrentBoard();

    if (next == -1) {
        setBoard(current);
        vector<string>* blank = constructBoardString(-1, -1, -1, 0);

        string s = "\n";
        for (int i = 0; i < blank->size(); i++) {
            s += (*blank)[i] + "\n\n\n";
        }

        setBoard(hold);
        delete blank;
        return s;
    }

    vector<int> changedPegs;
    int target = -1;
    
    __int128 xorResult = current ^ next;
    __int128 andResult = xorResult & next;
    setBoard(andResult);
    for (int i = 0; i < size; i++) {
        if (board[i] == 1) {
            target = i;
        } 
    }

    setBoard(xorResult);
    for (int i = 0; i < size; i++) {
        if (board[i] == 1 && i != target) {
            changedPegs.push_back(i);
        } 
    }

    setBoard(next);
    vector<string>* labeled = constructBoardString(target, changedPegs[0], changedPegs[1], 10);
    vector<string>* blank = constructBoardString(-1, -1, -1, 0);

    string s = "\n";
    for (int i = 0; i < labeled->size(); i++) {
        s += (*labeled)[i] + (*blank)[i] + "\n\n\n";
    }

    setBoard(hold);
    delete labeled;
    delete blank;
    return s;
}

vector<string>* FibboGen::constructBoardString(int to, int from1, int from2, int rightPad) {
    vector<string>* vec = new vector<string>();

    string s = "";
    int leftpad = 0;
    int nextIndex = 0;
    string space = " ";
    string theChar;

    for (int i = 0; i < order; i++) {
        leftpad = (1 + 3 * ( order - 1 ) ) - i * 3;
        leftpad--;

        for (int j = 0; j < leftpad; j++) {
            s += space;
        }

        for (int j = 0; j < i + 1; j++) {
            if (nextIndex == to) {
                theChar = "(+)";
            } else if (nextIndex == from1 || nextIndex == from2) {
                theChar = "(-)";
            } else if (board[nextIndex]) {
                theChar = "(X)";
            } else {
                theChar = "( )";
            }

            if (j != i) {
                s += theChar + "   ";
            } else {
                s += theChar;
                if (rightPad > 0) {
                    for (int j = 0; j < leftpad + rightPad; j++) {
                        s += space;
                    }
                }
                vec->push_back(s);
                s = "";
            }
            nextIndex++;
        }
    }
    return vec;
}

int getStepsA(int size){
    return size - floor((float) size / 2) - 1;
}

int getStepsB(int size) {
    return size - ceil((float) size / 2) - 1;
}

__int128 reverse(__int128 a, int size) {
    __int128 b = 0;
    for (int i = 0; i < size; i++) {
        b *= 2;
        b += a % 2;
        a /= 2;
    }
    return b;
}

__int128 getComplement(__int128 a, int size) {
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

__int128 getComplementIfExists(unordered_set<__int128>* a, unordered_set<__int128>* b, int size, bool log) {
    if (log) cout << "Searching for complements... " << a->size() << " " << b->size() << endl;
    unordered_set<__int128>::iterator itA = a->begin();

    vector<__int128> solutionSet;

    for (; itA != a->end(); itA++) {
        if (b->find(getComplement(*itA, size)) != b->end()) {
            solutionSet.push_back(*itA);
        }
    }
    if (log) cout << "Solutions found: " << solutionSet.size() << endl;

    if (!log && solutionSet.size() == 0) {
        cout << "Sorry! No solutions were found. Try different parameters next time." << endl;
    }

    if (solutionSet.size() > 0) {
        return solutionSet[(int) ( getRand() * solutionSet.size() )];   
    } else {
        return (__int128) 0;
    }
}

bool getSoln(int order, int start, int target, Hypers* hypers, bool log) {

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

    timer t;
    t.start();

    while (numSteps < stepsA || numSteps < stepsB) {
        numSteps++;

        if (numSteps <= stepsA) {
            stepsATaken++;

            if (log) {
                cout << endl << "Step A" << numSteps << endl;
                // cout << "Matrix: [";
                // for (int i = 0; i < size; i++) {
                //     cout << " " << matrix[i];
                // }
                // cout << " ]" << endl;
                __int128 enc = 0;
                for (int i = 0; i < size; i++) {
                    enc *= 2;
                    if (matrix[i] == 1) enc++;
                }
                FibboGen matGen(order);
                cout << matGen.getBoardString(enc, -1) << endl;
            }

            if (numSteps == 1) {
                genA.takeFirstStep(hypers->getLoad1(), matrix);
            } else if (numSteps < hypers->getBound1()) {
                genA.stepDown(hypers->getLoad1(), matrix);
            } else if (numSteps < hypers->getBound2()) {
                genA.stepDown(hypers->getLoad2(), matrix);
            } else {
                genA.stepDown(hypers->getLoad3(), matrix);
            }

            delete[] matrix;
            matrix = genA.getBottomLayerMatrix(size - stepsATaken - 1);
        }
        
        if (numSteps <= stepsB) {
            stepsBTaken++;

            if (log) {
                cout << endl << "Step B" << numSteps << endl;
                // cout << "Matrix: [";
                // for (int i = 0; i < size; i++) {
                //     cout << " " << matrix[i];
                // }
                // cout << " ]" << endl;
                __int128 enc = 0;
                for (int i = 0; i < size; i++) {
                    enc *= 2;
                    if (matrix[i] == 1) enc++;
                }
                FibboGen matGen(order);
                cout << matGen.getBoardString(enc, -1) << endl;
            }

            if (numSteps == 1) {
                genB.takeFirstStep(hypers->getLoad1(), matrix);
            } else if (numSteps < hypers->getBound1()) {
                genB.stepDown(hypers->getLoad1(), matrix);
            } else if (numSteps < hypers->getBound2()) {
                genB.stepDown(hypers->getLoad2(), matrix);
            } else {
                genB.stepDown(hypers->getLoad3(), matrix);
            }

            delete[] matrix;
            matrix = genB.getBottomLayerMatrix(size - stepsBTaken - 1);
        }
    }

    if (log) {
        cout << endl << "Steps taken by A: " << stepsATaken << endl;
        cout << "Steps taken by B: " << stepsBTaken << endl;
    }

    delete[] matrix;

    __int128 complement = getComplementIfExists(genA.getBottomLayer(), genB.getBottomLayer(), size, log);
    if (complement == 0) return false;

    vector<__int128>* halfA = genA.getFirstHalfOfSoln(complement);
    vector<__int128>* halfB = genB.getFirstHalfOfSoln(getComplement(complement, size));

    FibboGen gen(order);
    // Push starting board position onto halfA
    gen.setStart(start);
    halfA->push_back(gen.getEncodedBoard());

    // Push ending board onto halfB
    gen.setStart(target);
    halfB->push_back(gen.getEncodedBoard());

    if ((*halfA)[0] != getComplement((*halfB)[0], size)) {
        cout << "Halves do not intersect" << endl;
    }

    // Build solution vector
    vector<__int128> solnVec;
    for (int i = halfA->size() - 1; i >= 0; i--) {
        solnVec.push_back((*halfA)[i]);
    }
    for (int i = 1; i < halfB->size(); i++) {
        solnVec.push_back(getComplement((*halfB)[i], size));
    }

    if (solnVec.size() != size - 1) {
        cout << "Solution vector is not the correct size" << endl;
    }

    t.stop();
    cout << "Solved in " << t.getTime() << " seconds" << endl;

    // Print solution
    cout << endl << endl;
    cout << "START: Position " << start << " is empty" << endl;
    cout << gen.getBoardString(solnVec[0], -1) << endl << endl;
    cout << "Press Enter to Continue ";
    cin.ignore();
    cout << endl;

    for (int i = 0; i < solnVec.size() - 1; i++) {
        cout << "MOVE " << i + 1 << endl;
        cout << gen.getBoardString(solnVec[i], solnVec[i + 1]) << endl;
        cout << endl;
        cin.ignore();
    }

    cout << "SOLVED! Position " << target << " is filled" << endl;
    cout << gen.getBoardString(solnVec[solnVec.size() - 1], -1) << endl;

    delete halfA;
    delete halfB;

    return true;
}

int main(int argc, char** argv) {
    srand(time(NULL));
    vector<Hypers*> pegHypers = {
        NULL, NULL, NULL, NULL,
        new Hypers(2),
        new Hypers(2),
        new Hypers(2),
        new Hypers(2, 4, 1.6),
        new Hypers(2, 4, 1.6),
        new Hypers(2, 4, 1.6),
        new Hypers(1.9, 5, 1.5),
        new Hypers(1.9, 5, 1.5, 15, 1.4),
        new Hypers(1.9, 5, 1.4, 20, 1.3),
        new Hypers(1.8, 5, 1.4, 25, 1.1),
        new Hypers(1.8, 4, 1.3, 30, 1.1),
        new Hypers(2, 13, 2.5, 20, 0.96),
    };

    int order = stoi(argv[1]);
    if (order < 4 || order > 15) {
        cout << "Order " << order << " is invalid" << endl;
        return 1;
    }

    FibboGen gen(order);
    int start = stoi(argv[2]);
    if (start < 0 || start > gen.getSize() - 1) {
        cout << "Start " << start << " is not valid for order " << order << endl;
        return 1;
    }
    int target = stoi(argv[3]);
    if (target < 0 || target > gen.getSize() - 1) {
        cout << "Target " << target << " is not valid for order " << order << endl;
        return 1;
    }

    bool manual = false;
    bool log = false;
    for (int i = 4; i < argc; i++) {
        if (strncmp(argv[i], "-l", 2) == 0 || strncmp(argv[i], "--log", 5) == 0) {
            log = true;
        }
        if (strncmp(argv[i], "-m", 2) == 0 || strncmp(argv[i], "--manual", 8) == 0) {
            manual = true;
        }
    }

    if (pegHypers[order] == NULL) {
        cout << endl << "*** Manual parameters are required for order " << order << "." << endl << endl;
        manual = true;
    }

    Hypers* hypers = NULL;
    Hypers* manualHypers = NULL;
    if (manual) {
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

        manualHypers = new Hypers(stof(load1), stoi(boundary1), stof(load2), stoi(boundary2), stof(load3));
        hypers = manualHypers;
    } else {
        hypers = pegHypers[order];
    }

    getSoln(order, start, target, hypers, log);
    
    for (int i = 0; i < pegHypers.size(); i++) {
        if (pegHypers[i] != NULL) {
            delete pegHypers[i];
        }
    }
    if (manual) {
        delete manualHypers;
    }
    return 0;
}