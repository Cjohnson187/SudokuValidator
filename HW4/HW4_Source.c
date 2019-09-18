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

unsigned long int row_tid[9] = { 0 };
unsigned long int column_tid[9] = { 0 };
unsigned long int block_tid[9] = { 0 };


int sudoku_2d[9][9] = { 0 };

typedef struct {
	int top_row;
	int bottom_row;
	int left_column;
	int right_column;
} parameters;

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

	for (i = top; i < top + 3; i++) {
		for (j = left; j < left + 3; j++) {
			int temp = sudoku_2d[i][j];
			if (temp < 1 || temp > 9 || temp_array[temp - 1] == 1) {
				// block invalid
				blocks_bool[top + left / 3] = 0;
				printf("\nTID-[0X%lx] TRow: %d, BRow: %d, LCol: %d, RCol: %d Subgrid# %d is invalid!    \n", pthread_self(), top, bottom, left, right, (top + left / 3)+1);
				block_tid[top + left / 3] = pthread_self();
				pthread_exit(0);
			}
			else {
				temp_array[temp - 1] = 1;
			}
		}
	}
	// block valid
	printf("\nTID-[0X%lx] TRow: %d, BRow: %d, LCol: %d, RCol: %d subgrid# %d is valid!    \n", pthread_self(), top, bottom, left, right, (top + left / 3) + 1);
	block_tid[top + left / 3] = pthread_self();
	blocks_bool[top + left / 3] = TRUE;
	pthread_exit(0);

}


// method to check rows
void* check_row(void* para) {

	parameters* data = (parameters*) para;

	int current_row = data->top_row;
	int end_row = data->bottom_row;
	int current_column = data->left_column;
	int end_column = data->right_column;
	if (current_column != 0 || current_row > 8) {
		printf("\ninvalid row format\n");
		pthread_exit(0);
	}

	int temp_array[9] = { 0 };
	int i = 0;
	for (i = 0; i < 9; i++) {

		int temp = sudoku_2d[current_row][i];

		if (temp > 9 || temp < 1 || temp_array[temp-1] == 1) { 
			printf("\nTID-[0X%lx] TRow: %d, BRow: %d, LCol: %d, RCol: %d Row# %d is invalid!\n", pthread_self(), current_row, end_row, current_column, end_column, (current_row) + 1);
			row_tid[current_row] = pthread_self();
			rows_bool[current_row] = FALSE;
			pthread_exit(0);
		}
		else temp_array[temp - 1] = 1;
	}
	printf("\nTID-[0X%lx] TRow: %d, BRow: %d, LCol: %d, RCol: %d Row# %d is Valid!\n", pthread_self(), current_row, end_row, current_column, end_column, (current_row)+1);
	row_tid[current_row] = pthread_self();
	rows_bool[current_row] = TRUE;
	pthread_exit(0);
}

// method to check columns
void* check_column(void* para) {

	parameters* data = (parameters*)para;

	int current_row = data->top_row;
	int end_row = data->bottom_row;
	int current_column = data->left_column;
	int end_column = data->right_column;
	if (current_row != 0 || current_column > 8) {
		printf("\ninvalid column format\n");
		pthread_exit(0);
	}

	int temp_array[9] = { 0 };
	int i = 0;
	for (i = 0; i < 9; i++) {

		int temp = sudoku_2d[i][current_column];

		if (temp > 9 || temp < 1 || temp_array[temp - 1] == 1) {
			printf("\nTID-[0X%lx] TRow: %d, BRow: %d, LCol: %d, RCol: %d Column# %d is Invalid!    \n", pthread_self(), current_row, end_row, current_column, end_column, (current_column)+1);
			column_tid[current_column] = pthread_self();
			column_bool[current_column] = FALSE;
			pthread_exit(0);
		}
		else temp_array[temp - 1] = 1;
	}
	printf("\nTID-[0X%lx] TRow: %d, BRow: %d, LCol: %d, RCol: %d Column# %d is Valid!    \n", pthread_self(), current_row, end_row, current_column, end_column, (current_column)+1);
	column_tid[current_column] = pthread_self();
	column_bool[current_column] = TRUE;
	pthread_exit(0);
}

// load sudoku
void* load_sudoku() {

	int ch;
	FILE* fptr;
	fptr = fopen("input3.txt", "r");
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


// print out sudoku
void* print_sudoku() {
	for (int i = 0; i < 9; i++) {
		printf("\n ");

		for (int j = 0; j < 9; j++) {
			printf("%d	", sudoku_2d[i][j]);
		}
		printf("\n\n");
	}
	return 0;
}


// run main
int main(void) {

	pthread_t thread[27];

	load_sudoku();

	print_sudoku();

	int thread_index = 0;

	// columns
	parameters* column_data[9] = {
		(parameters*)malloc(sizeof(parameters)),
		(parameters*)malloc(sizeof(parameters)),
		(parameters*)malloc(sizeof(parameters)),

		(parameters*)malloc(sizeof(parameters)),
		(parameters*)malloc(sizeof(parameters)),
		(parameters*)malloc(sizeof(parameters)),

		(parameters*)malloc(sizeof(parameters)),
		(parameters*)malloc(sizeof(parameters)),
		(parameters*)malloc(sizeof(parameters)),
	};

	for (int i = 0; i < 9; i++) {
		column_data[i]->top_row = 0;
		column_data[i]->bottom_row = 8;
		column_data[i]->left_column = i;
		column_data[i]->right_column = i;
	}

	for (int i = 0; i < 9; i++) pthread_create(&thread[thread_index++], NULL, check_column, column_data[i]);

	for (int k = 0; k < 9; k++) pthread_join(thread[k], NULL);

	// rows
	parameters* row_data[9] = { 
		(parameters*)malloc(sizeof(parameters)),
		(parameters*)malloc(sizeof(parameters)),
		(parameters*)malloc(sizeof(parameters)),

		(parameters*)malloc(sizeof(parameters)),
		(parameters*)malloc(sizeof(parameters)),
		(parameters*)malloc(sizeof(parameters)),

		(parameters*)malloc(sizeof(parameters)),
		(parameters*)malloc(sizeof(parameters)),
		(parameters*)malloc(sizeof(parameters)),
	};
	for (int i = 0; i < 9; i++) {
		row_data[i]->top_row = i;
		row_data[i]->bottom_row = i;
		row_data[i]->left_column = 0;
		row_data[i]->right_column = 8;
	}
	for (int i = 0; i < 9; i++) pthread_create(&thread[thread_index++], NULL, check_row, row_data[i]);

	for (int k = 0; k < 9; k++) pthread_join(thread[k], NULL);

	// blocks
	parameters* block_data[9] = { 
		(parameters*)malloc(sizeof(parameters)),
		(parameters*)malloc(sizeof(parameters)),
		(parameters*)malloc(sizeof(parameters)),

		(parameters*)malloc(sizeof(parameters)),
		(parameters*)malloc(sizeof(parameters)),
		(parameters*)malloc(sizeof(parameters)),

		(parameters*)malloc(sizeof(parameters)),
		(parameters*)malloc(sizeof(parameters)),
		(parameters*)malloc(sizeof(parameters)),

	};

	for (int i = 0; i < 9; i +=3) {
		block_data[i]->top_row = i;
		block_data[i]->bottom_row = i+2;
		block_data[i]->left_column = 0;
		block_data[i]->right_column = 2;

		block_data[i+1]->top_row = i;
		block_data[i+1]->bottom_row = i + 2;
		block_data[i+1]->left_column = 3;
		block_data[i+1]->right_column = 5;

		block_data[i+2]->top_row = i;
		block_data[i+2]->bottom_row = i + 2;
		block_data[i+2]->left_column = 6;
		block_data[i+2]->right_column = 8;
	}

	for (int i = 0; i < 9; i++) {
		pthread_create(&thread[thread_index++], NULL, check_block, block_data[i]);
	}

	for (int k = 0; k < 9; k++) pthread_join(thread[k], NULL);

	bool all_checks = TRUE;

	// set status of main check for validity
	for (int k = 0; k < 9; k++) {
		if (column_bool[k] == FALSE) {
			printf("\nColumn:0X%lx is Invalid\n", column_tid[k]);
				all_checks = FALSE;
		}
		else if (column_bool[k] == TRUE) printf("\nColumn:0X%lx is Valid\n", column_tid[k]);
	}

	for (int k = 0; k < 9; k++) {
		if (rows_bool[k] == FALSE) {
			printf("\nRow:0X%lx is Invalid\n", row_tid[k]);
				all_checks = FALSE;
		}
		else if (column_bool[k] == TRUE) printf("\nRow:0X%lx is Valid\n", row_tid[k]);
	}

	for (int k = 0; k < 9; k++) {
		if (blocks_bool[k] == FALSE) {
			printf("\nSubgrid:0X%lx is Invalid\n", block_tid[k]);
			all_checks = FALSE;
		}
		else if (blocks_bool[k] == TRUE) printf("\nSubgrid:0X%lx is Valid\n", block_tid[k]);
	}

	if (all_checks == FALSE) {
		printf("\n\nSudoku Puzzle is Invalid\n\n");
		return 0;
	}


	printf("\n\nSudoku Puzzle is Valid\n\n");
	return 0;
}


