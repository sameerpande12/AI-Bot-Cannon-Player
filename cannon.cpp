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
    string messageToBestChild;
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
        messageToBestChild = "";
        numRows = nRows;
        numCols = nCols;

        if(white){

            myPawn = 'w';
            myTower='W';
            oppPawn='b';
            oppTower='B';
        }
    }

    string generateMove(){

        int cost = MaxVal(INT_MIN,INT_MAX,0,3);

        updateBoard(messageToBestChild);
        return messageToBestChild;
    }
    void updateBoard(string move){
        cout<<"Move to make "<<move<<endl;
        if(move.length()==0)return;//the case of stalemate
        vector<string> tokens;
        int i = 0;
        int j = 1;
        while(j<move.length()){
            
            if(move[j]==' ' && move[j-1]!=' '){
                tokens.push_back(move.substr(i,j));
            }
            else if(move[j]!=' ' && move[j-1]==' '){
                i = j;
            }
            j++;
            if(j==move.length()){
                if(i<j){
                    tokens.push_back(move.substr(i,j));
                }
            }
            
        }
        for(int i = 0;i<tokens.size();i++){
            cout<<tokens[i]<<endl;
        }
        int bx = stoi(tokens[1]);
        int by = stoi(tokens[2]);

        int ex = stoi(tokens[4]);
        int ey = stoi(tokens[5]);

        char type = tokens[3][0];
        if(type=='M'){
            board[ex][ey] = myPawn;
            board[bx][by] = ' ';
        }
        else{
            board[ex][ey]= ' ';
        }

    }


    vector<string> getNeighbours(int i , int j){
                vector<string> neighbours;
                if(isWhite){
                    
                    vector<vector<char>>tempBoard = board;   
                    if(tempBoard[i][j]==myPawn){
                        //regular soldier capture
                        for(int dy = 1;dy>=-2;dy--){
                            if(dy==0){
                                for(int dx = -1;dx<=1;dx+=2){
                                    if(j+dx>=0 && j+dx <numCols){
                                        
                                        if(tempBoard[i+dy][j+dx]==oppPawn){
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
                                        if(tempBoard[i+dy][j+dx]==oppPawn || tempBoard[i+dy][j+dx]== ' '){
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
                                        for(int dx = -1 ;dx<=1;dx+=2){
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
                                w    O     w
                                    w   R    w
                                    w      w
                                */

                                if(tempBoard[i+1][j+1] == tempBoard[i][j]==tempBoard[i-1][j-1]){

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

                                    //movement

                                    if(i-2>=0 && j-2 >=0){
                                        if(tempBoard[i-2][j-2]== ' '){
                                            //move
                                            neighbours.push_back("S "+to_string(i+1)+" "+to_string(j+1)+" M "+to_string(i-2)+" "+to_string(j-2));
                                        }
                                    }

                                    if(i+2>=0 && j+2 >=0){
                                        if(tempBoard[i+2][j+2]== ' '){
                                            //move
                                            neighbours.push_back("S "+to_string(i-1)+" "+to_string(j-1)+" M "+to_string(i+2)+" "+to_string(j+2));
                                        }
                                    }

                                }

                                if(tempBoard[i-1][j+1]==tempBoard[i][j] == tempBoard[i+1][j-1]){
                                    if(i-3 >=0 && j+3 < numCols){
                                        if(tempBoard[i-2][j+2]==' ' && tempBoard[i-3][j+3]!=myPawn && tempBoard[i-3][j+3]!=myTower ){
                                            //shot
                                            neighbours.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i-3)+" "+to_string(j-3));
                                        }
                                    }

                                    if(i+3 < numRows && j-3 >=0){
                                        if(tempBoard[i+2][j-2]==' ' && tempBoard[i+3][j-3]!=myPawn && tempBoard[i+3][j-3]!=myTower){
                                            //shot
                                            neighbours.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i+3)+" "+to_string(j-3));
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

                            if(tempBoard[i][j]==tempBoard[i+1][j]==tempBoard[i-1][j]){
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
                            if(tempBoard[i][j-1]==tempBoard[i][j]==tempBoard[i][j+1]){

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
                    vector<vector<char>>tempBoard = board;
                        
                        if(tempBoard[i][j]==myPawn){
                            //regular soldier capture
                            for(int dy = -1;dy<=2;dy++){
                                if(dy==0){
                                    for(int dx = -1;dx<=1;dx+=2){
                                        if(j+dx>=0 && j+dx <numCols){
                                            
                                            if(tempBoard[i+dy][j+dx]==oppPawn){
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
                                            if(tempBoard[i+dy][j+dx]==oppPawn || tempBoard[i+dy][j+dx]== ' '){
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
                                            for(int dx = -1 ;dx<=1;dx+=2){
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
                                    b     O     b
                                      b   R    b
                                        b     b
                                    */

                                    if(tempBoard[i+1][j+1] == tempBoard[i][j]==tempBoard[i-1][j-1]){

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

                                        //movement

                                        if(i-2>=0 && j-2 >=0){
                                            if(tempBoard[i-2][j-2]== ' '){
                                                //move
                                                neighbours.push_back("S "+to_string(i+1)+" "+to_string(j+1)+" M "+to_string(i-2)+" "+to_string(j-2));
                                            }
                                        }

                                        if(i+2>=0 && j+2 >=0){
                                            if(tempBoard[i+2][j+2]== ' '){
                                                //move
                                                neighbours.push_back("S "+to_string(i-1)+" "+to_string(j-1)+" M "+to_string(i+2)+" "+to_string(j+2));
                                            }
                                        }

                                    }

                                    if(tempBoard[i-1][j+1]==tempBoard[i][j] == tempBoard[i+1][j-1]){
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

                                if(tempBoard[i][j]==tempBoard[i+1][j]==tempBoard[i-1][j]){
                                    /*
                                    b
                                    b
                                    b
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

                                //  b b b 
                                if(tempBoard[i][j-1]==tempBoard[i][j]==tempBoard[i][j+1]){

                                    if(j-3>=0 ){
                                        //shot
                                        if(tempBoard[i][j-2]==' ' && tempBoard[i][j-3]!=myPawn && tempBoard[i][j-3]!=myTower){
                                            neighbours.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i)+" "+to_string(j-3));
                                        }
                                    }

                                    if(j+3 < numCols){
                                        if(tempBoard[i][j+2]==' '&& tempBoard[i][j+3]!=myPawn && tempBoard[i][j+3]!=myTower){
                                            neighbours.push_back("S "+to_string(i)+" "+to_string(j)+" B "+to_string(i+3)+" "+to_string(j+3));
                                            //shot
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
    int utility(bool getMyUtility){
        //if getMyUtility then check how favourable is the board for me
        // else check how favourable the board is for opponent
        return 0;

    }
    State getState(string move){
        //"S 1 2 M 4 6"
        vector<string> tokens;
        int i = 0;
        int j = 1;
        while(j<move.length()){
            
            if(move[j]==' ' && move[j-1]!=' '){
                tokens.push_back(move.substr(i,j));
            }
            else if(move[j]!=' ' && move[j-1]==' '){
                i = j;
            }
            j++;
            if(j==move.length()){
                if(i<j){
                    tokens.push_back(move.substr(i,j));
                }
            }
            
        }

        int bx = stoi(tokens[1]);
        int by = stoi(tokens[2]);

        int ex = stoi(tokens[4]);
        int ey = stoi(tokens[5]);

        vector<vector<char>>newBoard;
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
        
        int neighbour_count = 0;
        messageToBestChild = "";//make sure that you don't effect future moves when you have stalemate
        for(int i = 0;i<numRows;i++){
            for(int j=0;j<numCols;j++){
                if(board[i][j]==myPawn){
                    vector<string> neighbours = getNeighbours(i,j);
                    neighbour_count += neighbours.size();
                    for(int iter = 0;iter< neighbours.size();iter++){
                        State newstate = getState(neighbours[iter]);
                        int child = newstate.MinVal(alpha,beta,depth+1,maxDepth);
                        if(alpha>child){
                            alpha = child;
                        }
                        if(child>maxChild){maxChild = child;
                            messageToBestChild = neighbours[iter];
                        }

                        if(alpha>=beta){
                            messageToBestChild = neighbours[iter];
                            return child;
                        }
                    }
                }
                    
                    
            }
        }

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

        vector<vector<char>> tempBoard;
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
                        if(beta<child){
                            beta = child;
                        }
                        if(child<minChild){messageToBestChild = neighbours[iter];minChild = child;}

                        if(alpha>=beta){
                            messageToBestChild = neighbours[iter];
                            return child;
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
        return minChild;
        }
    }
    

};
void printBoard(vector<vector<char>> board,int numRows,int numCols){
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
        if(i==numRows-1){
            for(int j = 1;j<numCols;j+=2){
                board[i][j]='B';   
            }
        }

    }
    
    State current_state = State(isWhite,numRows,numCols,board);
    printBoard(current_state.board,numRows,numCols);
    
    if(isWhite){
        string myMove= current_state.generateMove();
        cout<<myMove<<endl;
        printBoard(current_state.board,numRows,numCols);
        
    }
    while(true){

    
        string opponentsMove;
        cin>>opponentsMove;
        // printBoard(current_state.board,numRows,numCols);
        // cout<<"About to update board"<<endl;
        
        // current_state.updateBoard(opponentsMove);
        // cout<<"Updated board"<<endl;
        

        // if(current_state.isGameOver())
        //     break;
        
        string myMove = current_state.generateMove();
        cout<<myMove<<endl;
        printBoard(current_state.board,numRows,numCols);
        // if(current_state.isGameOver())
            // break;
        
    }
    

    return 0;

}