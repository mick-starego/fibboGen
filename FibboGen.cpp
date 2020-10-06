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
    count = 0;

    this->order = order;
    size = calculateSize(order);

    board = new bool[size];

    for (int i = 0; i < size; i++) {
        board[i] = true;
    }

    solutions = 0;
    step = -1;
    parentNode = NULL;
    head = NULL;

    setDirections();

    for (int i = 0; i < size - 1; i++) {
        boardNodes.push_back(new unordered_map<int, TreeNode*>());
    }
}

FibboGen::~FibboGen() {
    delete[] dirOne;
    delete[] dirTwo;
    delete[] dirThree;
    delete[] board;

    destroyTree();
    for (int i = 0; i < size - 1; i++) {
        delete boardNodes[i];
    }
}

void FibboGen::destroyTree() {
    if (parentNode != NULL) {
        timer t;
        t.start();
        populateAllPointers(parentNode);
        t.stop();
        cout << "All pointers populated in " << t << " milliseconds" << endl;

        unordered_set<TreeNode*>::iterator it = allPointers.begin();
        for (; it != allPointers.end(); it++) {
            delete *it;
        }
        delete parentNode;
        parentNode = NULL;
        head = NULL;
        allPointers.clear();
        for (int i = 0; i < size - 1; i++) {
            delete boardNodes[i];
        }
        boardNodes.clear();
        for (int i = 0; i < size - 1; i++) {
            boardNodes.push_back(new unordered_map<int, TreeNode*>());
        }
    }
}

void FibboGen::populateAllPointers(TreeNode* node) {
    unordered_map<int, TreeNode*>::iterator it;
    for (int i = 0; i < size - 1; i++) {
        it = boardNodes[i]->begin();
        for (; it != boardNodes[i]->end(); it++) {
            allPointers.insert(it->second);
        }
    }
}

int FibboGen::getNodeCount() {
    allPointers.clear();
    populateAllPointers(parentNode);
    int count = allPointers.size();
    allPointers.clear();
    return count;
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

    // for (int i = 0; i < (size + order - 5); i++) {
    //     if (dirOne[i] == NULL) {
    //         cout << "NULL ";
    //         continue;
    //     }
    //     cout << *(dirOne[i]) << " ";
    // }
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

//! Resets the board to its initial state
/*!
 * Sets all positions on the board to true except
 * for start.
 */
void FibboGen::resetBoard() {
    for (int i = 0; i < size; i++) {
        board[i] = true;
    }
    if (start >= 0) {
        board[start] = false;
    }
}

//! Clear all solution variables
/*!
 * Remove all elements from moves, set solved to false,
 * and set solveTime to 0. The board is then reset to its 
 * initial state.
 */
void FibboGen::clear() {
    resetBoard();
}

//! Access private member solved
/*! 
 * \return true if solved, false otherwise 
 */
bool FibboGen::isSolved() {
    return solved;
}

//! Access private member start
/*! 
 * \return start
 */
int FibboGen::getStart() {
    return start;
}

int FibboGen::getSize() {
    return size;
}

//! Set private member start
/*! 
 * Sets the starting position of the puzzle; i.e. the 
 * only position on the board that will start without
 * a peg.
 *
 * This method clears all solution variables (moves, solved
 * solveTime), resets the board to its initial state, and
 * uses setPositions() to recalculate the possible target
 * positions from the new starting point.
 *
 * \param start the starting position of the puzzle
 */
void FibboGen::setStart(int start) {
    this->start = start;
    clear();
}

//! Access private member target
/*! 
 * \return target
 */
int FibboGen::getTarget() {
    return target;
}

//! Set private member target
/*! 
 * Sets the target position of the puzzle; i.e. the 
 * only position on the board that will have a peg
 * in it when the puzzle is solved.
 *
 * This method clears all solution variables (moves, solved
 * solveTime), and resets the board to its initial state.
 *
 * \param target the end position for the puzzle
 */
void FibboGen::setTarget(int target) {
    this->target = target;
    clear();
}

int FibboGen::getStep() {
    return step;
}

TreeNode* FibboGen::getParentNode() {
    return parentNode;
}

int FibboGen::getNodeCountAtLayer(int level) {
    if (level >= 0 && level < boardNodes.size()) {
        return boardNodes[level]->size();
    } else {
        return -1;
    }
}

void FibboGen::generateSolutionTree() {
    setStart(-1);
    destroyTree();

    // Full board
    head = new TreeNode(encodeCurrentBoard(), 0);
    parentNode = head;

    for (int i = 0; i < size; i++) {
        setStart(i);
        TreeNode* newNode = new TreeNode(encodeCurrentBoard(), 1);
        newNode->addParent(head);
        boardNodes[0]->insert({encodeCurrentBoard(), newNode});
    }
    cout << "Added head to children" << endl;

    for (int i = 1; i < size - 1; i++) {
        unordered_map<int, TreeNode*>* boards = boardNodes[i - 1];
        unordered_map<int, TreeNode*>::iterator it = boards->begin();
        for (; it != boards->end(); it++) {
            parentNode = it->second;
            setBoard(it->first);
            backtrack(parentNode->getStep() + 1);
        }
        cout << "Step " << i << " complete" << endl;
    }

    parentNode = head;
    cout << "Begin prune" << endl;
    timer t;
    t.start();
    // prune();
    t.stop();
    cout << "Prune complete in " << t << " milliseconds" << endl;
}

void FibboGen::backtrack(int step) {
    bool* (*dir) = NULL;

    for (int d = 0; d < 3; d++) {

        if (d == 0) {
            dir = dirOne;
        } else if (d == 1) {
            dir = dirTwo;
        } else {
            dir= dirThree;
        }
        
        for (int i = 0; i < (size - 2); i++) {
            if (dir[i] == NULL || dir[i+1] == NULL || dir[i+2] == NULL) continue;

            if (*(dir[i]) == 1 && *(dir[i+1]) == 1 && *(dir[i+2]) == 0) {
                *(dir[i]) = 0;
                *(dir[i+1]) = 0;
                *(dir[i+2]) = 1;
                unordered_map<int, TreeNode*>::iterator it = boardNodes[step - 1]->find(encodeCurrentBoard());
                
                if (it != boardNodes[step - 1]->end()) {
                    TreeNode* existingNode = it->second;
                    existingNode->addParent(parentNode);
                } else {
                    TreeNode* newNode = new TreeNode(encodeCurrentBoard(), step);
                    boardNodes[step - 1]->insert({encodeCurrentBoard(), newNode});
                    newNode->addParent(parentNode);
                }

                *(dir[i]) = 1;
                *(dir[i+1]) = 1;
                *(dir[i+2]) = 0;
                
            } else if (*(dir[i]) == 0 && *(dir[i+1]) == 1 && *(dir[i+2]) == 1) {
                *(dir[i]) = 1;
                *(dir[i+1]) = 0;
                *(dir[i+2]) = 0;
                unordered_map<int, TreeNode*>::iterator it = boardNodes[step - 1]->find(encodeCurrentBoard());

                if (it != boardNodes[step - 1]->end()) {
                    TreeNode* existingNode = it->second;
                    existingNode->addParent(parentNode);
                } else {
                    TreeNode* newNode = new TreeNode(encodeCurrentBoard(), step);
                    boardNodes[step - 1]->insert({encodeCurrentBoard(), newNode});
                    newNode->addParent(parentNode);
                }
                
                *(dir[i]) = 0;
                *(dir[i+1]) = 1;
                *(dir[i+2]) = 1;
            }
        }
    }
}

int FibboGen::findPos(bool* ptr) {
    for (int i = 0; i < size; i++) {
        if (&(board[i]) == ptr) return i;
    }
    return -1;
}

bool FibboGen::checkSolved() {
    int count = 0;
    int oneIndex = 0;
    for (int i = 0; i < size; i++) {
        if (board[i] == 1) {
            oneIndex = i;
        }
        count += board[i];
    }
    if (count == 1) {

        if (target == -1) {
            solutions++;
            return true;
        } else {
            return oneIndex == target;
        }
    }
    return false;
}

int FibboGen::encodeCurrentBoard() {
    int enc = 0;
    for (int i = 0; i < size; i++) {
        enc *= 2;
        if (board[i] == 1) enc++;
    }
    return enc;
}

void FibboGen::prune() {
    markNodesToKeep();
    cout << "Pruning" << endl;

    for (int i = 0; i < boardNodes.size(); i++) {

        vector<TreeNode*> stagedForErasure;

        unordered_map<int, TreeNode*>::iterator it = boardNodes[i]->begin();
        for (; it != boardNodes[i]->end(); it++) {
            if (!(it->second->isMarkedToKeep())) {
                stagedForErasure.push_back(it->second);
            }
        }

        vector<TreeNode*>::iterator eraseIt = stagedForErasure.begin();
        for (; eraseIt != stagedForErasure.end(); eraseIt++) {
            setBoard((*eraseIt)->getData());
            boardNodes[i]->erase(encodeCurrentBoard());
            delete *eraseIt;
        }
    }
}

void FibboGen::markNodesToKeep() {
    unordered_map<int, TreeNode*>::iterator bottomLayerIt = boardNodes[boardNodes.size() - 1]->begin();
    for (; bottomLayerIt != boardNodes[boardNodes.size() - 1]->end(); bottomLayerIt++) {
        bottomLayerIt->second->markToKeep();
    }

    for (int i = boardNodes.size() - 1; i > 0; i--) {
        cout << "Marking layer " << i << endl;
        unordered_map<int, TreeNode*>::iterator it = boardNodes[i]->begin();

        for (; it != boardNodes[i]->end(); it++) {
            if (it->second->isMarkedToKeep()) {
                vector<TreeNode*>::iterator parentsIt = it->second->getParentsBegin();

                for (; parentsIt != it->second->getParentsEnd(); parentsIt++) {
                    (*parentsIt)->markToKeep();
                }
            }
        }
    }
}

void FibboGen::deleteDeadNodes(unordered_set<TreeNode*>* stagedForDelete) {
    unordered_set<TreeNode*>::iterator it = stagedForDelete->begin();
    for (; it != stagedForDelete->end(); it++) {
        for (int i = 0; i < size -1; i++) {
            boardNodes[i]->erase((*it)->getData());
        }
        delete *it;
    }
}

string FibboGen::boardString(int position) {

    string s = "\n";

    int leftpad = 0;
    int nextIndex = 0;
    string space = " ";
    string theChar;

    for (int i = 0; i < order; i++) {
        leftpad = (1 + 3 * ( order - 1 ) ) - i * 3;

        for (int j = 0; j < leftpad; j++) {
            s += space;
        }

        for (int j = 0; j < i + 1; j++) {
            if (nextIndex == position && board[nextIndex]) {
                theChar = "*";
            } else if (board[nextIndex]) {
                theChar = "X";
            }
            else {
                theChar = ".";
            }

            if (j != i) {
                s += theChar + "     ";
            } else {
                s += theChar + "\n";
            }
            nextIndex++;
        }
        s += "\n\n";
    }
    return s;
}

void FibboGen::printMoves() {
    if (!solved) {
        cout << "Not Solved." << endl;
    }
    for (int i = moves.size() - 1; i >= 0; i--) {
        cout << moves[i] << endl;
    }
}

void FibboGen::printBoardStateCounts() {
    for (int i = 0; i < size - 2; i++) {
        cout << "Move " << i + 1 << ": " << boardNodes[i]->size() << endl;
    }
    unordered_map<int, TreeNode*>::iterator it = boardNodes.back()->begin();
    for (; it != boardNodes.back()->end(); it++) {
        bitset<32> x(it->first);
        cout << x << endl;
    }
}

string FibboGen::constructBoardString(int enc) {
    bool tempBoard[size];
    for (int i = 0; i < size; i++) {
        tempBoard[i] = board[i];
    }
    setBoard(enc);
    string s = boardString(-1);
    for (int i = 0; i < size; i++) {
        board[i] = tempBoard[i];
    }
    return s;
}

unordered_set<TreeNode*>& FibboGen::getAllNodes() {
    unordered_set<TreeNode*>& allNodes = *(new unordered_set<TreeNode*>());
    allPointers.clear();
    populateAllPointers(parentNode);
    allNodes = allPointers;
    allNodes.insert(parentNode);
    allPointers.clear();
    return allNodes;
}

void FibboGen::writeBinaryFile() {
    bool* universe = new bool[pow(2, size)] { 0 };

    for (int i = 0; i < boardNodes.size(); i++) {
        unordered_map<int, TreeNode*>::iterator it = boardNodes[i]->begin();
        for (; it != boardNodes[i]->end(); it++) {
            universe[it->first] = true;
        }
    }

    ofstream wf;
    wf.open("peg_solns_" + to_string(order) + ".dat", ios::binary | ios::out);

    int nodeCount = 0;
    int i = 0;
    unsigned char bitBuffer;
    int fileSize = pow(2, size);
    while (i < fileSize) {
        for (int j = 0; j < 8; j++) {
            if (i + j < fileSize) {
                bitBuffer |= universe[i+j] << j;
                nodeCount += universe[i+j];
            }
        }
        wf.write((const char *) &bitBuffer, sizeof(bitBuffer));
        bitBuffer = 0;
        i += 8;
    }
    wf.close();

    cout << "bin node count: " << nodeCount << endl;

    delete[] universe;
}

/*
 * Formats:
 * 0 - Typescript Map (.ts)
 * 1 - CSV including num_children (.csv)
 * 2 - Binary
 */
void FibboGen::generate(int format) {
    generateSolutionTree();

    int nodeCount = 0;
    cout << endl;
    for (int i = 0; i < getSize() - 1; i++) {
        cout << "Layer " << i << " has " << getNodeCountAtLayer(i) << " nodes" << endl;
        nodeCount += getNodeCountAtLayer(i);
    }
    cout << "Total num nodes: " << nodeCount << endl;

    prune();

    nodeCount = 0;
    cout << endl;
    for (int i = 0; i < getSize() - 1; i++) {
        cout << "Layer " << i << " has " << getNodeCountAtLayer(i) << " nodes" << endl;
        nodeCount += getNodeCountAtLayer(i);
    }
    cout << "Total num nodes: " << nodeCount << endl;

    return;

    string fileExt;
    if (format == 0) {
        fileExt = ".ts";
    } else if (format == 1) {
        fileExt = ".csv";
    } else {
        writeBinaryFile();
        return;
    }

    // ofstream file;
    // file.open("peg_solns_" + to_string(order) + fileExt);

    // if (format == 0) {
    //     // Header for ts file
    //     file << "\nexport class Peg" << order << "Data {\n\tstatic readonly DATA = new Map([\n";
    // } else if (format == 1) {
    //     // Header for csv file
    //     file << "TREE OF ALL PEG GAME SOLUTIONS\nORDER: " << order << "\n-------------------------------\n" << endl;
    //     file << "node,num_children,children" << endl;
    // }

    // if (format == 0) file << "\t\t[ ";
    // file << head->getData() << ",";
    // if (format == 1) file << head->getNumChildren();

    // if (format == 0) file << " [";
    // for (int i = 0; i < head->getNumChildren(); i++) {
    //     if (format == 1) file << ",";
    //     file << head->getChildByIndex(i)->getData();
    //     if (format == 0 && i + 1 != head->getNumChildren()) file << ", ";
    // }
    // if (format == 0) file << "]],";
    // file << "\n";

    // for (int i = 0; i < boardNodes.size(); i++) {
    //     unordered_map<int, TreeNode*>::iterator it = boardNodes[i]->begin();
    //     for (; it != boardNodes[i]->end(); it++) {

    //         if (format == 0) file << "\t\t[ ";
    //         file << it->second->getData() << ",";
    //         if (format == 1) file << it->second->getNumChildren();

    //         if (format == 0) file << " [";
    //         for (int i = 0; i < it->second->getNumChildren(); i++) {
    //             if (format == 1) file << ",";
    //             file << it->second->getChildByIndex(i)->getData();
    //             if (format == 0 && i + 1 != it->second->getNumChildren()) file << ", ";
    //         }
    //         if (format == 0) file << "]],";
    //         file << "\n";
    //     }
    // }
    // if (format == 0) file << "\t]);\n}";
    // file.close();
}

bool validateOrder(char* arg) {
    int order = stoi(arg);
    if (order > 4 && order < 8) return true;
    return false;
}

bool validateAndSetFormatArgs(char* arg0, char* arg1, int& format) {
    string option(arg0);
    string value(arg1);

    if (option == "--format" || option == "-f" || option == "-F" || option == "--FORMAT") {
        if (value == "ts" || value == ".ts") {
            format = 0;
        } else if (value == "csv" || value == ".csv") {
            format = 1;
        } else if (value == "bin") {
            format = 2;
        } else {
            cout << value << " is not a valid format" << endl;
            return false;
        }
    } else {
        cout << option << " is not a valid option" << endl;
        return false;
    }
    return true;
}

bool validateMode(char* arg) {
    string mode(arg);
    if (mode == "-r" || mode == "-R") {
        return true;
    }
    return false;
}

int getDefaultFormat(int order) {
    if (order == 5) return 0;
    if (order == 6) return 1;
    return 2;
}

bool validateAndSetArguments(int argc, char** argv, int& order, int& format, bool& mode) {
    if (argc < 2) {
        cout << "Must provide an argument for 'order'" << endl;
        return 1;
    } else {
        if (!validateOrder(argv[1])) {
            cout << argv[1] << " is not a valid argument for 'order'" << endl;
            return 1;
        } else {
            order = stoi(argv[1]);
        }

        if (argc == 2) {
            // set defaults and exit
            format = getDefaultFormat(order);

        } else if (argc == 3) {
            // argv[2] must be -r
            bool valid = validateMode(argv[2]);
            if (!valid) {
                cout << argv[2] << " is not a valid argument for 'mode'" << endl;
                return 1;
            } else {
                format = 2;
                mode = false;
            }

        } else if (argc == 4) {
            // argv[2] must be --format, argv[3] must be ts, csv, or bin
            if (!validateAndSetFormatArgs(argv[2], argv[3], format)) return 1;

        } else {
            cout << "Too many arguments" << endl;
            return 1;
        }
    }
    return 0;
}

void deleteRefs(int& order, int& format, bool& mode) {
    delete &order;
    delete &format;
    delete &mode;
}

void FibboGen::readTreeAndGetRandSoln(int order) {
    ifstream in;
    in.open("peg_solns_" + to_string(order) + ".dat", ios::binary | ios::in);

    unsigned char c;
    int nodeCount = 0;
    int bitCount = 0;
    int fileSize = pow(2, size) / 8;
    for (int j = 0; j < fileSize ; j++) {
        in.read((char *) &c, 1);
        for (int i = 0; i < 8; i++) {
            nodeCount += c % 2;
            c >>= 1;
            bitCount++;
        }
    }

    in.close();

    cout << "Read in node count: " << nodeCount << endl;
    cout << "Bit count: " << bitCount << endl;
}

int main(int argc, char** argv) {
    int& order = *(new int(0)); // order is required. No default
    int& format = *(new int(0)); // default is set separately
    bool& mode = *(new bool(true)); // default is write

    bool status = validateAndSetArguments(argc, argv, order, format, mode);
    if (status) {
        deleteRefs(order, format, mode);
        return 1;
    }

    FibboGen gen(order);

    if (mode) {
        gen.generate(format);
    } else {
        cout << "I don't know how to read yet!" << endl;
        gen.readTreeAndGetRandSoln(order);
    }

    deleteRefs(order, format, mode);
    return 0;
}