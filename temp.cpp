#include <iostream>
#include <unordered_map> 
#include <string>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <random>
#include <fstream>
#include<bits/stdc++.h>
using namespace std;
bool MyPlayerIsWhite;
long countNode = 0;
unordered_map<string, pair<int,pair<float,string> > > Transposition;


float pawnWeight=1;
float directionWeight=10;
float cannonWeight=10;
float townHallWeight=1000;

// int maxDepth=5;

class State
{
    public:
    int M;
    int N;
    vector<vector<char> > board;
    int WhitePawn;
    int BlackPawn;
    int WhiteTownHall;
    int BlackTownHall;
    int WhiteCannon;
    int BlackCannon;
    float White_directionality;
    float Black_directionality;
    bool isWhite;
    int maxTownHalls;
    int blockedWhitePawn;
    int blockedBlackPawn;

    int dangerForWhiteTownHall;
    int dangerForBlackTownHall;
        
    State(int Mm, int Nn, bool isW)
    {
        M = Mm;
        N = Nn;
        vector<char> vec(N,' ');
        for(int i = 0;i<M;i++)
            board.push_back(vec);
        WhitePawn=0;
        BlackPawn=0;
        WhiteTownHall=0;
        BlackTownHall=0;
        WhiteCannon=0;
        BlackCannon=0;
        White_directionality=0;
        Black_directionality=0;
        blockedBlackPawn=0;
        blockedBlackPawn=0;
        isWhite = isW;
        dangerForBlackTownHall = 0;
        dangerForWhiteTownHall = 0;
        maxTownHalls = board[0].size()/2;
    }

    void Initialise()
    {
        for(int j = 0;j<board[0].size();j++){
            if(j%2 == 0){
                board[0][j] = 'W';
                board[board.size()-1][j] = 'b';
                board[board.size()-2][j] = 'b';
                board[board.size()-3][j] = 'b';
            }
            else{
                board[board.size()-1][j] = 'B';
                board[0][j] = 'w';
                board[1][j] = 'w';
                board[2][j] = 'w';
            }        
        }
        updateCounts();    
    }

    void Initialise2(){
        
        vector<string>rows;
        // rows.push_back("W W W W ");
        // rows.push_back("        ");
        // rows.push_back("   b    ");
        // rows.push_back("     b  ");
        // rows.push_back("   b  w ");
        // rows.push_back("b b b  w");
        // rows.push_back("bbb  b w");
        // rows.push_back(" B B B w");

        rows.push_back("W W W W ");
        rows.push_back("   b    ");
        rows.push_back("        ");
        rows.push_back("     b  ");
        rows.push_back("   b  w ");
        rows.push_back("b b b  w");
        rows.push_back("bbb  b  ");
        rows.push_back(" B B Bww");

        for(int i =0 ;i<8;i++){
            for(int j = 0;j<rows[i].size();j++){
                board[i][j] = rows[i].at(j);
            }
        }
        updateCounts();

    }

    void Copy(vector<vector<char> > bboard, string move){
        board = bboard;
        MakeMove(move);
    }

    pair<vector<string>,string > Moves(){
        vector<string> pawn_moves;
        vector<string> pawn_kills;
        vector<string> pawn_town_kills;
        vector<string> cannon_moves;
        vector<string> cannon_shots;
        vector<string> cannon_blank_shots;
        vector<string> cannon_town_shots;
        string req = "";
        char white = 'w';
        char White = 'W';
        char black = 'b';
        char Black = 'B';

        if(!isWhite){
            white = 'b';
            White = 'B';
            black = 'w';
            Black = 'W';
        }
        bool surround = false;
        
        
        for(int i = 0;i<M;i++){
            for(int j = 0;j<N;j++){
                req = req + board[i][j];
                if(board[i][j] == white){
                    surround = false;
                    if(isWhite){
                        if(i+1 < M){
                            if(board[i+1][j] == ' '){
                                pawn_moves.push_back("S " + to_string(j) + " " + to_string(i) + " M " + to_string(j) + " " + to_string(i+1));
                            }
                            else if(board[i+1][j] == black){
                                surround = true;
                                pawn_kills.push_back("S " + to_string(j) + " " + to_string(i) + " M " + to_string(j) + " " + to_string(i+1));
                            }
                            else if(board[i+1][j] == Black){
                                //surround = true;
                                pawn_town_kills.push_back("S " + to_string(j) + " " + to_string(i) + " M " + to_string(j) + " " + to_string(i+1));
                            }
                        }

                        if(i+1 < M && j+1 < N){
                            if(board[i+1][j+1] == ' '){
                                pawn_moves.push_back("S " + to_string(j) + " " + to_string(i) + " M " + to_string(j+1) + " " + to_string(i+1));
                            }
                            else if(board[i+1][j+1] == black){
                                surround = true;
                                pawn_kills.push_back("S " + to_string(j) + " " + to_string(i) + " M " + to_string(j+1) + " " + to_string(i+1));
                            }
                            else if(board[i+1][j+1] == Black){
                                //surround = true;
                                pawn_town_kills.push_back("S " + to_string(j) + " " + to_string(i) + " M " + to_string(j+1) + " " + to_string(i+1));
                            }
                        }

                        if(i+1 < M && j-1 >=0){
                            if(board[i+1][j-1] == ' '){
                                pawn_moves.push_back("S " + to_string(j) + " " + to_string(i) + " M " + to_string(j-1) + " " + to_string(i+1));           
                            }
                            else if(board[i+1][j-1] == black){
                                surround = true;
                                pawn_kills.push_back("S " + to_string(j) + " " + to_string(i) + " M " + to_string(j-1) + " " + to_string(i+1));           
                            }
                            else if(board[i+1][j-1] == Black){
                                //surround = true;
                                pawn_town_kills.push_back("S " + to_string(j) + " " + to_string(i) + " M " + to_string(j-1) + " " + to_string(i+1));
                            }
                        }

                        if(j+1 < N){
                            if(board[i][j+1] == black){
                                surround = true;
                                pawn_kills.push_back("S " + to_string(j) + " " + to_string(i) + " M " + to_string(j+1) + " " + to_string(i));           
                            }
                            else if(board[i][j+1] == Black){
                                //surround = true;
                                pawn_town_kills.push_back("S " + to_string(j) + " " + to_string(i) + " M " + to_string(j+1) + " " + to_string(i));           
                            }
                        }

                        if(j-1 >=0){
                            if(board[i][j-1] == black){
                                surround = true;
                                pawn_kills.push_back("S " + to_string(j) + " " + to_string(i) + " M " + to_string(j-1) + " " + to_string(i));           
                            }
                            else if(board[i][j-1] == Black){
                                //surround = true;
                                pawn_town_kills.push_back("S " + to_string(j) + " " + to_string(i) + " M " + to_string(j-1) + " " + to_string(i));           
                            }
                        }

                        if(i-2 >= 0 && j-2 >= 0 && surround){
                            if(board[i-2][j-2] == ' '){
                                pawn_moves.push_back("S " + to_string(j) + " " + to_string(i) + " M " + to_string(j-2) + " " + to_string(i-2));           
                            }
                            else if(board[i-2][j-2] == black){
                                pawn_kills.push_back("S " + to_string(j) + " " + to_string(i) + " M " + to_string(j-2) + " " + to_string(i-2));           
                            }
                        }

                        if(i-2 >= 0 && surround){
                            if(board[i-2][j] == ' '){
                                pawn_moves.push_back("S " + to_string(j) + " " + to_string(i) + " M " + to_string(j) + " " + to_string(i-2));           
                            }
                            else if(board[i-2][j] == black){
                                pawn_kills.push_back("S " + to_string(j) + " " + to_string(i) + " M " + to_string(j) + " " + to_string(i-2));           
                            }
                        }

                        if(i-2 >= 0 && j+2 < N && surround){
                            if(board[i-2][j+2] == ' '){
                                pawn_moves.push_back("S " + to_string(j) + " " + to_string(i) + " M " + to_string(j+2) + " " + to_string(i-2));           
                            }
                            else if(board[i-2][j+2] == black){
                                pawn_kills.push_back("S " + to_string(j) + " " + to_string(i) + " M " + to_string(j+2) + " " + to_string(i-2));           
                            }
                        }
                    }
                    else{
                        if(i-1 >= 0){
                            if(board[i-1][j] == ' '){
                                pawn_moves.push_back("S " + to_string(j) + " " + to_string(i) + " M " + to_string(j) + " " + to_string(i-1));
                            }
                            else if(board[i-1][j] == black){
                                surround = true;
                                pawn_kills.push_back("S " + to_string(j) + " " + to_string(i) + " M " + to_string(j) + " " + to_string(i-1));
                            }
                            else if(board[i-1][j] == Black){
                                //surround = true;
                                pawn_town_kills.push_back("S " + to_string(j) + " " + to_string(i) + " M " + to_string(j) + " " + to_string(i-1));
                            }
                        }

                        if(i-1 >= 0 && j+1 < N){
                            if(board[i-1][j+1] == ' '){
                                pawn_moves.push_back("S " + to_string(j) + " " + to_string(i) + " M " + to_string(j+1) + " " + to_string(i-1));
                            }
                            else if(board[i-1][j+1] == black){
                                surround = true;
                                pawn_kills.push_back("S " + to_string(j) + " " + to_string(i) + " M " + to_string(j+1) + " " + to_string(i-1));
                            }
                            else if(board[i-1][j+1] == Black){
                                //surround = true;
                                pawn_town_kills.push_back("S " + to_string(j) + " " + to_string(i) + " M " + to_string(j+1) + " " + to_string(i-1));
                            }
                        }

                        if(i-1 >= 0 && j-1 >=0){
                            if(board[i-1][j-1] == ' '){
                                pawn_moves.push_back("S " + to_string(j) + " " + to_string(i) + " M " + to_string(j-1) + " " + to_string(i-1));           
                            }
                            else if(board[i-1][j-1] == black){
                                surround = true;
                                pawn_kills.push_back("S " + to_string(j) + " " + to_string(i) + " M " + to_string(j-1) + " " + to_string(i-1));           
                            }
                            else if(board[i-1][j-1] == Black){
                                //surround = true;
                                pawn_town_kills.push_back("S " + to_string(j) + " " + to_string(i) + " M " + to_string(j-1) + " " + to_string(i-1));
                            }
                        }

                        if(j+1 < N){
                            if(board[i][j+1] == black){
                                surround = true;
                                pawn_kills.push_back("S " + to_string(j) + " " + to_string(i) + " M " + to_string(j+1) + " " + to_string(i));           
                            }
                            else if(board[i][j+1] == Black){
                                //surround = true;
                                pawn_town_kills.push_back("S " + to_string(j) + " " + to_string(i) + " M " + to_string(j+1) + " " + to_string(i));           
                            }
                        }

                        if(j-1 >=0){
                            if(board[i][j-1] == black){
                                surround = true;
                                pawn_kills.push_back("S " + to_string(j) + " " + to_string(i) + " M " + to_string(j-1) + " " + to_string(i));           
                            }
                            else if(board[i][j-1] == Black){
                                //surround = true;
                                pawn_town_kills.push_back("S " + to_string(j) + " " + to_string(i) + " M " + to_string(j-1) + " " + to_string(i));           
                            }
                        }

                        if(i+2 < M && j-2 >= 0 && surround){
                            if(board[i+2][j-2] == ' '){
                                pawn_moves.push_back("S " + to_string(j) + " " + to_string(i) + " M " + to_string(j-2) + " " + to_string(i+2));           
                            }
                            else if(board[i+2][j-2] == black){
                                pawn_kills.push_back("S " + to_string(j) + " " + to_string(i) + " M " + to_string(j-2) + " " + to_string(i+2));           
                            }
                        }

                        if(i+2 < M && surround){
                            if(board[i+2][j] == ' '){
                                pawn_moves.push_back("S " + to_string(j) + " " + to_string(i) + " M " + to_string(j) + " " + to_string(i+2));           
                            }
                            else if(board[i+2][j] == black){
                                pawn_kills.push_back("S " + to_string(j) + " " + to_string(i) + " M " + to_string(j) + " " + to_string(i+2));           
                            }
                        }

                        if(i+2 < M  && j+2 < N && surround){
                            if(board[i+2][j+2] == ' '){
                                pawn_moves.push_back("S " + to_string(j) + " " + to_string(i) + " M " + to_string(j+2) + " " + to_string(i+2));           
                            }
                            else if(board[i+2][j+2] == black){
                                pawn_kills.push_back("S " + to_string(j) + " " + to_string(i) + " M " + to_string(j+2) + " " + to_string(i+2));           
                            }
                        }
                    }

                    if(i+1 < M && i-1 >=0 && board[i+1][j] == white && board[i-1][j] == white)
                    {
                        if(i+2 < M && board[i+2][j] == ' ')
                        {
                            cannon_moves.push_back("S " + to_string(j) + " " + to_string(i-1) + " M " + to_string(j) + " " + to_string(i+2));
                            if(i+3<M){
                                if(board[i+3][j] == black)
                                    cannon_shots.push_back("S " + to_string(j) + " " + to_string(i) + " B " + to_string(j) + " " + to_string(i+3));
                                else if(board[i+3][j] == Black)
                                    cannon_town_shots.push_back("S " + to_string(j) + " " + to_string(i) + " B " + to_string(j) + " " + to_string(i+3));
                                else if(board[i+3][j] == ' ')
                                    cannon_blank_shots.push_back("S " + to_string(j) + " " + to_string(i) + " B " + to_string(j) + " " + to_string(i+3));

                            }
                            if(i+4<M){
                                if(board[i+4][j] == black)
                                    cannon_shots.push_back("S " + to_string(j) + " " + to_string(i) + " B " + to_string(j) + " " + to_string(i+4));
                                else if(board[i+4][j] == Black)
                                    cannon_town_shots.push_back("S " + to_string(j) + " " + to_string(i) + " B " + to_string(j) + " " + to_string(i+4));
                                else if(board[i+4][j] == ' ')
                                    cannon_blank_shots.push_back("S " + to_string(j) + " " + to_string(i) + " B " + to_string(j) + " " + to_string(i+4));
                            }
                        }
                        if(i-2>=0 && board[i-2][j] == ' ')
                        {
                            cannon_moves.push_back("S " + to_string(j) + " " + to_string(i+1) + " M " + to_string(j) + " " + to_string(i-2));
                            if(i-3>=0){
                                if(board[i-3][j] == black)
                                    cannon_shots.push_back("S " + to_string(j) + " " + to_string(i) + " B " + to_string(j) + " " + to_string(i-3));
                                else if(board[i-3][j] == Black)
                                    cannon_town_shots.push_back("S " + to_string(j) + " " + to_string(i) + " B " + to_string(j) + " " + to_string(i-3));
                                else if(board[i-3][j] == ' ')
                                    cannon_blank_shots.push_back("S " + to_string(j) + " " + to_string(i) + " B " + to_string(j) + " " + to_string(i-3));

                            }
                            if(i-4 >=0){
                                if(board[i-4][j] == black)
                                    cannon_shots.push_back("S " + to_string(j) + " " + to_string(i) + " B " + to_string(j) + " " + to_string(i-4));
                                else if(board[i-4][j] == Black)
                                    cannon_town_shots.push_back("S " + to_string(j) + " " + to_string(i) + " B " + to_string(j) + " " + to_string(i-4));
                                else if(board[i-4][j] == ' ')
                                    cannon_blank_shots.push_back("S " + to_string(j) + " " + to_string(i) + " B " + to_string(j) + " " + to_string(i-4));

                            }
                        }
                    }

                    if(j-1 >=0 && j+1 < N && board[i][j-1] == white && board[i][j+1] == white)
                    {
                        if(j+2 < N && board[i][j+2] == ' ')
                        {
                            cannon_moves.push_back("S " + to_string(j-1) + " " + to_string(i) + " M " + to_string(j+2) + " " + to_string(i));
                            if(j+3<N){
                                if(board[i][j+3] == black)
                                    cannon_shots.push_back("S " + to_string(j) + " " + to_string(i) + " B " + to_string(j+3) + " " + to_string(i));
                                else if(board[i][j+3] == Black)
                                    cannon_town_shots.push_back("S " + to_string(j) + " " + to_string(i) + " B " + to_string(j+3) + " " + to_string(i));
                                else if(board[i][j+3] == ' ')
                                    cannon_town_shots.push_back("S " + to_string(j) + " " + to_string(i) + " B " + to_string(j+3) + " " + to_string(i));

                            }
                            if(j+4<N){
                                if(board[i][j+4] == black)
                                    cannon_shots.push_back("S " + to_string(j) + " " + to_string(i) + " B " + to_string(j+4) + " " + to_string(i));
                                else if(board[i][j+4] == Black)
                                    cannon_town_shots.push_back("S " + to_string(j) + " " + to_string(i) + " B " + to_string(j+4) + " " + to_string(i));
                                else if(board[i][j+4] == ' ')
                                    cannon_town_shots.push_back("S " + to_string(j) + " " + to_string(i) + " B " + to_string(j+4) + " " + to_string(i));

                            }
                        }
                        if(j-2>=0 && board[i][j-2] == ' ')
                        {
                            cannon_moves.push_back("S " + to_string(j+1) + " " + to_string(i) + " M " + to_string(j-2) + " " + to_string(i));
                            if(j-3>=0){
                                if(board[i][j-3] == black)
                                    cannon_shots.push_back("S " + to_string(j) + " " + to_string(i) + " B " + to_string(j-3) + " " + to_string(i));
                                else if(board[i][j-3] == Black)
                                    cannon_town_shots.push_back("S " + to_string(j) + " " + to_string(i) + " B " + to_string(j-3) + " " + to_string(i));
                                else if(board[i][j-3] == ' ')
                                    cannon_town_shots.push_back("S " + to_string(j) + " " + to_string(i) + " B " + to_string(j-3) + " " + to_string(i));

                            }
                            if(j-4>=0){
                                if(board[i][j-4] == black)
                                    cannon_shots.push_back("S " + to_string(j) + " " + to_string(i) + " B " + to_string(j-4) + " " + to_string(i));
                                else if(board[i][j-4] == Black)
                                    cannon_town_shots.push_back("S " + to_string(j) + " " + to_string(i) + " B " + to_string(j-4) + " " + to_string(i));
                                else if(board[i][j-4] == ' ')
                                    cannon_town_shots.push_back("S " + to_string(j) + " " + to_string(i) + " B " + to_string(j-4) + " " + to_string(i));

                            }
                        }
                    }

                    if(i+1 < M && i-1 >= 0 && j+1 < N && j-1 >= 0 && board[i+1][j+1] == white && board[i-1][j-1] == white)
                    {
                        if(i+2<M && j+2 <N && board[i+2][j+2] == ' ')
                        {
                            cannon_moves.push_back("S " + to_string(j-1) + " " + to_string(i-1) + " M " + to_string(j+2) + " " + to_string(i+2));
                            if(i+3<M && j+3<N){
                                if(board[i+3][j+3] == black)
                                    cannon_shots.push_back("S " + to_string(j) + " " + to_string(i) + " B " + to_string(j+3) + " " + to_string(i+3));
                                else if(board[i+3][j+3] == Black)
                                    cannon_town_shots.push_back("S " + to_string(j) + " " + to_string(i) + " B " + to_string(j+3) + " " + to_string(i+3));
                                else if(board[i+3][j+3] == ' ')
                                    cannon_town_shots.push_back("S " + to_string(j) + " " + to_string(i) + " B " + to_string(j+3) + " " + to_string(i+3));

                            }
                            if(i+4<M && j+4<N){
                                if(board[i+4][j+4] == black)
                                    cannon_shots.push_back("S " + to_string(j) + " " + to_string(i) + " B " + to_string(j+4) + " " + to_string(i+4));
                                else if(board[i+4][j+4] == Black)
                                    cannon_town_shots.push_back("S " + to_string(j) + " " + to_string(i) + " B " + to_string(j+4) + " " + to_string(i+4));
                                else if(board[i+4][j+4] == ' ')
                                    cannon_town_shots.push_back("S " + to_string(j) + " " + to_string(i) + " B " + to_string(j+4) + " " + to_string(i+4));

                            }
                        }
                        if(i-2>=0 && j-2>=0 && board[i-2][j-2] == ' ')
                        {
                            cannon_moves.push_back("S " + to_string(j+1) + " " + to_string(i+1) + " M " + to_string(j-2) + " " + to_string(i-2));
                            if(i-3>=0 && j-3>=0){
                                if(board[i-3][j-3] == black)
                                    cannon_shots.push_back("S " + to_string(j) + " " + to_string(i) + " B " + to_string(j-3) + " " + to_string(i-3));
                                else if(board[i-3][j-3] == Black)
                                    cannon_town_shots.push_back("S " + to_string(j) + " " + to_string(i) + " B " + to_string(j-3) + " " + to_string(i-3));
                                else if(board[i-3][j-3] == ' ')
                                    cannon_town_shots.push_back("S " + to_string(j) + " " + to_string(i) + " B " + to_string(j-3) + " " + to_string(i-3));

                            }
                            if(i-4>=0 && j-4>=0){
                                if(board[i-4][j-4] == black)
                                    cannon_shots.push_back("S " + to_string(j) + " " + to_string(i) + " B " + to_string(j-4) + " " + to_string(i-4));
                                else if(board[i-4][j-4] == Black)
                                    cannon_town_shots.push_back("S " + to_string(j) + " " + to_string(i) + " B " + to_string(j-4) + " " + to_string(i-4));
                                else if(board[i-4][j-4] == ' ')
                                    cannon_town_shots.push_back("S " + to_string(j) + " " + to_string(i) + " B " + to_string(j-4) + " " + to_string(i-4));

                            }
                        }

                    }

                    if(i+1 < M && j+1 < N && i-1 >=0 && j-1 >=0 && board[i+1][j-1] == white && board[i-1][j+1] == white)
                    {
                        if(i+2 < M && j-2>=0 && board[i+2][j-2] == ' ')
                        {
                            cannon_moves.push_back("S " + to_string(j+1) + " " + to_string(i-1) + " M " + to_string(j-2) + " " + to_string(i+2));
                            if(i+3<M && j-3 >=0){
                                if(board[i+3][j-3] == black)
                                    cannon_shots.push_back("S " + to_string(j) + " " + to_string(i) + " B " + to_string(j-3) + " " + to_string(i+3));
                                else if(board[i+3][j-3] == Black)
                                    cannon_town_shots.push_back("S " + to_string(j) + " " + to_string(i) + " B " + to_string(j-3) + " " + to_string(i+3));
                                else if(board[i+3][j-3] == ' ')
                                    cannon_town_shots.push_back("S " + to_string(j) + " " + to_string(i) + " B " + to_string(j-3) + " " + to_string(i+3));

                            }
                            if(i+4<M && j-4 >=0){
                                if(board[i+4][j-4] == black)
                                    cannon_shots.push_back("S " + to_string(j) + " " + to_string(i) + " B " + to_string(j-4) + " " + to_string(i+4));
                                else if(board[i+4][j-4] == Black)
                                    cannon_town_shots.push_back("S " + to_string(j) + " " + to_string(i) + " B " + to_string(j-4) + " " + to_string(i+4));
                                else if(board[i+4][j-4] == ' ')
                                    cannon_town_shots.push_back("S " + to_string(j) + " " + to_string(i) + " B " + to_string(j-4) + " " + to_string(i+4));

                            }
                        }
                        if(i-2>=0 && j+2 < N && board[i-2][j+2] == ' ')
                        {
                            cannon_moves.push_back("S " + to_string(j-1) + " " + to_string(i+1) + " M " + to_string(j+2) + " " + to_string(i-2));
                            if(i-3>=0 && j+3 < N){
                                if(board[i-3][j+3] == black)
                                    cannon_shots.push_back("S " + to_string(j) + " " + to_string(i) + " B " + to_string(j+3) + " " + to_string(i-3));
                                else if(board[i-3][j+3] == Black)
                                    cannon_town_shots.push_back("S " + to_string(j) + " " + to_string(i) + " B " + to_string(j+3) + " " + to_string(i-3));
                                else if(board[i-3][j+3] == ' ')
                                    cannon_town_shots.push_back("S " + to_string(j) + " " + to_string(i) + " B " + to_string(j+3) + " " + to_string(i-3));

                            }
                            if(i-4>=0 && j+4 < N){
                                if(board[i-4][j+4] == black)
                                    cannon_shots.push_back("S " + to_string(j) + " " + to_string(i) + " B " + to_string(j+4) + " " + to_string(i-4));
                                else if(board[i-4][j+4] == Black)
                                    cannon_town_shots.push_back("S " + to_string(j) + " " + to_string(i) + " B " + to_string(j+4) + " " + to_string(i-4));
                                else if(board[i-4][j+4] == ' ')
                                    cannon_town_shots.push_back("S " + to_string(j) + " " + to_string(i) + " B " + to_string(j+4) + " " + to_string(i-4));

                            }
                        }
                    }

                }
            }
            
        }

        if(BlackTownHall<=(maxTownHalls-2) || WhiteTownHall<=(maxTownHalls-2) || ( !isWhite && (BlackPawn-blockedBlackPawn)==0) ||  ( isWhite && (WhitePawn-blockedWhitePawn)== 0)){
            cannon_town_shots.clear();
            return pair<vector<string>, string>(cannon_town_shots,req);
        }
        cannon_town_shots.insert(cannon_town_shots.end(), pawn_town_kills.begin(), pawn_town_kills.end());
        cannon_town_shots.insert(cannon_town_shots.end(), cannon_shots.begin(), cannon_shots.end());
        cannon_town_shots.insert(cannon_town_shots.end(), cannon_moves.begin(), cannon_moves.end());
        
        cannon_town_shots.insert(cannon_town_shots.end(), pawn_kills.begin(), pawn_kills.end());
        cannon_town_shots.insert(cannon_town_shots.end(), pawn_moves.begin(), pawn_moves.end());
        cannon_town_shots.insert(cannon_town_shots.end(), cannon_blank_shots.begin(), cannon_blank_shots.end());
        //std::random_shuffle ( cannon_town_shots.begin(), cannon_town_shots.end() );
        //cout << cannon_town_shots.size() << "\n";
        
        return pair<vector<string>, string >(cannon_town_shots, req);
    }
    void MakeMove(string s){
        WhitePawn = 0;
        BlackPawn = 0;
        WhiteCannon = 0;
        BlackCannon = 0;
        WhiteTownHall = 0;
        BlackTownHall = 0;
        if(s.at(6) == 'M'){
            board[(int)s.at(10) - 48][(int)s.at(8) - 48] = board[(int)s.at(4) - 48][(int)s.at(2) - 48];
            board[(int)s.at(4) - 48][(int)s.at(2) - 48] = ' ';
        }
        else{
            board[(int)s.at(10) - 48][(int)s.at(8) - 48] = ' ';
        }  
        updateCounts();
        /*
        for(int i = 0;i<board.size();i++)
        {
            for(int j = 0;j<board[0].size();j++)
            {
                if(board[i][j] == 'w'){
                    WhitePawn++;
                    if(i-1>=0 && i+1 < M && board[i-1][j] == 'w' && board[i+1][j] == 'w')
                        WhiteCannon++;
                    if(j-1>=0 && j+1 < N && board[i][j-1] == 'w' && board[i][j+1] == 'w')
                        WhiteCannon++;
                    if(i-1>=0 && i+1 < M && j-1>=0 && j+1 < N && board[i-1][j-1] == 'w' && board[i+1][j+1] == 'w')
                        WhiteCannon++;
                    if(i-1>=0 && i+1 < M && j-1>=0 && j+1 < N && board[i-1][j+1] == 'w' && board[i+1][j-1] == 'w')
                        WhiteCannon++;
                }
                if(board[i][j] == 'b'){
                    BlackPawn++;
                    if(i-1>=0 && i+1 < M && board[i-1][j] == 'b' && board[i+1][j] == 'b')
                        BlackCannon++;
                    if(j-1>=0 && j+1 < N && board[i][j-1] == 'b' && board[i][j+1] == 'b')
                        BlackCannon++;
                    if(i-1>=0 && i+1 < M && j-1>=0 && j+1 < N && board[i-1][j-1] == 'b' && board[i+1][j+1] == 'b')
                        BlackCannon++;
                    if(i-1>=0 && i+1 < M && j-1>=0 && j+1 < N && board[i-1][j+1] == 'b' && board[i+1][j-1] == 'b')
                        BlackCannon++;
                }
                if(board[i][j] == 'W'){
                    WhiteTownHall++;
                }
                if(board[i][j] == 'B'){
                    BlackTownHall++;
                }
            }
        }    
        */  
    }

    string printBoard(){
        string ans;
        for(int i = 0;i<M;i++){
            for(int j = 0;j<N;j++)
                ans = ans + board[i][j] + " ";
            ans += "\n";
        }
        return ans;
    }

    
    pair<float,string> AlphaBetaPrune(float alpha, float beta, bool maximizingPlayer, int depth,int maxDepth){
        countNode++;
        
        /*float value;
        string bestMove = "";
        if(depth == maxDepth)
            return pair<float,string>(evaluate(),bestMove);
        pair<vector<string>,string > children = Moves();
        if(children.size()==0)
            return pair<float,string>(evaluate(),bestMove);
        */
        float value;
        string bestMove = "";
        
        int limit_depth = 5;
        if(MyPlayerIsWhite && WhitePawn <= 3*N/4+1){
            limit_depth = 6;
        }
        if(MyPlayerIsWhite && WhitePawn <= 3*N/4-1){
            limit_depth = 7;
        }

        // if(!MyPlayerIsWhite && BlackPawn <= 3*N/4+1){
        //     limit_depth = 6;
        // }
        // if(!MyPlayerIsWhite && BlackPawn <= 3*N/4-1){
        //     limit_depth = 7;
        // }

        if(depth == limit_depth)
            return pair<float,string>(evaluate(),bestMove);
        
        string ss = Encode() + to_string(maximizingPlayer);

        auto p = Transposition.find(ss);
        //cout << typeid(p).name() << endl;
        if(p != Transposition.end()){
            //cout << Transposition.find(board)->second.first.second << " " << depth << "\n";
           
            if(p->second.first >= (limit_depth -depth)){
                //cout << Transposition.find(board)->second.first.second << " " << depth << "\n";
                //cout << to_string(p->second.first.first == maximizingPlayer) << "\n";
                //cout << to_string(p->second.first) << " " << limit_depth -depth << "\n";
                return p->second.second;
            }
        }
        pair<vector<string>,string> moves = Moves();
        vector<string> children = moves.first;
        string req = moves.second + to_string(maximizingPlayer);
        if(children.size()==0)
            return pair<float,string>(evaluate(),bestMove);

        if(maximizingPlayer)
        {
            float value = (float)INT32_MIN;
            for(int i = 0;i<children.size();i++)
            {
                State* s = new State(M,N, !isWhite);
                (*s).Copy(board,children[i]);
                //bestMove = children[i];
                
                float current = (*s).AlphaBetaPrune(alpha, beta,false,depth+1,maxDepth).first;
                if(current > value){
                    value = current;
                    bestMove = children[i];
                }
                //value = std::max(value, (*s).AlphaBetaPrune(alpha, beta,false,depth+1).first);
                alpha = std::max(alpha,value);
                if(alpha>=beta){
                    //cout << "break";
                    break;
                }
                delete s;
            }
            pair<float,string> a (value,bestMove);
            //pair<bool,int> c (maximizingPlayer,limit_depth-depth);
            pair<int,pair<float,string> > d (limit_depth-depth,a);
            pair<string, pair<int,pair<float,string> > > b (req,d);
            Transposition.insert(b);
            return a;
            //return pair<float,string>(value,bestMove);
        }
        else
        {
            value = (float)INT32_MAX;
            for(int i = 0;i<children.size();i++)
            {
                State* s = new State(M,N,!isWhite);
                (*s).Copy(board,children[i]);
                //bestMove = children[i];
                
                float current = (*s).AlphaBetaPrune(alpha, beta,true,depth+1,maxDepth).first;
                if(current < value){
                    value = current;
                    bestMove = children[i];
                }
                //value = std::min(value, (*s).AlphaBetaPrune(alpha, beta,true,depth+1).first);
                beta = std::min(beta,value);
                if(alpha>=beta){
                    //cout << "break";
                    break;
                }
                delete s;
            }
            pair<float,string> a (value,bestMove);
            //pair<bool,int> c (maximizingPlayer,limit_depth-depth);
            pair<int,pair<float,string> > d (limit_depth-depth,a);
            pair<string, pair<int,pair<float,string> > > b (req,d);
            Transposition.insert(b);
            return a;
            //return pair<float,string>(value,bestMove);
        }
    }
    void updateCounts(){
        WhitePawn = 0;
        WhiteCannon = 0;
        BlackPawn = 0;
        BlackCannon = 0;
        WhiteTownHall = 0;
        BlackTownHall = 0;
        White_directionality = 0.0;
        Black_directionality = 0.0;
        blockedBlackPawn=0;
        blockedWhitePawn=0;
        for(int i = 0;i<board.size();i++)
        {
            for(int j = 0;j<board[0].size();j++)
            {
                if(board[i][j] == 'w'){
                    WhitePawn++;
                    if(i==M-2){
                        if(j+1<N && board[i+1][j+1]=='B')dangerForBlackTownHall++;
                        if(board[i+1][j]=='B')dangerForBlackTownHall++;
                        if(j-1>=0 && board[i+1][j-1])dangerForBlackTownHall++;
                    }

                    White_directionality += i;
                    if(i-1>=0 && i+1 < M && board[i-1][j] == 'w' && board[i+1][j] == 'w')
                        WhiteCannon++;
                    if(j-1>=0 && j+1 < N && board[i][j-1] == 'w' && board[i][j+1] == 'w')
                        WhiteCannon++;
                    if(i-1>=0 && i+1 < M && j-1>=0 && j+1 < N && board[i-1][j-1] == 'w' && board[i+1][j+1] == 'w')
                        WhiteCannon++;
                    if(i-1>=0 && i+1 < M && j-1>=0 && j+1 < N && board[i-1][j+1] == 'w' && board[i+1][j-1] == 'w')
                        WhiteCannon++;
                    try{
                        if(i==M-1){//check if it is blocked
                        //in this state when the white soldier has reached the end -> it will be blocked -if cannot retreat, if no townhall adjacent
                            bool isBlocked=true;
                            if( isBlocked && j-1>=0 && board[i][j-1]=='B')
                                isBlocked=false;
                            
                            if( isBlocked && j+1 < N && board[i][j+1]=='B')
                                isBlocked=false;
                            
                            if( isBlocked && j-1>=0 && board[i][j-1]=='b'){
                                
                                if( board[i-2][j] != 'w')isBlocked=false;
                                if( j-2>=0 && board[i-2][j-2] !='w')isBlocked=false;
                                if( j+2<N  && board[i-2][j+2] !='w')isBlocked=false;
                            }

                            if( isBlocked && j+1 < N && board[i][j+1]=='b'){
                                if( board[i-2][j] != 'w')isBlocked=false;
                                if( j-2>=0 && board[i-2][j-2] !='w')isBlocked=false;
                                if( j+2<N  && board[i-2][j+2] !='w')isBlocked=false;
                            }

                            if(isBlocked)blockedWhitePawn++;   

                        }
                    }
                    catch(exception e){;}
                }
                if(board[i][j] == 'b'){
                    BlackPawn++;
                    if(i==1){
                        if(j-1>=0 && board[0][j-1]=='W')dangerForWhiteTownHall++;
                        if(j+1<N && board[0][j+1]=='W')dangerForWhiteTownHall++;
                        if(board[0][j]=='W')dangerForWhiteTownHall++;
                    }
                    Black_directionality +=( board.size()-1 - i);
                    if(i-1>=0 && i+1 < M && board[i-1][j] == 'b' && board[i+1][j] == 'b')
                        BlackCannon++;
                    if(j-1>=0 && j+1 < N && board[i][j-1] == 'b' && board[i][j+1] == 'b')
                        BlackCannon++;
                    if(i-1>=0 && i+1 < M && j-1>=0 && j+1 < N && board[i-1][j-1] == 'b' && board[i+1][j+1] == 'b')
                        BlackCannon++;
                    if(i-1>=0 && i+1 < M && j-1>=0 && j+1 < N && board[i-1][j+1] == 'b' && board[i+1][j-1] == 'b')
                        BlackCannon++;
                    try{
                        if(i==0){
                            bool isBlocked = true;

                            if(isBlocked && j-1 >= 0 && board[i][j-1]=='W')isBlocked=false;
                            if(isBlocked && j+1 < N  && board[i][j+1]=='W')isBlocked=false;
                            if(isBlocked && j-1 >=0 && board[i][j-1]=='w'){
                                if(board[2][j]!='b')isBlocked=false;
                                if(j-2>=0 && board[2][j-2]!='b')isBlocked=false;
                                if(j+2<N && board[2][j+2]!='b')isBlocked=false;
                            }
                            if(isBlocked && j+1<N && board[i][j+1]=='w'){
                                if(board[2][j]!='b')isBlocked=false;
                                if(j-2>=0 && board[2][j-2]!='b')isBlocked=false;
                                if(j+2<N && board[2][j+2]!='b')isBlocked=false;
                            }
                            if(isBlocked)blockedBlackPawn++;
                        }
                    }
                    catch(exception e){;}
                }
                if(board[i][j] == 'W'){
                    WhiteTownHall++;
                }
                if(board[i][j] == 'B'){
                    BlackTownHall++;
                }
            }
        }
        if(WhitePawn > 0)White_directionality = White_directionality/WhitePawn;
        if(BlackPawn > 0)Black_directionality = Black_directionality/BlackPawn;

        
    }
    string Encode(){
        string s = "";
        for(int i= 0;i<M;i++)
            for(int j =0;j<N;j++)
                s = s + board[i][j];
        return s;
    }
    float evaluate(){
        
          
        // if(WhiteTownHall <=2 && !MyPlayerIsWhite)
        //     return INT32_MAX/2;
        // if(BlackTownHall <=2 && MyPlayerIsWhite)
        //     return INT32_MAX/2;
        // if(WhiteTownHall <=2 && MyPlayerIsWhite)
        //     return -INT32_MIN/2;
        // if(BlackTownHall <=2 && !MyPlayerIsWhite)
        //     return -INT32_MIN/2;

        //if you are in a losing situation then you might want to have a draw 
        //if you are in a winning situation you might want to avoid a draw

        // if going forward means you are going to kill yourself then you don't want to do it right ? 
        if(WhiteTownHall <= maxTownHalls-2 && !MyPlayerIsWhite)
            return townHallWeight * 4;
        
        if( BlackTownHall <= maxTownHalls-2 && MyPlayerIsWhite)
            return townHallWeight * 4;
        
        if(WhiteTownHall <= maxTownHalls -2 && MyPlayerIsWhite)
            return -townHallWeight* 4;
        if( BlackTownHall <= maxTownHalls-2 && !MyPlayerIsWhite)
            return -townHallWeight * 4;
            

        float a = (WhitePawn - BlackPawn) + directionWeight*((WhitePawn * White_directionality - BlackPawn *Black_directionality)) 
            + cannonWeight*(WhiteCannon - BlackCannon)
            + townHallWeight*(WhiteTownHall - BlackTownHall);
        if(MyPlayerIsWhite)
            return a - 80*dangerForWhiteTownHall;
        else
            return (-a) - 80 * dangerForBlackTownHall;
        

    }
};


int main(int argc, char *argv[])
{
    // State st(8,8,true);
    // st.Initialise2();
    // MyPlayerIsWhite = true;
    // vector<string> moves = st.Moves().first;
    // // for(int i = 0;i<moves.size();i++)cout<<moves[i]<<endl;
    
    // for(int i = 0;i<moves.size();i++){
    //     State* newState = new State(8,8, true);
    //     newState->Initialise2();
        
    //     newState->MakeMove(moves[i]);

    //     cout<<moves[i]<<": "<<newState->evaluate()<<endl;
    //     for(int p=0;p<8;p++){
    //         for(int q=0;q<8;q++){
    //             cout<<newState->board[p][q];
    //         }
    //         cout<<endl;
    //     }
    //     cout<<"---------------------------------"<<endl;;
    // }
    

    // pair<float,string> pruned_state =st.AlphaBetaPrune((float)INT32_MIN,(float)INT32_MAX,true,0,5);
    // cout<<"Chosen move "<<pruned_state.second<<" "<<pruned_state.first;
    // // st.MakeMove(pruned_state.first);
    // cout<<"Chosen Move "<<pruned_state.second<<" "
    // return 0;
    int M;
    int N;
    int white;
    float timeleft;
    cin >> white;
    cin >> M;
    cin >> N;
    cin >> timeleft;
    timeleft = 0.92 * timeleft;// to account for error in time measurement

    // maxDepth = 5;
    State s(M,N,false);
    if(white == 2)
        MyPlayerIsWhite = true;
    else
        MyPlayerIsWhite = false;
    
    s.Initialise();
    int myPawns = 3 * s.BlackTownHall;
    string S;
    string X;
    string Y;
    string Mo;
    string Xt;
    string Yt;


    if(MyPlayerIsWhite)
    {
        cin >> S;
        cin >> X;
        cin >> Y;
        cin >> Mo;
        cin >> Xt;
        cin >> Yt;
        s.MakeMove(S+" "+X+ " "+Y+" "+Mo+" "+Xt+" "+Yt);
        s.isWhite = !s.isWhite;
    }
    
    float isWhite = 1;
    if(!MyPlayerIsWhite)isWhite = -1;

    float learning_rate = 0;

    ifstream paramFile;
    paramFile.open("param2.txt");
    paramFile>>learning_rate;
    paramFile>>pawnWeight;
    paramFile>>directionWeight;
    paramFile>>cannonWeight;
    paramFile>>townHallWeight;


    ofstream outfile;
    outfile.open("weights2.txt");
    outfile<<pawnWeight<<" "<<directionWeight<<" "<<cannonWeight<<" "<<townHallWeight<<"\n";
    outfile.close();

    int maxDepth = 4;
    int mymoves = 0;
    while(true)
    {
        if(mymoves >= 0){
            clock_t begin = clock();
            pair<float,string> pruned_state =s.AlphaBetaPrune((float)INT32_MIN,(float)INT32_MAX,true,0,maxDepth);  
            string move = pruned_state.second;
            s.MakeMove(move);

            S = move.at(0);
            X = move.at(2);
            Y = move.at(4);
            Mo = move.at(6);
            Xt = move.at(8);
            Yt = move.at(10);
            cout << S+" "+X+ " "+Y+" "+Mo+" "+Xt+" "+Yt <<"\n";
            
            clock_t end = clock();
            timeleft = timeleft - (end - begin)/CLOCKS_PER_SEC;

            float backed_up_value = pruned_state.first;
            float initial_value = s.evaluate();
            float delta = backed_up_value - initial_value ;
            

            float delta_sign = 1;
            if(delta < 0)delta_sign = -1;
            pawnWeight = pawnWeight + delta_sign * learning_rate*(s.WhitePawn - s.BlackPawn)*isWhite;
            cannonWeight = cannonWeight + delta_sign * learning_rate * (s.WhiteCannon-s.BlackCannon)*isWhite;
            directionWeight = directionWeight + delta_sign *learning_rate * (s.White_directionality - s.Black_directionality)*isWhite;
            // townHallWeight = townHallWeight + delta_sign * learning_rate * (s.WhiteTownHall - s.BlackTownHall - s.Black_directionality)*isWhite;
            outfile.open("weights2.txt",std::ios_base::app);
            outfile<<pawnWeight<<" "<<directionWeight<<" "<<cannonWeight<<" "<<townHallWeight<<"\n";
            outfile.close();
        }
        else{
            string move;
            if(MyPlayerIsWhite){
                if(mymoves==0)
                    move = "S "+to_string(N-3)+" 0 M "+to_string(N-2)+" 1\n";
                else
                    move = "S 3 0 M 2 1\n";
            }
            else{
                if(mymoves == 0)
                    move = "S 2 "+to_string(M-1)+" M 1 "+to_string(M-2)+"\n";
                else
                    move = "S "+to_string(N-4)+" "+to_string(M-1) + " M "+to_string(N-3)+" "+to_string(M-2)+"\n";
            }
            mymoves++;
            s.MakeMove(move);
            cout<<move;

        }

        
        s.isWhite = !s.isWhite;

        cin >> S;
        cin >> X;
        cin >> Y;
        cin >> Mo;
        cin >> Xt;
        cin >> Yt;
        s.MakeMove(S+" "+X+ " "+Y+" "+Mo+" "+Xt+" "+Yt);
        s.isWhite = !s.isWhite;
        
        
    }
    // outfile.close();
    
    return 0;
}
