#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#define SIZE 40

void read_vector(double x[], int n) {
	for(int i = 0; i < n; ++i) {
		scanf("%lf", x++);
	}
}

void print_vector(double x[], int n) {
	for(int i = 0; i < n; ++i) {
		printf("%.4f ", x[i]);
	}
	printf("\n");
}

void read_mat(double A[][SIZE], int m, int n) {
	for(int i = 0; i < m; ++i) {
		for(int j = 0; j < n; ++j) {
			scanf("%lf", &A[i][j]);
		}
	}
}

void print_mat(double A[][SIZE], int m, int n) {
	for(int i = 0; i < m; ++i) {
		for(int j = 0; j < n; ++j) {
			printf("%.4f ", A[i][j]);
		}
		printf("\n");
	}
}

// 1. Calculate matrix product, AB = A X B
// A[m][p], B[p][n], AB[m][n]
void mat_product(double A[][SIZE], double B[][SIZE], double AB[][SIZE], int m, int p, int n) {
    for (int i = 0; i < m; i++){
        for (int j = 0; j < n; j++){
            for (int k = 0; k < p; k++){
                AB[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}


// 2. Matrix triangulation and determinant calculation - simplified version
// (no rows' swaps). If A[i][i] == 0, function returns NAN.
// Function may change A matrix elements.
double gauss_simplified(double A[][SIZE], int n) {
    for (int k = 0; k < n; k++){
        if (A[k][k] == 0){
            return NAN;
        }
        for (int i = k + 1; i < n; i++){
            double r = A[i][k]/A[k][k];
            A[i][k] = 0;
            for (int j = k + 1; j < n; j++){
                A[i][j] -= r*A[k][j];
            }
        }
    }
    double total = 1;
    for (int i = 0; i < n; i++){
        total *= A[i][i];
    }
    return total;
}

// 3. Matrix triangulation, determinant calculation, and Ax = b solving - extended version
// (Swap the rows so that the row with the largest, leftmost nonzero entry is on top. While
// swapping the rows use index vector - do not copy entire rows.)
// If max A[i][i] < eps, function returns 0.
// If det != 0 && b != NULL && x != NULL then vector x should contain solution of Ax = b.
int find_index_of_max_element(double A[][SIZE], const int indices[], int i, int n){
    double max = A[indices[i]][i];
    int max_index = indices[i];
    for (int j = i + 1; j < n; j++)
    {
        if (fabs(A[indices[j]][i]) > fabs(max))
        {
            max = A[indices[j]][i];
            max_index = j;
        }
    }
    return max_index;
}
void swap_int_elements(int array[], int i, int j){
    int temp;
    temp = array[i];
    array[i] = array[j];
    array[j] = temp;
}
void swap_double_elements(double array[], int i, int j){
    double temp;
    temp = array[i];
    array[i] = array[j];
    array[j] = temp;
}
void maximize_diagonal_element(double A[][SIZE], int indices[], double x[], int i, int n, int * sign){
    int max_index = find_index_of_max_element(A, indices, i, n);
    if (max_index != indices[i]) {
        swap_int_elements(indices, i, max_index);
        swap_double_elements(x, i, max_index);
        * sign = * sign * (-1);
    }
}
void subtract_rows_below_diagonal_element(double A[][SIZE], const int indices[], double x[], int i, int n){
    for (int r = i + 1; r < n; r++)
    {
        double div = A[indices[r]][i] / A[indices[i]][i];
        for (int c = i; c < n; c++) {
            A[indices[r]][c] -= div * A[indices[i]][c];
        }
        x[r] -= div * x[i];
    }
}
bool is_any_diagonal_element_less_than_eps(double A[][SIZE], const int indices[], const double eps, int n){
    for (int i = 0; i < n; i++){
        if (fabs(A[indices[i]][i]) < eps){
            return true;
        }
    }
    return false;
}
double determinant_of_triangular_matrix(double A[][SIZE], const int indices[], int sign, int n){
    double det = 1;
    for (int i = 0; i < n; i++){
        det = det * A[indices[i]][i];
    }
    det = det * sign;
    return det;
}
void solve_system_of_equations(double A[][SIZE], const int indices[], double x[], int n){
    for (int i = n - 1; i >= 0; i--) {
        for (int j = n - 1; j >= i + 1; j--) {
            x[i] -= A[indices[i]][j] * x[j];
        }
        x[i] = x[i] / A[indices[i]][i];
    }
}
double gauss(double A[][SIZE], const double b[], double x[], const int n, const double eps)
{
    int sign = 1;
    int indices[n];
    for (int i = 0; i < n; i++) {
        x[i] = b[i];
        indices[i] = i;
    }
    for (int i = 0; i < n; i++) {
        maximize_diagonal_element(A, indices, x, i, n, &sign);
        subtract_rows_below_diagonal_element(A, indices, x, i, n);
    }
    if (is_any_diagonal_element_less_than_eps(A, indices, eps, n)) return 0;
    solve_system_of_equations(A, indices, x, n);
    double det = determinant_of_triangular_matrix(A, indices, sign, n);
    return det;
}

void transform_into_identity_matrix(double matrix[][SIZE], int n){
    for (int i = 0; i < n; i++){
        matrix[i][i] = 1;
    }
}
// 4. Returns the determinant; B contains the inverse of A (if det(A) != 0)
// If max A[i][i] < eps, function returns 0.
void maximize_diag_el(double A[][SIZE], int indices[], int i, int n){
    int max_index = find_index_of_max_element(A, indices, i, n);
    if (max_index != indices[i]) {
        swap_int_elements(indices, i, max_index);
    }
}
void fill_array_with_indexes(int array[], int n){
    for (int i = 0; i < n; i++) {
        array[i] = i;
    }
}
void reduce_diag_el_row(double A[][SIZE], double B[][SIZE], int const indices[], int i, int n){
    double reducer = 1/A[indices[i]][i];
    for (int c = 0; c < n; c++){
        A[indices[i]][c] *= reducer;
        B[indices[i]][c] *= reducer;
    }
}
void subtract_rows_below_diag_el(double A[][SIZE], double B[][SIZE], const int indices[], int i, int n){
    for (int r = i + 1; r < n; r++)
    {
        double div = A[indices[r]][i] / A[indices[i]][i];
        for (int c = 0; c < n; c++) {
            A[indices[r]][c] -= div * A[indices[i]][c];
            B[indices[r]][c] -= div * B[indices[i]][c];
        }
    }
}
void subtract_rows_above_diag_el(double A[][SIZE], double B[][SIZE], const int indices[], int i, int n){
    for (int r = i - 1; r >= 0; r--)
    {
       double div = A[indices[r]][i] / A[indices[i]][i];
       for (int c = 0; c < n; c++) {
            A[indices[r]][c] -= div * A[indices[i]][c];
            B[indices[r]][c] -= div * B[indices[i]][c];
       }
    }
}

void restore_default_order(double B[][SIZE], int indices[], int n){
    double A[SIZE][SIZE];
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            A[i][j] = B[indices[i]][j];
        }
    }
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            B[i][j] = A[i][j];
        }
    }
}
double calculate_determinant(double A[][SIZE], int n){
    int max_row;
    double det = 1, max, temp;
    for (int k = 0; k < n - 1; k++) {
        max = fabs(A[k][k]);
        max_row = k;
        for (int i = k + 1; i < n; i++) {
            if (fabs(A[i][k]) > max) {
                max = fabs(A[i][k]);
                max_row = i;
            }
        }
        if (max_row != k) {
            for (int j = k; j < n; j++) {
                temp = A[k][j];
                A[k][j] = A[max_row][j];
                A[max_row][j] = temp;
            }
            det *= -1;
        }

        for (int i = k + 1; i < n; i++) {
            double factor = A[i][k] / A[k][k];
            for (int j = k + 1; j < n; j++) {
                A[i][j] -= factor * A[k][j];
            }
            A[i][k] = 0;
        }
    }

    for (int i = 0; i < n; i++) {
        det *= A[i][i];
    }
    return det;
}
double matrix_inv(double A[][SIZE], double B[][SIZE], int n, double eps) {
    double copy_A[SIZE][SIZE];
    for (int i = 0; i<n; i++){
        for(int j = 0; j < n; j++){
            copy_A[i][j] = A[i][j];
         }
    }
    transform_into_identity_matrix(B, n);
    int indices[n];
    fill_array_with_indexes(indices, n);
    for (int i = 0; i < n; i++) {
        maximize_diag_el(A, indices,  i, n);
        if (A[indices[i]][i] == 0) return 0;
        reduce_diag_el_row(A, B, indices, i, n);
        subtract_rows_below_diag_el(A, B, indices, i, n);
        subtract_rows_above_diag_el(A, B, indices, i, n);
    }
    double det = calculate_determinant(copy_A, n);
    restore_default_order(B, indices, n);
    return det;
}


int main(void) {

	double A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];
	double b[SIZE], x[SIZE], det, eps = 1.e-13;

	int to_do;
	int m, n, p;

	scanf ("%d", &to_do);

	switch (to_do) {
		case 1:
			scanf("%d %d %d", &m, &p, &n);
			read_mat(A, m, p);
			read_mat(B, p, n);
			mat_product(A, B, C, m, p, n);
			print_mat(C, m, n);
			break;
		case 2:
			scanf("%d", &n);
			read_mat(A, n, n);
			printf("%.4f\n", gauss_simplified(A, n));
			break;
		case 3:
			scanf("%d", &n);
			read_mat(A,n, n);
			read_vector(b, n);
			det = gauss(A, b, x, n, eps);
			printf("%.4f\n", det);
			if (det) print_vector(x, n);
			break;
		case 4:
			scanf("%d", &n);
			read_mat(A, n, n);
			det = matrix_inv(A, B, n, eps);
			printf("%.4f\n", det);
            if (det) print_mat(B, n, n);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

