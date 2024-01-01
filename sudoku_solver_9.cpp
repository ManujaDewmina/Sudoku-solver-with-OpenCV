#include "sudoku_solver.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <ctime>
#include <string>

using namespace std;

#define MAX_K 1000

#define SIZE 9

string inputFileN_9;

const int SIZE_SQUARED = SIZE*SIZE;
const int SIZE_SQRT = sqrt((double)SIZE);
const int ROW_NB = SIZE*SIZE*SIZE;
const int COL_NB = 4 * SIZE*SIZE;

struct Node Head_9;
struct Node* HeadNode_9 = &Head_9;
struct Node* solution_9[MAX_K];
struct Node* orig_values_9[MAX_K];
bool matrix_9[ROW_NB][COL_NB] = { { 0 } };
bool isSolved_9 = false;
void MapSolutionToGrid_9(int Sudoku[][SIZE]);
void PrintGrid_9(int Sudoku[][SIZE]);

clock_t timer_9, timer2_9;

//DLX functions
void coverColumn_9(Node* col) {
	col->left->right = col->right;
	col->right->left = col->left;
	for (Node* node = col->down; node != col; node = node->down) {
		for (Node* temp = node->right; temp != node; temp = temp->right) {
			temp->down->up = temp->up;
			temp->up->down = temp->down;
			temp->head->size--;
		}
	}
}

void uncoverColumn_9(Node* col) {
	for (Node* node = col->up; node != col; node = node->up) {
		for (Node* temp = node->left; temp != node; temp = temp->left) {
			temp->head->size++;
			temp->down->up = temp;
			temp->up->down = temp;
		}
	}
	col->left->right = col;
	col->right->left = col;
}

void search_9(int k) {

	if (HeadNode_9->right == HeadNode_9) {
		timer2_9 = clock() - timer_9;
		int Grid[SIZE][SIZE] = { {0} };
		MapSolutionToGrid_9(Grid);
		string outputFileN_9 = inputFileN_9.substr(0, inputFileN_9.find(".txt")) + "_output.txt";
		ofstream outputFile(outputFileN_9);
		if (!outputFile.is_open()) {
			cerr << "Error opening file: " << outputFileN_9 << endl;
			return;
		}
		for (int i = 0; i < SIZE; ++i) {
			for (int j = 0; j < SIZE; ++j) {
				outputFile << Grid[i][j] << " ";
			}
			outputFile << endl;
		}
		outputFile.close();
		cout << "File created and written successfully: " << inputFileN_9.substr(0, inputFileN_9.find(".txt")) + "_output.txt" << endl << endl;
		cout << "Solved Puzzle:" << endl;
		PrintGrid_9(Grid);
		cout << "Time Elapsed: " << (float)timer2_9 / CLOCKS_PER_SEC << " seconds.\n\n";
		timer_9 = clock();
		isSolved_9 = true;
		return;
	}

	//Choose Column Object Deterministically: Choose the column with the smallest Size
	Node* Col = HeadNode_9->right;
	for (Node* temp = Col->right; temp != HeadNode_9; temp = temp->right)
		if (temp->size < Col->size)
			Col = temp;

	coverColumn_9(Col);

	for (Node* temp = Col->down; temp != Col; temp = temp->down) {
		solution_9[k] = temp;
		for (Node* node = temp->right; node != temp; node = node->right) {
			coverColumn_9(node->head);
		}

		search_9(k + 1);

		temp = solution_9[k];
		solution_9[k] = NULL;
		Col = temp->head;
		for (Node* node = temp->left; node != temp; node = node->left) {
			uncoverColumn_9(node->head);
		}
	}

	uncoverColumn_9(Col);
}

void MapSolutionToGrid_9(int Sudoku[][SIZE]) {
	for (int i = 0; solution_9[i] != NULL; i++) {
			Sudoku[solution_9[i]->rowID[1]-1][solution_9[i]->rowID[2]-1] = solution_9[i]->rowID[0];
	}
	for (int i = 0; orig_values_9[i] != NULL; i++) {
		Sudoku[orig_values_9[i]->rowID[1] - 1][orig_values_9[i]->rowID[2] - 1] = orig_values_9[i]->rowID[0];
	}
}

//BUILD THE INITIAL MATRIX CONTAINING ALL POSSIBILITIES
void BuildSparseMatrix_9(bool matrix_9[ROW_NB][COL_NB]) {

	//Constraint 1: There can only be one value in any given cell
	int j = 0, counter = 0;
	for (int i = 0; i < ROW_NB; i++) { //iterate over all rows
		matrix_9[i][j] = 1;
		counter++;
		if (counter >= SIZE) {
			j++;
			counter = 0;
		}
	}

	//Constraint 2: There can only be one instance of a number in any given row
	int x = 0;
	counter = 1;
	for (j = SIZE_SQUARED; j < 2 * SIZE_SQUARED; j++) {
		for (int i = x; i < counter*SIZE_SQUARED; i += SIZE)
			matrix_9[i][j] = 1;

		if ((j + 1) % SIZE == 0) {
			x = counter*SIZE_SQUARED;
			counter++;
		}
		else
			x++;
	}

	//Constraint 3: There can only be one instance of a number in any given column
	j = 2 * SIZE_SQUARED;
	for (int i = 0; i < ROW_NB; i++)
	{
		matrix_9[i][j] = 1;
		j++;
		if (j >= 3 * SIZE_SQUARED)
			j = 2 * SIZE_SQUARED;
	}

	//Constraint 4: There can only be one instance of a number in any given region
	x = 0;
	for (j = 3 * SIZE_SQUARED; j < COL_NB; j++) {

		for (int l = 0; l < SIZE_SQRT; l++) {
			for (int k = 0; k<SIZE_SQRT; k++)
				matrix_9[x + l*SIZE + k*SIZE_SQUARED][j] = 1;
		}

		int temp = j + 1 - 3 * SIZE_SQUARED;

		if (temp % (int)(SIZE_SQRT * SIZE) == 0)
			x += (SIZE_SQRT - 1)*SIZE_SQUARED + (SIZE_SQRT - 1)*SIZE + 1;
		else if (temp % SIZE == 0)
			x += SIZE*(SIZE_SQRT - 1) + 1;
		else
			x++;
	}
}

//BUILD A TOROIDAL DOUBLY LINKED LIST OUT OF THE SPARSE MATRIX
void BuildLinkedList_9(bool matrix_9[ROW_NB][COL_NB]) {

	Node* header = new Node;
	header->left = header;
	header->right = header;
	header->down = header;
	header->up = header;
	header->size = -1;
	header->head = header;
	Node* temp = header;

	//Create all Column Nodes
	for (int i = 0; i < COL_NB; i++) {
		Node* newNode = new Node;
		newNode->size = 0;
		newNode->up = newNode;
		newNode->down = newNode;
		newNode->head = newNode;
		newNode->right = header;
		newNode->left = temp;
		temp->right = newNode;
		temp = newNode;
	}

	int ID[3] = { 0,1,1 };
	//Add a Node for each 1 present in the sparse matrix_9 and update Column Nodes accordingly
	for (int i = 0; i < ROW_NB; i++) {
		Node* top = header->right;
		Node* prev = NULL;

		if (i != 0 && i%SIZE_SQUARED == 0) {
			ID[0] -= SIZE - 1;
			ID[1]++;
			ID[2] -= SIZE - 1;
		}
		else if (i!= 0 && i%SIZE == 0) {
			ID[0] -= SIZE - 1;
			ID[2]++;
		}
		else {
			ID[0]++;
		}

		for (int j = 0; j < COL_NB; j++, top = top->right) {
			if (matrix_9[i][j]) {
				Node* newNode = new Node;
				newNode->rowID[0] = ID[0];
				newNode->rowID[1] = ID[1];
				newNode->rowID[2] = ID[2];
				if (prev == NULL) {
					prev = newNode;
					prev->right = newNode;
				}
				newNode->left = prev;
				newNode->right = prev->right;
				newNode->right->left = newNode;
				prev->right = newNode;
				newNode->head = top;
				newNode->down = top;
				newNode->up = top->up;
				top->up->down = newNode;
				top->size++;
				top->up = newNode;
				if (top->down == top)
					top->down = newNode;
				prev = newNode;
			}
		}
	}
	HeadNode_9 = header;
}

//COVERS VALUES THAT ARE ALREADY PRESENT IN THE GRID
void TransformListToCurrentGrid_9(int Puzzle[][SIZE]) {
	int index = 0;
	for(int i = 0 ; i<SIZE; i++ )
		for(int j = 0 ; j<SIZE; j++)
			if (Puzzle[i][j] > 0) {
				Node* Col = NULL;
				Node* temp = NULL;
				for (Col = HeadNode_9->right; Col != HeadNode_9; Col = Col->right) {
					for (temp = Col->down; temp != Col; temp = temp->down)
						if (temp->rowID[0] == Puzzle[i][j] && (temp->rowID[1] - 1) == i && (temp->rowID[2] - 1) == j)
							goto ExitLoops;
				}
ExitLoops:		coverColumn_9(Col);
				orig_values_9[index] = temp;
				index++;
				for (Node* node = temp->right; node != temp; node = node->right) {
					coverColumn_9(node->head);
				}
			}
}

void SolveSudoku_9(int Sudoku[][SIZE], string inputFileName) {
	timer_9 = clock();
	//Functions to turn a Sudoku grid into an Exact Cover problem
	BuildSparseMatrix_9(matrix_9);
	BuildLinkedList_9(matrix_9);
	TransformListToCurrentGrid_9(Sudoku);
	inputFileN_9 = inputFileName;
	search_9(0);
	if (!isSolved_9)
		cout << "No Solution!" << endl;
	isSolved_9 = false;
}

void PrintGrid_9(int Sudoku[][SIZE]){
	string extBorder = "+", intBorder = "|";
	int counter = 1;
	int additional = 0;
	if (SIZE > 9)
		additional = SIZE;
	for (int i = 0; i < ((SIZE +SIZE_SQRT - 1) * 2 +additional+ 1); i++) {
		extBorder += '-';
		if (i > 0 && i % ((SIZE_SQRT*2+SIZE_SQRT*(SIZE>9)+1)*counter + counter-1) == 0) {
			intBorder += '+';
			counter++;
		}
		else
			intBorder += '-';
	}
	extBorder += '+';
	intBorder += "|";
	cout << extBorder << endl;
	for (int i = 0; i<SIZE; i++){
		cout << "| ";
		for (int j = 0; j<SIZE; j++){
			if (Sudoku[i][j] == 0)
				cout << ". ";
			else
				cout << Sudoku[i][j] << " ";
			if (additional > 0 && Sudoku[i][j]<10)
				cout << " ";
			if ((j+1)%SIZE_SQRT == 0)
				cout << "| ";
		}
		cout << endl;
		if ((i + 1) % SIZE_SQRT == 0 && (i+1)<SIZE)
			cout << intBorder << endl;
	}
	cout << extBorder << endl << endl;
}