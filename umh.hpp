
#ifndef UMH_H_
#define UMH_H_

#include <cctype>
#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <vector>
#include <unistd.h>
#include <cstdlib>
#include <limits>


using namespace std;
/* Sources from https://en.wikipedia.org/wiki/ANSI_escape_code */
const string NOSQCOLOR =   "48;5;0";
const string VALIDSQCOLOR     =    "48;5;7";
const string PLAYER1COLOR     =    "38;5;35";
const string PLAYER2COLOR     =    "38;5;20";
extern int numNonJumps;

#define P1RegPiece    'q'
#define P2RegPiece    'b'
#define P1King        'Q'
#define P2King        'B'
#define EmptyPiece    ' '

class moven;
class jump;



class board
{
public:
    char pieces[8][4];//for each row there are only four slots available to move
    char color;
    list<moven*> moveList;
    board();
    ~board();
    board(const board&);
    int cornerCase(char, char);
    int heuristic();
    void startUp();
    bool tieGame;
    static bool isAgent[2];
    static int timeLimit;
    static void selectPlayer();
    static void convertMoves(const string&);
    char getTurn(){ return color; }
    char *colorPieces(char);
    bool isAgentTurn();
    bool terminal(); 
    bool movesAvailable();
    bool isValidPos(int, int);
    bool jumpsAvailable();
    void jumpAvailable(std::list<jump*>&, char c, int, int, jump*);
    bool jumpConditions(int, int, int, int);
    bool listMoves();
    void nextTurn();
    void turnKing(const int&, const int&);
    void reset();
    void createJump(std::list<jump*>&, char, int, int, int, int, int, int, jump*);
    void createJumpMove(std::list<jump*>&);
    void checkNeighbors(int&, int&);
    void createMove(const int&, const int&, int, int);
    void convert(const int&, const int&,string&);
    void printBoard();
    void printMoves();
    void makeMove(moven*);
    void undoMove(moven*);
    void inputCommand();
    int createKey(int, int, int, int, int, int);
    int reverse(int);
    friend class game;
    friend class heuritstic;
    template <class T> friend class pointers;
};

class jump
{
public:
    int numTimes, xs, ys, x, y, xend, yend, key;
    jump* prev;
    char jumpingPiece;
    bool noNextMove;
    char c;
    jump(char jpingp, char piece, int xs, int ys, int xc, int yc, int xe, int ye, jump* p, int k):
        prev(p), jumpingPiece(jpingp), noNextMove(true), numTimes(0), c(piece), xs(xs), ys(ys),
         x(xc), y(yc), xend(xe), yend(ye), key(k){}
    friend class move;
    friend class board;
    friend class heuritistic;
};


class moven
{
    char movePiece;
    int xi, yi, xf, yf;
    string command;
    list<jump*> jumpPoints;
    moven(char c, int xs, int ys, int xe, int ye): movePiece(c), xi(xs), yi(ys), xf(xe), yf(ye) {}
    ~moven();
    friend class board;
    friend class game;
    friend class heuristic;
};


#endif /* UMH_HPP_ */
