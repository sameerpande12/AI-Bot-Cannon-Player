#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>

#include<climits>
using namespace std;



class State{

    public:
    bool isWhite;
    vector<vector<char>> board;
    string parentMessage;
    int numRows;
    int numCols;
    char myPawn;
    char myTower;

    char oppPawn;
    char oppTower;
    State(bool white,int nRows, int nCols,vector<vector<char>>inputBoard,string inputMessage=""){

        isWhite = white;
        board = inputBoard;
        parentMessage = inputMessage;
        numRows = nRows;
        numCols = nCols;

        if(white){

            myPawn = 'w';
            myTower='W';
            oppPawn='b';
            oppTower='B';
        }
        // for(int i =0 ;i<numRows;i++){
        //     vector<char> row;
        //     for(int j = 0;j<numCols;j++){
        //         row.push_back(' ');
        //     }
        //     board.push_back(row);
        // }
        // for(int i = 0;i<3;i++){
        //         for(int j = 1;j<numCols;j+=2){
        //             board[i][j]='w';
        //         }
        //         if(i==0){
        //             for(int j=0;j<numCols;j+=2)
        //                board[i][j]='W';
        //         }
        // }

        // for(int i = numRows-1;i>=numRows-3;i--){
        //     for(int j = 0;j<numCols;j+=2){
        //         board[i][j]='b';
        //     }
        //     if(i==numRows-i){
        //         for(int j = 0;j<numCols;j+=2){
        //             board[i][j]='B';   
        //         }
        //     }

        // }
    }

    string generateMove(){


    }
    void updateBoard(string opponentsMove){

    }

    bool isGameOver(){

    }

    int utility(bool getMyUtility){


    }

    int MaxVal(int alpha,int beta,int depth, int maxDepth){
        if(depth>=maxDepth)utility(true);

        vector<vector<char>> tempBoard;
        tempBoard = board;
        int maxChild = INT_MIN;
        while(true){

                if(isWhite){
                    
                    for(int i = 0;i<board.size;i++){
                        for(int j = 0;j<board.size;j++){
                            
                            if(tempBoard[i][j]==myPawn){
                                //regular soldier capture
                                for(int dy = 1;dy>=-2;dy--){
                                    if(dy==0){
                                        for(int dx = -1;dx<=1;dx+=2){
                                            if(j+dx>=0 && j+dx <numCols){
                                                
                                                if(tempBoard[i+dy][j+dx]==oppPawn){
                                                    vector<vector<char>>nextBoard = tempBoard;
                                                    nextBoard[i][j]=' ';
                                                    nextBoard[i+dy][j+dx]=myPawn;
                                                }
                                            }
                                        }
                                    }
                                    else if(dy==-1)continue;
                                    else if(dy==1){

                                        if(i+dy >= numRows)continue;

                                        for(int dx=-1;dx<=1;dx++){
                                            if(j+dx>=0 && j+dx<numCols){
                                                if(tempBoard[i+dy][j+dx]==oppPawn || tempBoard[i+dy][j+dx]== ' '){
                                                    vector<vector<char>>nextBoard = tempBoard;
                                                    nextBoard[i][j]=' ';
                                                    nextBoard[i+dy][j+dx]=myPawn;
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
                                                for(int dx = -1 ;dx<=1;dx+=2){
                                                    if(j+dx >=0 && j+dx <numCols){
                                                        if(tempBoard[i+dy][j+dx]!=myPawn && tempBoard[i+dy][j+dx]!=myTower){
                                                            vector<vector<char>>nextBoard = tempBoard;
                                                            nextBoard[i][j]=' ';
                                                            nextBoard[i+dy][j+dx]=myPawn;
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
                                        w    O     w
                                         w   R    w
                                          w      w
                                        */

                                       if(tempBoard[i+1][j+1] == tempBoard[i][j]==tempBoard[i-1][j-1]){

                                           if(  i+3 <numRows && j+3 <numCols){
                                               if(tempBoard[i+2][j+2]==' ' && tempBoard[i+3][j+3]!=myPawn && tempBoard[i+3][j+3]!=myTower){
                                                   //shot
                                               }


                                           }

                                           if( i-3 >=0 && j-3 >=0){
                                               if(tempBoard[i-2][j-2] == ' ' && tempBoard[i-3][j-3]!=myPawn && tempBoard[i-3][j-3]!=myTower){
                                                   //shot
                                               }

                                           }

                                           //movement

                                           if(i-2>=0 && j-2 >=0){
                                               if(tempBoard[i-2][j-2]== ' '){
                                                   //move
                                               }
                                           }

                                            if(i+2>=0 && j+2 >=0){
                                               if(tempBoard[i+2][j+2]== ' '){
                                                   //move
                                               }
                                           }

                                       }

                                       if(tempBoard[i-1][j+1]==tempBoard[i][j] == tempBoard[i+1][j-1]){
                                           if(i-3 >=0 && j+3 < numCols){
                                               if(tempBoard[i-2][j+2]==' ' && tempBoard[i-3][j+3]!=myPawn && tempBoard[i-3][j+3]!=myTower ){
                                                   //shot
                                               }
                                           }

                                           if(i+3 < numRows && j-3 >=0){
                                               if(tempBoard[i+2][j-2]==' ' && tempBoard[i+3][j-3]!=myPawn && tempBoard[i+3][j-3]!=myTower){
                                                   //shot
                                               }
                                           }
                                            //movement
                                           if(i-2 >= 0 && j+2 < numCols){
                                               if(tempBoard[i-2][j+2]== ' '){
                                                //move
                                               }
                                           }

                                           if(i+2 < numRows && j-2 >=0){
                                               if(tempBoard[i+2][j-2]==' '){
                                                   //move
                                               }
                                           }
                                       }

                                    }

                                    if(tempBoard[i][j]==tempBoard[i+1][j]==tempBoard[i-1][j]){
                                        /*
                                        w
                                        w
                                        w
                                        */
                                       if(i+3 < numRows){
                                           if(tempBoard[i+2][j]==' ' && tempBoard[i+3][j]!=myPawn && tempBoard[i+3][j]!=myTower){
                                               //shot
                                           }
                                       }
                                       if(i-3 >= 0){
                                           if(tempBoard[i-2][j]==' ' && tempBoard[i-3][j]!=myPawn && tempBoard[i-3][j]!=myTower){
                                               //shot
                                           }
                                       }

                                       //movement
                                       if(i+2<numRows){
                                           if(tempBoard[i+2][j]==' '){
                                               //move
                                           }

                                       }
                                       if(i-2 >=0){
                                           if(tempBoard[i-2][j]==' '){
                                               //move
                                           }
                                           
                                       }
                                    }

                                }
                                if(j-1>=0 && j+1 <numCols){

                                    //  w w w 
                                    if(tempBoard[i][j-1]==tempBoard[i][j]==tempBoard[i][j+1]){

                                        if(j-3>=0 ){
                                            //shot
                                            if(tempBoard[i][j-2]==' ' && tempBoard[i][j-3]!=myPawn && tempBoard[i][j-3]!=myTower){

                                            }
                                        }

                                        if(j+3 < numCols){
                                            if(tempBoard[i][j+2]==' '&& tempBoard[i][j+3]!=myPawn && tempBoard[i][j+3]!=myTower){
                                                //shot
                                            }
                                        }

                                        //movement
                                        if(j+2<numCols){
                                            if(tempBoard[i][j+2]==' '){
                                                //move right
                                            }
                                        }
                                        if(j-2 >= 0){
                                            if(tempBoard[i][j-2]==' '){
                                                //moveLeft
                                            }
                                        }


                                    }
                                }


                            }

                        }

                    }

                }
                else{
                    
                    for(int i = 0;i<board.size;i++){
                        for(int j = 0;j<board.size;j++){
                            
                            if(tempBoard[i][j]==myPawn){
                                //regular soldier capture
                                for(int dy = -1;dy<=2;dy++){
                                    if(dy==0){
                                        for(int dx = -1;dx<=1;dx+=2){
                                            if(j+dx>=0 && j+dx <numCols){
                                                
                                                if(tempBoard[i+dy][j+dx]==oppPawn){
                                                    vector<vector<char>>nextBoard = tempBoard;
                                                    nextBoard[i][j]=' ';
                                                    nextBoard[i+dy][j+dx]=myPawn;
                                                }
                                            }
                                        }
                                    }
                                    else if(dy==1)continue;
                                    else if(dy==-1){

                                        if(i+dy < 0)continue;

                                        for(int dx=-1;dx<=1;dx++){
                                            if(j+dx>=0 && j+dx<numCols){
                                                if(tempBoard[i+dy][j+dx]==oppPawn || tempBoard[i+dy][j+dx]== ' '){
                                                    vector<vector<char>>nextBoard = tempBoard;
                                                    nextBoard[i][j]=' ';
                                                    nextBoard[i+dy][j+dx]=myPawn;
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
                                                for(int dx = -1 ;dx<=1;dx+=2){
                                                    if(j+dx >=0 && j+dx <numCols){
                                                        if(tempBoard[i+dy][j+dx]!=myPawn && tempBoard[i+dy][j+dx]!=myTower){
                                                            vector<vector<char>>nextBoard = tempBoard;
                                                            nextBoard[i][j]=' ';
                                                            nextBoard[i+dy][j+dx]=myPawn;
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
                                        b    O     b
                                         b   R    b
                                          b      b
                                        */

                                       if(tempBoard[i+1][j+1] == tempBoard[i][j]==tempBoard[i-1][j-1]){

                                           if(  i+3 <numRows && j+3 <numCols){
                                               if(tempBoard[i+2][j+2]==' ' && tempBoard[i+3][j+3]!=myPawn && tempBoard[i+3][j+3]!=myTower){
                                                   //shot
                                               }


                                           }

                                           if( i-3 >=0 && j-3 >=0){
                                               if(tempBoard[i-2][j-2] == ' ' && tempBoard[i-3][j-3]!=myPawn && tempBoard[i-3][j-3]!=myTower){
                                                   //shot
                                               }

                                           }

                                           //movement

                                           if(i-2>=0 && j-2 >=0){
                                               if(tempBoard[i-2][j-2]== ' '){
                                                   //move
                                               }
                                           }

                                            if(i+2>=0 && j+2 >=0){
                                               if(tempBoard[i+2][j+2]== ' '){
                                                   //move
                                               }
                                           }

                                       }

                                       if(tempBoard[i-1][j+1]==tempBoard[i][j] == tempBoard[i+1][j-1]){
                                           if(i-3 >=0 && j+3 < numCols){
                                               if(tempBoard[i-2][j+2]==' ' && tempBoard[i-3][j+3]!=myPawn && tempBoard[i-3][j+3]!=myTower ){
                                                   //shot
                                               }
                                           }

                                           if(i+3 < numRows && j-3 >=0){
                                               if(tempBoard[i+2][j-2]==' ' && tempBoard[i+3][j-3]!=myPawn && tempBoard[i+3][j-3]!=myTower){
                                                   //shot
                                               }
                                           }
                                            //movement
                                           if(i-2 >= 0 && j+2 < numCols){
                                               if(tempBoard[i-2][j+2]== ' '){
                                                //move
                                               }
                                           }

                                           if(i+2 < numRows && j-2 >=0){
                                               if(tempBoard[i+2][j-2]==' '){
                                                   //move
                                               }
                                           }
                                       }

                                    }

                                    if(tempBoard[i][j]==tempBoard[i+1][j]==tempBoard[i-1][j]){
                                        /*
                                        b
                                        b
                                        b
                                        */
                                       if(i+3 < numRows){
                                           if(tempBoard[i+2][j]==' ' && tempBoard[i+3][j]!=myPawn && tempBoard[i+3][j]!=myTower){
                                               //shot
                                           }
                                       }
                                       if(i-3 >= 0){
                                           if(tempBoard[i-2][j]==' ' && tempBoard[i-3][j]!=myPawn && tempBoard[i-3][j]!=myTower){
                                               //shot
                                           }
                                       }

                                       //movement
                                       if(i+2<numRows){
                                           if(tempBoard[i+2][j]==' '){
                                               //move
                                           }

                                       }
                                       if(i-2 >=0){
                                           if(tempBoard[i-2][j]==' '){
                                               //move
                                           }
                                           
                                       }
                                    }

                                }
                                if(j-1>=0 && j+1 <numCols){

                                    //  b b b 
                                    if(tempBoard[i][j-1]==tempBoard[i][j]==tempBoard[i][j+1]){

                                        if(j-3>=0 ){
                                            //shot
                                            if(tempBoard[i][j-2]==' ' && tempBoard[i][j-3]!=myPawn && tempBoard[i][j-3]!=myTower){

                                            }
                                        }

                                        if(j+3 < numCols){
                                            if(tempBoard[i][j+2]==' '&& tempBoard[i][j+3]!=myPawn && tempBoard[i][j+3]!=myTower){
                                                //shot
                                            }
                                        }

                                        //movement
                                        if(j+2<numCols){
                                            if(tempBoard[i][j+2]==' '){
                                                //move right
                                            }
                                        }
                                        if(j-2 >= 0){
                                            if(tempBoard[i][j-2]==' '){
                                                //moveLeft
                                            }
                                        }


                                    }
                                }


                            }

                        }

                    }

                }



            // string inputMessage;
            // State newState = State(!isWhite,numRows,numCols,tempBoard,inputMessage);
            // int child = newState.MinVal(alpha,beta,depth+1,maxDepth);
            // if(child > maxChild)maxChild = child;
            // if(alpha > child){
            //     alpha = child;
            // }
            // if(alpha >= beta){
            //     return child;
            // }
        }
        return maxChild;
    }

    int MinVal(int alpha,int beta, int depth,int maxDepth){
        if(depth>=maxDepth)utility(false);

        vector<vector<char>> tempBoard;
        tempBoard = board;

        int minChild = INT_MAX;
        while(true){

            string inputMessage;
            State newState = State(!isWhite,numRows,numCols,tempBoard,inputMessage);
            int child = newState.MaxVal(alpha,beta,depth+1,maxDepth);

            if(child < minChild){
                minChild = child;
            }
            if(beta < child){
                beta = child;
            }
            if(alpha >= beta){
             return child;
            }
        }
        return minChild;
    }
    

};

int main(){

    vector<vector<char>> board;
    int numRows = 8;
    int numCols = 8;
    bool isWhite = true;

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
        if(i==numRows-i){
            for(int j = 0;j<numCols;j+=2){
                board[i][j]='B';   
            }
        }

    }

    State current_state = State(isWhite,numRows,numCols,board);

    if(isWhite){
        string myMove= current_state.generateMove();
        cout<<myMove<<endl;
        if(current_state.isGameOver())return 0;
    }
    while(true){

    
        string opponentsMove;
        cin>>opponentsMove;
        current_state.updateBoard(opponentsMove);

        if(current_state.isGameOver())
            break;
        
        string myMove = current_state.generateMove();
        cout<<myMove<<endl;

        if(current_state.isGameOver())
            break;
        
    }
    

    return 0;

}