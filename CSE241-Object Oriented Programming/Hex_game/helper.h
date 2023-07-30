/*
	Abdurrahman BULUT
	abdurrahmanbulut@yahoo.com

	Hex game with Functional Programming
	helper.h is a header file that includes funcs prototypes.

*/
#ifndef HELPER_H_BULUT
#define HELPER_H_BULUT


#include <iostream>
#include <string>
#include <random>
#include <fstream>

using namespace std;

const int DIM = 12;

enum cellState {cross = 'x', circle = 'o', dot = '.', big_cross = 'X', big_circle = 'O'};


void print_puzzle(const char board[DIM][DIM], int size);

int game_option(const string& text, const string& text2, const string& text3);

int size_of_board();

void initial_board(char board[DIM][DIM], int size, bool initial);

int check_move(char board[][DIM], int size, string user);

void move(char board[][DIM], int size, string user, int userx);

bool is_safeO(char board[DIM][DIM], int x, int y);

bool solve_puzzle_x(char board[DIM][DIM], int size);

bool solve_puzzle_o(char board[DIM][DIM], int size);

bool win_check_x(char board[DIM][DIM], int x, int y, int size, char checker_back[][DIM], int diections[6][2]);

bool win_check_o(char board[DIM][DIM], int x, int y, int size, char checker_back[][DIM], int diections[6][2]);

void getRandom(int size, int& random);

bool checkComputerMove(char board[][DIM], int comp_x, int comp_y);

void computerMove(char board[][DIM], int size, int comp_x, int comp_y, string user);

void readFromFile(char board[DIM][DIM], string filename, int size, int userOrComp);

void writeToFile(char board[DIM][DIM], string filename, const int size, int userOrComp);

#endif