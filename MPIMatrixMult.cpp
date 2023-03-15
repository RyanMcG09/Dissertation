#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <math.h>
#include <string>
#include <fstream>
#include <stdio.h>
#include <mpi.h>
#include <chrono>

//define variables
using namespace std;
const int n = 2;
double matrixA[n][n];
double matrixB[n][n];
double matrixC[n][n];
double matrixD[n][n];
double populate_matrix(int n);
double parallel_for_multiplication_MPI(int to, int from, int p, int rank);
void saveMatrix(const char* title);
void print_matrix(double m[n][n]);
double testingMatrixValue();
void saveMatrixTest(const char* title);
int main(int argc, char** argv)
{
	//MPI Variables
	int myrank, noProcessors;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	MPI_Comm_size(MPI_COMM_WORLD, &noProcessors);
	//set up local variables
	const int numIterations = 1;
	double popTime = 0;
	if (myrank == 0) {
		popTime = populate_matrix(n);
//		testingMatrixValue();
	}
	MPI_Barrier(MPI_COMM_WORLD);
	//Get the average time value over different number of cores for speedup
	int from = myrank * (n / noProcessors);
	int to = (myrank+1) * (n / noProcessors);
	double endTimetot = 0;
	double allTimes = 0;
	auto begin = std::chrono::high_resolution_clock::now();
	for (int j = 1; j <= numIterations; j++) {
		MPI_Barrier(MPI_COMM_WORLD);
		double itTime = parallel_for_multiplication_MPI(to,from, noProcessors, myrank);
		if (myrank == 0){
			allTimes += itTime;
		}
		//printf("Time taken for core %d on iteration %d = %f \n",myrank,j,itTime);
		
	}
	auto end = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
	if (myrank == 0) {
		printf("Average time taken for a %d x %d matrix over %d cores and over %d iterations is %f s\n", n, n, noProcessors, numIterations, allTimes/numIterations);
	}
	MPI_Barrier(MPI_COMM_WORLD);
	if (myrank == 0)
	{
		printf("###Final Printing here###\n");
		printf("###A = ###\n");
		print_matrix(matrixA);
		printf("###B = ###\n");
		print_matrix(matrixB);
		printf("###C = ###\n");
		print_matrix(matrixC);
	}
	
	
	//save times to .txt file
	if (myrank == 0) {
		ofstream outfile;
		string filename = "MPISizes.txt";
		outfile.open(filename, std::ios_base::app);
		outfile << noProcessors << "," << popTime << "," << time << "\n";
		outfile.close();
	}
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();
//	const char* title = "MPIMatrix.txt";
//	saveMatrix(title);
//	testingMatrixValue();
//	const char* title1 = "TestMatrix.txt";
//	saveMatrixTest(title1);
	return 0;
}
void saveMatrix(const char* title)
{
	int i,j;
	FILE *f1;
	const char* Title = title;
	f1 = fopen(Title, "w");
	if (f1 == NULL)
	{
		printf("file coult not be opened");
		exit(-1);
	}

	for (i=0; i<n;i++)
	{
		for (j=0;j<n;j++)
		{
			fprintf(f1,"%5.2f ", matrixC[i][j]);
		}
		fprintf(f1,"\n");
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
			fprintf(f1, "%5.2f ", matrixD[i][j]);
		}
		fprintf(f1, "\n");
	}
}
double testingMatrixValue() {
	auto begin = std::chrono::high_resolution_clock::now();	
	printf("Testing without MPI\n");
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
	printf("Elapsed time for test = %f \n",std::chrono::duration<double>(elapsed).count());
}
double populate_matrix(int n) {

	//different seed for random values
	srand(time(NULL));
	auto begin = std::chrono::high_resolution_clock::now();
	int count = 1;
	//populate matrices
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			//matrixA[i][j] = rand()%10+1;
			//matrixB[i][j] = rand()%10+1;
			matrixA[i][j] = count;
			matrixB[i][j] = count;
			count = count + 1;
		}
	}
	auto end = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);

	//print_matrix(matrixA);
	//print_matrix(matrixB);
	return std::chrono::duration<double>(elapsed).count();
	
}
double parallel_for_multiplication_MPI(int to, int from, int p, int rank) {
	//complete parallel matrix multiplication with mpi
//	printf("computing slice %d (from row %d to %d)\n", rank, from, to - 1);
	double ARows[n / p][n];
	double CRows[n / p][n];
	double BCols[n][n / p];

	double matrixBT[n][n];

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			matrixBT[i][j] = matrixB[j][i];
		}
	}
//	MPI_Bcast(matrixB, n * n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Scatter(matrixA, n * n / p, MPI_DOUBLE, ARows, n * n / p, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Scatter(matrixB, n * n / p, MPI_DOUBLE, BCols, n * n / p, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);
	double startTime = MPI_Wtime();
	
	if (rank == 1){
	printf("####First BCols####\n");
	int i_, j_ = 0;
	for (i_ = 0; i_ < n; i_++){
		printf("\n\t| ");
		for (j_ = 0; j_ < n; j_++){
			printf("%2f",BCols[i_][j_]);
		}
	}
	printf("\n####First ARows####\n");
	for (i_ = 0; i_ < n/p; i_++){
		printf("\n\t| ");
		for (j_ = 0; j_ < n; j_++){
			printf("%2f",ARows[i_][j_]);
		}
	}
	}
	for (int k = 0; k < n/p; k++) {
		for (int i = 0; i < n; i++) {
			CRows[k][i] = 0;
			for (int j = 0; j < n; j++) {
				//CRows[k][i] = CRows[k][i] + ARows[k][j] * matrixB[j][i];
				CRows[k][i] = CRows[k][i] + ARows[k][j] * BCols[j][i];
			}
		}
	}
	MPI_Gather(CRows, n * n / p, MPI_DOUBLE, matrixC, n * n / p, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	double endTime = MPI_Wtime();
	double tt = endTime - startTime;
	return tt;

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

