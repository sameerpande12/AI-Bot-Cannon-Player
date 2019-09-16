#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <fstream> 
#include<climits>
#include <sstream> 
#include <stdlib.h>    
#include <time.h>  
using namespace std;

int w_townhalls = 40;
int w_blocked = 0;
int w_vulnerable = 0;

int w_pawns = 7;
int w_pawn_moves = 0;
int w_pawn_kills = 0;
int w_town_kills = 6;
int w_cannon = 0;
int w_pawn_shot = 4;
int w_town_shot = 20;

class State{

    public:
    bool isWhite;
    vector<vector<char> > board;
    string parentMessage;
    string messageToBestChild;
    int numRows;
    int numCols;
    char myPawn;
    char myTower;

    char oppPawn;
    char oppTower;
    State(bool white,int nRows, int nCols,vector<vector<char> >inputBoard,string inputMessage=""){

        isWhite = white;
        board = inputBoard;
        parentMessage = inputMessage;
        messageToBestChild = "";
        numRows = nRows;
        numCols = nCols;

        if(white){

            myPawn = 'w';
            myTower='W';
            oppPawn='b';
            oppTower='B';
        }
        else{
            myPawn = 'b';
            myTower='B';
            oppPawn='w';
            oppTower='W';
        }
    }

    string generateMove(){
        
        int cost = MaxVal(INT_MIN,INT_MAX,0,3);
        
        updateBoard(messageToBestChild,true);
        return messageToBestChild;
    }
    void updateBoard(string move,bool isMyPawn){
        // cout<<"Move to make "<<move<<endl;
        if(move.length()==0)return;//the case of stalemate
        vector<string> tokens;
        string word = "";
        for(int i =0 ;i<move.length();i++){
            if(move[i]==' '){
                tokens.push_back(word);
                // cout<<"pushing:"<<word<<endl;
                word = "";
            }
            else{
                word = word + move[i];
            }
        }
        if(word.length()>0)tokens.push_back(word);
        
        int bx = stoi(tokens[1]);
        int by = stoi(tokens[2]);

        int ex = stoi(tokens[4]);
        int ey = stoi(tokens[5]);

        char type = tokens[3][0];
        
        // cout<<"be"<<endl;
        // for(int i =0 ;i<5;i++)cout<<tokens[i]<<endl;
        
        if(type=='M'){
            if(isMyPawn)
                board[ex][ey] = myPawn;
            else{
                // cout<<"Converted position (ex,ey)=("<<ex<<","<<ey<<") from \'"<<board[ex][ey]<<"\' to ";
                board[ex][ey] = oppPawn;
                //cout<<board[ex][ey]<<endl;
                
            }
            board[bx][by] = ' ';
        }
        else{
            board[ex][ey]= ' ';
        }

    }


    vector<string> getNeighbours(int i , int j){
                vector<string> neighbours;
                if(isWhite){
                    
                    vector<vector<char> >tempBoard = board;   
                    if(tempBoard[i][j]==myPawn){
                        //regular soldier capture
                        for(int dy = 1;dy>=-2;dy--){
                            if(dy==0){
                                for(int dx = -1;dx<=1;dx+=2){
                                    if(j+dx>=0 && j+dx <numCols){
                                        
                                        if(tempBoard[i+dy][j+dx]==oppPawn || tempBoard[i+dy][j+dx]==oppTower){
                                            neighbours.push_back("S "+to_string(i)+" "+to_string(j)+" M "+to_string(i+dy)+" "+to_string(j+dx));
                                            // vector<vector<char>>nextBoard = tempBoard;
                                            // nextBoard[i][j]=' ';
                                            // nextBoard[i+dy][j+dx]=myPawn;
                                        }
                                    }
                                }
                            }
                            else if(dy==-1)continue;
                            else if(dy==1){

                                if(i+dy >= numRows)continue;

                                for(int dx=-1;dx<=1;dx++){
                                    if(j+dx>=0 && j+dx<numCols){
                                        if(tempBoard[i+dy][j+dx]==oppPawn || tempBoard[i+dy][j+dx]==oppTower || tempBoard[i+dy][j+dx]== ' '){
                                            // vector<vector<char>>nextBoard = tempBoard;
                                            // nextBoard[i][j]=' ';
                                            // nextBoard[i+dy][j+dx]=myPawn;
                                            neighbours.push_back("S "+to_string(i)+" "+to_string(j)+" M "+to_string(i+dy)+" "+to_string(j+dx));
                                        }
                                    }
                                }
                            }
                            else if(dy==-2){
                                    if(i+dy<0)continue;

                                    bool hasEnemy = false;
                                    for(int dx=-1;dx<=1;dx+=2){
                                        if( j+dx >=0 && j+dx<numCols ){
                                            if(tempBoard[i+0][j+dx]==oppPawn){
                                                hasEnemy = true;
                                                break;
                                            }
                                        }
                                    }
                                    for(int dx=-1;dx<=1;dx+=1){
                                        if(i+1 >= numRows)continue;
                                        if(j+dx >=0 && j+dx<numCols){
                                            if(tempBoard[i+1][j+dx]==oppPawn){
                                                hasEnemy = true;
                                                break;
                                            }
                                        }
                                    }

                                    if(hasEnemy){
                                        //killing by jumping over
                                        for(int dx = -2 ;dx<=2;dx+=2){
                                            if(j+dx >=0 && j+dx <numCols){
                                                if(tempBoard[i+dy][j+dx]!=myPawn && tempBoard[i+dy][j+dx]!=myTower){
                                                    // vector<vector<char>>nextBoard = tempBoard;
                                                    // nextBoard[i][j]=' ';
                                                    // nextBoard[i+dy][j+dx]=myPawn;
                                                    neighbours.push_back("S "+to_string(i)+" "+to_string(j)+" M "+to_string(i+dy)+" "+to_string(j+dx));
                                                }
                                            }
                                        }

                                    }

                            }

                        }

                        //cannons check if cannon shooting is possible

                        if( i+1 < numRows && i-1 >=0){
                            if( j-1 >=0 && j+1 < numCols ){
                                /*
                                w     O       w
                                  w   R     w
                                    w      w
                                */

                                if( (tempBoard[i+1][j+1] == tempBoard[i][j]) && (tempBoard[i][j]==tempBoard[i-1][j-1])){

                                    if(  i+3 <numRows && j+3 <numCols){
                                        if(tempBoard[i+2][j+2]==' ' && tempBoard[i+3][j+3]!=myPawn && tempBoard[i+3][j+3]!=myTower){
                                            //shot
                                            neighbours.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i+3)+" "+to_string(j+3));
                                        }


                                    }

                                    if( i-3 >=0 && j-3 >=0){
                                        if(tempBoard[i-2][j-2] == ' ' && tempBoard[i-3][j-3]!=myPawn && tempBoard[i-3][j-3]!=myTower){
                                            //shot
                                            neighbours.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i-3)+" "+to_string(j-3));
                                        }

                                    }
                                    //3shots begin

                                    if(  i+4 <numRows && j+4 <numCols){
                                        if(tempBoard[i+2][j+2]==' ' && tempBoard[i+3][j+3] == ' ' && tempBoard[i+4][j+4]!=myPawn && tempBoard[i+4][j+4]!=myTower){
                                            //shot
                                            neighbours.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i+4)+" "+to_string(j+4));
                                        }


                                    }

                                    if( i-4 >=0 && j-4 >=0){
                                        if(tempBoard[i-2][j-2] == ' '&& tempBoard[i-3][j-3] == ' ' && tempBoard[i-4][j-4]!=myPawn && tempBoard[i-4][j-4]!=myTower){
                                            //shot
                                            neighbours.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i-4)+" "+to_string(j-4));
                                        }

                                    }
                                    //movement

                                    if(i-2>=0 && j-2 >=0){
                                        if(tempBoard[i-2][j-2]== ' '){
                                            //move
                                            neighbours.push_back("S "+to_string(i+1)+" "+to_string(j+1)+" M "+to_string(i-2)+" "+to_string(j-2));
                                        }
                                    }

                                    if(i+2<numRows && j+2 <numCols){
                                        if(tempBoard[i+2][j+2]== ' '){
                                            //move
                                            neighbours.push_back("S "+to_string(i-1)+" "+to_string(j-1)+" M "+to_string(i+2)+" "+to_string(j+2));
                                        }
                                    }

                                }

                                if( (tempBoard[i-1][j+1]==tempBoard[i][j]) && ( tempBoard[i][j]== tempBoard[i+1][j-1])){
                                    if(i-3 >=0 && j+3 < numCols){
                                        if(tempBoard[i-2][j+2]==' ' && tempBoard[i-3][j+3]!=myPawn && tempBoard[i-3][j+3]!=myTower ){
                                            //shot
                                            neighbours.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i-3)+" "+to_string(j+3));
                                        }
                                    }

                                    if(i+3 < numRows && j-3 >=0){
                                        if(tempBoard[i+2][j-2]==' ' && tempBoard[i+3][j-3]!=myPawn && tempBoard[i+3][j-3]!=myTower){
                                            //shot
                                            neighbours.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i+3)+" "+to_string(j-3));
                                        }
                                    }
                                    //3 shots 
                                    if(i-4 >=0 && j+4 < numCols){
                                        if(tempBoard[i-2][j+2]==' ' &&  tempBoard[i-3][j+3] == ' '&& tempBoard[i-4][j+4]!=myPawn && tempBoard[i-4][j+4]!=myTower ){
                                            //shot
                                            neighbours.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i-4)+" "+to_string(j+4));
                                        }
                                    }

                                    if(i+4 < numRows && j-4 >=0){
                                        if(tempBoard[i+2][j-2]==' ' && tempBoard[i+3][j-3]==' ' && tempBoard[i+4][j-4]!=myPawn && tempBoard[i+4][j-4]!=myTower){
                                            //shot
                                            neighbours.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i+4)+" "+to_string(j-4));
                                        }
                                    }

                                    //movement
                                    if(i-2 >= 0 && j+2 < numCols){
                                        if(tempBoard[i-2][j+2]== ' '){
                                        //move
                                            neighbours.push_back("S "+to_string(i+1)+" "+to_string(j-1)+" M "+to_string(i-2)+" "+to_string(j+2));
                                        }
                                    }

                                    if(i+2 < numRows && j-2 >=0){
                                        if(tempBoard[i+2][j-2]==' '){
                                            //move
                                            neighbours.push_back("S "+to_string(i-1)+" "+to_string(j+1)+" M "+to_string(i+2)+" "+to_string(j-2));
                                        }
                                    }
                                }

                            }

                            if((tempBoard[i][j]==tempBoard[i+1][j]) && (tempBoard[i][j] ==tempBoard[i-1][j])){
                                /*
                                w
                                w
                                w
                                */
                                if(i+3 < numRows){
                                    if(tempBoard[i+2][j]==' ' && tempBoard[i+3][j]!=myPawn && tempBoard[i+3][j]!=myTower){
                                        //shot
                                        neighbours.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i+3)+" "+to_string(j));
                                    }
                                }
                                if(i-3 >= 0){
                                    if(tempBoard[i-2][j]==' ' && tempBoard[i-3][j]!=myPawn && tempBoard[i-3][j]!=myTower){
                                        //shot
                                        neighbours.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i-3)+" "+to_string(j));
                                    }
                                }
                                //3shots
                                if(i+4 < numRows){
                                    if(tempBoard[i+2][j]==' ' && tempBoard[i+3][j]==' ' && tempBoard[i+4][j]!=myPawn && tempBoard[i+4][j]!=myTower){
                                        //shot
                                        neighbours.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i+4)+" "+to_string(j));
                                    }
                                }
                                if(i-4 >= 0){
                                    if(tempBoard[i-2][j]==' ' && tempBoard[i-3][j]==' ' && tempBoard[i-4][j]!=myPawn && tempBoard[i-4][j]!=myTower){
                                        //shot
                                        neighbours.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i-4)+" "+to_string(j));
                                    }
                                }

                                //movement
                                if(i+2<numRows){
                                    if(tempBoard[i+2][j]==' '){
                                        //move
                                        neighbours.push_back("S "+to_string(i-1)+" "+to_string(j)+" M "+to_string(i+2)+" "+to_string(j));
                                    }

                                }
                                if(i-2 >=0){
                                    if(tempBoard[i-2][j]==' '){
                                        //move
                                        neighbours.push_back("S "+to_string(i+1)+" "+to_string(j)+" M "+to_string(i-2)+" "+to_string(j));
                                    }
                                    
                                }
                            }

                        }
                        if(j-1>=0 && j+1 <numCols){

                            //  w w w 
                            if((tempBoard[i][j-1]==tempBoard[i][j] ) && (tempBoard[i][j]==tempBoard[i][j+1])){

                                if(j-3>=0 ){
                                    //shot
                                    if(tempBoard[i][j-2]==' ' && tempBoard[i][j-3]!=myPawn && tempBoard[i][j-3]!=myTower){
                                        neighbours.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i)+" "+to_string(j-3));
                                    }
                                }

                                if(j+3 < numCols){
                                    if(tempBoard[i][j+2]==' '&& tempBoard[i][j+3]!=myPawn && tempBoard[i][j+3]!=myTower){
                                        //shot
                                        neighbours.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i)+" "+to_string(j+3));
                                    }
                                }
                                //3shots

                                if(j-4>=0 ){
                                    //shot
                                    if(tempBoard[i][j-2]==' ' && tempBoard[i][j-3]==' ' && tempBoard[i][j-4]!=myPawn && tempBoard[i][j-4]!=myTower){
                                        neighbours.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i)+" "+to_string(j-4));
                                    }
                                }

                                if(j+4 < numCols){
                                    if(tempBoard[i][j+2]==' '&& tempBoard[i][j+3]==' '&& tempBoard[i][j+4]!=myPawn && tempBoard[i][j+4]!=myTower){
                                        //shot
                                        neighbours.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i)+" "+to_string(j+4));
                                    }
                                }

                                //movement
                                if(j+2<numCols){
                                    if(tempBoard[i][j+2]==' '){
                                        //move right
                                        neighbours.push_back("S "+to_string(i)+" "+to_string(j-1)+" M "+to_string(i)+" "+to_string(j+2));
                                    }
                                }
                                if(j-2 >= 0){
                                    if(tempBoard[i][j-2]==' '){
                                        //moveLeft
                                        neighbours.push_back("S "+to_string(i)+" "+to_string(j+1)+" M "+to_string(i)+" "+to_string(j-2));
                                    }
                                }


                            }
                        }


                    }
                }
                else{
                    vector<vector<char> >tempBoard = board;
                        
                        if(tempBoard[i][j]==myPawn){
                            //regular soldier capture
                            for(int dy = -1;dy<=2;dy++){
                                if(dy==0){
                                    for(int dx = -1;dx<=1;dx+=2){
                                        if(j+dx>=0 && j+dx <numCols){
                                            
                                            if(tempBoard[i+dy][j+dx]==oppPawn || tempBoard[i+dy][j+dx]==oppTower ){
                                                // vector<vector<char>>nextBoard = tempBoard;
                                                // nextBoard[i][j]=' ';
                                                // nextBoard[i+dy][j+dx]=myPawn;
                                                neighbours.push_back("S "+to_string(i)+" "+to_string(j)+" M "+to_string(i+dy)+" "+to_string(j+dx));
                                            }
                                        }
                                    }
                                }
                                else if(dy==1)continue;
                                else if(dy==-1){

                                    if(i+dy < 0)continue;

                                    for(int dx=-1;dx<=1;dx++){
                                        if(j+dx>=0 && j+dx<numCols){
                                            if(tempBoard[i+dy][j+dx]==oppPawn || tempBoard[i+dy][j+dx]==oppTower || tempBoard[i+dy][j+dx]== ' '){
                                                // vector<vector<char>>nextBoard = tempBoard;
                                                // nextBoard[i][j]=' ';
                                                // nextBoard[i+dy][j+dx]=myPawn;
                                                neighbours.push_back("S "+to_string(i)+" "+to_string(j)+" M "+to_string(i+dy)+" "+to_string(j+dx));
                                            }
                                        }
                                    }
                                }
                                else if(dy==2){
                                        if(i+dy>=numRows)continue;

                                        bool hasEnemy = false;
                                        for(int dx=-1;dx<=1;dx+=2){
                                            if( j+dx >=0 && j+dx<numCols ){
                                                if(tempBoard[i+0][j+dx]==oppPawn){
                                                    hasEnemy = true;
                                                    break;
                                                }
                                            }
                                        }
                                        for(int dx=-1;dx<=1;dx+=1){
                                            if(i-1 < 0)continue;
                                            if(j+dx >=0 && j+dx<numCols){
                                                if(tempBoard[i-1][j+dx]==oppPawn){
                                                    hasEnemy = true;
                                                    break;
                                                }
                                            }
                                        }

                                        if(hasEnemy){
                                            //killing by jumping over
                                            for(int dx = -2 ;dx<=2;dx+=2){
                                                if(j+dx >=0 && j+dx <numCols){
                                                    if(tempBoard[i+dy][j+dx]!=myPawn && tempBoard[i+dy][j+dx]!=myTower){
                                                        // vector<vector<char>>nextBoard = tempBoard;
                                                        // nextBoard[i][j]=' ';
                                                        // nextBoard[i+dy][j+dx]=myPawn;
                                                        neighbours.push_back("S "+to_string(i)+" "+to_string(j)+" M "+to_string(i+dy)+" "+to_string(j+dx));
                                                    }
                                                }
                                            }

                                        }

                                }

                            }

                            //cannons check if cannon shooting is possible
                            if( i+1 < numRows && i-1 >=0){
                                if( j-1 >=0 && j+1 < numCols ){
                                    /*
                                    w     O       w
                                      w   R     w
                                        w      w
                                    */

                                    if((tempBoard[i+1][j+1] == tempBoard[i][j]) &&   (tempBoard[i][j]==tempBoard[i-1][j-1])){
                                        
                                        if(  i+3 <numRows && j+3 <numCols){
                                            
                                            if(tempBoard[i+2][j+2]==' ' && tempBoard[i+3][j+3]!=myPawn && tempBoard[i+3][j+3]!=myTower){
                                                //shot
                                                neighbours.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i+3)+" "+to_string(j+3));
                                            }


                                        }
                                        

                                        if( i-3 >=0 && j-3 >=0){
                                            if(tempBoard[i-2][j-2] == ' ' && tempBoard[i-3][j-3]!=myPawn && tempBoard[i-3][j-3]!=myTower){
                                                //shot
                                                neighbours.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i-3)+" "+to_string(j-3));
                                            }

                                        }
                                        
                                        //3shots begin

                                        if(  i+4 <numRows && j+4 <numCols){
                                            if(tempBoard[i+2][j+2]==' ' && tempBoard[i+3][j+3] == ' ' && tempBoard[i+4][j+4]!=myPawn && tempBoard[i+4][j+4]!=myTower){
                                                //shot
                                                neighbours.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i+4)+" "+to_string(j+4));
                                            }


                                        }
                                        
                                        if( i-4 >=0 && j-4 >=0){
                                            if(tempBoard[i-2][j-2] == ' '&& tempBoard[i-3][j-3] == ' ' && tempBoard[i-4][j-4]!=myPawn && tempBoard[i-4][j-4]!=myTower){
                                                //shot
                                                neighbours.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i-4)+" "+to_string(j-4));
                                            }

                                        }
                                        //movement
                                        
                                        if(i-2>=0 && j-2 >=0){
                                            if(tempBoard[i-2][j-2]== ' '){
                                                //move
                                                neighbours.push_back("S "+to_string(i+1)+" "+to_string(j+1)+" M "+to_string(i-2)+" "+to_string(j-2));
                                            }
                                        }
                                       
                                        if(i+2<numRows && j+2 <numCols){
                                            if(tempBoard[i+2][j+2]== ' '){
                                                //move
                                                neighbours.push_back("S "+to_string(i-1)+" "+to_string(j-1)+" M "+to_string(i+2)+" "+to_string(j+2));
                                            }
                                        }

                                    }
                                    
                                    if((tempBoard[i-1][j+1]==tempBoard[i][j])  && (tempBoard[i][j]== tempBoard[i+1][j-1])){
                                        
                                        if(i-3 >=0 && j+3 < numCols){
                                            if(tempBoard[i-2][j+2]==' ' && tempBoard[i-3][j+3]!=myPawn && tempBoard[i-3][j+3]!=myTower ){
                                                //shot
                                                neighbours.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i-3)+" "+to_string(j+3));
                                            }
                                        }

                                        if(i+3 < numRows && j-3 >=0){
                                            if(tempBoard[i+2][j-2]==' ' && tempBoard[i+3][j-3]!=myPawn && tempBoard[i+3][j-3]!=myTower){
                                                //shot
                                                neighbours.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i+3)+" "+to_string(j-3));
                                            }
                                        }
                                        //3 shots 
                                        if(i-4 >=0 && j+4 < numCols){
                                            if(tempBoard[i-2][j+2]==' ' &&  tempBoard[i-3][j+3] == ' '&& tempBoard[i-4][j+4]!=myPawn && tempBoard[i-4][j+4]!=myTower ){
                                                //shot
                                                neighbours.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i-4)+" "+to_string(j+4));
                                            }
                                        }

                                        if(i+4 < numRows && j-4 >=0){
                                            if(tempBoard[i+2][j-2]==' ' && tempBoard[i+3][j-3]==' ' && tempBoard[i+4][j-4]!=myPawn && tempBoard[i+4][j-4]!=myTower){
                                                //shot
                                                neighbours.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i+4)+" "+to_string(j-4));
                                            }
                                        }

                                        //movement
                                        if(i-2 >= 0 && j+2 < numCols){
                                            if(tempBoard[i-2][j+2]== ' '){
                                            //move
                                                neighbours.push_back("S "+to_string(i+1)+" "+to_string(j-1)+" M "+to_string(i-2)+" "+to_string(j+2));
                                            }
                                        }

                                        if(i+2 < numRows && j-2 >=0){
                                            if(tempBoard[i+2][j-2]==' '){
                                                //move
                                                neighbours.push_back("S "+to_string(i-1)+" "+to_string(j+1)+" M "+to_string(i+2)+" "+to_string(j-2));
                                            }
                                        }
                                    }
                                    

                                }

                                if((tempBoard[i][j]==tempBoard[i+1][j]) && (tempBoard[i][j]==tempBoard[i-1][j])){
                                    /*
                                    w
                                    w
                                    w
                                    */
                                    if(i+3 < numRows){
                                        if(tempBoard[i+2][j]==' ' && tempBoard[i+3][j]!=myPawn && tempBoard[i+3][j]!=myTower){
                                            //shot
                                            neighbours.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i+3)+" "+to_string(j));
                                        }
                                    }
                                    if(i-3 >= 0){
                                        if(tempBoard[i-2][j]==' ' && tempBoard[i-3][j]!=myPawn && tempBoard[i-3][j]!=myTower){
                                            //shot
                                            neighbours.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i-3)+" "+to_string(j));
                                        }
                                    }
                                    //3shots
                                    if(i+4 < numRows){
                                        if(tempBoard[i+2][j]==' ' && tempBoard[i+3][j]==' ' && tempBoard[i+4][j]!=myPawn && tempBoard[i+4][j]!=myTower){
                                            //shot
                                            neighbours.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i+4)+" "+to_string(j));
                                        }
                                    }
                                    if(i-4 >= 0){
                                        if(tempBoard[i-2][j]==' ' && tempBoard[i-3][j]==' ' && tempBoard[i-4][j]!=myPawn && tempBoard[i-4][j]!=myTower){
                                            //shot
                                            neighbours.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i-4)+" "+to_string(j));
                                        }
                                    }

                                    //movement
                                    if(i+2<numRows){
                                        if(tempBoard[i+2][j]==' '){
                                            //move
                                            neighbours.push_back("S "+to_string(i-1)+" "+to_string(j)+" M "+to_string(i+2)+" "+to_string(j));
                                        }

                                    }
                                    if(i-2 >=0){
                                        if(tempBoard[i-2][j]==' '){
                                            //move
                                            neighbours.push_back("S "+to_string(i+1)+" "+to_string(j)+" M "+to_string(i-2)+" "+to_string(j));
                                        }
                                        
                                    }
                                }

                            }
                            if(j-1>=0 && j+1 <numCols){

                                //  w w w 
                                if((tempBoard[i][j-1]==tempBoard[i][j]) && (tempBoard[i][j] ==tempBoard[i][j+1])){

                                    if(j-3>=0 ){
                                        //shot
                                        if(tempBoard[i][j-2]==' ' && tempBoard[i][j-3]!=myPawn && tempBoard[i][j-3]!=myTower){
                                            neighbours.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i)+" "+to_string(j-3));
                                        }
                                    }

                                    if(j+3 < numCols){
                                        if(tempBoard[i][j+2]==' '&& tempBoard[i][j+3]!=myPawn && tempBoard[i][j+3]!=myTower){
                                            //shot
                                            neighbours.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i)+" "+to_string(j+3));
                                        }
                                    }
                                    //3shots

                                    if(j-4>=0 ){
                                        //shot
                                        if(tempBoard[i][j-2]==' ' && tempBoard[i][j-3]==' ' && tempBoard[i][j-4]!=myPawn && tempBoard[i][j-4]!=myTower){
                                            neighbours.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i)+" "+to_string(j-4));
                                        }
                                    }

                                    if(j+4 < numCols){
                                        if(tempBoard[i][j+2]==' '&& tempBoard[i][j+3]==' '&& tempBoard[i][j+4]!=myPawn && tempBoard[i][j+4]!=myTower){
                                            //shot
                                            neighbours.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i)+" "+to_string(j+4));
                                        }
                                    }

                                    //movement
                                    if(j+2<numCols){
                                        if(tempBoard[i][j+2]==' '){
                                            //move right
                                            neighbours.push_back("S "+to_string(i)+" "+to_string(j-1)+" M "+to_string(i)+" "+to_string(j+2));
                                        }
                                    }
                                    if(j-2 >= 0){
                                        if(tempBoard[i][j-2]==' '){
                                            //moveLeft
                                            neighbours.push_back("S "+to_string(i)+" "+to_string(j+1)+" M "+to_string(i)+" "+to_string(j-2));
                                        }
                                    }


                                }
                            }

                        }

                        

                    

                }

                return neighbours;
    }

    vector<int> TownAndExposedTown(bool Mine)
    {
        int blocked = 0;
        int vulnerable = 0;
        int townHalls = 0;
        if((Mine && myPawn == 'w') || (!Mine && myPawn == 'b'))
        {
            for(int j=0;j<numCols;j++){
                if(j%2 == 0){
                    if(board[0][j] == 'W'){
                        townHalls++;
                        if(j-1 >=0 && board[board.size()-1][j-1] == 'w')
                            blocked++;
                        else if(j-1 >=0 && board[board.size()-1][j-1] == 'b')
                            vulnerable++;

                        if(j+1 <board.size() && board[board.size()-1][j+1] == 'w')
                            blocked++;
                        else if(j+1 <board.size() && board[board.size()-1][j+1] == 'b')
                            vulnerable++;

                        if(j-1 >=0 && board[board.size()-2][j-1] == 'w')
                            blocked++;
                        else if(j-1 >=0 && board[board.size()-2][j-1] == 'b')
                            vulnerable++;

                        if(board[board.size()-2][j] == 'w')
                            blocked++;
                        else if(board[board.size()-2][j] == 'b')
                            vulnerable++;

                        if(j+1 <board.size() && board[board.size()-2][j+1] == 'w')
                            blocked++;
                        else if(j+1 <board.size() && board[board.size()-2][j+1] == 'b')
                            vulnerable++;
                    }
                }
            }

        }
        else{
            for(int j=0;j<numCols;j++){
                if(j%2 == 1){
                    if(board[numRows-1][j] == 'B'){
                        townHalls++;
                        if(j-1 >=0 && board[numRows-1][j-1] == 'b')
                            blocked++;
                        else if(j-1 >=0 && board[numRows-1][j-1] == 'w')
                            vulnerable++;

                        if(j+1 <board.size() && board[numRows-1][j+1] == 'b')
                            blocked++;
                        else if(j+1 <board.size() && board[numRows-1][j+1] == 'w')
                            vulnerable++;

                        if(j-1 >=0 && board[numRows-2][j-1] == 'b')
                            blocked++;
                        else if(j-1 >=0 && board[numRows-2][j-1] == 'w')
                            vulnerable++;

                        if(board[1][j] == 'b')
                            blocked++;
                        else if(board[1][j] == 'w')
                            vulnerable++;

                        if(j+1 <board.size() && board[numRows-2][j+1] == 'b')
                            blocked++;
                        else if(j+1 <board.size() && board[numRows-2][j+1] == 'w')
                            vulnerable++;
                    }
                }
            }
        }
        vector<int> value;
        value.push_back(townHalls);
        value.push_back(blocked);
        value.push_back(vulnerable);
        ofstream ofs("cannon.txt", std::ofstream::out);
        ofs << townHalls << "\n";
        ofs << blocked << "\n";
        ofs << vulnerable << "\n";
        ofs.close();        
        return value;
    }

    vector<int> PawnsAndKills(bool Mine)
    {
        int pawn = 0;
        int freedom = 0;
        int kill = 0;
        int townkill = 0;
        bool surround = false;
        int cannon = 0;
        int pawn_shot = 0;
        int town_shot = 0;

        if((Mine && myPawn == 'w') || (!Mine && myPawn == 'b')){
            //cout << parentMessage;
            for(int i = 0;i<board.size();i++)
            {
                //cout <<"\n";
                for(int j = 0;j<board[0].size();j++)
                {
                    //cout << board[i][j] << " ";
                    if(board[i][j] == 'w')
                    {
                        pawn++;
                        if(j-1>=0){
                            if(board[i][j-1] == 'b'){
                                surround = true;
                                kill++;
                            }
                            else if(board[i][j-1] == 'B'){
                                surround = true;
                                townkill++;
                            }
                        } 
 
                        if(j+1 < board.size()){ 
                            if(board[i][j+1] == 'b'){
                                surround = true;
                                kill++;
                            }
                            else if(board[i][j+1] == 'B'){
                                surround = true;
                                townkill++;
                            }
                        }
                        
                        if(j-1 >=0 && i+1 <board.size()){
                            if(board[i+1][j-1] == 'b'){
                                surround = true;
                                kill++;
                            }
                            else if(board[i+1][j-1] == 'B'){
                                surround = true;
                                townkill++;
                            }
                            else if(board[i+1][j-1] == ' ')
                                freedom++;
                        }
                        if(i+1 <board.size()){
                            if(board[i+1][j] == 'b'){
                                surround = true;
                                kill++;
                            }
                            else if(board[i+1][j] == 'B'){
                                surround = true;
                                townkill++;
                            }
                            else if(board[i+1][j] == ' ')
                                freedom++;
                        }
                        if(j+1 < board.size() && i+1 <board.size()){
                            if(board[i+1][j+1] == 'b'){
                                surround = true;
                                kill++;
                            }
                            else if(board[i+1][j+1] == 'B'){
                                surround = true;
                                townkill++;
                            }
                            else if(board[i+1][j+1] == ' ')
                                freedom++;
                        }

                        
                        if(surround == true)
                        {
                            if(i-2 >=0)
                            {
                                if(j-2 >= 0){
                                    if(board[i-2][j-2] == ' ')
                                        freedom++;
                                    else if(board[i-2][j-2] == 'w')
                                        kill++;
                                }
                                if(j+2 <board.size()){
                                    if(board[i-2][j+2] == ' ')
                                        freedom++;
                                    else if(board[i-2][j+2] == 'w')
                                        kill++;
                                }
                            }
                        }
                        if(j+1 < board.size() && j-1 >=0)
                            if(board[i][j+1] == 'w')
                                if(board[i][j-1] == 'w'){
                                    cannon++;
                                    if(j-3 >= 0 && board[i][j-2] != ' ')
                                    {
                                        
                                        if(board[i][j-3] == 'b')
                                            pawn_shot++;
                                        if(board[i][j-3] == 'B')
                                            town_shot++;
                                        if(j-4 >= 0 && board[i][j-3] == ' ')
                                        {
                                            if(board[i][j-4] == 'b')
                                                pawn_shot++;
                                            if(board[i][j-4] == 'B')
                                                town_shot++;
                                        }
                                    }
                                    if(j+3 < board.size() && board[i][j+2] != ' ')
                                    {
                                        
                                        if(board[i][j+3] == 'b')
                                            pawn_shot++;
                                        if(board[i][j+3] == 'B')
                                            town_shot++;
                                        if(j+4 < board.size() && board[i][j+3] == ' ')
                                        {
                                            if(board[i][j+4] == 'b')
                                                pawn_shot++;
                                            if(board[i][j+4] == 'B')
                                                town_shot++;
                                        }
                                    }
                                }
                        if(i+1 < board.size() && i-1 >=0)
                            if(board[i+1][j] == 'w')
                                if(board[i-1][j] == 'w'){
                                    cannon++;
                                    if(i-3 >= 0 && board[i-2][j] != ' ')
                                    {
                                        
                                        if(board[i-3][j] == 'b')
                                            pawn_shot++;
                                        if(board[i-3][j] == 'B')
                                            town_shot++;
                                        if(i-4 >=0 && board[i-3][j] == ' ')
                                        {
                                            if(board[i-4][j] == 'b')
                                                pawn_shot++;
                                            if(board[i-4][j] == 'B')
                                                town_shot++;
                                        }
                                    }
                                    if(i+3 < board.size() && board[i+2][j] != ' ')
                                    {
                                        
                                        if(board[i+3][j] == 'b')
                                            pawn_shot++;
                                        if(board[i+3][j] == 'B')
                                            town_shot++;
                                        if(i+4 < board.size() && board[i+3][j] == ' ')
                                        {
                                            if(board[i+4][j] == 'b')
                                                pawn_shot++;
                                            if(board[i+4][j] == 'B')
                                                town_shot++;
                                        }
                                    }
                                }
                        if(i+1 < board.size() && i-1 >=0 && j+1 < board.size() && j-1 >=0)
                            if(board[i-1][j+1] == 'w')
                                if(board[i+1][j-1] == 'w'){
                                    cannon++;
                                    if(i-3 >= 0 && j+3 < board.size() && board[i-2][j+2] != ' ')
                                    {
                                        
                                        if(board[i-3][j+3] == 'b')
                                            pawn_shot++;
                                        if(board[i-3][j+3] == 'B')
                                            town_shot++;
                                        if(i-4 >= 0 && j+4 < board.size() && board[i-3][j+3] == ' ')
                                        {
                                            if(board[i-4][j+4] == 'b')
                                                pawn_shot++;
                                            if(board[i-4][j+4] == 'B')
                                                town_shot++;
                                        }
                                    }
                                    if(j-3 >= 0 && i+3 < board.size() && board[i+2][j-2] != ' ')
                                    {
                                        
                                        if(board[i+3][j-3] == 'b')
                                            pawn_shot++;
                                        if(board[i+3][j-3] == 'B')
                                            town_shot++;
                                        if(j-4 >= 0 && i+4 < board.size() && board[i+3][j-3] == ' ')
                                        {
                                            if(board[i+4][j-4] == 'b')
                                                pawn_shot++;
                                            if(board[i+4][j-4] == 'B')
                                                town_shot++;
                                        }
                                    }
                                }
                        if(i+1 < board.size() && i-1 >=0 && j+1 < board.size() && j-1 >=0)
                            if(board[i-1][j-1] == 'w')
                                if(board[i+1][j+1] == 'w'){
                                    cannon++;
                                    if(i+3 < board.size() && j+3 < board.size() && board[i+2][j+2] != ' ')
                                    {
                                        
                                        if(board[i+3][j+3] == 'b')
                                            pawn_shot++;
                                        if(board[i+3][j+3] == 'B')
                                            town_shot++;
                                        if(i+4 < board.size() && j+4 < board.size() && board[i+3][j+3] == ' ')
                                        {
                                            if(board[i+4][j+4] == 'b')
                                                pawn_shot++;
                                            if(board[i+4][j+4] == 'B')
                                                town_shot++;
                                        }
                                    }
                                    if(i-3 >=0 && j-3 >=0 && board[i-2][j-2] != ' ')
                                    {
                                        
                                        if(board[i-3][j-3] == 'b')
                                            pawn_shot++;
                                        if(board[i-3][j-3] == 'B')
                                            town_shot++;
                                        if(i-4 >=0 && j-4 >=0 && board[i-3][j-3] == ' ')
                                        {
                                            if(board[i-4][j-4] == 'b')
                                                pawn_shot++;
                                            if(board[i-4][j-4] == 'B')
                                                town_shot++;
                                        }
                                    }
                                }
                    }
                }
            }
        }
        else{
            for(int i = 0;i<board.size();i++)
            {
                for(int j = 0;j<board[0].size();j++)
                {
                    if(board[i][j] == 'b')
                    {
                        pawn++;
                        if(j-1>=0){
                            if(board[i][j-1] == 'w'){
                                surround = true;
                                kill++;
                            }
                            else if(board[i][j-1] == 'W'){
                                surround = true;
                                townkill++;
                            }
                        } 
 
                        if(j+1 < board.size()){ 
                            if(board[i][j+1] == 'w'){
                                surround = true;
                                kill++;
                            }
                            else if(board[i][j+1] == 'W'){
                                surround = true;
                                townkill++;
                            }
                        }
                        
                        if(j-1 >=0 && i-1 >=0){
                            if(board[i-1][j-1] == 'w'){
                                surround = true;
                                kill++;
                            }
                            else if(board[i-1][j-1] == 'W'){
                                surround = true;
                                townkill++;
                            }
                            else if(board[i-1][j-1] == ' ')
                                freedom++;
                        }
                        if(i-1 >=0){
                            if(board[i-1][j] == 'w'){
                                surround = true;
                                kill++;
                            }
                            else if(board[i-1][j] == 'W'){
                                surround = true;
                                townkill++;
                            }
                            else if(board[i-1][j] == ' ')
                                freedom++;
                        }
                        if(j+1 < board.size() && i-1 >=0){
                            if(board[i-1][j+1] == 'w'){
                                surround = true;
                                kill++;
                            }
                            else if(board[i-1][j+1] == 'W'){
                                surround = true;
                                townkill++;
                            }
                            else if(board[i-1][j+1] == ' ')
                                freedom++;
                        }

                        
                        if(surround == true)
                        {
                            if(i+2 <board.size())
                            {
                                if(j-2 >= 0){
                                    if(board[i+2][j-2] == ' ')
                                        freedom++;
                                    else if(board[i+2][j-2] == 'b')
                                        kill++;
                                }
                                if(j+2 <board.size()){
                                    if(board[i+2][j+2] == ' ')
                                        freedom++;
                                    else if(board[i+2][j+2] == 'b')
                                        kill++;
                                }
                            }
                        }

                        if(j+1 < board.size() && j-1 >=0)
                            if(board[i][j+1] == 'b')
                                if(board[i][j-1] == 'b'){
                                    cannon++;
                                    if(j-3 >= 0 && board[i][j-2] != ' ')
                                    {
                                        
                                        if(board[i][j-3] == 'w')
                                            pawn_shot++;
                                        if(board[i][j-3] == 'W')
                                            town_shot++;
                                        if(j-4 >= 0 && board[i][j-3] == ' ')
                                        {
                                            if(board[i][j-4] == 'w')
                                                pawn_shot++;
                                            if(board[i][j-4] == 'W')
                                                town_shot++;
                                        }
                                    }
                                    if(j+3 < board.size() && board[i][j+2] != ' ')
                                    {
                                        
                                        if(board[i][j+3] == 'w')
                                            pawn_shot++;
                                        if(board[i][j+3] == 'W')
                                            town_shot++;
                                        if(j+4 < board.size() && board[i][j+3] == ' ')
                                        {
                                            if(board[i][j+4] == 'w')
                                                pawn_shot++;
                                            if(board[i][j+4] == 'W')
                                                town_shot++;
                                        }
                                    }
                                }
                        if(i+1 < board.size() && i-1 >=0)
                            if(board[i+1][j] == 'b')
                                if(board[i-1][j] == 'b'){
                                    cannon++;
                                    if(i-3 >= 0 && board[i-2][j] != ' ')
                                    {
                                        
                                        if(board[i-3][j] == 'w')
                                            pawn_shot++;
                                        if(board[i-3][j] == 'W')
                                            town_shot++;
                                        if(i-4 >=0 && board[i-3][j] == ' ')
                                        {
                                            if(board[i-4][j] == 'w')
                                                pawn_shot++;
                                            if(board[i-4][j] == 'W')
                                                town_shot++;
                                        }
                                    }
                                    if(i+3 < board.size() && board[i+2][j] != ' ')
                                    {
                                        
                                        if(board[i+3][j] == 'w')
                                            pawn_shot++;
                                        if(board[i+3][j] == 'W')
                                            town_shot++;
                                        if(i+4 < board.size() && board[i+3][j] == ' ')
                                        {
                                            if(board[i+4][j] == 'w')
                                                pawn_shot++;
                                            if(board[i+4][j] == 'W')
                                                town_shot++;
                                        }
                                    }
                                }
                        if(i+1 < board.size() && i-1 >=0 && j+1 < board.size() && j-1 >=0)
                            if(board[i-1][j+1] == 'b')
                                if(board[i+1][j-1] == 'b'){
                                    cannon++;
                                    if(i-3 >= 0 && j+3 < board.size() && board[i-2][j+2] != ' ')
                                    {
                                        
                                        if(board[i-3][j+3] == 'w')
                                            pawn_shot++;
                                        if(board[i-3][j+3] == 'W')
                                            town_shot++;
                                        if(i-4 >= 0 && j+4 < board.size() && board[i-3][j+3] == ' ')
                                        {
                                            if(board[i-4][j+4] == 'w')
                                                pawn_shot++;
                                            if(board[i-4][j+4] == 'W')
                                                town_shot++;
                                        }
                                    }
                                    if(j-3 >= 0 && i+3 < board.size() && board[i+2][j-2] != ' ')
                                    {
                                        
                                        if(board[i+3][j-3] == 'w')
                                            pawn_shot++;
                                        if(board[i+3][j-3] == 'W')
                                            town_shot++;
                                        if(j-4 >= 0 && i+4 < board.size() && board[i+3][j-3] == ' ')
                                        {
                                            if(board[i+4][j-4] == 'w')
                                                pawn_shot++;
                                            if(board[i+4][j-4] == 'W')
                                                town_shot++;
                                        }
                                    }
                                }
                        if(i+1 < board.size() && i-1 >=0 && j+1 < board.size() && j-1 >=0)
                            if(board[i-1][j-1] == 'b')
                                if(board[i+1][j+1] == 'b'){
                                    cannon++;
                                    if(i+3 < board.size() && j+3 < board.size() && board[i+2][j+2] != ' ')
                                    {
                                        
                                        if(board[i+3][j+3] == 'w')
                                            pawn_shot++;
                                        if(board[i+3][j+3] == 'W')
                                            town_shot++;
                                        if(i+4 < board.size() && j+4 < board.size() && board[i+3][j+3] == ' ')
                                        {
                                            if(board[i+4][j+4] == 'w')
                                                pawn_shot++;
                                            if(board[i+4][j+4] == 'W')
                                                town_shot++;
                                        }
                                    }
                                    if(i-3 >=0 && j-3 >=0 && board[i-2][j-2] != ' ')
                                    {
                                        
                                        if(board[i-3][j-3] == 'w')
                                            pawn_shot++;
                                        if(board[i-3][j-3] == 'W')
                                            town_shot++;
                                        if(i-4 >=0 && j-4 >=0 && board[i-3][j-3] == ' ')
                                        {
                                            if(board[i-4][j-4] == 'w')
                                                pawn_shot++;
                                            if(board[i-4][j-4] == 'W')
                                                town_shot++;
                                        }
                                    }
                                }
                    }
                }
            }
        }
        vector<int> value;
        value.push_back(pawn);
        value.push_back(kill);
        value.push_back(townkill);
        value.push_back(freedom);
        value.push_back(cannon);
        value.push_back(pawn_shot);
        value.push_back(town_shot);
        return value;
    }

    int utility(bool getMyUtility){
    //return rand() % 100;
    
    vector<int> town = TownAndExposedTown(getMyUtility);
    vector<int> opptown = TownAndExposedTown(!getMyUtility);
    vector<int> pawn = PawnsAndKills(getMyUtility);
    vector<int> opppawn = PawnsAndKills(!getMyUtility);
    
    //int w_townhalls = 8;
    //int w_blocked = 3;
    //int w_vulnerable = -2;

    //int w_pawns = 1;
    //int w_pawn_moves = 0.5;
    //int w_pawn_kills = 4;
    //int w_town_kills = 10;
    //int w_cannon = 3;
    //int w_pawn_shot = 4;
    //int w_town_shot = 10;

    int a = w_townhalls*(town[0]-opptown[0])
            +w_blocked*(town[1]-opptown[1])
            +w_vulnerable*(town[2]-opptown[2])
            +w_pawns*(pawn[0]-opppawn[0])
            +w_pawn_kills*(pawn[1]-opppawn[1])
            +w_town_kills*(pawn[2]-opppawn[2])
            +w_pawn_moves*(pawn[3]-opppawn[3])
            +w_cannon*(pawn[4]-opppawn[4])
            +w_pawn_shot*(pawn[5]-opppawn[5])
            +w_town_shot*(pawn[6]-opppawn[6]);
    //cout << town[0] << "\n";
    //cout << town[1]<< "\n";
    //cout << town[2]<< "\n";
    //cout << pawn[0]<< "\n";
    //cout << pawn[1]<< "\n";
    //cout << pawn[2]<< "\n";
    //cout << pawn[3]<< "\n";
    //cout << pawn[4]<< "\n";
    //cout << pawn[5]<< "\n";
    //cout << pawn[6]<< "\n";
    //cout << "\n";
    //cout << a << "\n";
    /* cout << a << "\n";
    cout << parentMessage << "\n";
    for(int i =0;i<numRows;i++){
        for(int j = 0;j<numCols;j++)
            cout << board[i][j];
        cout << "\n";
    }*/
    //cout <<"\n";
    return a;
    }
    State getState(string move){
        //"S 1 2 M 4 6"
        
        if(move.length()==0)return State(!isWhite,numRows,numCols,board,move);//the case of stalemate
        vector<string> tokens;
        string word = "";
        for(int i =0 ;i<move.length();i++){
            if(move[i]==' '){
                tokens.push_back(word);
                // cout<<"pushing:"<<word<<endl;
                word = "";
            }
            else{
                word = word + move[i];
            }
        }
        if(word.length()>0)tokens.push_back(word);
        
        int bx = stoi(tokens[1]);
        
        int by = stoi(tokens[2]);
        
        int ex = stoi(tokens[4]);
        
        int ey = stoi(tokens[5]);
        
        vector<vector<char> >newBoard;
        newBoard = board;
        char type = tokens[3][0];
        if(type=='M'){
            newBoard[ex][ey] = myPawn;
            newBoard[bx][by] = ' ';
        }
        else{
            newBoard[ex][ey]= ' ';
        }

        State newState = State(!isWhite,numRows,numCols,newBoard,move);
        return newState;

    }

    int getMyTownHalls(){

        int count = 0;
        for(int i =0 ;i<board.size();i++){
            for(int j =0 ;j<board.size();j++){
                if(board[i][j]==myTower)count ++ ;
            }
        }
        return count;

    }
    int getOppnTownHalls(){
        
        int count = 0;
        for(int i =0 ;i<board.size();i++){
            for(int j =0 ;j<board.size();j++){
                if(board[i][j]==oppTower)count ++ ;
            }
        }
        return count;
    }
    int MaxVal(int alpha,int beta,int depth, int maxDepth){
        
        if(depth>=maxDepth)return utility(true);

        int maxChild = INT_MIN;
        //ofstream ofs("test.txt",std::ofstream::out);
        int neighbour_count = 0;
        messageToBestChild = "";//make sure that you don't effect future moves when you have stalemate
        for(int i = 0;i<numRows;i++){
            for(int j=0;j<numCols;j++){
                
                if(board[i][j]==myPawn){
                    
                    //cout << "N";
                    
                    vector<string> neighbours = getNeighbours(i,j);
                    
                    neighbour_count += neighbours.size();
                    
                    
                    for(int iter = 0;iter< neighbours.size();iter++){
                        
                        State newstate = getState(neighbours[iter]);
                        
                        //ofs << neighbours[iter] << "\n";
                        
                        int child = newstate.MinVal(alpha,beta,depth+1,maxDepth);
                        
                        if(child>maxChild)
                        {
                            maxChild = child;
                            messageToBestChild = neighbours[iter];
                        }
                        if(maxChild>alpha){
                            //cout << "Updating";
                            alpha = maxChild;
                        }
                        //if(alpha == 0)
                        //    cout<<alpha<<" "<<beta<<" "<<depth<<endl;
                        if(alpha>=beta){
                            //cout <<"Pruned";
                            //messageToBestChild = neighbours[iter];
                            return maxChild;
                        }
                    }
                }
                    
                    
            }
        }
        
        //ofs.close();
        if(neighbour_count == 0){
            //you should check if in the stalemate condition which has occured you win or loose
            messageToBestChild = "";//make sure that you don't effect future moves when you have stalemate
            int myhalls = getMyTownHalls();
            int opphalls = getOppnTownHalls();
            if(myhalls > opphalls){
                maxChild = INT_MAX-1;
                return INT_MAX-1;//since a stalemate condition
            }
            else if(myhalls == opphalls){
                maxChild = INT_MAX-2;
                return INT_MAX - 2;//since worse than a stalemate
            }
            else{
                maxChild = INT_MIN;
                return INT_MIN; 
            } 
        }

        
        return maxChild;
            
    }

    int MinVal(int alpha,int beta, int depth,int maxDepth){
        if(depth>=maxDepth)return utility(false);
        
        vector<vector<char> > tempBoard;
        tempBoard = board;

        int minChild = INT_MAX;
        messageToBestChild = "";//make sure that you don't effect future moves when you have stalemate
        int neighbour_count = 0;
        for(int i = 0;i<numRows;i++){
            for(int j=0;j<numCols;j++){
                if(board[i][j]==myPawn){
                    vector<string> neighbours = getNeighbours(i,j);
                    
                    neighbour_count += neighbours.size();
                    for(int iter = 0;iter< neighbours.size();iter++){
                        State newstate = getState(neighbours[iter]);
                        
                        int child = newstate.MaxVal(alpha,beta,depth+1,maxDepth);
                        //cout<<"Child :"<<child<< " minChild "<<minChild<<" node depth "<<depth<<endl;
                        if(child<minChild)
                        {messageToBestChild = neighbours[iter];minChild = child;}
                        if(minChild<beta){
                            beta = minChild;
                        }
                        

                        if(alpha>=beta){
                            //cout <<"Pruned";
                            //messageToBestChild = neighbours[iter];
                            return minChild;
                        }
                    }
                }
                    
                    
            }
        }

        if(neighbour_count==0){//since it is minval the utility of the opposition is actually the required utility
            messageToBestChild = "";//make sure that you don't effect future moves when you have stalemate  
            int myhalls = getMyTownHalls();
            int opphalls = getOppnTownHalls();
            if(myhalls < opphalls){
                minChild = INT_MAX-1;
                return INT_MAX-1;//since a stalemate condition
            }
            else if(myhalls == opphalls){
                minChild = INT_MAX-2;
                return INT_MAX - 2;//since worse than a stalemate
            }
            else{
                minChild = INT_MIN;
                return INT_MIN; 
            
        }
        
        }
        return minChild;
    }
    

};
void printBoard(vector<vector<char> > board,int numRows,int numCols){
    // cout<<"began printing"<<endl;
    // cout<<board.size()<<" "<<(board[0]).size()<<endl;
    for(int i =0;i<numRows;i++)
    {
        for(int j  = 0;j<numCols;j++){
            if(board[i][j]==' ')cout<<'.';
            else cout<<board[i][j];
            cout<<" ";
        }
        cout<<endl;
    }

    cout<<endl<<endl;

}

string correct_change(string input){
    //cout << "Entered";
    string s = input;
    vector<string> result;
    istringstream iss(s);
    for(string s; iss >> s; )
        result.push_back(s);
    string r = result[0] + " "+result[2] + " "+ result[1] + " "+ result[3] + " "+ result[5] +  " "+result[4];
    
    return r;
}

int main(){

    vector<vector<char> > board;
    int numRows = 8;
    int numCols = 8;
    int isB = 0;
    bool isBlack = true;
    float timeleft;
    int blank;
    
    srand (time(0));
    cin >> isB;
    cin  >> numRows;
    cin >> numCols;
    cin >> timeleft;
    //cin >> blank;
    //cout << "S" <<blank;
    if(isB == 1)
        isBlack = true;
    else
        isBlack = false;

    

    for(int i =0 ;i<numRows;i++){
        vector<char> row;
        for(int j = 0;j<numCols;j++){
            row.push_back(' ');
        }
        board.push_back(row);
    }
    for(int i = 0;i<3;i++){
            for(int j = 1;j<numCols;j+=2){
                board[i][j]='w';
            }
            if(i==0){
                for(int j=0;j<numCols;j+=2)
                    board[i][j]='W';
            }
    }

    for(int i = numRows-1;i>=numRows-3;i--){
        for(int j = 0;j<numCols;j+=2){
            board[i][j]='b';
        }
        if(i==numRows-1){
            for(int j = 1;j<numCols;j+=2){
                board[i][j]='B';   
            }
        }

    }
    
    State current_state = State(!isBlack,numRows,numCols,board);
    //printBoard(current_state.board,numRows,numCols);
    
    if(isBlack){
        string myMove= current_state.generateMove();
        myMove = correct_change(myMove);
        cout<<myMove<<endl;
        //printBoard(current_state.board,numRows,numCols);
        
    }
    while(true){

    
        string opponentsMove = "";
        //getline(cin,opponentsMove);
        char s;
        char xs;
        char ys;
        char m;
        char xm;
        char ym;
        cin >> s;
        cin >> xs;
        cin >> ys;
        cin >> m;
        cin >> xm;
        cin >> ym;
        opponentsMove = opponentsMove + s +" "+ys+" " +xs+" "+m+" "+ym+" "+xm;
        
        
        //break;
        
        // cout<<"About to update board"<<endl;
        
        current_state.updateBoard(opponentsMove,false);
        
        //printBoard(current_state.board,numRows,numCols);
        //printBoard(current_state.board,numRows,numCols);
        // cout<<"Updated board"<<endl;
        

        // if(current_state.isGameOver())
        //     break;
        
        string myMove = current_state.generateMove();
        myMove = correct_change(myMove);
        cout<<myMove<<"\n";
        //printBoard(current_state.board,numRows,numCols);
        // if(current_state.isGameOver())
            // break;
        
    }
    

    return 0;

}
