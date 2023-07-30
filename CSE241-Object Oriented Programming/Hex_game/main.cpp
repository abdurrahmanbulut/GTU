/*
	Abdurrahman BULUT
	abdurrahmanbulut08@gmail.com

	Hex game with Functional Programming
	Main function
	
*/

#include <iostream>
#include <string>

#include "helper.h"


using namespace std;

int main() {

   char board[DIM][DIM], user1_letter;
   string user1, user2, fileName; //input like E4 F5

   int who_play, size, userOrComp = 0;
   auto flag = 1;  //it exits userx from loop.
   decltype (flag) game_flag = 1; //it makes a loop for new game.


   while(game_flag){

	//it selects the game type
	who_play = game_option("Select game type:", "1) user1 & user2", "2) user & computer");

	if(who_play == 1 || who_play == 2){
	
	  //if size is -1 , that means a wrong selection
	  size = size_of_board();
	  initial_board(board, size, 0); 
	
	}

	if(who_play == 1){ //user1 vs user2
	  
	  flag = 1;

	  while (flag)
	  {	
		print_puzzle(board, size);

		cout << "User 1 : ";
		cin >> user1;
		//if consumer enter save or load for second times or more
		while(user1 == "SAVE" || user1 == "LOAD" || user1 == "save" || user1 == "load"){

		 if(user1 == "SAVE" || user1 == "LOAD" || user1 == "save" || user1 == "load"){
			cin >> fileName;

			if(user1 == "SAVE" || user1 == "save"){
				writeToFile(board, fileName, size, 0);// o -> userVSuser 1->userVScomputer
				cout << "SAVED!\n";
				cout << "User 1 : ";
				cin >> user1;
			}
			else if(user1 == "LOAD" || user1 == "load"){
				readFromFile(board, fileName, size, userOrComp);
				print_puzzle(board, size);
				cout << "LOADED!\n";
				cout << "User 1 : ";
				cin >> user1;
			}

		 }
		}

		 while(check_move(board, size, user1)){

			cerr << "User 1! : ";
		    cin >> user1;
		 }
		 move(board, size, user1, 1);
		

		if(solve_puzzle_x(board, size)){

			print_puzzle(board, size);
			cout << "User1 won!" << endl;
			flag = 0;
			{  
			   int play_again = 1;

			   while(play_again == 1){

				cout << "Do you want to play again?\n" << "1. Yes \n2. No\n";
				cin >> play_again;
				
				if(play_again == 2){
			 	    game_flag = 0;
					play_again = 0;
					cout << "Good Bye!";
				}
				else if(play_again == 1)
					play_again = 0;
					
				else{
					cerr << "try again!\n";
					play_again = 1;
				}
				cin.clear(); //it empties inside of cin. 
      			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			   }
			}
		}
		else
			print_puzzle(board, size);

   		 if(flag){
		 
		  cout << "User 2 : ";
		  cin >> user2;
		  
		  while(user2 == "SAVE" || user2 == "LOAD" || user2 == "save" || user2 == "load"){
		   
		   if(user2 == "SAVE" || user2 == "LOAD" || user2 == "save" || user2 == "load"){
			cin >> fileName;

			if(user2 == "SAVE" || user2 == "save"){
				writeToFile(board, fileName, size, 0);// o -> userVSuser 1->userVScomputer
				cout << "SAVED!\n";
				cout << "User 2 : ";
				cin >> user2;
			}
			else if(user2 == "LOAD" || user2 == "load"){
				readFromFile(board, fileName, size, userOrComp);
				print_puzzle(board, size);
				cout << "LOADED!\n";
				cout << "User 2 : ";
				cin >> user2;
			}

		   }
		  }
		 while(check_move(board, size, user2)){
			
			cerr << "User 2! : ";
		    cin >> user2;
		 }

		 move(board, size, user2, 2);
		
		 if(solve_puzzle_o(board, size)){
			print_puzzle(board, size);
			
			cout << "User2 won!" << endl;
			flag = 0;
			{  
			   auto play_again = 1;

			   while(play_again){

				cout << "Do you want to play again?\n" << "1. Yes \n2. No\n";
				cin >> play_again;
				
				if(play_again == 2){
			 	    game_flag = 0;
					play_again = 0;
					cout << "Good Bye!";
				}
				else if(play_again == 1)
					play_again = 0;
				else
					cerr << "try again!\n";

			   }
			}
		 }
		}
	  }
	}
	else if(who_play == 2) { //computer vs user
	
	  flag = 1;
	  char move_var;
	  int comp_x;
	  int comp_y;

	  print_puzzle(board, size);

	  while (flag)
	  {	
		
		cout << "User 1 : ";
		cin >> user1;
		
		while(user1 == "SAVE" || user1 == "LOAD" || user1 == "save" || user1 == "load"){

		 if(user1 == "SAVE" || user1 == "LOAD" || user1 == "save" || user1 == "load"){
			cin >> fileName;

			if(user1 == "SAVE" || user1 == "save"){
				writeToFile(board, fileName, size, 0);// o -> userVSuser 1->userVScomputer
				cout << "SAVED!\n";
				cout << "User 1 : ";
				cin >> user1;
			}
			else if(user1 == "LOAD" || user1 == "load"){
				readFromFile(board, fileName, size, userOrComp);
				print_puzzle(board, size);
				cout << "LOADED!\n";
				cout << "User 1 : ";
				cin >> user1;
			}

		 }
		}
		while(check_move(board, size, user1)){

			cerr << "User 1! : ";
		    cin >> user1;
		}

		move(board, size, user1, 1);

		if(solve_puzzle_x(board, size)){

			print_puzzle(board, size);
			cout << "User1 won!" << endl;
			flag = 0;
			{  
			   auto play_again = 1;

			   while(play_again){

				cout << "Do you want to play again?\n" << "1. Yes \n2. No\n";
				cin >> play_again;
				
				if(play_again == 2){
			 	    game_flag = 0;
					play_again = 0;
					cout << "Good Bye!";
				}
				else if(play_again == 1)
					play_again = 0;
					
				else
					cerr << "try again!\n";

			   }
			}
		}
		else if(!(solve_puzzle_x(board, size)))
			print_puzzle(board, size);

		if(flag){
			
			getRandom(size, comp_x);
			getRandom(size, comp_y);

		 while(!(checkComputerMove(board, comp_x, comp_y))){
			
			getRandom(size, comp_x);
			getRandom(size, comp_y);
		 }
		 move_var = comp_y + 65; //A
		 
		 computerMove(board, size, comp_x, comp_y, user1);


		 if(solve_puzzle_o(board, size)){
			print_puzzle(board, size);
			
			cout << "Computer won!" << endl;
			flag = 0;
			{  
			   auto play_again = 1;

			   while(play_again){

				cout << "Do you want to play again?\n" << "1. Yes \n2. No\n";
				cin >> play_again;
				
				if(play_again == 2){
			 	    game_flag = 0;
					play_again = 0;
					cout << "Good Bye!";
				}
				else if(play_again == 1)
					play_again = 0;
				else
					cerr << "try again!\n";

			   }
			}
		 }
		 else if(!(solve_puzzle_o(board, size)))
			print_puzzle(board, size);
		}
	  }
	}
	else //wrong stuation
		cerr << "try again!\n";

   }

	return 0;
}
