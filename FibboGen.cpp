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
    cout << endl;
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
    enc /= 2;
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
        parentNode->addChild(newNode);
        boardNodes[0]->insert({encodeCurrentBoard(), newNode});
    }
    cout << "Added children to head" << endl;

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
    prune();
    t.stop();
    cout << "Prune complete in " << t << " milliseconds" << endl;
}

TreeNode* FibboGen::getNodeByData(TreeNode* node, int data) {
    if (node->getData() == data) {
        return node;
    } else if (node->getNumChildren() == 0) {
        return NULL;
    } else {
        vector<TreeNode*>::iterator it = node->getAllChildren().begin();
        for (; it != node->getAllChildren().end(); it++) {
            TreeNode* foundNode = getNodeByData(*it, data);
            if (foundNode != NULL) {
                return foundNode;
            }
        }
        return NULL;
    }
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
                    parentNode->addChild(existingNode);
                } else {
                    TreeNode* newNode = new TreeNode(encodeCurrentBoard(), step);
                    boardNodes[step - 1]->insert({encodeCurrentBoard(), newNode});
                    parentNode->addChild(newNode);
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
                    parentNode->addChild(existingNode);
                } else {
                    TreeNode* newNode = new TreeNode(encodeCurrentBoard(), step);
                    boardNodes[step - 1]->insert({encodeCurrentBoard(), newNode});
                    parentNode->addChild(newNode);
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
    int enc = 2;
    for (int i = 0; i < size; i++) {
        if (board[i] == 1) {
            enc++;
        }
        enc *= 2;
    }
    return enc;
}

void FibboGen::prune() {
    unordered_set<TreeNode*>* stagedForDelete = new unordered_set<TreeNode*>();
    pruneHelper(parentNode, stagedForDelete);
    deleteDeadNodes(stagedForDelete);
    removeNullChildren(parentNode);
    delete stagedForDelete;
}

void FibboGen::pruneHelper(TreeNode* node, unordered_set<TreeNode*>* stagedForDelete) {
    if (node->getStep() == size - 1) {
        return;
    } else if (node->getNumChildren() == 0) {
        TreeNode* parent = path.back();
        parent->deleteChild(node);
        stagedForDelete->insert(node);
    } else {
        path.push_back(node);
        vector<TreeNode*>::iterator it = node->getChildrenBegin();
        for (; it != node->getChildrenEnd(); it++) {
            TreeNode* child = *it;
            if (child != NULL) {
                pruneHelper(child, stagedForDelete);
            }
        }
        path.pop_back();
        if (node->getNumChildren() == 0) {
            path.back()->deleteChild(node);
            stagedForDelete->insert(node);
        }
    }
}

void FibboGen::removeNullChildren(TreeNode* node) {
    node->removeNullChildren();
    vector<TreeNode*>::iterator it = node->getChildrenBegin();
    for (; it != node->getChildrenEnd(); it++) {
        removeNullChildren(*it);
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

int countPathsToBottom(TreeNode* node) {
    if (node->getNumChildren() == 0) {
        return 1;
    } else {
        vector<TreeNode*>::iterator it = node->getChildrenBegin();
        int count = 0;
        for (; it != node->getChildrenEnd(); it++) {
            count += countPathsToBottom(*it);
        }
        return count;
    }
}

int countNodesOnBottom(TreeNode* node) {
    if (node->getNumChildren() == 0 && !node->ignore()) {
        return 1;
    } else {
        vector<TreeNode*>::iterator it = node->getChildrenBegin();
        int count = 0;
        for (; it != node->getChildrenEnd(); it++) {
            count += countNodesOnBottom(*it);
        }
        return count;
    }
}

void test() {
    FibboGen gen(5);
    int paths = 0;
    int bottomNodes = 0;
    int nodes = 0;

    int expectedNumPaths = (29760 * 3) + (85258 * 3) + (1550 * 3) + (14880 * 6);
    int expectedBottomNodes = 15;
    int expectedNumNodes = 5053;

    gen.generateSolutionTree();
    paths = countPathsToBottom(gen.getParentNode());
    bottomNodes = countNodesOnBottom(gen.getParentNode());
    unordered_set<TreeNode*>& allNodes = gen.getAllNodes();
    nodes = allNodes.size();
    if (paths == expectedNumPaths) {
        cout << "PASS Correct Number of Paths" << endl;
    } else {
        cout << "FAIL Yielded " << paths << " paths, expected " << expectedNumPaths << endl;
    }
    if (bottomNodes == expectedBottomNodes) {
        cout << "PASS Correct Number of Bottom Nodes" << endl;
    } else {
        cout << "FAIL Yielded " << bottomNodes << " bottom nodes, expected " << expectedBottomNodes << endl;
    }
    if (nodes == expectedNumNodes) {
        cout << "PASS Correct Number of Nodes" << endl;
    } else {
        cout << "FAIL Yielded " << nodes << " nodes, expected " << expectedNumNodes << endl;
    }

    delete &allNodes;
}

void generate() {
    srand(time(NULL));

    FibboGen gen(5);
    gen.generateSolutionTree();
    TreeNode* head = gen.getParentNode();

    string buffer = gen.constructBoardString(head->getData());

    TreeNode* selectedNode = head;
    for (int i = 0; i < 14; i++) {
        int selector = rand() % selectedNode->getNumChildren();
        selectedNode = selectedNode->getChildByIndex(selector);
        buffer += gen.constructBoardString(selectedNode->getData());
    }
    cout << buffer << endl;
}

int main(int argc, char** argv) {
    FibboGen gen(stoi(argv[1]));
    gen.generateSolutionTree();

    // cout << endl;
    // for (int i = 0; i < gen.getSize() - 1; i++) {
    //     cout << "Layer " << i << " has " << gen.getNodeCountAtLayer(i) << " nodes" << endl;
    // }

    unordered_set<TreeNode*>& allNodes = gen.getAllNodes();
    cout << "All nodes count: " << allNodes.size() << endl;
    
    ofstream file;
    file.open("peg_solns_6.ts");

    unordered_set<TreeNode*>::iterator it = allNodes.begin();
    file << "export class PegData {\n\tstatic readonly DATA = new Map([\n";
    for (; it != allNodes.end(); it++) {
        file << "\t\t[ ";
        file << (*it)->getData() << ", ";
        vector<TreeNode*>::iterator children = (*it)->getChildrenBegin();
        file << " [";
        for (; children != (*it)->getChildrenEnd(); children++) {
            file << (*children)->getData() << ", ";
        }
        file << "] ],\n";
    }
    file << "\t]);\n}";
    file.close();

    delete &allNodes;
    return 0;
}