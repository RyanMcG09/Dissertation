#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <math.h>
#include <string>
#include <fstream>
#include <chrono>

//define variables
using namespace std;
const int n = 1000;
double matrixA[n][n];
double matrixB[n][n];
double matrixC[n][n];
double matrixD[n][n];
void populate_matrix(int n);
double parallel_for_multiplication(int n, int threads);
void saveMatrix(const char* title);
void print_matrix(double m[n][n]);
void testingMatrixValue();

int main() {
	//set up local variables
	const int maxCores = 32;
	const int startPoint = 32;
	const int numIterations = 1;
	double xy[maxCores - startPoint + 1][2];
	populate_matrix(n);
	//print_matrix(matrixA);
	//print_matrix(matrixB);
	int count = 1;

	//Get the average time value over one core for speedup
	double time = 0;
	for (int i = 1; i < numIterations + 1; i++) {
		time += parallel_for_multiplication(n, 1);
	}
	time = time / numIterations;
	printf("Average time taken for a %d x %d matrix over %d cores and over %d iterations is %f s\n", n, n, 1, numIterations, time);
	xy[0][0] = time;
	xy[0][1] = 1;

	//Get the average time value over different number of cores for speedup
	for (int i = startPoint; i < maxCores + 1; i*=2) {
		double time = 0;
		for (int j = 1; j <= numIterations; j++) {
			time += parallel_for_multiplication(n, i);
		}
		time = time / numIterations;
		printf("Average time taken for a %d x %d matrix over %d cores and over %d iterations is %f s\n", n, n, i, numIterations, time);
		xy[count][0] = time;
		xy[count][1] = i;
		count++;
	}
	//save times to .txt file
	string filename = "resultsSize" + std::to_string(n) + ".txt";
	ofstream myfile(filename);
	if (myfile.is_open())
	{
		for (int i = 0; i < count; i++) {
			myfile << xy[i][0] << "," << xy[i][1] << "\n";
		}
		myfile.close();
	}
	const char* title = "OpenMPMatrix.txt";
	saveMatrix(title);
	testingMatrixValue();
	const char* title1 = "OpenMPMatrixTest.txt";
	saveMatrix(title1);
	return 0;
}

void populate_matrix(int n) {

	//different seed for random values
	srand(time(NULL));

	//populate matrices
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			matrixA[i][j] = rand()%10+1;
			matrixB[i][j] = rand()%10+1;
		}
	}
}
void testingMatrixValue() {
	auto begin = std::chrono::high_resolution_clock::now();
	printf("Testing without OpenMP\n");
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			matrixD[i][j] = 0;
			for (int k = 0; k < n; k++) {
				matrixD[i][j] = matrixD[i][j] + matrixA[i][k] * matrixB[k][j];
			}
		}
	}
	auto end = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
	printf("Time Taken for test with no interface is = %f \n", std::chrono::duration<double>(elapsed).count());
	
}
double parallel_for_multiplication(int n, int threads) {
	//complete parallel matrix multiplication with omp
	double startTime = omp_get_wtime();
#pragma omp parallel for shared(matrixA,  matrixB, matrixC) schedule(dynamic) num_threads(threads)
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			matrixC[i][j] = 0;
			for (int k = 0; k < n; k++) {
				matrixC[i][j] = matrixC[i][j] + matrixA[i][k] * matrixB[k][j];
			}
		}
	}
	double endTime = omp_get_wtime();
	return endTime - startTime;

}
void saveMatrix(const char* title)
{
	int i, j;
	FILE* f1;
	const char* Title = title;
	f1 = fopen(Title, "w");
	if (f1 == NULL)
	{
		printf("file coult not be opened");
		exit(-1);
	}

	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			fprintf(f1, "%5.2f ", matrixC[i][j]);
		}
		fprintf(f1, "\n");
	}

}
void saveMatrixTest(const char* title)
{
	int i, j;
	FILE* f1;
	const char* Title = title;
	f1 = fopen(Title, "w");
	if (f1 == NULL)
	{
		printf("file coult not be opened");
		exit(-1);
	}

	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			fprintf(f1, "%5.2f", matrixD[i][j]);
		}
		fprintf(f1, "\n");
	}
}
void print_matrix(double m[n][n])
{
	int i, j = 0;
	for (i = 0; i < n; i++) {
		printf("\n\t| ");
		for (j = 0; j < n; j++)
			printf("%2f ", m[i][j]);
		printf("|");
	}
	printf("\n");
}
//g++ -fopenmp MatrixMultiplication.cpp -o anyname.cpp
// ./anyname.cpp
//python3 
