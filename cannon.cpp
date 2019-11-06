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

using namespace std;
bool MyPlayerIsWhite;
long countNode = 0;
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
        isWhite = isW;
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

    void Copy(vector<vector<char> > bboard, string move){
        board = bboard;
        MakeMove(move);
    }

    vector<string> Moves(){
        vector<string> pawn_moves;
        vector<string> pawn_kills;
        vector<string> pawn_town_kills;
        vector<string> cannon_moves;
        vector<string> cannon_shots;
        vector<string> cannon_blank_shots;
        vector<string> cannon_town_shots;

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
        
        if(BlackTownHall<=2 || WhiteTownHall<=2 || BlackPawn==0 || WhitePawn == 0){
            cannon_town_shots.clear();
            return cannon_town_shots;
        }
        for(int i = 0;i<board.size();i++){
            for(int j = 0;j<board[0].size();j++){
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
                                surround = true;
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
                        }

                        if(i+1 < M && j-1 >=0){
                            if(board[i+1][j-1] == ' '){
                                pawn_moves.push_back("S " + to_string(j) + " " + to_string(i) + " M " + to_string(j-1) + " " + to_string(i+1));           
                            }
                            else if(board[i+1][j-1] == black){
                                surround = true;
                                pawn_kills.push_back("S " + to_string(j) + " " + to_string(i) + " M " + to_string(j-1) + " " + to_string(i+1));           
                            }
                        }

                        if(j+1 < M){
                            if(board[i][j+1] == black){
                                surround = true;
                                pawn_kills.push_back("S " + to_string(j) + " " + to_string(i) + " M " + to_string(j+1) + " " + to_string(i));           
                            }
                            else if(board[i][j+1] == Black){
                                surround = true;
                                pawn_town_kills.push_back("S " + to_string(j) + " " + to_string(i) + " M " + to_string(j+1) + " " + to_string(i));           
                            }
                        }

                        if(j-1 >=0){
                            if(board[i][j-1] == black){
                                surround = true;
                                pawn_kills.push_back("S " + to_string(j) + " " + to_string(i) + " M " + to_string(j-1) + " " + to_string(i));           
                            }
                            else if(board[i][j-1] == Black){
                                surround = true;
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
                                surround = true;
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
                        }

                        if(i-1 >= 0 && j-1 >=0){
                            if(board[i-1][j-1] == ' '){
                                pawn_moves.push_back("S " + to_string(j) + " " + to_string(i) + " M " + to_string(j-1) + " " + to_string(i-1));           
                            }
                            else if(board[i-1][j-1] == black){
                                surround = true;
                                pawn_kills.push_back("S " + to_string(j) + " " + to_string(i) + " M " + to_string(j-1) + " " + to_string(i-1));           
                            }
                        }

                        if(j+1 < M){
                            if(board[i][j+1] == black){
                                surround = true;
                                pawn_kills.push_back("S " + to_string(j) + " " + to_string(i) + " M " + to_string(j+1) + " " + to_string(i));           
                            }
                            else if(board[i][j+1] == Black){
                                surround = true;
                                pawn_town_kills.push_back("S " + to_string(j) + " " + to_string(i) + " M " + to_string(j+1) + " " + to_string(i));           
                            }
                        }

                        if(j-1 >=0){
                            if(board[i][j-1] == black){
                                surround = true;
                                pawn_kills.push_back("S " + to_string(j) + " " + to_string(i) + " M " + to_string(j-1) + " " + to_string(i));           
                            }
                            else if(board[i][j-1] == Black){
                                surround = true;
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
        cannon_town_shots.insert(cannon_town_shots.end(), cannon_shots.begin(), cannon_shots.end());
        cannon_town_shots.insert(cannon_town_shots.end(), cannon_moves.begin(), cannon_moves.end());
        cannon_town_shots.insert(cannon_town_shots.end(), pawn_town_kills.begin(), pawn_town_kills.end());
        cannon_town_shots.insert(cannon_town_shots.end(), pawn_kills.begin(), pawn_kills.end());
        cannon_town_shots.insert(cannon_town_shots.end(), pawn_moves.begin(), pawn_moves.end());
        cannon_town_shots.insert(cannon_town_shots.end(), cannon_blank_shots.begin(), cannon_blank_shots.end());
        //std::random_shuffle ( cannon_town_shots.begin(), cannon_town_shots.end() );
        //cout << cannon_town_shots.size() << "\n";
        return cannon_town_shots;
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

    void printBoard(){
        for(int i = 0;i<M;i++){
            for(int j = 0;j<N;j++)
                cout << board[i][j] << " ";
            cout << "\n";
        }
    }

    
    pair<int,string> AlphaBetaPrune(int alpha, int beta, bool maximizingPlayer, int depth){
        countNode++;
        
        int value;
        string bestMove = "";
        if(depth == 4)
            return pair<int,string>(evaluate(),bestMove);
        vector<string> children = Moves();
        if(children.size()==0)
            return pair<int,string>(evaluate(),bestMove);
        if(maximizingPlayer)
        {
            value = -2000000;
            for(int i = 0;i<children.size();i++)
            {
                State* s = new State(M,N, !isWhite);
                (*s).Copy(board,children[i]);
                //bestMove = children[i];
                
                int current = (*s).AlphaBetaPrune(alpha, beta,false,depth+1).first;
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
            return pair<int,string>(value,bestMove);
        }
        else
        {
            value = 2000000;
            for(int i = 0;i<children.size();i++)
            {
                State* s = new State(M,N,!isWhite);
                (*s).Copy(board,children[i]);
                //bestMove = children[i];
                
                int current = (*s).AlphaBetaPrune(alpha, beta,true,depth+1).first;
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
            return pair<int,string>(value,bestMove);
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
        for(int i = 0;i<board.size();i++)
        {
            for(int j = 0;j<board[0].size();j++)
            {
                if(board[i][j] == 'w'){
                    WhitePawn++;
                    White_directionality += i;
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
                    Black_directionality +=( 7 - i);
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
        if(WhitePawn > 0)White_directionality = White_directionality/WhitePawn;
        if(BlackPawn > 0)Black_directionality = Black_directionality/BlackPawn;

        
    }
    int evaluate(){
        
          
        if(WhiteTownHall <=2 && !MyPlayerIsWhite)
            return 10000;
        if(BlackTownHall <=2 && MyPlayerIsWhite)
            return 10000;
        if(WhiteTownHall <=2 && MyPlayerIsWhite)
            return -10000;
        if(BlackTownHall <=2 && !MyPlayerIsWhite)
            return -10000;
        int a = (WhitePawn - BlackPawn) + (int) 10*((WhitePawn * White_directionality - BlackPawn *Black_directionality)) 
            + 10*(WhiteCannon - BlackCannon)
            + 100*(WhiteTownHall - BlackTownHall);
        if(MyPlayerIsWhite)
            return a;
        else
            return (-a);
        

    }
};


int main(int argc, char *argv[])
{
    ofstream outfile;
    outfile.open("file.txt");
    int M;
    int N;
    int white;
    float timeleft;
    cin >> white;
    cin >> M;
    cin >> N;
    cin >> timeleft;
    
    State s(M,N,false);
    if(white == 2)
        MyPlayerIsWhite = true;
    else
        MyPlayerIsWhite = false;
    outfile << MyPlayerIsWhite << " ";
    outfile.close();
    s.Initialise();

    string S;
    string X;
    string Y;
    string Mo;
    string Xt;
    string Yt;

    //s.MakeMove("S 3 8 M 2 7");
    //s.MakeMove("S 2 2 M 3 3");
    //s.MakeMove("S 5 6 M 6 5");
    //s.MakeMove("S 2 3 M 5 3");
    //s.MakeMove("S 3 7 M 4 6");
    //s.MakeMove("S 4 3 B 4 6");
    //s.MakeMove("S 1 8 M 4 5");
    //s.isWhite = !s.isWhite;
    //s.printBoard();
    

    if(MyPlayerIsWhite)
    {
        cin >> S;
        cin >> X;
        cin >> Y;
        cin >> Mo;
        cin >> Xt;
        cin >> Yt;
        //X = X.at(0)-1;
        //Y = Y.at(0)-1;
        //Xt = Xt.at(0)-1;
        //Yt = Yt.at(0)-1;
        s.MakeMove(S+" "+X+ " "+Y+" "+Mo+" "+Xt+" "+Yt);
        //cout << S+" "+X+ " "+Y+" "+Mo+" "+Xt+" "+Yt << "\n";
        s.isWhite = !s.isWhite;
        //s.printBoard();
    }
    while(true)
    {
        string move = s.AlphaBetaPrune(-200000,200000,true,0).second;
        // cout<<"move variable stores now "<< move<<endl;
        //string move = "S 2 6 M 2 5";
        
        
        s.MakeMove(move);
        // cout<<"IIIII"<<endl;

        S = move.at(0);
        X = move.at(2);
        Y = move.at(4);
        Mo = move.at(6);
        Xt = move.at(8);
        Yt = move.at(10);
        cout << S+" "+X+ " "+Y+" "+Mo+" "+Xt+" "+Yt <<"\n";
        
        s.isWhite = !s.isWhite;

        cin >> S;
        cin >> X;
        cin >> Y;
        cin >> Mo;
        cin >> Xt;
        cin >> Yt;
        //X = X.at(0)-1;
        //Y = Y.at(0)-1;
        //Xt = Xt.at(0)-1;
        //Yt = Yt.at(0)-1;
        s.MakeMove(S+" "+X+ " "+Y+" "+Mo+" "+Xt+" "+Yt);
        //cout << S+" "+X+ " "+Y+" "+Mo+" "+Xt+" "+Yt << "\n";
        s.isWhite = !s.isWhite;
        
    }
    
    //s.printBoard();
    //s.MakeMove("S 2 0 M 2 3");
    //s.MakeMove("S 2 1 M 2 4");
    //s.MakeMove("S 2 4 B 2 7");
    //s.MakeMove("S 2 4 M 2 5");
    //vector<string> mov = s.Moves();
    //State s1(8,8);
    //s1.Copy(s.board,"S 0 2 M 2 3");
    //s1.MakeMove("S 1 2 M 3 3");
    //for(int i =0;i<mov.size();i++)
    //{
     //   cout << mov[i] << "\n";
    //}
    /*
    s.printBoard();
    cout << s.WhitePawn << " " << s.BlackPawn << " " << s.WhiteTownHall << " " << s.BlackTownHall << " "
            << s.WhiteCannon << " " << s.BlackCannon << "\n";*/
    //cout << "\n";
    //s.MakeMove("S 2 0 M 2 3");
    //s.MakeMove("S 1 7 M 1 4");
    //s.printBoard();
    return 0;
}