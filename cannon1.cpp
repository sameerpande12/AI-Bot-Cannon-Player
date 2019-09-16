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

int w_townhalls = 100;
int w_pawns = 60;
int w_cannon = 10;
int w_verti_cannon = 30;
int global_count = 0;

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
        
        int cost = 0;
        if(global_count<2)
            cost = MaxVal(INT_MIN,INT_MAX,0,3);
        else if(global_count<6)
            cost = MaxVal(INT_MIN,INT_MAX,0,3);
        else
            cost = MaxVal(INT_MIN,INT_MAX,0,4);
        
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
                vector<string> neighboursKill;
                vector<string> neighboursMove;
                vector<string> blankShots;
                if(isWhite){
                    
                    vector<vector<char> >tempBoard = board;   
                    if(tempBoard[i][j]==myPawn){
                        //regular soldier capture

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
                                            if(tempBoard[i+3][j+3]==' ')
                                                blankShots.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i+3)+" "+to_string(j+3));
                                            else
                                                neighboursKill.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i+3)+" "+to_string(j+3));
                                        }


                                    }

                                    if( i-3 >=0 && j-3 >=0){
                                        if(tempBoard[i-2][j-2] == ' ' && tempBoard[i-3][j-3]!=myPawn && tempBoard[i-3][j-3]!=myTower){
                                            //shot
                                            if(tempBoard[i-3][j-3]==' ')
                                                blankShots.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i-3)+" "+to_string(j-3));
                                            else
                                                neighboursKill.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i-3)+" "+to_string(j-3));
                                        }

                                    }
                                    //3shots begin

                                    if(  i+4 <numRows && j+4 <numCols){
                                        if(tempBoard[i+2][j+2]==' '  && tempBoard[i+4][j+4]!=myPawn && tempBoard[i+4][j+4]!=myTower){
                                            //shot
                                            if(tempBoard[i+4][j+4]==' ')
                                                blankShots.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i+4)+" "+to_string(j+4));
                                            else 
                                                neighboursKill.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i+4)+" "+to_string(j+4));
                                        }


                                    }

                                    if( i-4 >=0 && j-4 >=0){
                                        if(tempBoard[i-2][j-2] == ' ' && tempBoard[i-4][j-4]!=myPawn && tempBoard[i-4][j-4]!=myTower){
                                            //shot
                                            if(tempBoard[i-4][j-4]==' ')
                                                blankShots.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i-4)+" "+to_string(j-4));
                                            else
                                                neighboursKill.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i-4)+" "+to_string(j-4));        

                                        }

                                    }
                                    //movement

                                    if(i-2>=0 && j-2 >=0){
                                        if(tempBoard[i-2][j-2]== ' '){
                                            //move
                                            neighboursMove.push_back("S "+to_string(i+1)+" "+to_string(j+1)+" M "+to_string(i-2)+" "+to_string(j-2));
                                        }
                                    }

                                    if(i+2<numRows && j+2 <numCols){
                                        if(tempBoard[i+2][j+2]== ' '){
                                            //move
                                            neighboursMove.push_back("S "+to_string(i-1)+" "+to_string(j-1)+" M "+to_string(i+2)+" "+to_string(j+2));
                                        }
                                    }

                                }

                                if( (tempBoard[i-1][j+1]==tempBoard[i][j]) && ( tempBoard[i][j]== tempBoard[i+1][j-1])){
                                    if(i-3 >=0 && j+3 < numCols){
                                        if(tempBoard[i-2][j+2]==' ' && tempBoard[i-3][j+3]!=myPawn && tempBoard[i-3][j+3]!=myTower ){
                                            //shot
                                            if(tempBoard[i-3][j+3]==' ')
                                                blankShots.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i-3)+" "+to_string(j+3));
                                            else
                                                neighboursKill.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i-3)+" "+to_string(j+3));
                                        }
                                    }

                                    if(i+3 < numRows && j-3 >=0){
                                        if(tempBoard[i+2][j-2]==' ' && tempBoard[i+3][j-3]!=myPawn && tempBoard[i+3][j-3]!=myTower){
                                            //shot
                                            if(tempBoard[i+3][j-3]==' ')
                                                blankShots.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i+3)+" "+to_string(j-3));
                                            else 
                                                neighboursKill.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i+3)+" "+to_string(j-3));
                                        }
                                    }
                                    //3 shots 
                                    if(i-4 >=0 && j+4 < numCols){
                                        if(tempBoard[i-2][j+2]==' ' &&  tempBoard[i-3][j+3] == ' '&& tempBoard[i-4][j+4]!=myPawn && tempBoard[i-4][j+4]!=myTower ){
                                            //shot
                                            if(tempBoard[i-4][j+4]==' ')
                                                blankShots.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i-4)+" "+to_string(j+4));
                                            else
                                                neighboursKill.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i-4)+" "+to_string(j+4));
                                        }
                                    }

                                    if(i+4 < numRows && j-4 >=0){
                                        if(tempBoard[i+2][j-2]==' '  && tempBoard[i+4][j-4]!=myPawn && tempBoard[i+4][j-4]!=myTower){
                                            //shot
                                            if(tempBoard[i+4][j-4]== ' ')
                                                blankShots.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i+4)+" "+to_string(j-4));
                                            else
                                                neighboursKill.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i+4)+" "+to_string(j-4));
                                        }
                                    }

                                    //movement
                                    if(i-2 >= 0 && j+2 < numCols){
                                        if(tempBoard[i-2][j+2]== ' '){
                                        //move
                                            neighboursMove.push_back("S "+to_string(i+1)+" "+to_string(j-1)+" M "+to_string(i-2)+" "+to_string(j+2));
                                        }
                                    }

                                    if(i+2 < numRows && j-2 >=0){
                                        if(tempBoard[i+2][j-2]==' '){
                                            //move
                                            neighboursMove.push_back("S "+to_string(i-1)+" "+to_string(j+1)+" M "+to_string(i+2)+" "+to_string(j-2));
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
                                        if(tempBoard[i+3][j]==' ')
                                           blankShots.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i+3)+" "+to_string(j));
                                        else
                                            neighboursKill.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i+3)+" "+to_string(j));
                                    }
                                }
                                if(i-3 >= 0){
                                    if(tempBoard[i-2][j]==' ' && tempBoard[i-3][j]!=myPawn && tempBoard[i-3][j]!=myTower){
                                        //shot
                                        if(tempBoard[i-3][j]==' ')
                                            blankShots.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i-3)+" "+to_string(j));
                                        else
                                            neighboursKill.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i-3)+" "+to_string(j));

                                    }
                                }
                                //3shots
                                if(i+4 < numRows){
                                    if(tempBoard[i+2][j]==' ' && tempBoard[i+4][j]!=myPawn && tempBoard[i+4][j]!=myTower){
                                        //shot
                                        if(tempBoard[i+4][j]==' ')
                                            blankShots.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i+4)+" "+to_string(j));
                                        else
                                            neighboursKill.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i+4)+" "+to_string(j));
                                    }
                                }
                                if(i-4 >= 0){
                                    if(tempBoard[i-2][j]==' ' && tempBoard[i-4][j]!=myPawn && tempBoard[i-4][j]!=myTower){
                                        //shot
                                        if(tempBoard[i-4][j]==' ')
                                            blankShots.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i-4)+" "+to_string(j));
                                        else
                                            neighboursKill.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i-4)+" "+to_string(j));

                                    }
                                }

                                //movement
                                if(i+2<numRows){
                                    if(tempBoard[i+2][j]==' '){
                                        //move
                                        
                                            neighboursMove.push_back("S "+to_string(i-1)+" "+to_string(j)+" M "+to_string(i+2)+" "+to_string(j));
                                    }

                                }
                                if(i-2 >=0){
                                    if(tempBoard[i-2][j]==' '){
                                        //move
                                        neighboursMove.push_back("S "+to_string(i+1)+" "+to_string(j)+" M "+to_string(i-2)+" "+to_string(j));
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
                                        if(tempBoard[i][j-3]==' ')
                                            blankShots.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i)+" "+to_string(j-3));
                                        else
                                            neighboursKill.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i)+" "+to_string(j-3));
                                    }
                                }

                                if(j+3 < numCols){
                                    if(tempBoard[i][j+2]==' '&& tempBoard[i][j+3]!=myPawn && tempBoard[i][j+3]!=myTower){
                                        //shot
                                        if(tempBoard[i][j+3]==' ')
                                            blankShots.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i)+" "+to_string(j+3));
                                        else
                                            neighboursKill.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i)+" "+to_string(j+3));
                                    }
                                }
                                //3shots

                                if(j-4>=0 ){
                                    //shot
                                    if(tempBoard[i][j-2]==' '  && tempBoard[i][j-4]!=myPawn && tempBoard[i][j-4]!=myTower){
                                        if(tempBoard[i][j-4]==' ')
                                            blankShots.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i)+" "+to_string(j-4));
                                        else
                                            neighboursKill.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i)+" "+to_string(j-4));
                                    }
                                }

                                if(j+4 < numCols){
                                    if(tempBoard[i][j+2]==' '&& tempBoard[i][j+4]!=myPawn && tempBoard[i][j+4]!=myTower){
                                        //shot
                                        if(tempBoard[i][j+4])
                                            blankShots.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i)+" "+to_string(j+4));
                                        else
                                            neighboursKill.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i)+" "+to_string(j+4));
                                    }
                                }

                                //movement
                                if(j+2<numCols){
                                    if(tempBoard[i][j+2]==' '){
                                        //move right
                                        neighboursMove.push_back("S "+to_string(i)+" "+to_string(j-1)+" M "+to_string(i)+" "+to_string(j+2));
                                    }
                                }
                                if(j-2 >= 0){
                                    if(tempBoard[i][j-2]==' '){
                                        //moveLeft
                                        neighboursMove.push_back("S "+to_string(i)+" "+to_string(j+1)+" M "+to_string(i)+" "+to_string(j-2));
                                    }
                                }


                            }
                        }

                        for(int dy = 1;dy>=-2;dy--){
                            if(dy==0){
                                for(int dx = -1;dx<=1;dx+=2){
                                    if(j+dx>=0 && j+dx <numCols){
                                        
                                        if(tempBoard[i+dy][j+dx]==oppPawn || tempBoard[i+dy][j+dx]==oppTower){
                                            neighboursKill.push_back("S "+to_string(i)+" "+to_string(j)+" M "+to_string(i+dy)+" "+to_string(j+dx));
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
                                            if(tempBoard[i+dy][j+dx]==' ')
                                                neighboursMove.push_back("S "+to_string(i)+" "+to_string(j)+" M "+to_string(i+dy)+" "+to_string(j+dx));
                                            else
                                                neighboursKill.push_back("S "+to_string(i)+" "+to_string(j)+" M "+to_string(i+dy)+" "+to_string(j+dx));
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
                                                    if(tempBoard[i+dy][j+dx]==' ')
                                                        neighboursMove.push_back("S "+to_string(i)+" "+to_string(j)+" M "+to_string(i+dy)+" "+to_string(j+dx));
                                                    else
                                                        neighboursKill.push_back("S "+to_string(i)+" "+to_string(j)+" M "+to_string(i+dy)+" "+to_string(j+dx));
                                                }
                                            }
                                        }

                                    }

                            }

                        }



                    }
                }
                else{
                    vector<vector<char> >tempBoard = board;
                        
                        if(tempBoard[i][j]==myPawn){

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
                                                if(tempBoard[i+3][j+3]==' ')
                                                    blankShots.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i+3)+" "+to_string(j+3));
                                                else
                                                    neighboursKill.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i+3)+" "+to_string(j+3));
                                            }


                                        }

                                        if( i-3 >=0 && j-3 >=0){
                                            if(tempBoard[i-2][j-2] == ' ' && tempBoard[i-3][j-3]!=myPawn && tempBoard[i-3][j-3]!=myTower){
                                                //shot
                                                if(tempBoard[i-3][j-3]==' ')
                                                    blankShots.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i-3)+" "+to_string(j-3));
                                                else
                                                    neighboursKill.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i-3)+" "+to_string(j-3));
                                            }

                                        }
                                        //3shots begin

                                        if(  i+4 <numRows && j+4 <numCols){
                                            if(tempBoard[i+2][j+2]==' '  && tempBoard[i+4][j+4]!=myPawn && tempBoard[i+4][j+4]!=myTower){
                                                //shot
                                                if(tempBoard[i+4][j+4]==' ')
                                                    blankShots.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i+4)+" "+to_string(j+4));
                                                else 
                                                    neighboursKill.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i+4)+" "+to_string(j+4));
                                            }


                                        }

                                        if( i-4 >=0 && j-4 >=0){
                                            if(tempBoard[i-2][j-2] == ' ' && tempBoard[i-4][j-4]!=myPawn && tempBoard[i-4][j-4]!=myTower){
                                                //shot
                                                if(tempBoard[i-4][j-4]==' ')
                                                    blankShots.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i-4)+" "+to_string(j-4));
                                                else
                                                    neighboursKill.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i-4)+" "+to_string(j-4));        

                                            }

                                        }
                                        //movement

                                        if(i-2>=0 && j-2 >=0){
                                            if(tempBoard[i-2][j-2]== ' '){
                                                //move
                                                neighboursMove.push_back("S "+to_string(i+1)+" "+to_string(j+1)+" M "+to_string(i-2)+" "+to_string(j-2));
                                            }
                                        }

                                        if(i+2<numRows && j+2 <numCols){
                                            if(tempBoard[i+2][j+2]== ' '){
                                                //move
                                                neighboursMove.push_back("S "+to_string(i-1)+" "+to_string(j-1)+" M "+to_string(i+2)+" "+to_string(j+2));
                                            }
                                        }

                                    }

                                    if( (tempBoard[i-1][j+1]==tempBoard[i][j]) && ( tempBoard[i][j]== tempBoard[i+1][j-1])){
                                        if(i-3 >=0 && j+3 < numCols){
                                            if(tempBoard[i-2][j+2]==' ' && tempBoard[i-3][j+3]!=myPawn && tempBoard[i-3][j+3]!=myTower ){
                                                //shot
                                                if(tempBoard[i-3][j+3]==' ')
                                                    blankShots.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i-3)+" "+to_string(j+3));
                                                else
                                                    neighboursKill.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i-3)+" "+to_string(j+3));
                                            }
                                        }

                                        if(i+3 < numRows && j-3 >=0){
                                            if(tempBoard[i+2][j-2]==' ' && tempBoard[i+3][j-3]!=myPawn && tempBoard[i+3][j-3]!=myTower){
                                                //shot
                                                if(tempBoard[i+3][j-3]==' ')
                                                    blankShots.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i+3)+" "+to_string(j-3));
                                                else 
                                                    neighboursKill.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i+3)+" "+to_string(j-3));
                                            }
                                        }
                                        //3 shots 
                                        if(i-4 >=0 && j+4 < numCols){
                                            if(tempBoard[i-2][j+2]==' ' &&  tempBoard[i-3][j+3] == ' '&& tempBoard[i-4][j+4]!=myPawn && tempBoard[i-4][j+4]!=myTower ){
                                                //shot
                                                if(tempBoard[i-4][j+4]==' ')
                                                    blankShots.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i-4)+" "+to_string(j+4));
                                                else
                                                    neighboursKill.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i-4)+" "+to_string(j+4));
                                            }
                                        }

                                        if(i+4 < numRows && j-4 >=0){
                                            if(tempBoard[i+2][j-2]==' '  && tempBoard[i+4][j-4]!=myPawn && tempBoard[i+4][j-4]!=myTower){
                                                //shot
                                                if(tempBoard[i+4][j-4]== ' ')
                                                    blankShots.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i+4)+" "+to_string(j-4));
                                                else
                                                    neighboursKill.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i+4)+" "+to_string(j-4));
                                            }
                                        }

                                        //movement
                                        if(i-2 >= 0 && j+2 < numCols){
                                            if(tempBoard[i-2][j+2]== ' '){
                                            //move
                                                neighboursMove.push_back("S "+to_string(i+1)+" "+to_string(j-1)+" M "+to_string(i-2)+" "+to_string(j+2));
                                            }
                                        }

                                        if(i+2 < numRows && j-2 >=0){
                                            if(tempBoard[i+2][j-2]==' '){
                                                //move
                                                neighboursMove.push_back("S "+to_string(i-1)+" "+to_string(j+1)+" M "+to_string(i+2)+" "+to_string(j-2));
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
                                            if(tempBoard[i+3][j]==' ')
                                            blankShots.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i+3)+" "+to_string(j));
                                            else
                                                neighboursKill.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i+3)+" "+to_string(j));
                                        }
                                    }
                                    if(i-3 >= 0){
                                        if(tempBoard[i-2][j]==' ' && tempBoard[i-3][j]!=myPawn && tempBoard[i-3][j]!=myTower){
                                            //shot
                                            if(tempBoard[i-3][j]==' ')
                                                blankShots.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i-3)+" "+to_string(j));
                                            else
                                                neighboursKill.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i-3)+" "+to_string(j));

                                        }
                                    }
                                    //3shots
                                    if(i+4 < numRows){
                                        if(tempBoard[i+2][j]==' ' && tempBoard[i+4][j]!=myPawn && tempBoard[i+4][j]!=myTower){
                                            //shot
                                            if(tempBoard[i+4][j]==' ')
                                                blankShots.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i+4)+" "+to_string(j));
                                            else
                                                neighboursKill.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i+4)+" "+to_string(j));
                                        }
                                    }
                                    if(i-4 >= 0){
                                        if(tempBoard[i-2][j]==' ' && tempBoard[i-4][j]!=myPawn && tempBoard[i-4][j]!=myTower){
                                            //shot
                                            if(tempBoard[i-4][j]==' ')
                                                blankShots.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i-4)+" "+to_string(j));
                                            else
                                                neighboursKill.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i-4)+" "+to_string(j));

                                        }
                                    }

                                    //movement
                                    if(i+2<numRows){
                                        if(tempBoard[i+2][j]==' '){
                                            //move
                                            
                                                neighboursMove.push_back("S "+to_string(i-1)+" "+to_string(j)+" M "+to_string(i+2)+" "+to_string(j));
                                        }

                                    }
                                    if(i-2 >=0){
                                        if(tempBoard[i-2][j]==' '){
                                            //move
                                            neighboursMove.push_back("S "+to_string(i+1)+" "+to_string(j)+" M "+to_string(i-2)+" "+to_string(j));
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
                                            if(tempBoard[i][j-3]==' ')
                                                blankShots.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i)+" "+to_string(j-3));
                                            else
                                                neighboursKill.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i)+" "+to_string(j-3));
                                        }
                                    }

                                    if(j+3 < numCols){
                                        if(tempBoard[i][j+2]==' '&& tempBoard[i][j+3]!=myPawn && tempBoard[i][j+3]!=myTower){
                                            //shot
                                            if(tempBoard[i][j+3]==' ')
                                                blankShots.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i)+" "+to_string(j+3));
                                            else
                                                neighboursKill.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i)+" "+to_string(j+3));
                                        }
                                    }
                                    //3shots

                                    if(j-4>=0 ){
                                        //shot
                                        if(tempBoard[i][j-2]==' '  && tempBoard[i][j-4]!=myPawn && tempBoard[i][j-4]!=myTower){
                                            if(tempBoard[i][j-4]==' ')
                                                blankShots.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i)+" "+to_string(j-4));
                                            else
                                                neighboursKill.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i)+" "+to_string(j-4));
                                        }
                                    }

                                    if(j+4 < numCols){
                                        if(tempBoard[i][j+2]==' '&& tempBoard[i][j+4]!=myPawn && tempBoard[i][j+4]!=myTower){
                                            //shot
                                            if(tempBoard[i][j+4])
                                                blankShots.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i)+" "+to_string(j+4));
                                            else
                                                neighboursKill.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i)+" "+to_string(j+4));
                                        }
                                    }

                                    //movement
                                    if(j+2<numCols){
                                        if(tempBoard[i][j+2]==' '){
                                            //move right
                                            neighboursMove.push_back("S "+to_string(i)+" "+to_string(j-1)+" M "+to_string(i)+" "+to_string(j+2));
                                        }
                                    }
                                    if(j-2 >= 0){
                                        if(tempBoard[i][j-2]==' '){
                                            //moveLeft
                                            neighboursMove.push_back("S "+to_string(i)+" "+to_string(j+1)+" M "+to_string(i)+" "+to_string(j-2));
                                        }
                                    }


                                }
                            }
                            //regular soldier capture
                            for(int dy = -1;dy<=2;dy++){
                                if(dy==0){
                                    for(int dx = -1;dx<=1;dx+=2){
                                        if(j+dx>=0 && j+dx <numCols){
                                            
                                            if(tempBoard[i+dy][j+dx]==oppPawn || tempBoard[i+dy][j+dx]==oppTower ){
                                                // vector<vector<char>>nextBoard = tempBoard;
                                                // nextBoard[i][j]=' ';
                                                // nextBoard[i+dy][j+dx]=myPawn;
                                                
                                                    neighboursKill.push_back("S "+to_string(i)+" "+to_string(j)+" M "+to_string(i+dy)+" "+to_string(j+dx));
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
                                                if(tempBoard[i+dy][j+dx]==' ')
                                                    neighboursMove.push_back("S "+to_string(i)+" "+to_string(j)+" M "+to_string(i+dy)+" "+to_string(j+dx));
                                                else 
                                                    neighboursKill.push_back("S "+to_string(i)+" "+to_string(j)+" M "+to_string(i+dy)+" "+to_string(j+dx));
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
                                                        neighboursKill.push_back("S "+to_string(i)+" "+to_string(j)+" M "+to_string(i+dy)+" "+to_string(j+dx));
                                                    }
                                                }
                                            }

                                        }

                                }

                            }

                            //cannons check if cannon shooting is possible
                            

                        }

                        

                    

                }
                for(int i =0;i<neighboursMove.size();i++){
                    neighboursKill.push_back(neighboursMove[i]);
                }
                for(int i = 0;i<blankShots.size();i++)
                    neighboursKill.push_back(blankShots[i]);

                return neighboursKill;
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
                    if(board[0][j] == 'W')
                        townHalls++;
                }
            }
        }
        else{
            for(int j=0;j<numCols;j++){
                if(j%2 == 1){
                    if(board[numRows-1][j] == 'B')
                        townHalls++;
                }
            }
        }
        vector<int> value;
        value.push_back(townHalls);/* 
        ofstream ofs("cannon.txt", std::ofstream::out);
        ofs << townHalls << "\n";
        ofs << blocked << "\n";
        ofs << vulnerable << "\n";
        ofs.close();     */   
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
        int verti_cannon = 0;

        if((Mine && myPawn == 'w') || (!Mine && myPawn == 'b')){
            //cout << parentMessage << "\n";
            for(int i = 0;i<board.size();i++)
            {
                //cout <<"\n";
                for(int j = 0;j<board[0].size();j++)
                {
                    //cout << board[i][j] << " ";
                    if(board[i][j] == 'w')
                    {
                        pawn++;
                        
                        if(j+1 < board.size() && j-1 >=0)
                            if(board[i][j+1] == 'w')
                                if(board[i][j-1] == 'w')
                                    cannon++;
                                    
                        if(i+1 < board.size() && i-1 >=0)
                            if(board[i+1][j] == 'w')
                                if(board[i-1][j] == 'w')
                                    verti_cannon++;
                                    
                        if(i+1 < board.size() && i-1 >=0 && j+1 < board.size() && j-1 >=0)
                            if(board[i-1][j+1] == 'w')
                                if(board[i+1][j-1] == 'w')
                                    cannon++;
                                    
                        if(i+1 < board.size() && i-1 >=0 && j+1 < board.size() && j-1 >=0)
                            if(board[i-1][j-1] == 'w')
                                if(board[i+1][j+1] == 'w')
                                    cannon++;
                                    
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
                        
                        if(j+1 < board.size() && j-1 >=0)
                            if(board[i][j+1] == 'b')
                                if(board[i][j-1] == 'b')
                                    cannon++;
                                    
                        if(i+1 < board.size() && i-1 >=0)
                            if(board[i+1][j] == 'b')
                                if(board[i-1][j] == 'b')
                                    verti_cannon++;
                                    
                        if(i+1 < board.size() && i-1 >=0 && j+1 < board.size() && j-1 >=0)
                            if(board[i-1][j+1] == 'b')
                                if(board[i+1][j-1] == 'b')
                                    cannon++;
                                    
                        if(i+1 < board.size() && i-1 >=0 && j+1 < board.size() && j-1 >=0)
                            if(board[i-1][j-1] == 'b')
                                if(board[i+1][j+1] == 'b')
                                    cannon++;
                                    
                    }
                }
            }
        }
        vector<int> value;
        //cout <<"\n"<<parentMessage<<"\n";
        value.push_back(pawn);
        value.push_back(cannon);
        value.push_back(verti_cannon);
        //cout << pawn << "\n";
        //cout <<cannon << "\n";
        //cout << verti_cannon << "\n";
        //cout << "\n";
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
    if(opptown[0] <= 2)
        return INT_MAX;
    int pawnaggression = pawn[0] - opppawn[0];
    int townaggression = town[0] - opptown[0];
    //if(town[0] ==3 && town[2] >0)
    //    return -12000;
    if(opptown[0] >2 && opppawn[0] < 1)
        return INT_MIN;
    int a = 0;
    /* 
    if(pawnaggression+2*townaggression > 3)
        a = w_townhalls*(town[0]-opptown[0])
                +w_blocked*(town[1]-opptown[1])
                +w_vulnerable*(town[2]-opptown[2])
                +w_pawns*(pawn[0]-2*opppawn[0])
                +2*w_pawn_kills*(pawn[1]-opppawn[1])
                +3*w_town_kills*(pawn[2]-opppawn[2])
                +w_pawn_moves*(pawn[3]-opppawn[3])
                +w_cannon*(pawn[4]-opppawn[4])
                +2*w_pawn_shot*(pawn[5]-opppawn[5])
                +w_town_shot*(pawn[6]-opppawn[6])
                +40*(pawn[7]-opppawn[7]);
    else if((pawnaggression+2*townaggression < 5))
        a = w_townhalls*(4*town[0]-opptown[0])
                +4*w_blocked*(town[1]-opptown[1])
                +2*w_vulnerable*(town[2]-opptown[2])
                +w_pawns*(pawn[0]-opppawn[0])
                +0.5*w_pawn_kills*(pawn[1]-opppawn[1])
                +w_town_kills*(pawn[2]-opppawn[2])
                +w_pawn_moves*(pawn[3]-opppawn[3])
                +w_cannon*(pawn[4]-opppawn[4])
                +w_pawn_shot*(pawn[5]-opppawn[5])
                +w_town_shot*(pawn[6]-opppawn[6])
                +40*(pawn[7]-opppawn[7]);*/
    a = w_townhalls*(town[0]-opptown[0])
                +w_pawns*(pawn[0]-opppawn[0])
                +w_cannon*(pawn[1]-opppawn[1])
                +w_verti_cannon*(pawn[2]-opppawn[2]);
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
        //ofstream ofs("cannon.txt", std::ofstream::out);
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
                        //ofs << blocked << "\n";
                        //ofs << vulnerable << "\n";
         
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
        //ofs.close();
        if(neighbour_count == 0){
            //you should check if in the stalemate condition which has occured you win or loose
            messageToBestChild = "";//make sure that you don't effect future moves when you have stalemate
            int myhalls = getMyTownHalls();
            int opphalls = getOppnTownHalls();
            if(myhalls > opphalls){
                maxChild = -50;
                return -50;//since a stalemate condition
            }
            else if(myhalls == opphalls){
                maxChild = -50;
                return -50;//since worse than a stalemate
            }
            else{
                maxChild = -50;
                return -50; 
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
                minChild = -50;
                return -50;//since a stalemate condition
            }
            else if(myhalls == opphalls){
                minChild = -50;
                return -50;//since worse than a stalemate
            }
            else{
                minChild = -50;
                return -50; 
            
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

    /* ifstream ifs("weight1.txt",std::ifstream::in);
    ifs >> w_townhalls;
    ifs >>  w_pawns;
    ifs >>  w_cannon;
    ifs >>  w_verti_cannon;*/

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
        global_count++;
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
        global_count++;
        //printBoard(current_state.board,numRows,numCols);
        //printBoard(current_state.board,numRows,numCols);
        // cout<<"Updated board"<<endl;
        

        // if(current_state.isGameOver())
        //     break;
        
        string myMove = current_state.generateMove();
        global_count++;
        myMove = correct_change(myMove);
        cout<<myMove<<"\n";
        //printBoard(current_state.board,numRows,numCols);
        // if(current_state.isGameOver())
            // break;
        
    }
    

    return 0;

}
