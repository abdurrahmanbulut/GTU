/*
	Abdurrahman BULUT
	abdurrahmanbulut@yahoo.com

	Hex game with Functional Programming
	helper.cpp includes implementation of functions which are in helper.h

*/
#include "helper.h"


//it prints any two dimensional array as hex game.
void print_puzzle(const char board[][DIM], int size){

	for(char l = 97; l < 97 + size; l++)
	      cout << ' ' << l;
	cout << "\n";
		  
	for(int i=0; i < size; i++){
	
	    cout << i+1;

	   for(int j=0; j < i; j++)
	      cout << ' ';
	   
	   for(int k=0; k < size; k++)
	      cout << board[i][k] << ' ';
	   
	cout << endl ;

	}
}
//this func. selects if it is user vs user or user vs computer.
int game_option(const string& text, const string& text2, const string& text3){
	
	int option=0;

	while(option != 1 && option != 2){
	
	  cout << text << endl << text2 << endl << text3 << endl;
	  cin >> option;

	  if(option != 1 && option != 2)
	  	cerr << "Try again\n";
	  
	  cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
	return option;	
}
//it define board size to play.
int size_of_board(){
	
	int selected=-1, size;

	while(selected == -1){

	  cout << "select the size of the board: \n" << "1) 6x6\n"<< "2) 7x7\n"
	  << "3) 8x8\n"<< "4) 9x9\n"<< "5) 10x10\n"<< "6) 11x11\n"<< "7) 12x12\n"; 

	  cin >> selected;

	  if(selected == 1)
		size = 6;
	  else if(selected == 2)
		size = 7;
	  else if(selected == 3)
	 	size = 8;
	  else if(selected == 4)
		size = 9;
	  else if(selected == 5)
		size = 10;
	  else if(selected == 6)
		size = 11;
	  else if(selected == 7)
		size = 12;
	  else
		selected = -1;
	  
	  if (size == -1)
	    cerr << "Try again!\n";
	  
	  
	  cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
	return size;
}
//fill it with '.' and ' ' for beginnig.
void initial_board(char board[][DIM], int size, bool initial = 0){

	cellState Dot = dot;

	if(initial == 0){

		for (int i = 0; i < size; i++){
			for (int j = 0; j < size; j++)
				board[i][j] = Dot;
		}
		for (int i = size; i < DIM; i++)
			for (int j = size; j < DIM; j++)
				board[i][j] = ' ';
	}
	else if(initial == 1){
		
		for(int i=0; i<size; i++)
        	for(int j=0; j<size; j++)
            	board[i][j] = 0;			
	
	}
}
//it fill the board with x or o by looking at the userx
int check_move(char board[][DIM], int size, string user){
	
	cellState Circle = circle;
	cellState Cross = cross;

	int flag=0;
	int x = user[0];
	int y = user[1];
	int z = user[2];
	
	if(!((user[0] >= 'a' && user[0] < ('a' + size)) || (user[0] >= 'A' && user[0] < ('A' + size))))
		flag = 1;

	if(!(user[1] >= '1' && user[1] < '1' + size) )
		flag = 1;
	
	if(user[2] != '\0'){
		if(!(user[2] >= '0' && user[2] <= '2'))
			flag = 1;
		if(!(user[1] >= '1' && user[1] <= '9'))
			flag = 1;
	}
	
	if(!(user[2] == '\0' || user[3] == '\0'))
		flag = 1;
	
	

	if(x <= 76 && x >= 65)
		x = x - 65;
	else if(x <= 108 && x >= 97)
		x = x - 97;

	y = y - 49;

	if(board[y][x] == Circle || board[y][x] == Cross)	
		flag = 1;

	return flag;
}
void move(char board[][DIM], int size, string user, int userx){
	
	cellState Circle = circle;
	cellState Cross = cross;

	int x = user[0]; //A
	int y = user[1]; // 1
	int z = user[2]; // 4

	if(user[1] == '1' && user[2] == '0')
		y = 9;
	else if(user[1] == '1' && user[2] == '2')
		y = 11;
	else
		y -= 49;
	 		

	if(x <= 76 && x >= 65)
		x = x - 65;
	else if(x <= 108 && x >= 97)
		x = x - 97;

	for(int i=0; i < size; i++){
	   
	   	for(int j=0; j < size; j++){

		  if(userx == 1)
		    if(y == i && x == j )
		  	  if(board[i][j] != Cross && board[i][j] != Circle)
			    board[i][j] = Cross;
		
		  if(userx == 2)
		    if(y == i && x == j)
			  if(board[i][j] != Circle && board[i][j] != Cross)
			    board[i][j] = Circle;
	   	}
	}
}
//it check the path and if any user won, it returns true and solution will be solution array.
bool solve_puzzle_x(char board[][DIM], int size) 
{	
	cellState Cross = cross;

	int i=0, directions[6][2] = {{1, 0}, {1, -1}, {0, -1}, {0, 1}, {-1, 0}, {-1, 1}};
	char checker_back[DIM][DIM]; //it checks the back path. if it went the place, the func makes it 1. 
	
	while(i < size){
	
		initial_board(checker_back, size, 1); 	
    
	  	if (board[i][0] == Cross && win_check_x(board, i, 0, size, checker_back, directions))
			return true;
	
	  	i++;
	}
	return false;
}
//this func. checks the end of the game. if it returns 1, then 'x' will be winner.
//it uses recursive func. to check the path for user which take x.
bool win_check_x(char board[][DIM], int x, int y, int size, char checker_back[][DIM], int directions[6][2]) 
{ 
	cellState Cross = cross;
	cellState Big_cross = big_cross;
	
	int checker_x, checker_y, i=0;

    if ( y == size - 1){

		board[x][y] = Big_cross;
        return true;
    }

	checker_back[x][y] = 1;

   while (i < size)
   {
	   
	    checker_x = x;
	    checker_y = y;

	    checker_x += directions[i][0];
	    checker_y += directions[i][1];
		
		if(checker_x < size && checker_x >= 0 && checker_y < size && checker_y >= 0 && checker_back[checker_x][checker_y] == 0 
		   && board[checker_x][checker_y] == Cross && win_check_x(board, checker_x, checker_y, size,  checker_back, directions))
		{
                board[x][y] = Big_cross;
                return true;
        }
		i++;
   }

    return false; 
}
//it check the path and if any user won, it returns true and solution will be solution array.
bool solve_puzzle_o(char board[][DIM], int size) 
{	

	cellState Circle = circle;

	int i=0, directions[6][2] = {{1, 0}, {1, -1}, {0, -1}, {0, 1}, {-1, 0}, {-1, 1}};
	char checker_back[DIM][DIM]; //it checks the back path. if it went the place, the func makes it 1. 
	
	while(i < size){
	
		initial_board(checker_back, size, 1); 	
    
	  	if (board[0][i] == Circle && win_check_o(board, 0, i, size, checker_back, directions))
			return true;
	
	  	i++;
	}
	return false;
}
//this func. checks the end of the game. if it returns 1, then 'x' will be winner.
//it uses recursive func. to check the path for user which take x.
bool win_check_o(char board[][DIM], int x, int y, int size, char checker_back[][DIM], int directions[6][2]) 
{ 
	cellState Big_circle = big_circle;
	cellState Circle = circle;
	
	int checker_x, checker_y, i=0;

    if ( x == size - 1){

		board[x][y] = Big_circle;
        return true;
    }

	checker_back[x][y] = 1;

   while (i < size)
   {
	   
	    checker_x = x;
	    checker_y = y;

	    checker_x += directions[i][0];
	    checker_y += directions[i][1];
		
		if(checker_x < size && checker_x >= 0 && checker_y < size && checker_y >= 0 && checker_back[checker_x][checker_y] == 0 
		   && board[checker_x][checker_y] == Circle && win_check_o(board, checker_x, checker_y, size,  checker_back, directions))
		{
                board[x][y] = Big_circle;
                return true;
        }
		i++;
   }
   
    return false; 
}
//it generate random number between 0 and size
void getRandom(int size, int& random){
	
	random_device rd;  //Will be used to obtain a seed for the random number engine
    mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    uniform_real_distribution<> dis(0.0, size - 0.01);
	
	random = static_cast<int>(dis(gen));

}
//it check the possible cell is empty or not. if it is not empty, return 0;
bool checkComputerMove(char board[][DIM], int comp_x = 0, int comp_y = 0){

	cellState Circle = circle;
	cellState Cross = cross;

	bool flag = 1;

	if (board[comp_x][comp_y] == Cross || board[comp_x][comp_y] == Circle )
		flag = 0;
	
	return flag;
}
/*
it checks right cell and right up cell of user move after the user. first option right cell
 because of barrier to user. if that cell is full , it checks right up cell. if that is also full, move randomly.
*/
void computerMove(char board[][DIM], int size, int comp_x, int comp_y, string user){
	
	cellState Circle = circle;
	cellState Cross = cross;

	auto flag = true;
	int x = user[0]; //A
	int y = user[1]; // 1
	int z = user[2]; // 4
	char letter = 0;

	if(user[1] == '1' && user[2] == '0')
		y = 9;
	else if(user[1] == '1' && user[2] == '2')
		y = 11;
	else
		y -= 49;
	 		

	if(x <= 76 && x >= 65)
		x = x - 65;
	else if(x <= 108 && x >= 97)
		x = x - 97;

	for(int i=0; i < size && flag == true; i++){
	   
	   	for(int j=0; j < size && flag == true; j++){
			
			if(y == i && x == j){
			  if(board[i][j + 1] != Circle && board[i][j + 1] != Cross && i < size //it goes to a point right side or up right side
			  									 && i >= 0 && j + 1 >= 0 && j + 1 < size)
			  { 
			    board[i][j + 1] = Circle;
				flag = false;
				letter = j + 66;
				cout << "Computer: " << letter << i + 1 << endl;
			  }
			  else if(board[i - 1][j + 1] != Cross && board[i - 1][j + 1] != Circle && i - 1 < size 
			  									 && i - 1 >= 0 && j + 1 >= 0 && j + 1 < size)
			  {
				board[i - 1][j + 1] = Circle;
				flag = false;
				letter = j + 66;
				cout << "Computer: " << letter << i << endl;
			  }
			}
		    else if(comp_x == i && comp_y == j){
			  if(board[i][j] != Circle && board[i][j] != Cross){
			    board[i][j] = Circle;
				flag = false;
				letter = j + 65;
				cout << "Computer: " << letter << i + 1 << endl;
			  }	
		   }
		}
	}
}
//it takes puzzle board, filename, board size and option those user vs user or user vs computer.
//and it save the board to the file is name filename string.
void writeToFile(char board[DIM][DIM], string filename, const int size, int userOrComp){

	ofstream myFile;

	myFile.open(filename);

	for(int i=0; i < size; i++){
		for(int j=0; j < size; j++){
			myFile << board[i][j];
		}
	}
	myFile << ' ';
	myFile << size << ' ' << userOrComp;//userOrComp  = 0 -> user vs user. userOrComp  = 1 -> user vs comp  

	myFile.close();
}
//it takes puzzle board, filename, board size and option those user vs user or user vs computer.
//and it load the file to the board.
void readFromFile(char board[DIM][DIM], string filename, int size, int userOrComp){

	ifstream myFile;

	char temp;

	myFile.open(filename);

	for(int i=0; i < size; i++){
		for(int j=0; j < size; j++){
			myFile >> board[i][j];
			//cout << board[i][j]; debug
		}
	}
	myFile >> temp;
	myFile >> size >> temp >> userOrComp;//userOrComp  = 0 -> user vs user. userOrComp  = 1 -> user vs comp  

	myFile.close();
}