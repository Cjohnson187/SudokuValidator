/*
Chris Johnson
CS3600-001
HW #3
program to validate given sudoku solution in text 
file seperated by tabs and lines.
*/



//#include <Mspthrd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define _CRT_SECURE_NO_WARNINGS
#define num_child_threads 27

typedef int bool;

#define TRUE 1
#define FALSE 0

char* file_name = "input.txt";
FILE* filepoint;

int sum[num_child_threads];
//int check[27] = { 0 };

bool rows_bool[9] = { TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE };
bool column_bool[9] = { TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE };
bool blocks_bool[9] = { TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE };

int rows_2D[9][9] = { 0 };
int columns_2D[9][9] = { 0 };
int blocks_2D[9][9] = { 0 };


//void *runner(void* param);

typedef struct {
	int top_row;
	int bottom_row;
	int left_column;
	int right_column;
} parameters;



int sudoku_puzzle[9][9] = {
	{4,3,5,2,6,9,7,8,1},
	{6,8,2,4,7,1,4,9,3},
	{1,9,7,8,3,4,5,6,2},
	{8,2,6,1,9,5,3,4,7},
	{3,7,4,6,8,2,9,1,5},
	{9,5,1,7,4,3,6,2,8},
	{5,1,9,3,2,6,8,7,4},
	{2,4,8,9,5,7,1,3,6},
	{7,6,3,4,1,8,2,5,9}
};/*
void* check_block(void* param) {

	parameters* data = (parameters*) data;

	int current_row = data->row;
	int current_column = data->column;


}

void* check_row(void* param) {

	parameters* data = (parameters*)data;

	int current_row = data->row;
	int current_column = data->column;


}*/

void* check_row(void* para) {

	parameters* data = (parameters*) para;

	int current_row = data->top_row;
	int current_column = data->left_column;
	if (current_column != 0 || current_row > 8) {
		//printf("\ncurrent column%d  /  current row%d\n", current_column, current_row);
		printf("\ninvalid row format\n");
		pthread_exit(0);
	}

	int temp_array[9] = { 0 };
	int i = 0;
	for (i = 0; i < 9; i++) {

		int temp = rows_2D[current_row][i];
		//printf("\n current row  %d , and val = %d and i = %d ", current_row+1 , rows_2D[current_row][i], i);

		//printf("\n in (for) temp  %d ", temp);

		if (temp > 9 || temp < 1 || temp_array[temp-1] == 1) { 
			printf("\ntemp  %d ", temp);
			printf(" current row %d invalid \n", current_row+1);
			rows_bool[current_row] = FALSE;
			pthread_exit(0);
		}
		else temp_array[temp - 1] = 1;
	}
	rows_bool[current_row] = 1;
	pthread_exit(0);
}

void* check_column(void* para) {

	parameters* data = (parameters*)para;

	int current_row = data->top_row;
	int current_column = data->left_column;
	if (current_row != 0 || current_column > 8) {
		//printf("\ncurrent column%d  /  current row%d\n", current_column, current_row);
		printf("\ninvalid column format c - %d,  r - %d  \n", current_column, current_row);
		pthread_exit(0);
	}

	int temp_array[9] = { 0 };
	int i = 0;
	for (i = 0; i < 9; i++) {

		int temp = rows_2D[i][current_column];
		//printf("\n current row  %d , and val = %d and i = %d ", current_row+1 , rows_2D[current_row][i], i);

		//printf("\n in (for) temp  %d ", temp);

		if (temp > 9 || temp < 1 || temp_array[temp - 1] == 1) {
			printf("\ntemp  %d ", temp);
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
		rows_2D[i][j] = ch-'0';
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
			printf("%d	", rows_2D[i][j]);
		}
		printf("\n");

	}
	return 0;
}


// run main
int main(void) {
	printf("\nmain\n");

	pthread_t thread[27];

	load_sudoku();

	///for (int h = 0; h < 9; h++) printf("\nrow h = %d  %d\n", rows_bool[h], h);

	print_sudoku();

	int x = 0;
	int z = 0;
	/*
	for (z = 0; z < 9; z++) {
		for (x = 0; x < 9; x++) {
			printf("\nprinted rows[%d][%d]%c", z, x, rows_2D[z][x]);
		}
		printf("\nx reached 9");
		printf("\n\n");

	}*/
	printf("\ndoneish1\n");

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

		}
	}
	printf("\ndoneish2\n");

	for (int k = 0; k < num_child_threads-9; k++) pthread_join(thread[k], NULL);

	printf("\ndoneish3\n");
	bool all_checks = TRUE;

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


