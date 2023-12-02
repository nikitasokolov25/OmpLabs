#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<mpi.h>
#include<iostream>
#include<fstream>

int number = 0; 
int rank = 0; 

void inputData(double* matrix, double* vector, int size)
{                     
	std::ifstream input1("matrix.txt");
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			input1 >> matrix[i * size + j];
		}
	}

	std::ifstream input2("vector.txt");
	for (int i = 0; i < size; i++)
	{
		input2 >> vector[i];
	}
}

void initialize(double*& matrix, double*& vector, double*& result, double*& procRows, double*& procResult, int& size, int& rowNum) 
{
	if (rank == 0) {
		std::cout << "matrix and vector size is ";
		scanf("%d", &size);
	}

	MPI_Bcast(&size, 1, MPI_INT, 0, MPI_COMM_WORLD); 
	int restRows = size;

	for (int i = 0; i < rank; i++)
		restRows = restRows - restRows / (number - i);
	rowNum = restRows / (number - rank);
	vector = new double[size];
	result = new double[size];
	procRows = new double[rowNum * size];
	procResult = new double[rowNum];

	if (rank == 0) {
		matrix = new double[size * size];
		inputData(matrix, vector, size);
	}
}

void dataDistribute(double* matrix, double* procRows, double* vector, int size, int rowNum)
{
	int *sendNum;
	int *sendInd; 
	int restRows = size;

	MPI_Bcast(vector, size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	sendInd = new int[number];
	sendNum = new int[number];
	rowNum = (size / number);
	sendNum[0] = rowNum * size;
	sendInd[0] = 0;

	for (int i = 1; i < number; i++)
	{
		restRows -= rowNum;
		rowNum = restRows / (number - i);
		sendNum[i] = rowNum * size;
		sendInd[i] = sendInd[i - 1] + sendNum[i - 1];
	}
	MPI_Scatterv(matrix, sendNum, sendInd, MPI_DOUBLE, procRows, sendNum[rank], MPI_DOUBLE, 0, MPI_COMM_WORLD);
}

void replication(double* procResult, double* result, int size, int rowNum)
{
	int *receiveNum;
	int *receiveInd; 
	int restRows = size;

	receiveNum = new int[number];
	receiveInd = new int[number]; 
	receiveInd[0] = 0;
	receiveNum[0] = size / number;

	for (int i = 1; i < number; i++)
	{
		restRows -= receiveNum[i - 1];
		receiveNum[i] = restRows / (number - i);
		receiveInd[i] = receiveInd[i - 1] + receiveNum[i - 1];
	}
	MPI_Allgatherv(procResult, receiveNum[rank], MPI_DOUBLE, result, receiveNum, receiveInd, MPI_DOUBLE, MPI_COMM_WORLD);
}

void parallelResultCalculate(double* procRows, double* vector, double* procResult, int size, int rowNum)
{
	for (int i = 0; i < rowNum; i++)
	{
		procResult[i] = 0;
		for (int j = 0; j < size; j++)
		{
			procResult[i] += procRows[i*size + j] * vector[j];
		}
	}
}

void printMatrix(double* matrix, int rowsCount, int columnsCount)
{
	for (int i = 0; i < rowsCount; i++)
	{
		for (int j = 0; j < columnsCount; j++)
			std::cout << matrix[i*columnsCount + j];
		std::cout << "\n";
	}
}

void printVector(double* vector, int size)
{
	for (int i = 0; i < size; i++)
		std::cout << vector[i] << " ";
	std::cout << "\n";
}

int main(int argc, char** argv)
{
	double* matrix;
	double* vector; 
	double* result;
	int size = 0; 
	int rowNum = 0;
	double* procRows; 
	double* procResult;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &number);

	initialize(matrix, vector, result, procRows, procResult, size, rowNum);
	dataDistribute(matrix, procRows, vector, size, rowNum);
	parallelResultCalculate(procRows, vector, procResult, size, rowNum);
	replication(procResult, result, size, rowNum);

	if (rank == 0)
	{
		std::cout << "matrix:\n"; 
		printMatrix(matrix, size, size);
		std::cout << "vector:\n";
		printVector(vector, size);
		std::cout << "result vector:\n";
		printVector(result, size);
	}

	MPI_Finalize();
	return 0;
}
