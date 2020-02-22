/* UI && Legal Moves, also heuristic functions*/
#include "umh.hpp"
using namespace std;

board::board() {    reset(); }
board::~board()
{
    while (!moveList.empty())
    {
        delete moveList.front();
        moveList.pop_front();
    }
}
bool board::isAgent[2];
char column[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
int board::timeLimit = 0;
int numNonJumps;
bool tieGame = false;
bool jumpAvail = false;


//heuritsic function
int board::heuristic()
{
    int x, y, b, c, d;
     x = y = b = c = d = 0;
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j <4; ++j)
        {
            if (pieces[i][j] == P2RegPiece)
            {
                x = x + 2;
                if (i == 0)
                    b += 9;
                else
                {
                    b += i;
                    
                }
                c += 1;
            }
            else if (pieces[i][j] == P1RegPiece)
            {
                y -=2;
                if (i == 7)
                    b -= 9;
                else b -= (7 - i);
                c -= 1;
            }
            else if (pieces[i][j] == P2King)
            {
                x = x+3;
                c += 1;
            }
            else if (pieces[i][j] == P1King)
            {
                y -= 3;
                c -= 1;
            }
        }
    if (c > 0 && y >= -8)
        d -= cornerCase(P1RegPiece, P2RegPiece);
    else if (c < 0 && x<= 8)
        d += cornerCase(P2RegPiece, P1RegPiece);
    x = x * 500000;
    y *= 500000;
    b *= 100000;
    c *= 1000;
    d *= 100;
    int e = rand() % 100;
    if (color == P1RegPiece)
        e = -e;
    return x + y + b + c + d + e;
}

int board::cornerCase(char a, char b)
{
    
    int c = 0;
    if (tolower(pieces[6][3]) == a || tolower(pieces[7][3]) == a)
      {
          c += 9;
          if (tolower(pieces[4][2]) == b)
              c -= 1;
          if (tolower(pieces[5][2]) == b)
              c -= 2;
          if (tolower(pieces[5][3]) == b)
              c -= 2;
          if (tolower(pieces[6][2]) == b)
              c -= 2;
          if (tolower(pieces[6][3]) == b)
              c -= 3;
          if (tolower(pieces[7][3]) == b)
              c -= 3;
      }
    
    if (tolower(pieces[0][0]) == a || tolower(pieces[1][0]) == a)
    {
        c += 9;
        if (tolower(pieces[0][0]) == b)
            c -= 3;
        if (tolower(pieces[1][0]) == b)
            c -= 3;
        if (tolower(pieces[1][1]) == b)
            c -= 2;
        if (tolower(pieces[2][0]) == b)
            c -= 2;
        if (tolower(pieces[2][1]) == b)
            c -= 2;
        if (tolower(pieces[3][1]) == b)
            c -= 1;
    }
  
    return c;
}

void board::inputCommand()
{
    printMoves();
    cout << "Enter move number: ";
    int moveNum;
    cin >> moveNum;
    while (moveNum - 1 >= moveList.size())
    {
        cout << "This is an invalid move." << endl;
        cout << " Please enter another move: " << endl;
         cin >> moveNum;
    }
    list<moven*>::iterator it = moveList.begin();
    advance(it, moveNum - 1);
    makeMove(*it);

}

void board::startUp()
{
    reset();
    selectPlayer();
    tieGame = false;
  
}
 
void board::selectPlayer()
{
    bool b = true;
    char c ;
    while (b)
    {
        cout << "Will player # 1 be a computer? (Y/N):" << endl;
        cin >> c;
        if (tolower(c) == 'y')
        {
            board::isAgent[0] = true;
            b = false;
        }
        else if (tolower(c) == 'n')
        {
            board::isAgent[0] = false;
            b = false;
        }
    }
    b = true;
    c = ' ';
    while (b)
    {
        cout << "Will player # 2 be a computer? (Y/N):" << endl;
        cin >> c;
        if (tolower(c) == 'y')
        {
            board::isAgent[1] = true;
            b = false;
        }
        else if (tolower(c) == 'n')
        {
            board::isAgent[1] = false;
            b = false;
        }
    }
    if (board::isAgent[0] == true || board::isAgent[1] == true)
    {
        cout << "Enter a time limit for the computer in seconds: " << endl;
        if (timeLimit < 30){
        cin >> timeLimit;
        }
    }
}

board::board(const board& board): color(board.color)
{
    for (int i = 0; i != 8; ++i)
        for (int j = 0; j != 4; ++j)
            pieces[i][j] = board.pieces[i][j];
}

void board::convertMoves(const string& s)
{
    string::const_iterator it = s.begin();
    int temp = (*it) - '0';
    it += 2;
    cout << column[(*it)-'0'] << 8 - temp;
    it += 2;
    while (*it != '-')
    {
        temp = (*it) - '0';
        cout << " -> ";
        it += 2;
        cout << column[(*it)-'0'] << 8 - temp;
        it += 2;
    }
}


bool board::isAgentTurn()
{
        if (color == P2RegPiece && isAgent[0])
            return true;
        if (color == P1RegPiece && isAgent[1])
            return true;
        return false;
}

void board::nextTurn()
{
        if (color == P1RegPiece)
            color = P2RegPiece;
        else
            color = P1RegPiece;
}

bool board::movesAvailable()
{
    if (jumpsAvailable() == true){
        return true;
    }
    if (listMoves() == true){
        return true;
    }
    return false;
}

bool board::terminal() //The terminal condition
{
    if (!movesAvailable()){
        return true;
    }
    return false;
}

void board::turnKing(const int& x, const int& y)
{
    if (x == 0 && pieces[x][y] == P1RegPiece)
        pieces[x][y] = P1King;
    if (x == 7 && pieces[x][y] == P2RegPiece)
        pieces[x][y] = P2King;
}

bool board::isValidPos(int a, int b)
{
    if (a >= 0 && a < 8 && b >= 0 && b < 4)
        return true;
    else
        return false;
}

void board::reset()
{
    color = P2RegPiece;
    for (int i = 0; i != 3; ++i)
        for (int j = 0; j != 4; ++j)
            pieces[i][j] = P2RegPiece;
    for (int i = 3; i != 5; ++i)
        for (int j = 0; j != 4; ++j)
            pieces[i][j] = 'e';//empty set in the middle
    for (int i = 5; i != 8; ++i)
        for (int j = 0; j != 4; ++j)
            pieces[i][j] = P1RegPiece;
}


char *board::colorPieces(char piece) {
    char *buf = (char *) malloc(30);

    if (piece == P1RegPiece) {
        sprintf(buf, "\033[%s;1;%sm %c \033[0m", VALIDSQCOLOR.c_str(), PLAYER1COLOR.c_str(), P1RegPiece);
    }
    else if (piece == P1King) {
        sprintf(buf, "\033[%s;1;%sm %c \033[0m", VALIDSQCOLOR.c_str(), PLAYER1COLOR.c_str(), P1King);
    }
    else if (piece == P2RegPiece) {
        sprintf(buf, "\033[%s;1;%sm %c \033[0m", VALIDSQCOLOR.c_str(), PLAYER2COLOR.c_str(), P2RegPiece);
    }
    else if (piece == P2King) {
        sprintf(buf, "\033[%s;1;%sm %c \033[0m", VALIDSQCOLOR.c_str(), PLAYER2COLOR.c_str(), P2King);
    }
    else {
        sprintf(buf, "\033[%s;1;92m %c \033[0m", VALIDSQCOLOR.c_str(), EmptyPiece);
    }

    return buf;
}

/* source from https://www.daniweb.com/programming/software-development/code/476650/linux-console-ascii-box-drawing-characters */
void board::printBoard() {
    cout << "   \e(0\x6C\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x77\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x77\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x77\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x77\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x77\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x77\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x77\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x6B\e(B" << endl;
    for (int i = 0; i < 8; i++)
    {
        cout << " \033[1;36m" << (8-i ) << "\033[0m ";
        if (i % 2 == 0)
            cout << ("\e(0\x78\e(B\033[" + NOSQCOLOR + ";1;93m   \033[0m");

        for (int j = 0; j < 3; j++)
        {
            char *buf = colorPieces(pieces[i][j]);
            cout << "\e(0\x78\e(B" << buf << "\e(0\x78\e(B" << ("\033[" + NOSQCOLOR + ";1;92m   \033[0m");
            free(buf);
        }

        char *buf = colorPieces(pieces[i][3]);
        cout << "\e(0\x78\e(B" << buf << "\e(0\x78\e(B";
        free (buf);
        if (i % 2 == 1)
            cout << ("\033[" + NOSQCOLOR + ";1;92m   \033[0m\e(0\x78\e(B");
        cout << endl;

        if (i==7)
            cout << "   \e(0\x6D\e(B";
        else
            cout << "   \e(0\x74\e(B";
        if (i < 7)
            cout << "\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x6E\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x6E\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x6E\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x6E\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x6E\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x6E\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x6E\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B";
        else
            cout << "\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x76\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x76\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x76\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x76\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x76\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x76\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x76\e(B\e(0\x71\e(B\e(0\x71\e(B\e(0\x71\e(B";
        if (i==7)
            cout << "\e(0\x6A\e(B";
        else
            cout << "\e(0\x75\e(B";
        cout << endl;
    }

    cout << "\033[1;35m     A   B   C   D   E   F   G   H  \033[0m\n" << endl;
    char regPiece[30], kingPiece[30];
    sprintf(regPiece, "\033[1;%sm%c\033[0m", PLAYER2COLOR.c_str(), P2RegPiece);
    sprintf(kingPiece, "\033[1;%sm%c\033[0m", PLAYER2COLOR.c_str(), P2King);
    cout << "Player 1 Regular Piece: " << regPiece << " King1: " << kingPiece << endl;
    sprintf(regPiece, "\033[1;%sm%c\033[0m", PLAYER1COLOR.c_str(), P1RegPiece);
    sprintf(kingPiece, "\033[1;%sm%c\033[0m", PLAYER1COLOR.c_str(), P1King);
    cout << "Player2 Regular Piece is : " << regPiece << " King2: " << kingPiece << endl;
}

void board::printMoves()
{
    if (!jumpAvail)
        numNonJumps++;
    else
        numNonJumps = 0;
    if (numNonJumps > 65)
        tieGame = true;
    if (color == P1RegPiece)
        cout << "Player 2 turn" << endl;
    else cout << "Player 1 turn" << endl;
    cout << "Available Moves: " << endl;
    list<moven*>::const_iterator it = moveList.begin();
    for (int i = 1; it != moveList.end(); ++it)
    {
        cout << "Move number" << i << ": ";
        convertMoves((*it)->command);
        cout << endl;
        i++;
    }
    cout << endl;
}
/*Above are all board setups */


void board::makeMove(moven* m)
{
    if (!m->jumpPoints.empty())
    {
        list<jump*>::iterator it = m->jumpPoints.begin();
        for (; it != m->jumpPoints.end(); ++it)
            pieces[(*it)->x][(*it)->y] = 'e';
    }
    char c = pieces[m->xi][m->yi];
    pieces[m->xi][m->yi] = 'e';
    pieces[m->xf][m->yf] = c;

    turnKing(m->xf, m->yf);
    nextTurn();
}

void board::undoMove(moven* m)
{
    if (!m->jumpPoints.empty())
    {
        for (list<jump*>::iterator it = m->jumpPoints.begin(); it != m->jumpPoints.end(); ++it)
        {
            pieces[(*it)->xs][(*it)->ys] = 'e';
            pieces[(*it)->x][(*it)->y] = (*it)->c;
            pieces[(*it)->xend][(*it)->yend] = 'e';
        }
    }
    pieces[m->xf][m->yf] = 'e';
    pieces[m->xi][m->yi] = m->movePiece;
}






inline int board::createKey(int xs, int ys, int xj, int yj, int xe, int ye)
{
    return ye + xe*10 + yj*100 + xj*1000 + ys*10000 + xs*100000;
}

int board::reverse(int i)
{
    int num = 0;
    while (i/100 > 0)
    {
        num += i % 100;
        num *= 100;
        i /= 100;
    }
    num += i;
    return num;

}
void board::createJump(list<jump*>& jlist, char c, int xs, int ys, int xj, int yj, int xe, int ye, jump* jp)
{
    pieces[xs][ys] = 'e';
    int key = createKey(xs+1, ys+1, xj+1, yj+1, xe+1, ye+1);
    jump* jcheck = jp;
    while (jcheck != NULL)
    {
        if (key == jcheck->key || key == reverse(jcheck->key))
            return;
        jcheck = jcheck->prev;
    }
    jump* j = new jump(c, pieces[xj][yj], xs, ys, xj, yj, xe, ye, jp, key);
    if (jp != NULL)
        jp->noNextMove = false;
    jlist.push_front(j);
    jumpAvailable(jlist, c, xe, ye, j);
}

void board::createJumpMove(list<jump*>& jlist)
{
    if (!jlist.empty())
    {
        list<jump*>::const_iterator it = jlist.begin();
        for (; it != jlist.end(); ++it)
        {
            if ((*it)->noNextMove)
            {
                moven* m = new moven((*it)->jumpingPiece, -1, -1, -1, -1);
                jump* jp = (*it);
                while (jp != NULL)
                {
                    m->jumpPoints.push_front(jp);
                    ++jp->numTimes;
                    jp = jp->prev;
                }
                m->xi = m->jumpPoints.front()->xs;
                m->yi = m->jumpPoints.front()->ys;
                convert(m->jumpPoints.front()->xs, m->jumpPoints.front()->ys, m->command);
                for (list<jump*>::iterator it = m->jumpPoints.begin(); it != m->jumpPoints.end(); ++it)
                {
                    convert((*it)->xend, (*it)->yend, m->command);
                    if ((*it)->noNextMove)
                    {
                        m->xf = (*it)->xend;
                        m->yf = (*it)->yend;
                    }
                }

                moveList.push_back(m);
                m->command += "-1";
                undoMove(m);
            }
        }
    }
}
void board::jumpAvailable(list<jump*>& jlist, char c, int x, int y, jump* jp= NULL)
{
    if (tolower(c) == P2RegPiece || c == P1King) //direction for p2 regular piece and p1king
    {
        if (x % 2 == 0)
        {
            if (jumpConditions(x+1, y+1, x+2, y+1)){
                createJump(jlist, c, x, y, x+1, y+1, x+2, y+1, jp);
            }
            if (jumpConditions(x+1, y, x+2, y-1)){
                           createJump(jlist, c, x, y, x+1, y, x+2, y-1, jp);
            }
        }
        else
        {
            if (jumpConditions(x+1, y-1, x+2, y-1))
                createJump(jlist, c, x, y, x+1, y-1, x+2, y-1, jp);
            if (jumpConditions(x+1, y, x+2, y+1))
                createJump(jlist, c, x, y, x+1, y, x+2, y+1, jp);
        }
    }
    if (tolower(c) == P1RegPiece || c == P2King) //direction for p1 regular piece and p2 king
    {
        if (x % 2 == 0)
        {
            if (jumpConditions(x-1, y+1, x-2, y+1))
                createJump(jlist, c, x, y, x-1, y+1, x-2, y+1, jp);
            if (jumpConditions(x-1, y, x-2, y-1))
                createJump(jlist, c, x, y, x-1, y, x-2, y-1, jp);
        }
        else
        {
            if (jumpConditions(x-1, y-1, x-2, y-1))
                createJump(jlist, c, x, y, x-1, y-1, x-2, y-1, jp);
            if (jumpConditions(x-1, y, x-2, y+1))
                createJump(jlist, c, x, y, x-1, y, x-2, y+1, jp);
        }
    }
}

bool board::jumpsAvailable()
{
    while (!moveList.empty())
    {
        delete moveList.front();
        moveList.pop_front();
    }
    for (int i = 0; i!= 8; ++i)
    {
        for (int j = 0; j != 4; ++j)
        {
            if (pieces[i][j] == color || pieces[i][j] == toupper(color))
            {
        list<jump*> jList;
        jumpAvailable(jList, pieces[i][j], i, j, NULL);
                createJumpMove(jList);
            }
        }
    }

    if (moveList.empty())
        return (jumpAvail = false);
    return (jumpAvail = true);
}

bool board::jumpConditions(int xj, int yj, int xe, int ye)
{
    if (isValidPos(xj, yj) && isValidPos(xe, ye) && pieces[xj][yj] != 'e' &&
            pieces[xj][yj] != color && pieces[xe][ye] == 'e' &&  pieces[xj][yj] != std::toupper(color))
        return true;
    return false;
}

void board::convert(const int& x, const int& y, string& s)
{
     char c1 = '0' + x;
     char c2;
     if (x % 2 == 0)
     {
         c2 = '0' + (2*y + 1);
     }
     else
     {
         c2 = '0' + (2*y);
     }
     s += c1;
     s += ' ';
     s += c2;
     s += ' ';
}


void board::checkNeighbors(int& x, int& y)
{
    if (tolower(pieces[x][y]) == P2RegPiece || pieces[x][y] == P1King)
    {
        if (x % 2 == 0)
        {
            createMove(x, y, x+1, y);
            createMove(x, y, x+1, y+1);
        }
        else
        {
            createMove(x, y, x+1, y);
            createMove(x, y, x+1, y-1);
        }
    }
    //check for moven
    if (tolower(pieces[x][y]) == P1RegPiece || pieces[x][y] == P2King)
    {
        if (x % 2 == 0)
        {
            createMove(x, y, x-1, y);
            createMove(x, y, x-1, y+1);
        }
        else
        {
            createMove(x, y, x-1, y);
            createMove(x, y, x-1, y-1);
        }
    }
}

void board::createMove(const int& xi,const int& yi, int xf, int yf)
{
    if (isValidPos(xf, yf) && pieces[xf][yf] == 'e')
    {
        moven* m = new moven(pieces[xi][yi], xi, yi, xf, yf);
        moveList.push_back(m);
        convert(xi, yi, m->command);
        convert(xf, yf, m->command);
        m->command += "-1";
    }
}

bool board::listMoves()
{
    while (!moveList.empty())
    {
        delete moveList.front();
        moveList.pop_front();
    }
    if (color == P2RegPiece)
    {
        for (int i = 0; i!= 8; ++i)
            for (int j = 0; j != 4; ++j)
                if (pieces[i][j] == color || pieces[i][j] == toupper(color))
                    checkNeighbors(i, j);
    }
    else
    {
        for (int i = 7; i!= -1; --i)
            for (int j = 0; j != 4; ++j)
                if (pieces[i][j] == color || pieces[i][j] == toupper(color))
                    checkNeighbors(i, j);
    }

    if (moveList.empty())
        return false;
    return true;
}

moven::~moven()
{
    for (list<jump*>::iterator it = jumpPoints.begin(); it != jumpPoints.end(); ++it)
    {
        --(*it)->numTimes;
        if ((*it)->numTimes == 0)
            delete (*it);
    }
}
