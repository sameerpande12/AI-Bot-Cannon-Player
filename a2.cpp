#include <iostream>
#include <unordered_map> 
#include <string>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
//#include <boost/algorithm/string.hpp>
#include <algorithm>
#include <random>
#include <fstream>
#include <chrono>       // std::chrono::system_clock
using namespace std;
int M = 8;
int N = 8;
vector<vector<int> > board;

long long int LeftColumn = 0x8080808080808080;
long long int RightColumn = 0x0101010101010101;
long long int SecondLeftColumn = 0x4040404040404040;
long long int SecondRightColumn = 0x0202020202020202;
long long int ThirdRightColumn = 0x0404040404040404;
long long int ThirdLeftColumn = 0x2020202020202020;
long long int TopRow = 0xFF00000000000000;
long long int BottomRow = 0x00000000000000FF;
long long int TopBottom = TopRow | BottomRow;
long long int LeftRight = LeftColumn | RightColumn;
long long int Rim = TopBottom | LeftRight;
long long int LastTwoLeftRows = LeftColumn|SecondRightColumn;
long long int LastTwoRightRows = RightColumn|SecondRightColumn;
long long int LastThreeLeftRows = LeftColumn|SecondRightColumn|ThirdLeftColumn;
long long int LastThreeRightRows = RightColumn|SecondRightColumn|ThirdRightColumn;
long long int one = 1;

long long int LastFourRightRows = 0x0F0F0F0F0F0F0F0F;
long long int LastFourLeftRows = 0xF0F0F0F0F0F0F0F0;
long long int LastFiveRightRow = 0x1F1F1F1F1F1F1F1F;
long long int LastFiveLeftRows = 0xF1F1F1F1F1F1F1F1;
// incomplete
int begin_co;
int end_co;
bool move_co;
class State;
State *global_state;

vector<vector<int> > boardFrombit(long long int bboard)
{
    vector<vector<int> > myboard;
    vector<int> row(M,0);
    long long int one = 1;
    for(int i = 0;i<N;i++)
        myboard.push_back(row);
    
    for (int pos = 0 ; pos < 64 ; pos++) {
        if (bboard & (one << pos)) {
            myboard[7 - pos/8][7 - pos%8] =1;
        } 
    }
    return myboard;
}

void printBoard(vector<vector<int> > myboard)
{
    for(int i = 0;i<N;i++)
    {
        for(int j =0;j<M;j++)
        {
            cout << myboard[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n";
}

int bitCount(long long i) {
   i = i - ((i >> 1) & 0x5555555555555555L);
   i = (i & 0x3333333333333333L) + ((i >> 2) & 0x3333333333333333L);
   i = (i + (i >> 4)) & 0x0f0f0f0f0f0f0f0fL;
   i = i + (i >> 8);
   i = i + (i >> 16);
   i = i + (i >> 32);
   return (int)(i & 0x7f);
}

class State 
{ 
    public: 
    bool mine;
    long long int MyTownhall;
    long long int OppTownhall;
    long long int MyPawn;
    long long int OppPawn;

    //Cannon indicated by central position: RD = Right Diagonal, LD = Left Diagonal
    long long int MyCannonH;
    long long int OppCannonH;
    long long int MyCannonV;
    long long int OppCannonV;
    long long int MyCannonLD;
    long long int OppCannonLD;
    long long int MyCannonRD;
    long long int OppCannonRD;
    long long int Board;


    State(bool m, long long int mypawn=0x0000000000AAAAAA, long long int mytown = 0x0000000000000055, long long int opptown=0xAA00000000000000, long long int opppawn=0x5555550000000000)
    {
        mine = m;
        //MyTownhall = 0x0000000000000055;
        //OppTownhall= 0xAA00000000000000;
        //MyPawn = 0x0000000000AAAAAA;
        //OppPawn = 0x5555550000000000;
        MyTownhall = mytown;
        OppTownhall= opptown;
        MyPawn = mypawn;
        OppPawn = opppawn;
        MyCannonV = MyPawn & (MyPawn << 8) & (MyPawn >> 8) & (~TopBottom);
        MyCannonH = MyPawn & (MyPawn << 1) & (MyPawn >> 1) & (~LeftRight);
        MyCannonLD = MyPawn & (MyPawn >> 9) & (MyPawn << 9) & (~Rim);
        MyCannonRD = MyPawn & (MyPawn >> 7) && (MyPawn << 7) & (~Rim);
        OppCannonV = OppPawn & (OppPawn << 8) & (OppPawn >> 8) & (~TopBottom);
        OppCannonH = OppPawn & (OppPawn << 1) & (OppPawn >> 1) & (~LeftRight);
        OppCannonLD = OppPawn & (OppPawn >> 9) & (OppPawn << 9) & (~Rim);
        OppCannonRD = OppPawn & (OppPawn >> 7) & (OppPawn << 7) & (~Rim);
        Board = MyTownhall|OppTownhall|MyPawn|OppPawn;
   
    }

    //Function to determine the 'path' we took from one state to another. Specifically needed for shots
    int decipher(int pos)
    {
        long long int temp;
        temp = MyCannonRD & LastFourRightRows & (one << (pos + 21));
        if(temp != 0)
            return (pos+21);
        temp = MyCannonRD & LastFourLeftRows & (one << (pos - 21));
        if(temp != 0)
            return (pos-21);
        temp = MyCannonRD & LastThreeRightRows & (one << (pos + 28));
        if(temp != 0)
            return (pos+28);
        temp = MyCannonRD & LastThreeLeftRows & (one << (pos -28));
        if(temp != 0)
            return (pos-28);
        temp = MyCannonLD & LastFourLeftRows & (one << (pos + 18));
        if(temp != 0)
            return (pos+18);
        temp = MyCannonLD & LastFourRightRows & (one << (pos -18));
        if(temp != 0)
            return (pos-18);
        temp = MyCannonLD & LastThreeLeftRows & (one << (pos + 27));
        if(temp != 0)
            return (pos+27);
        temp = MyCannonLD & LastThreeRightRows & (one << (pos -27));
        if(temp != 0)
            return (pos-27);
        temp = MyCannonV & (one << (pos + 24));
        if(temp != 0)
            return (pos+24);
        temp = MyCannonV & (one << (pos -24));
        if(temp != 0)
            return (pos-24);
        temp = MyCannonV & (one << (pos + 32));
        if(temp != 0)
            return (pos+32);
        temp = MyCannonV & (one << (pos-32));
        if(temp != 0)
            return (pos-32);
        temp = MyCannonH & LastFourLeftRows & (one << (pos + 3));
        if(temp != 0)
            return (pos+3);
        temp = MyCannonH & LastFourRightRows & (one << (pos -3));
        if(temp != 0)
            return (pos-3);
        temp = MyCannonH & LastThreeLeftRows & (one << (pos + 4));
        if(temp != 0)
            return (pos+4);
        return pos-4;
    }

    //Evaluate Function
    int evaluate()
    {
        int TH = bitCount(MyTownhall);
        int OTH = bitCount(OppTownhall);
        int P = bitCount(MyPawn);
        int OP = bitCount(OppPawn);
        return TH + 2*P - OP - OTH;
    }

    //Get Neighbours, combined with alpha beta pruning
    int getNeighbours(int alpha, int beta, int depth)
    {
        int value;
        if(mine)
            value = std::numeric_limits<int>::min();
        else
            value = std::numeric_limits<int>::max();
        
        //Search Till Depth = 3
        if(depth == 3)
            return evaluate();
        
        
        //For maximising player    
        if(mine == true){
            long long int check;
            long long int p;
            long long int shoots =0;

            //Cannon Shots 
            for(int pos =0;pos<64;pos++)
            {
                p = MyCannonV & (one << pos);
                if(p){
                    shoots = shoots  
                    |(~(Board << 8) & (p << 24) & ~MyPawn & ~MyTownhall)
                    |(~(Board << 8) & ~(Board << 16) & (p << 32) & ~MyPawn & ~MyTownhall)
                    |(~(Board >> 8) & (p >> 24) & ~MyPawn & ~MyTownhall)
                    |(~(Board >> 8) & ~(Board >> 16) & (p >> 32) & ~MyPawn & ~MyTownhall);
                }
                p = MyCannonH & (one << pos);
                if(p){
                    shoots = shoots
                    |((~(Board << 1) & (p << 3) & ~MyPawn & ~MyTownhall)&LastFourLeftRows)
                    |(~(Board << 1) & ~(Board << 2) & (p << 4) & ~MyPawn & ~MyTownhall & LastThreeLeftRows)
                    |(~(Board >> 1) & (p >> 3) & ~MyPawn & ~MyTownhall & LastFourRightRows)
                    |(~(Board >> 1) & ~(Board >> 2) & (p >> 4) & ~MyPawn & ~MyTownhall & LastThreeRightRows);
                }
                p = MyCannonLD & (one << pos);
                if(p){
                    shoots = shoots
                    |((~(Board << 9) & (p << 27) & ~MyPawn & ~MyTownhall)&LastFourLeftRows)
                    |(~(Board << 9) & ~(Board << 18) & (p << 36) & ~MyPawn & ~MyTownhall & LastThreeLeftRows)
                    |(~(Board >> 9) & (p >> 27) & ~MyPawn & ~MyTownhall & LastFourRightRows)
                    |(~(Board >> 9) & ~(Board >> 18) & (p >> 36) & ~MyPawn & ~MyTownhall & LastThreeRightRows);
                }
                p = MyCannonRD & (one << pos);
                if(p){
                    shoots = shoots
                    |((~(Board << 7) & (p << 21) & ~MyPawn & ~MyTownhall)&LastFourRightRows)
                    |(~(Board << 7) & ~(Board << 14) & (p << 28) & ~MyPawn & ~MyTownhall & LastThreeRightRows)
                    |(~(Board >> 7) & (p >> 21) & ~MyPawn & ~MyTownhall & LastFourLeftRows)
                    |(~(Board >> 7) & ~(Board >> 14) & (p >> 28) & ~MyPawn & ~MyTownhall & LastThreeLeftRows);
                }
            }
            for(int pos = 0;pos<64;pos++)
            {
                check = shoots & (one << pos);
                if(check)
                {
                    State new_state = State(!mine,MyPawn,MyTownhall,OppTownhall&~check,OppPawn&(~check));
                    int res = new_state.getNeighbours(alpha, beta,depth+1);
                    cout << pos << "\n";
                    if(res > value)
                    {
                        value = res;
                        if(depth ==0){
                            move_co = false; 
                            begin_co = decipher(pos);
                            end_co = pos;
                            //*global_state = new_state;
                        }
                    }
                    if(value > alpha)
                        alpha = value;
                    if(alpha >= beta){
                        return value; 
                    } 
                              
                }
            }

            //Soldier Moves
            for(int pos = 0;pos<64;pos++)
            {
                
                p = MyPawn & (one << pos);
                if(p){
                    
                    long long int NonOpponentPawn = MyTownhall|OppTownhall|MyPawn;
                    //long long int p = 0x0000000000000001;
                    long long int possible = 
                    ((((p << 1) & ~RightColumn) | ((p>>1)&(~LeftColumn))) & OppPawn)
                    |((p<<8|(p<<7 & ~LeftColumn)|(p<<9 & ~RightColumn)) & ~NonOpponentPawn)
                    |(~NonOpponentPawn & (((p<<1|p>>1|p<<8|p<<7|p<<9) & OppPawn) != 0) & ((p>>16)|((p>>18)&(~SecondLeftColumn)&(~LeftColumn))|((p>>14)&(~SecondRightColumn)&(~RightColumn))) )
                
                    |((p << 24)&(MyPawn << 16)&(MyPawn<<8)&~Board)
                    |((p >> 24)&(MyPawn >> 16)&(MyPawn>>8)&~Board)

                    |(~LastThreeRightRows  & (p<<3)&(MyPawn<<2)&(MyPawn<<1)&~Board)
                    |(~LastThreeLeftRows & (p>>3)&(MyPawn>>2)&(MyPawn>>1)&~Board)

                    |(~LastThreeLeftRows  & (p<<27)&(MyPawn<<18)&(MyPawn<<9)&~Board)
                    |(~LastThreeRightRows & (p>>27)&(MyPawn>>18)&(MyPawn>>9)&~Board)

                    |(~LastThreeLeftRows  & (p<<21)&(MyPawn<<14)&(MyPawn<<7)&~Board)
                    |(~LastThreeRightRows & (p>>21)&(MyPawn>>14)&(MyPawn>>7)&~Board);
                   
                    for(int pos2 = 0;pos2<64;pos2++)
                    {
                        check = possible & (one << pos2); 
                        if(check)
                        {
                            State new_state = State(!mine,((MyPawn&~p)|check),MyTownhall,OppTownhall,(OppPawn&(~check)));
                            int res = new_state.getNeighbours(alpha, beta,depth+1);
                            
                            if(res > value)
                            {
                                value = res;
                                if(depth ==0){
                                move_co = true;
                                begin_co = pos;
                                end_co = pos2;
                                *global_state = new_state;
                                }
                            }
                            if(value > alpha)
                                alpha = value;
                            if(alpha >= beta){
                                cout << value << "\n";
                                return value;
                            }

                        }
                    }
                }        
            }
            
        }

        //For minimising player
        else{
            
            long long int check;
            long long int p;
            long long int shoots =0;
            //Cannon Shots
            for(int pos = 0;pos<64;pos++)
            {
                p = OppCannonV & (one << pos);
                if(p){
                    shoots = shoots  
                    |(~(Board << 8) & (p << 24) & ~OppPawn & ~OppTownhall)
                    |(~(Board << 8) & ~(Board << 16) & (p << 32) & ~OppPawn & ~OppTownhall)
                    |(~(Board >> 8) & (p >> 24) & ~OppPawn & ~OppTownhall)
                    |(~(Board >> 8) & ~(Board >> 16) & (p >> 32) & ~OppPawn & ~OppTownhall);
                }
                p = OppCannonH & (one << pos);
                if(p){
                    shoots = shoots
                    |((~(Board << 1) & (p << 3) & ~OppPawn & ~OppTownhall)&LastFourLeftRows)
                    |(~(Board << 1) & ~(Board << 2) & (p << 4) & ~OppPawn & ~OppTownhall & LastThreeLeftRows)
                    |(~(Board >> 1) & (p >> 3) & ~OppPawn & ~OppTownhall & LastFourRightRows)
                    |(~(Board >> 1) & ~(Board >> 2) & (p >> 4) & ~OppPawn & ~OppTownhall & LastThreeRightRows);
                }
                p = OppCannonLD & (one << pos);
                if(p){
                    shoots = shoots
                    |((~(Board << 9) & (p << 27) & ~OppPawn & ~OppTownhall)&LastFourLeftRows)
                    |(~(Board << 9) & ~(Board << 18) & (p << 36) & ~OppPawn & ~OppTownhall & LastThreeLeftRows)
                    |(~(Board >> 9) & (p >> 27) & ~OppPawn & ~OppTownhall & LastFourRightRows)
                    |(~(Board >> 9) & ~(Board >> 18) & (p >> 36) & ~OppPawn & ~OppTownhall & LastThreeRightRows);
                }
                p = OppCannonRD & (one << pos);
                if(p){
                    shoots = shoots
                    |((~(Board << 7) & (p << 21) & ~OppPawn & ~OppTownhall)&LastFourRightRows)
                    |(~(Board << 7) & ~(Board << 14) & (p << 28) & ~OppPawn & ~OppTownhall & LastThreeRightRows)
                    |(~(Board >> 7) & (p >> 21) & ~OppPawn & ~OppTownhall & LastFourLeftRows)
                    |(~(Board >> 7) & ~(Board >> 14) & (p >> 28) & ~OppPawn & ~OppTownhall & LastThreeLeftRows);
                }
            }

            for(int pos = 0;pos<64;pos++)
            {
                check = shoots & (one << pos);
                if(check)
                {
                    State new_state = State(!mine, MyPawn&(~check),MyTownhall&(~check),OppTownhall,OppPawn);
                    int res = new_state.getNeighbours(alpha, beta,depth+1);
                    
                    if(res < value)
                        value = res;
                    if(value < beta)
                        beta = value;
                    if(alpha >= beta){
                        return value; 
                    }                      
                }
            }
            
            //Soldier Moves
            for(int pos = 0;pos<64;pos++)
            {
                p = OppPawn & (one << pos);
                if(p){
                    long long int NonOpponentPawn = OppTownhall|MyTownhall|OppPawn;
                    //long long int p = 0x0000000000000001;
                    long long int possible = 
                    ((((p << 1) & ~RightColumn) | ((p>>1)&(~LeftColumn))) & MyPawn)
                    |((p>>8|(p>>7 & ~RightColumn)|(p>>9 & ~LeftColumn)) & ~NonOpponentPawn)
                    |(~NonOpponentPawn & (((p<<1|p>>1|p>>8|p>>7|p>>9) & MyPawn) != 0) & ((p<<16)|((p<<18)&(~SecondRightColumn)&(~RightColumn))|((p<<14)&(~SecondLeftColumn)&(~LeftColumn))) )
                
                    |((p << 24)&(OppPawn << 16)&(OppPawn<<8)&~Board)
                    |((p >> 24)&(OppPawn >> 16)&(OppPawn>>8)&~Board)

                    |(~LastThreeRightRows  & (p<<3)&(OppPawn<<2)&(OppPawn<<1)&~Board)
                    |(~LastThreeLeftRows & (p>>3)&(OppPawn>>2)&(OppPawn>>1)&~Board)

                    |(~LastThreeLeftRows  & (p<<27)&(OppPawn<<18)&(OppPawn<<9)&~Board)
                    |(~LastThreeRightRows & (p>>27)&(OppPawn>>18)&(OppPawn>>9)&~Board)

                    |(~LastThreeLeftRows  & (p<<21)&(OppPawn<<14)&(OppPawn<<7)&~Board)
                    |(~LastThreeRightRows & (p>>21)&(OppPawn>>14)&(OppPawn>>7)&~Board);
                    for(int pos2 = 0;pos2<64;pos2++)
                    {
                        
                        check = possible & (one << pos2);
                        if(check)
                        {
                            State new_state = State(!mine,(MyPawn&(~check)),MyTownhall,OppTownhall,((OppPawn&~p)|check));
                            int res = new_state.getNeighbours(alpha, beta,depth+1);
                            
                            if(res < value)
                                value = res;
                            if(value < beta)
                                beta = value;
                            if(alpha >= beta)
                                return value;
 
                        }
                    }
                }                
            }
        }
        return value;
    }
    

};

/* 
//Itiliase the global variable 'board' to the cannon initial position
void Initialise()
{
    for(int i= 0;i<1;i++)
        for(int j=0;j<M;j++)
        {
            if(j%2==0)
                board[i][j] = 2;
            else
                board[i][j] = 1;

        }
    for(int i= 1;i<3;i++)
        for(int j=0;j<M;j++)
        {
            if(j%2==1)
                board[i][j] = 1;
        }
    for(int i= N-1;i>N-2;i--)
        for(int j=0;j<M;j++)
        {
            if(j%2==1)
                board[i][j] = 2;
            else
                board[i][j] = 1;

        }
    for(int i =N-1;i>N-4;i--)
        for(int j=0;j<M;j++)
        {
            if(j%2==0)
                board[i][j] = 1;
        }
}

//takes in a board, and move S xs xy M xm xy and outputs a board
vector<vector<int> > returnMove(vector<vector<int> > inputBoard, char s, int xs, int ys, char mov, int xm, int ym)
{
    vector<vector<int> > outputBoard;
    vector<int> row(M,0);
    for(int i = 0;i<N;i++)
        outputBoard.push_back(row);
    cout << outputBoard.size() <<"\n";
    cout << outputBoard.size() << "\n";
     
    for(int i = 0;i<N;i++)
        for(int j =0;j<M;j++)
            outputBoard[i][j] = inputBoard[i][j];

    if(s != 'S')
        return vector<vector<int> >();
    else if(mov == 'M')
    {
        if(outputBoard[ym-1][xm-1] != 0)
            return vector<vector<int> >();
        else if(outputBoard[ys-1][xs-1] == 0)
            return vector<vector<int> >();
        else{
            outputBoard[ym-1][xm-1] = outputBoard[ys-1][xs-1];
            outputBoard[ys-1][xs-1] = 0;
            return outputBoard;
        }
    }
    else if(mov == 'B')
    {
        if(outputBoard[ym-1][xm-1] == 0)
            return vector<vector<int> >();
        else if(outputBoard[ys-1][xs-1] != 1)
            return vector<vector<int> >();
        else{
            outputBoard[ym-1][xm-1] = 0;
            return outputBoard;
        }
    }
    return vector<vector<int> >();
}

//change to the main board
bool makeMove(char s, int xs, int ys, char mov, int xm, int ym)
{
    if(s != 'S')
        return false;
    else if(mov == 'M')
    {
        if(board[ym-1][xm-1] != 0)
            return false;
        else if(board[ys-1][xs-1] == 0)
            return false;
        else{
            board[ym-1][xm-1] = board[ys-1][xs-1];
            board[ys-1][xs-1] = 0;
            return true;
        }
    }
    else if(mov == 'B')
    {
        if(board[ym-1][xm-1] == 0)
            return false;
        else if(board[ys-1][xs-1] != 1)
            return false;
        else{
            board[ym-1][xm-1] = 0;
            return true;
        }
    }
    return false;
}

*/

int main(int argc, char *argv[])
{

    State test(true,0x0000000000AAAAAA,0x0000000000000055, 0xAA00000000000000,0x5555550000000000);
    int neighbours = test.getNeighbours(-std::numeric_limits<int>::max(),std::numeric_limits<int>::max(),0);
    //cout << neighbours << "\n";
    //cout << begin_co << "\n" << end_co << "\n";
    printBoard(boardFrombit(test.Board));

    //cout << bitCount(0x5555550000000003) << "\n";
    //vector<State> neigh;
    
    //long long int b = test.Board;

    
    //cout << test.MyPawn << "\n";
    //printBoard(boardFrombit(b));

    //long long int a = 0x0000000040000000;
    //printBoard(boardFrombit(test.getNeighbours(a)));

    //a = 0x0000000000002000;
    //printBoard(boardFrombit((~LastThreeRightRows  & (a<<27)&(test.MyPawn<<18)&(test.MyPawn<<9)&~test.Board)));

    //a = 0x0000000000008000;
    //printBoard(boardFrombit(LastThreeRightRows));
    // printBoard(boardFrombit(SecondRightColumn));
    //  printBoard(boardFrombit(ThirdRightColumn));

    //c = 0x1000010000000000;
    //cout << c << "\n";
    /* int white;
    float timeleft;
    cin >> white;
    cin >> M;
    cin >> N;
    cin >> timeleft;

    char s,mov;
    int xs,ys,xm,ym;

    vector<int> row(M,0);
    for(int i = 0;i<N;i++)
        board.push_back(row);

    Initialise();
    for(int i = 0;i<N;i++)
    {
        for(int j =0;j<M;j++)
        {
            cout << board[i][j] << " ";
        }
        cout << "\n";
    }

    while(true)
    {
        cin >> s;
        cin >> xs;
        cin >> ys;
        cin >> mov;
        cin >> xm;
        cin >> ym;
        //cout << ym;
        //bool result = makeMove(s,xs,ys,mov,xm,ym);
        vector<vector<int> > result = returnMove(board,s,xs,ys,mov,xm,ym);

        
        if(result.empty())
            cout << "WRONG MOVE\n";
        else{
            for(int i = 0;i<N;i++)
            {
                for(int j =0;j<M;j++)
                {
                    cout << result[i][j] << " ";
                }
                cout << "\n";
            }
        }
    }

     
    if(white == 0){
        while(true)
        {
            string opponent_move;
            cout << "S 1 2 M 1 3";
            cin >> opponent_move;
        }
    }
    else{
        while(true)
        {
            string opponent_move;
            cin >> opponent_move;
            cout << "S 1 2 M 1 2";
            
        }
    }*/
}