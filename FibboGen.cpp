#include <iostream>
#include <set>
#include <iomanip>
#include <string>
#include <stack>
#include <fstream>
#include <sstream>
#include <vector>

#include "FibboGen.h"

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

    setDirections();
}

FibboGen::~FibboGen() {
    delete[] dirOne;
    delete[] dirTwo;
    delete[] dirThree;
    delete[] board;
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

//! Solve the puzzle. Sets moves, solveTime, and solved
/*!
 * Solves the puzzle unless the target position is not in 
 * the positions set. In this case, this method will return
 * false.
 *
 * If the target position is in the positions set, the following
 * operations will occur:
 *
 * 1. All solution variables (moves, solved, solveTime) are
 *    reset to thier initial values.
 * 2. The board is reset to its initial value; that is, all
 *    spaces on the board except for the starting position
 * 3. The puzzle is solved using backtrack() and each move is
 *    pushed onto the moves vector, which acts as a stack (the
 *    last element in moves is the first move)
 * 4. solveTime is set
 * 5. solved is set to true if there are 14 moves on the stack,
 *    false otherwise.
 * 6. Reset the board to its initial value.
 * 
 * \todo Create string representation for end position of the puzzle
 *
 * \returns solved
 */
bool FibboGen::solve() {
    
    // Check if target is in positions. If not, return false
    //! \todo uncomment
    // if (positions.find(target) == positions.end()) {
    //     return false;
    // }

    // Clear all solution variables and reset the board
    clear();

    // Store the starting position of the board. Bocktrack will
    // not push this position onto the moves stack. It will be 
    // pushed individually after backtrack exits. An argument of
    // -1 to boardString indicates that no position should be 
    // highlighted.
    string startPos = boardString(-1);

    //! TODO: Create a string representation of the end position of
    // the puzzle.
    // Here is where this string is pushed onto moves
    moves.push_back("END");

    backtrack();

    moves.push_back(startPos); // push the startPos onto stack

    solved = moves.size() == size;

    // Reset the board to its initial state
    resetBoard();

    return solved;
}

//! Counts the number of solutions from the start point
/*!
 * Returns the number of soultions with a given start point.
 * Does not change the state of any instance variables. This
 * method may take about 0.2 seconds to complete.
 * 
 * This method is only meant to be used for testing to ensure 
 * that backtrack is capable of finding all possible solutions. 
 * The values it produces can be compared to those at 
 * http://peggamesolutions.com/index.html.
 *
 * NOTE: The solutions variable in Peg is only meant to be
 * accessed by this method. Outside of this method, it
 * will always be set to zero. Its only purpose is to keep
 * a running count of solutions during one run of backtrack.
 *
 * \returns number of a soulutions from the start point
 * 
 */
int FibboGen::countSolutions() {

    // Reset board to its initial position. Even though
    // the board should already be in this state, this should
    // always be done before calling backtrack.
    resetBoard();

    solutions = 0;

    // This method needs to call backtrack with a target of -1, so that
    // it does not stop on any one solution and instead cycles through
    // all of them. This method should not change the value 
    // of target, so its original value is stored and reset after
    // backtrack is called.
    int originalTarget = target;
    target = -1;

    // Backtrack is called with setPositions=false and target=-1.
    // This tells backtrack to NOT manipulate positions or moves, 
    // to not stop after finding one solution, and to keep a running
    // total of the number solutions found using the solutions 
    // instance variable.
    backtrack();

    target = originalTarget; // reset target

    return solutions;
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
    board[start] = false;
}

//! Clear all solution variables
/*!
 * Remove all elements from moves, set solved to false,
 * and set solveTime to 0. The board is then reset to its 
 * initial state.
 */
void FibboGen::clear() {
    moves.clear();
    solved = false;
    resetBoard();
    solutions = 0;
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

bool FibboGen::backtrack() {
    bool* (*dir) = NULL;
    bool _solved = false;

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
                
                // Check if this move solved the puzzle or keep backtracking
                if ( (checkSolved() && target != -1) || backtrack()) {
                    _solved = true;
                    moves.push_back(boardString(findPos(dir[i+2]))); // push move onto stack
                }

                *(dir[i]) = 1;
                *(dir[i+1]) = 1;
                *(dir[i+2]) = 0;

                if (_solved) return true; // exit and backtrack out if solved
                
            } else if (*(dir[i]) == 0 && *(dir[i+1]) == 1 && *(dir[i+2]) == 1) {
                *(dir[i]) = 1;
                *(dir[i+1]) = 0;
                *(dir[i+2]) = 0;

                // Check if this move solved the puzzle or keep backtracking
                if ( (checkSolved() && target != -1) || backtrack()) {
                    _solved = true;
                    moves.push_back(boardString(findPos(dir[i]))); // push move onto stack
                }
                
                *(dir[i]) = 0;
                *(dir[i+1]) = 1;
                *(dir[i+2]) = 1;

                if (_solved) return true;
            }
        }
    }
    return false;
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

void test() {
    FibboGen gen(0, 5);
    int sol = gen.countSolutions();
    if (sol == 29760) {
        cout << "PASS Start Position 0" << endl;
    } else {
        cout << "FAIL Start Position 0 Yielded " << sol << " solutions" << endl;
    }

    gen.setStart(1);
    sol = gen.countSolutions();
    if (sol == 14880) {
        cout << "PASS Start Position 1" << endl;
    } else {
        cout << "FAIL Start Position 1 Yielded " << sol << " solutions" << endl;
    }

    gen.setStart(3);
    sol = gen.countSolutions();
    if (sol == 85258) {
        cout << "PASS Start Position 3" << endl;
    } else {
        cout << "FAIL Start Position 3 Yielded " << sol << " solutions" << endl;
    }

    gen.setStart(4);
    sol = gen.countSolutions();
    if (sol == 1550) {
        cout << "PASS Start Position 4" << endl;
    } else {
        cout << "FAIL Start Position 4 Yielded " << sol << " solutions" << endl;
    }
}

int main(int argc, char** argv) {
    test();

    return 0;
}