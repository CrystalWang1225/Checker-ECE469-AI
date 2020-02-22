#ifndef game_hpp
#define game_hpp

#include "umh.hpp"
#include <stdio.h>
#include <algorithm>
#include <time.h>
#include <iostream>
#include <limits>
#include <list>
#include <assert.h>
#include <algorithm>
#include <cctype>
#include <iostream>

template <class T>
class pointers
{
private:
        T* pData;
    public:
        pointers(T* pVal): pData(pVal) {}
        ~pointers()    {delete pData;}
        T& operator*() {return *pData;}
        T* operator->() {return pData;}
};
class game
{
    pointers<board> curBoard;
    static const int maxDepth;
    moven* bestMove;
    moven* tempBestMove;
    int maxD, curDepth;
    bool timeUp, gameOver, theEnd;
    time_t startTime, endTime, startTime2, endTime2;
    void printGame();
    void whosTurn();
    void isAITurn();
    int miniMaxAlpha(pointers<board>&, int, int, int);
    void endMessage();
    void tieMessage();

public:
    game();
    void startGame();
};


#endif /* GAME_HPP */

