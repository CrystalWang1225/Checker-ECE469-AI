/* Contains Minimax alpha beta prunning*/
#include "game.hpp"
#include "umh.hpp"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <iostream>
#include <limits>
#include <list>
#include <time.h>
using namespace std;
const int game::maxDepth = 15; //assume the maximum iterative depth as 15

int game::miniMaxAlpha(pointers<board>& sboard, int depth, int alpha, int beta){
    if (depth != maxD && sboard->terminal()){
        curDepth = maxD;
        theEnd = true;
        if (sboard->getTurn() == P1RegPiece){
            return numeric_limits<int>::max();
        }
        else {
            return numeric_limits<int>::min();
        }
    }
    theEnd = false;
    list<moven*>::iterator it = sboard ->moveList.begin();
    if (depth == 0){
        return sboard -> heuristic();
    }
    
    int compareAlpha = numeric_limits<int>::min();
    int compareBeta = numeric_limits<int>::max();
    if (sboard ->getTurn() == P2RegPiece){
        for (;it != sboard ->moveList.end(); ++it){
            time(&endTime);
            if (difftime(endTime, startTime) >= (board::timeLimit)){
                timeUp = true;
                break;
            }
            sboard->makeMove(*it);
            pointers<board> freshBoard(new board(*sboard));
            int v = miniMaxAlpha(freshBoard, depth -1, alpha, min(compareBeta, beta));
            sboard -> undoMove(*it);
            sboard -> nextTurn();
            if (v > alpha){ // if v has higher value, update alpha
                alpha = v;
                if (depth == maxD){
                    tempBestMove = (*it);
                }
            }
            if (alpha >= beta && depth < maxD){
                return alpha;
            }
        }
            if (!timeUp && depth == maxD){
                curDepth = depth;
            }
    
            return alpha;
        }
            else {
                for (; it != sboard->moveList.end(); ++it){
                    time (&endTime);
                    if (difftime(endTime, startTime) >= board::timeLimit -1){
                        timeUp = true;
                        break;
                    }
                    sboard ->makeMove(*it);
                    pointers<board> freshBoard(new board(*sboard));
                    int v = miniMaxAlpha(freshBoard, depth -1, max(compareAlpha, alpha), beta);
                    sboard ->undoMove(*it);
                    sboard ->nextTurn();
                    if (v < beta){ // if the value of v is smaller than v, update beta
                        beta = v;
                        if (depth == maxD){
                            tempBestMove = (*it);
                        }
                    }
                    if (alpha >= beta){
                        return beta;
                    }
                }
                if (!timeUp && depth == maxD){
                    curDepth = depth;
                }
                return beta;
            }
        }
        


game:: game() : curBoard(pointers<board>(new board())), bestMove(NULL), tempBestMove(NULL), maxD(0), curDepth(0), timeUp(false), gameOver(false), theEnd(false), startTime(0), startTime2(0), endTime2(0){
    
}

void game::startGame(){
    gameOver = false;
    curBoard -> startUp();
    while (!gameOver){
        printGame();
    }
    if (gameOver == true){
        cout << "The game has come to the end" << endl;
    }
}

void game:: tieMessage(){
    bool option = true;
    gameOver = true;
    cout << "It is a tie!!" << endl;
    cout <<"The game has ended, do you want to play again ?" << endl;
    char answer;
    cin >> answer;
    while (option){
        if (tolower(answer) == 'y'){
            option = false;
            startGame();
        }
        else if (tolower(answer) == 'n'){
            option = false;
        }
        else{
            cout << "You typed something wrong, valid input : Y/N" << endl;
            cin >> answer;
        }
    }
}


void game::endMessage(){
    cout << "The game has ended." << endl;
    bool option = true;
    gameOver = true;
    if (curBoard -> getTurn() == P1RegPiece){
        cout << "Player 1 wins !!!" << endl;
    }
    else {
        cout << "Player 2 wins !！" << endl;
    }
    char answer;
    cout << "Do you want to play again ?" << endl;
    cin >> answer;
    while (option == true){
        if (tolower(answer) == 'y'){
            option = false;
            startGame();
        }
        else if (tolower(answer) == 'n'){
            option = false;
            cout << "Thank you for playing and bye bye" << endl;
            exit(-1);
        }
        else {
            cout << "Invalid input, please enter Y/N" << endl;
            cin >> answer;
        }
    }
}

void game::printGame(){
    if (curBoard ->tieGame){
        tieMessage();
    }
    curBoard -> printBoard();
    if (curBoard ->terminal()){
        endMessage();
    }
    else if (curBoard -> isAgentTurn()){
        isAITurn();
    }
    else {
        curBoard -> inputCommand();
    }
}

void game::whosTurn(){
    curBoard -> makeMove(bestMove);
    cout << "Search completed to depth" << curDepth << endl;
    // if hasnt reached to the end in the given time or hasnt got to the maximum path
    if (theEnd == false && curDepth != maxDepth && timeUp){
        cout << "Ran out of time searching after depth " << curDepth << endl;
    }
    cout << "Total search time : " << difftime(endTime, startTime) << "seconds" << endl;
    cout << "AI chosed the move : " ;
    board::convertMoves(bestMove -> command);
    cout << endl;
    timeUp = false;
    theEnd = false;
    bestMove = NULL;
    tempBestMove = NULL;
}

void game::isAITurn(){
    curBoard -> printMoves();
    cout << "Please wait, AI is making a move" << endl;
    if (curBoard ->moveList.size() == 1){
        bestMove = curBoard -> moveList.front();
        time(&startTime);
        time(&endTime);
        curDepth = 0 ;
    }
    else {
        time (&startTime);
        for (int i = 0; i != maxDepth; ++i){
            time(&startTime2);
            maxD = i;
            miniMaxAlpha(curBoard, i, numeric_limits<int>::min(), numeric_limits<int>::max());
            time(&endTime2);
            if (difftime(endTime2, startTime2) >= (board::timeLimit)){
                time(& endTime);
                timeUp = true;
                break;
            }
            if (timeUp == true){
                break;
            }
            else {
                bestMove = tempBestMove;
            }
            if (theEnd == true){
                break;
            }
        }
    }
    whosTurn();
}
                    
