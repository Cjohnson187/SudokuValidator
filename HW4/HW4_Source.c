//#include <Mspthrd.h>
//#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define _CRT_SECURE_NO_WARNINGS
#define num_child_threads 27
//typedef int bool;

//#define TRUE 1
//#define FALSE 0

char* file_name = "input.txt";
FILE* filepoint;

int sum[num_child_threads];
int check[27] = { 0 };

bool rows_bool[9] = { TRUE };
bool columns_bool[9] = { TRUE };
bool blocks_bool[9] = { TRUE };

int rows_2D[9][9] = { 0 };
int columns_2D[9][9] = { 0 };
int blocks_2D[9][9] = { 0 };


//void *runner(void* param);

typedef struct {
	int row;
	int column;
} parameters;



int sample_sudoku[9][9] = {
	{4,3,5,2,6,9,7,8,1},
	{6,8,2,4,7,1,4,9,3},
	{1,9,7,8,3,4,5,6,2},
	{8,2,6,1,9,5,3,4,7},
	{3,7,4,6,8,2,9,1,5},
	{9,5,1,7,4,3,6,2,8},
	{5,1,9,3,2,6,8,7,4},
	{2,4,8,9,5,7,1,3,6},
	{7,6,3,4,1,8,2,5,9}
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


}*/

void* check_column(void* param) {

	parameters* data = (parameters*)data;

	int current_row = data->row;
	int current_column = data->column;


}

void* load_sudoku() {

	int ch;
	FILE* fptr;
	fptr = fopen("input.txt", "r");
	if (!fptr) {
		printf("file not found");
		return 0;
	}

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

void* fill_block() {


}

void* check_dup(int list[9][9]) {
	int standard_array[9] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
	int i = 0;
	int j = 0;
	int x = 0;
	int temp = 0;
	int occurences = 0;
	

	/*
	for (i = 0; i < 9; i++) {
		temp = list[x][i];


		for (j = 0; j < 9; j++) {
			if (temp == list[x][j]) occurences++;
		}
		if (occurences > 1) {
			bool_list[x] = FALSE;
			x++;
			i = 0;
		}
		if (i == 8) {
			x++;
			i = 0;
			bool_list[x] = TRUE;
		}
		if (x == 8) {
			break;
		}
	}*/




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


