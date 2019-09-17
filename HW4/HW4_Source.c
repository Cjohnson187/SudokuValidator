//#include <Mspthrd.h>
//#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define _CRT_SECURE_NO_WARNINGS
#define num_child_threads 27
typedef int bool;

#define TRUE 1
#define FALSE 0
#define UNCHECKED 2

char* file_name = "input.txt";
FILE* filepoint;

int sum[num_child_threads];
int check[27] = { 0 };

bool rows_bool[9] = { 2 };
bool columns_bool[9] = { 2 };
bool blocks_bool[9] = { 2 };

int rows_2D[9][9] = { 0 };
int columns_2D[9][9] = { 0 };
int blocks_2D[9][9] = { 0 };


//void *runner(void* param);

typedef struct {
	int row;
	int column;
} parameters;



int sample_sudoku[9][9] = {
	{1,2,3,4,5,6,7,8,9},
	{9,8,7,6,5,4,3,2,1},
	{0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,9},
	{0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,7,0},
	{0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0},
	{0,0,0,6,0,0,0,0,0}
};
/*
void* check_block(void* param) {

	parameters* data = (parameters*) data;

	int current_row = data->row;
	int current_column = data->column;


}

void* check_row(void* param) {

	parameters* data = (parameters*)data;

	int current_row = data->row;
	int current_column = data->column;


}

void* check_column(void* param) {

	parameters* data = (parameters*)data;

	int current_row = data->row;
	int current_column = data->column;


}
*/
void* load_sudoku() {

	int ch;
	FILE* fptr;
	fptr = fopen("input.txt", "r");
	if (!fptr) return 1;

	int i = 0;
	int j = 0;
	while ((ch = fgetc(fptr)) != EOF)
	{
		if (j > 8) {
			i++;
			j = 0;
		}

		if (ch == '\n' || ch == '\r') {
			continue;
		}

		rows_2D[i][j] = ch;
		j++;
	}


	fclose(fptr);
	return 0;
}

void* fill_v() {
	int i = 0;
	int j = 0;
	for (i = 0; i < 9; i++) {
		for (j = 0; j < 9; j++) {
			columns_2D[i][j] = rows_2D[j][i];

		}
	}
}




int main(void) {

	load_sudoku();
	fill_v();
	//printf("%d", rows_2D);
	
	int x = 0;
	int z = 0;
	for (z = 0; z < 9; z++) {
		for (x = 0; x < 9; x++) {
			printf("\nprinted rows[%d][%d]%c", z, x, rows_2D[z][x]);
		}
		printf("\nx reached 9");
		printf("\n\n");

	}


	return 0;
}


