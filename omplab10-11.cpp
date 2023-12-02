#include <mpi.h>
#include <stdio.h>

void setMessage(char *data, int rank)
{
	int length = sprintf(data, "My rank %d\n", rank);
	for (int i = length; i < 20; i++)
	{
		data[i] = 0;
	}
}

int main(int argc, char **argv)
{
	int rank = 0; 
	int size = 0;
	double time1 = 0.0;
	double time2 = 0.0;
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	char* wr;

	if (rank != 0)
	{
		wr = new char[20];
		setMessage(wr, rank);
		MPI_Send(wr, leng, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
	}
	else
	{
		wr = new char[20 * size];
		setMessage(wr, rank);
		time1 = MPI_Wtime();
		MPI_Gather(wr, 20, MPI_CHAR, wr, 20, MPI_CHAR, 0, MPI_COMM_WORLD);

		for (int i = 1; i < size; i++)
		{
			MPI_Recv(wr + 20 * i, 20, MPI_CHAR, i, 0, MPI_COMM_WORLD, &status);
		}

		time2 = MPI_Wtime();
		printf("Result: time= ", (time2 - time1) / 100);
		for (int i = 0; i < size; i++)
		{
			printf("%s", wr + 20 * i);
		}

	}
	MPI_Finalize();
	return 0;
}
