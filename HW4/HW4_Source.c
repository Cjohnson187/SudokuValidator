/*
Chris Johnson
CS3600-001
HW #3
program to validate given sudoku solution by rows, columns 
and blocks in text file seperated by tabs and lines.
*/



#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define _CRT_SECURE_NO_WARNINGS
#define num_child_threads 27

typedef int bool;

#define TRUE 1
#define FALSE 0

bool rows_bool[9] = { TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE };
bool column_bool[9] = { TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE };
bool blocks_bool[9] = { TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE };

int sudoku_2d[9][9] = { 0 };

typedef struct {
	int top_row;
	int bottom_row;
	int left_column;
	int right_column;
} parameters;


/*
int sudoku_puzzle[9][9] = {
	{4,3,5, 2,6,9, 7,8,1},
	{6,8,2, 4,7,1, 4,9,3},
	{1,9,7, 8,3,4, 5,6,2},

	{8,2,6, 1,9,5, 3,4,7},
	{3,7,4, 6,8,2, 9,1,5},
	{9,5,1, 7,4,3, 6,2,8},

	{5,1,9, 3,2,6, 8,7,4},
	{2,4,8, 9,5,7, 1,3,6},
	{7,6,3, 4,1,8, 2,5,9}
};
  */


// method to check blocks
void* check_block(void* para) {

	parameters* data = (parameters*) para;

	int top = data->top_row;
	int left = data->left_column;
	int bottom = data->bottom_row;
	int right = data->right_column;

	if (top > 6 || top % 3 != 0 || left > 6 || left % 3 != 0) {
		printf("\ninvalid block format\n");
		pthread_exit(0);
	}

	int temp_array[9] = { 0 };
	int i = 0; 
	int j = 0;

	for (i = top; i < left + 3; i++) {
		for (j = left; j < left + 3; j++) {
			int temp = sudoku_2d[i][j];
			if (temp < 1 || temp > 9 || temp_array[temp - 1] == 1) {
				blocks_bool[top + left / 3] = 0;

				pthread_exit(0);
			}
			else {
				temp_array[temp - 1] = 1;
			}
		}
	}

	blocks_bool[top + left / 3] = 1;

}


// method to check rows
void* check_row(void* para) {

	parameters* data = (parameters*) para;

	int current_row = data->top_row;
	int current_column = data->left_column;
	if (current_column != 0 || current_row > 8) {
		printf("\ninvalid row format\n");
		pthread_exit(0);
	}

	int temp_array[9] = { 0 };
	int i = 0;
	for (i = 0; i < 9; i++) {

		int temp = sudoku_2d[current_row][i];

		if (temp > 9 || temp < 1 || temp_array[temp-1] == 1) { 
			printf(" current row %d invalid \n", current_row+1);
			rows_bool[current_row] = FALSE;
			pthread_exit(0);
		}
		else temp_array[temp - 1] = 1;
	}
	rows_bool[current_row] = 1;
	pthread_exit(0);
}

// method to check columns
void* check_column(void* para) {

	parameters* data = (parameters*)para;

	int current_row = data->top_row;
	int current_column = data->left_column;
	if (current_row != 0 || current_column > 8) {
		printf("\ninvalid column format c - %d,  r - %d  \n", current_column, current_row);
		pthread_exit(0);
	}

	int temp_array[9] = { 0 };
	int i = 0;
	for (i = 0; i < 9; i++) {

		int temp = sudoku_2d[i][current_column];

		if (temp > 9 || temp < 1 || temp_array[temp - 1] == 1) {
			//printf("\ntemp  %d ", temp);
			printf(" current column %d invalid \n", current_column + 1);
			column_bool[current_column] = FALSE;
			pthread_exit(0);
		}
		else temp_array[temp - 1] = 1;
	}
	column_bool[current_column] = 1;
	pthread_exit(0);
}

// load sudoku
void* load_sudoku() {

	int ch;
	FILE* fptr;
	fptr = fopen("input2.txt", "r");
	if (!fptr) return 1;

	int i = 0;
	int j = 0;
	while ((ch = fgetc(fptr)) != EOF)
	{
		if (j > 8) {
			i++;
			j = 0;
		}

		if (ch == '\n' || ch == '\r' || ch == '\t') {
			continue;
		}
		sudoku_2d[i][j] = ch-'0';
		j++;
	}
	fclose(fptr);
	return 0;
}



// fill vert and block
/*
void* fill_v() {
	int i = 0;
	int j = 0;
	for (i = 0; i < 9; i++) {
		for (j = 0; j < 9; j++) {
			columns_2D[i][j] = rows_2D[j][i];
		}
	}
}

void* fill_b() {
	int i = 0;
	int j = 0;

}
*/



// print out sudoku
void* print_sudoku() {
	for (int i = 0; i < 9; i++) {
		printf("\n row %d		", i);

		for (int j = 0; j < 9; j++) {
			printf("%d	", sudoku_2d[i][j]);
		}
		printf("\n");

	}
	return 0;
}


// run main
int main(void) {

	pthread_t thread[27];

	load_sudoku();

	print_sudoku();

	int x = 0;
	int z = 0;

	int index = 0;
	int i = 0;
	int j = 0;
	for (i = 0; i < 9; i++) {
		for (j = 0; j < 9; j++) {
			if (j == 0) {
				parameters *row_data = (parameters*)malloc(sizeof(parameters));
				row_data->top_row = i;
				row_data->left_column = j;
				pthread_create(&thread[index++], NULL, check_row, row_data);
			}
			
			if (i == 0) {
				parameters *column_data = (parameters*)malloc(sizeof(parameters));
				column_data->top_row = i;
				column_data->left_column = j;
				pthread_create(&thread[index++], NULL, check_column, column_data);
			}

			if (i % 3 == 0 && j % 3 == 0) {
				parameters* block_data = (parameters*)malloc(sizeof(parameters));
				block_data->top_row = i;
				block_data->left_column = j;
				pthread_create(&thread[index++], NULL, check_block, block_data);
			}
		}
	}

	for (int k = 0; k < num_child_threads; k++) pthread_join(thread[k], NULL);

	bool all_checks = TRUE;

	// set status of main check for validity
	for (int k = 0; k < 9; k++) {
		if (rows_bool[k] == FALSE) {
			printf("\nrow #- %d is invalid\n", k+1);
			all_checks = FALSE;
			//return 0;
		}
		if (column_bool[k] == FALSE) {
			printf("\ncolumn #- %d is invalid\n", k+1);
			all_checks = FALSE;
			//return 0;
		}
		if (blocks_bool[k] == FALSE) {
			printf("\nblock #- %d is invalid\n", k+1);
			all_checks = FALSE;
			//return 0;
		}
	}

	if (all_checks == FALSE) {
		printf("\n\nSudoku puzzle is NOT valid\n\n");
		return 0;
	}


	printf("\n\nSudoku puzzle is valid\n\n");
	return 0;
}


