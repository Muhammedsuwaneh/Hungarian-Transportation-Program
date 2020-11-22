/*
*
*@author - Muhammed Suwaneh
*@ESOGU Department of Computer Engineering
*
*
*/
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#define size 10 // can be changed

using namespace std;

/* steps involve */
void generateMatrix(int[][size]);
void printMatrix(int[][size]);
void printArrays(int[], int);
void row_reduction(int[][size]);
void column_reduction(int[][size]);
void cover_Zeros(int[][size], int[][size]);
int cover_Rows(int[][size]);
int count_Col_Zeros(int[][size], int);
void mark_Rows(int[][size], int);
void count_Covered_Rows(int[][size], int*);
int cover_Columns(int[][size]);
int count_Row_Zeros(int[][size], int);
void mark_Columns(int[][size], int);
void count_Covered_Columns(int[][size], int*);
void shift_Zeros(int[][size], int[][size], int*);
void get_index(int[][size], int, int, int*, int*);
int final_Assigment(int[][size], int[][size], int[size][size]);

int main() {

	srand(time(NULL));

	int Original_Matrix[size][size], Experiment_Matrix[size][size];
	int i, j;

	generateMatrix(Original_Matrix);

	cout << "Original Matrix" << endl;

	printMatrix(Original_Matrix);

	for (i = 0; i < size; i++) {

		for (j = 0; j < size; j++) {

			Experiment_Matrix[i][j] = Original_Matrix[i][j];
		}
	}

	row_reduction(Experiment_Matrix);

	column_reduction(Experiment_Matrix);

	int temp_Matrix[size][size]; // to hold reduced matrice 

	for (i = 0; i < size; i++) {

		for (j = 0; j < size; j++) {

			temp_Matrix[i][j] = Experiment_Matrix[i][j];
		}
	}

	cover_Zeros(Experiment_Matrix, temp_Matrix); // cover all zeros in rows and columns to obtain an optimized matrix

	cout << "Optimized Matrix" << endl;
	printMatrix(Experiment_Matrix);

	int value;

	value = final_Assigment(Original_Matrix, Experiment_Matrix, temp_Matrix);

	cout << "Optimal Value: " << value << endl;
	cout << endl;
	system("pause");

} // main ends

void generateMatrix(int Matrix[][size]) {

	int i, j, upper = 100, lower = 0;

	for (i = 0; i < size; i++) {

		for (j = 0; j < size; j++) {

			Matrix[i][j] = (rand() % (upper - lower + 1)) + lower;
		}
	}

} // generateMatrix ends here

void printMatrix(int Matrix[][size]) {


	int i, j;

	for (i = 0; i < size; i++) {

		for (j = 0; j < size; j++) {

			cout << Matrix[i][j] << "\t";
		}

		cout << endl;
	}

	cout << "-----------------------------" << endl;

} // printMatrix ends here

void printArrays(int Array[], int n) {

	int i;

	cout << endl << "-----------------------------" << endl;

	for (i = 0; i < n; i++) {

		cout << Array[i] << "  ";
	}

	cout << endl << "-----------------------------" << endl;

} // printArrays ends here 


void row_reduction(int Matrix[][size]) {

	int i, j, save_mins[size], min;

	for (i = 0; i < size; i++) {

		min = Matrix[i][0];

		for (j = 0; j < size; j++) {

			if (min > Matrix[i][j]) {

				min = Matrix[i][j];
			}
		}

		save_mins[i] = min;
	}



	for (i = 0; i < size; i++) {

		for (j = 0; j < size; j++) {

			Matrix[i][j] = Matrix[i][j] - save_mins[i];

		}
	}

} // row_reduction ends here

void column_reduction(int Matrix[][size]) {

	int i, j, save_mins[size], min;

	for (i = 0; i < size; i++) {

		min = Matrix[0][i];

		for (j = 0; j < size; j++) {

			if (min > Matrix[j][i]) {

				min = Matrix[j][i];
			}
		}

		save_mins[i] = min;
	}


	for (i = 0; i < size; i++) {

		for (j = 0; j < size; j++) {

			Matrix[j][i] = Matrix[j][i] - save_mins[i];

		}
	}

} // column_reduction ends here

void cover_Zeros(int Matrix[][size], int temp_Matrix[][size]) {

	int row_size, col_size, lines_covered, uncovered[30] = { -1 };
	 int min, n;

	do {

		row_size = 0, col_size = 0, min = 0, n = 0, lines_covered = 0;

		row_size = cover_Rows(temp_Matrix);

		if (row_size < size) {

			col_size = cover_Columns(temp_Matrix);
		}

		lines_covered = col_size + row_size;

		if (lines_covered < size) {

			for (int i = 0; i < size; i++) {

				for (int j = 0; j < size; j++) {

					if (temp_Matrix[i][j] != -1) {

						uncovered[n] = temp_Matrix[i][j];
						n++;
					}
				}
			}

			/* find smallest uncovered */

			min = uncovered[0];

			for (int i = 0; i < n; i++) {

				if (min > uncovered[i]) {

					min = uncovered[i];
				}
			}

			shift_Zeros(Matrix, temp_Matrix, &min);


			/* copy new obtained matrice to temp */

			for (int i = 0; i < size; i++) {

				for (int j = 0; j < size; j++) {

					temp_Matrix[i][j] = Matrix[i][j];
				}
			}

		}

		if (lines_covered == size) {

			break;
		}

	} while (1);

} // cover_zeros ends here 

int count_Col_Zeros(int Matrix[][size], int col) {

	int count = 0;

	for (int i = 0; i < size; i++) {

		if (Matrix[i][col] == 0) {

			count++;
		}
	}

	return count;

} // count_Col_Zeros ends here

void mark_Rows(int Matrix[][size], int row) {


	for (int i = 0; i < size; i++) {

		Matrix[row][i] = -1;
	}


} // mark_Rows ends here

int cover_Rows(int Matrix[][size]) {

	int col_zeros, count, row_size;

	for (int i = 0; i < size; i++) {

		col_zeros = 0;

		for (int j = 0; j < size; j++) {

			if (Matrix[j][i] == 0) {

				col_zeros = count_Col_Zeros(Matrix, i);

				if (col_zeros == 1) {

					mark_Rows(Matrix, j);
				}
			}
		}
	}
   
	count_Covered_Rows(Matrix, &count);

	row_size = count;

	return row_size;

} // cover_Rows ends here 

void count_Covered_Rows(int Matrix[][size], int* covered_rows) {

	int count, covered = 0;

	for (int i = 0; i < size; i++) {

		count = 0;

		for (int j = 0; j < size; j++) {

			if (Matrix[i][j] == -1) {

				count++;
			}
		}

		/* means entire row is covered */
		if (count == size) {

			covered++;
		}
	}

	*covered_rows = covered;

} //  count_Covered_Rows ends here 


int cover_Columns(int Matrix[][size]) {

	int count, row_zeros, col_size;

	for (int i = 0; i < size; i++) {

		row_zeros = 0;

		for (int j = 0; j < size; j++) {

			if (Matrix[i][j] == 0) {

				//row_zeros = count_Row_Zeros(Matrix, i); // the commented code triggers errors 

				//if (row_zeros == 1) {

					mark_Columns(Matrix, j);
				//}
			}
		}
	}

	count_Covered_Columns(Matrix, &count);

	col_size = count;

	return col_size;

} // cover_Columns ends here 


int count_Row_Zeros(int Matrix[][size], int row) {

	int count = 0;

	for (int i = 0; i < size; i++) {

		if (Matrix[row][i] == 0) {

			count++;
		}
	}

	return count;

} // count_Row_Zeros ends here 

void mark_Columns(int Matrix[][size], int col) {


	for (int i = 0; i < size; i++) {

		Matrix[i][col] = -1;
	}

} // mark_Columns ends here 

void count_Covered_Columns(int Matrix[][size], int* covered_columns) {

	int count, covered = 0;

	for (int i = 0; i < size; i++) {

		count = 0;

		for (int j = 0; j < size; j++) {

			if (Matrix[j][i] == -1) {

				count++;
			}
		}

		/* means entire row is covered */
		if (count == size) {

			covered++;
		}
	}

	*covered_columns = covered;

} // count_Covered_Columns ends here

void shift_Zeros(int Matrix[][size], int temp_Matrix[][size], int* min) {

	int row, col;

	/* subtract minimum uncovered from all uncovered values */
	for (int i = 0; i < size; i++) {

		for (int j = 0; j < size; j++) {

			if (temp_Matrix[i][j] != -1) {

				Matrix[i][j] = Matrix[i][j] - *min;
			}
		}
	}

	/* add minimum to elements covered twice */

	for (int i = 0; i < size; i++) {

		row = 0, col = 0;

		for (int j = 0; j < size; j++) {

			if (temp_Matrix[i][j] == -1) {

				get_index(temp_Matrix, i, j, &row, &col);

				if (col == size && row == size) {

					Matrix[i][j] = Matrix[i][j] + *min;
				}
			}
		}
	}

} // shift_Zeros ends here

void get_index(int Matrix[][size], int row, int col, int* val, int* val_2) {

	int count = 0, count_1 = 0;

	for (int i = 0; i < size; i++) {

		if (Matrix[i][col] == -1) {

			count++;
		}
	}

	for (int i = 0; i < size; i++) {

		if (Matrix[row][i] == -1) {

			count_1++;
		}
	}

	*val = count;
	*val_2 = count_1;

} // get index ends here 

int final_Assigment(int Original_Matrix[][size], int Experiment_Matrix[][size], int temp_Matrix[size][size]) {

	int i, j, value = 0, rows[size], cols[size], count, count_1, n = 0, row_size;
	int optimal_Values[size];

	for (i = 0; i < size; i++) {

		count = 0;

		for (j = 0; j < size; j++) {

			if (Experiment_Matrix[j][i] == 0) {

				count = count_Col_Zeros(Experiment_Matrix, i);

				if (count == 1) {

					mark_Rows(Experiment_Matrix, j);
					optimal_Values[n] = Original_Matrix[j][i];
					n++;
				}
			}
		}
	}

	count_Covered_Rows(Experiment_Matrix, &row_size);

	if (row_size < size) {

		count_1 = 0;

		for (i = 0; i < size; i++) {

			for (j = 0; j < size; j++) {

				if (Experiment_Matrix[i][j] == 0) {

					count_1 = count_Row_Zeros(Experiment_Matrix, i);

						mark_Columns(Experiment_Matrix, j);
						optimal_Values[n] = Original_Matrix[i][j];
						n++;
				}
			}
		}
	}

	for (i = 0; i < n; i++) {

		value += optimal_Values[i];
	}

	return value;

} // final_Assigment ends here 

