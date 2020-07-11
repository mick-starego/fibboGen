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

#include "FibboGen.h"
#include "TreeNode.h"

using namespace std;

FibboGen::FibboGen(int start, int order) {

    this->start = start;
    this->target = -1;

    this->order = order;
    size = calculateSize(order);

    board = new bool[size];

    for (int i = 0; i < size; i++) {
        board[i] = true;
    }
    board[start] = false;

    solutions = 0;
    step = -1;
    parentNode = NULL;
    head = NULL;

    setDirections();

    for (int i = 0; i < size - 2; i++) {
        boardStates.push_back(new unordered_set<int>());
    }
}

FibboGen::~FibboGen() {
    delete[] dirOne;
    delete[] dirTwo;
    delete[] dirThree;
    delete[] board;

    destroyTree();
    for (int i = 0; i < size - 2; i++) {
        delete boardStates[i];
    }
}

void FibboGen::destroyTree() {
    if (parentNode != NULL) {
        populateAllPointers(parentNode);
        unordered_set<TreeNode*>::iterator it = allPointers.begin();
        for (; it != allPointers.end(); it++) {
            delete *it;
        }
        delete parentNode;
        parentNode = NULL;
        head = NULL;
        allPointers.clear();
        for (int i = 0; i < size - 2; i++) {
            delete boardStates[i];
        }
        boardStates.clear();
        for (int i = 0; i < size - 2; i++) {
            boardStates.push_back(new unordered_set<int>());
        }
    }
}

void FibboGen::populateAllPointers(TreeNode* node) {
    if (node->getNumChildren() == 0) {
        allPointers.insert(node);
    } else {
        vector<TreeNode*>::iterator it = node->getChildrenBegin();
        for (; it != node->getChildrenEnd(); it++) {
            populateAllPointers(*it);
            allPointers.insert(*it);
        }
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

void FibboGen::generateSolutionTree() {
    resetBoard();

    destroyTree();
    head = new TreeNode(encodeCurrentBoard(), 0);
    parentNode = head;
    backtrack(1);

    for (int i = 1; i < size - 2; i++) {
        unordered_set<int>* boards = boardStates[i - 1];
        unordered_set<int>::iterator it = boards->begin();
        for (; it != boards->end(); it++) {
            parentNode = getNodeByData(head, *it);
            setBoard(*it);
            backtrack(parentNode->getStep() + 1);
        }
    }

    parentNode = head;
    prune();
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
                
                if (boardStates[step - 1]->count(encodeCurrentBoard()) != 0) {
                    TreeNode* existingNode = getNodeByData(head, encodeCurrentBoard());
                    parentNode->addChild(existingNode);
                    existingNode->setParent(parentNode);
                } else {
                    TreeNode* newNode = new TreeNode(encodeCurrentBoard(), step);
                    boardStates[step - 1]->insert(encodeCurrentBoard());
                    parentNode->addChild(newNode);
                    newNode->setParent(parentNode);
                }

                *(dir[i]) = 1;
                *(dir[i+1]) = 1;
                *(dir[i+2]) = 0;
                
            } else if (*(dir[i]) == 0 && *(dir[i+1]) == 1 && *(dir[i+2]) == 1) {
                *(dir[i]) = 1;
                *(dir[i+1]) = 0;
                *(dir[i+2]) = 0;

                if (boardStates[step - 1]->count(encodeCurrentBoard()) != 0) {
                    TreeNode* existingNode = getNodeByData(head, encodeCurrentBoard());
                    parentNode->addChild(existingNode);
                    existingNode->setParent(parentNode);
                } else {
                    TreeNode* newNode = new TreeNode(encodeCurrentBoard(), step);
                    boardStates[step - 1]->insert(encodeCurrentBoard());
                    parentNode->addChild(newNode);
                    newNode->setParent(parentNode);
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
    if (node->getStep() == 13) {
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
        cout << "Move " << i + 1 << ": " << boardStates[i]->size() << endl;
    }
    unordered_set<int>::iterator it = boardStates.back()->begin();
    for (; it != boardStates.back()->end(); it++) {
        bitset<32> x(*it);
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

void test1() {
    FibboGen gen(0, 5);
    bool allSuccess = true;
    int paths = 0;
    int bottomNodes = 0;

    gen.generateSolutionTree();
    paths = countPathsToBottom(gen.getParentNode());
    bottomNodes = countNodesOnBottom(gen.getParentNode());
    if (paths == 29760) {
        cout << "PASS Start Position 0 Correct Number of Paths" << endl;
    } else {
        allSuccess = false;
        cout << "FAIL Start Position 0 Yielded " << paths << " paths" << endl;
    }
    if (bottomNodes == 4) {
        cout << "PASS Start Position 0 Correct Number of Bottom Nodes" << endl;
    } else {
        allSuccess = false;
        cout << "FAIL Start Position 0 Yielded " << bottomNodes << " bottom nodes" << endl;
    }

    gen.setStart(1);
    gen.generateSolutionTree();
    paths = countPathsToBottom(gen.getParentNode());
    bottomNodes = countNodesOnBottom(gen.getParentNode());
    if (paths == 14880) {
        cout << "PASS Start Position 1 Correct Number of Paths" << endl;
    } else {
        allSuccess = false;
        cout << "FAIL Start Position 1 Yielded " << paths << " paths" << endl;
    }
    if (bottomNodes == 4) {
        cout << "PASS Start Position 1 Correct Number of Bottom Nodes" << endl;
    } else {
        allSuccess = false;
        cout << "FAIL Start Position 1 Yielded " << bottomNodes << " bottom nodes" << endl;
    }

    gen.setStart(3);
    gen.generateSolutionTree();
    paths = countPathsToBottom(gen.getParentNode());
    bottomNodes = countNodesOnBottom(gen.getParentNode());
    if (countPathsToBottom(gen.getParentNode()) == 85258) {
        cout << "PASS Start Position 3 Correct Number of Paths" << endl;
    } else {
        allSuccess = false;
        cout << "FAIL Start Position 3 Yielded " << paths << " paths" << endl;
    }
    if (bottomNodes == 5) {
        cout << "PASS Start Position 3 Correct Number of Bottom Nodes" << endl;
    } else {
        allSuccess = false;
        cout << "FAIL Start Position 3 Yielded " << bottomNodes << " bottom nodes" << endl;
    }

    gen.setStart(4);
    gen.generateSolutionTree();
    paths = countPathsToBottom(gen.getParentNode());
    bottomNodes = countNodesOnBottom(gen.getParentNode());
    if (countPathsToBottom(gen.getParentNode()) == 1550) {
        cout << "PASS Start Position 4 Correct Number of Paths" << endl;
    } else {
        allSuccess = false;
        cout << "FAIL Start Position 4 Yielded " << paths << " paths" << endl;
    }
    if (bottomNodes == 1) {
        cout << "PASS Start Position 4 Correct Number of Bottom Nodes" << endl;
    } else {
        allSuccess = false;
        cout << "FAIL Start Position 4 Yielded " << bottomNodes << " bottom nodes" << endl;
    }

    if (allSuccess) {
        cout << endl << "SUCCESS All tests passed" << endl;
    } else {
        cout << endl << "FAIL All tests DID NOT pass" << endl;
    }
}

void test2() {
    FibboGen gen(0, 5);
    int paths = 0;
    int bottomNodes = 0;

    int expectedNumPaths = (29760 * 3) + (85258 * 3) + (1550 * 3) + (14880 * 6);
    int expectedBottomNodes = 15;

    gen.generateSolutionTree();
    paths = countPathsToBottom(gen.getParentNode());
    bottomNodes = countNodesOnBottom(gen.getParentNode());
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
}

void generate() {
    srand(time(NULL));

    FibboGen gen(rand() % 15, 5);
    gen.generateSolutionTree();
    TreeNode* head = gen.getParentNode();

    string buffer = gen.constructBoardString(head->getData());

    TreeNode* selectedNode = head;
    for (int i = 0; i < 13; i++) {
        int selector = rand() % selectedNode->getNumChildren();
        selectedNode = selectedNode->getChildByIndex(selector);
        buffer += gen.constructBoardString(selectedNode->getData());
    }
    cout << buffer << endl;
}

int main(int argc, char** argv) {
    test1();
    // generate();

    return 0;
}