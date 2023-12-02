#include <mpi.h>
#include <stdio.h>
#include <iostream>
#include <string>

int key;
std::string message;
std::string encrypt_message, decrypt_message;

char* encryptMessage(char* message)
{
	encrypt_message = "";
	int current_key = 2;
	int key = 1;
	int begin = 0;
	int end = strlen(message) / current_key;

	for (int j = 0; j < current_key; j++)
	{
		for (int i = begin; i < end; i++)
		{
			int temp = (message[i] + key);
			encrypt_message += (char)temp;
		}

		key++;
		begin = end;

		if (j == current_key - 2)
		{
			end = strlen(message);
		}
		else
		{
			end = end + strlen(message) / current_key;
		}
	}

	message = strcpy(new char[encrypt_message.length() + 1], encrypt_message.c_str());
	return message;
}

char* decryptMessage(char* message)
{
	int current_key = 2;
	int key = 1;
	int begin = 0;
	int end = strlen(message) / current_key;

	for (int j = 0; j < current_key; j++)
	{
		for (int i = begin; i < end; i++)
		{
			int temp = (message[i] - key);
			decrypt_message += (char)temp;
		}

		key++;
		begin = end;

		if (j == current_key - 2)
		{
			end = strlen(message);
		}
		else
		{
			end = end + strlen(message) / current_key;
		}
	}

	message = strcpy(new char[decrypt_message.length() + 1], decrypt_message.c_str());
	return message;
}


int main(int argc, char **argv)
{	
	int rank = 0;
	
	char send_data[30] = "send data";
	char rec_data[30] = "rec data";
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == 0)
	{
		std::cout << "Send data = " << send_data << "\n";
		std::cout << "Send data before sending = " << encryptMessage(send_data) << "\n";
		MPI_Send(send_data, 30, MPI_CHAR, 1, rank, MPI_COMM_WORLD);
		rank++;
	}
	if (rank == 1)
	{
		std::cout << "Receive data before receive = " << rec_data << "\n";
		MPI_Recv(rec_data, 30, MPI_CHAR, 0, 7, MPI_COMM_WORLD, &status);
		std::cout << "Receive data after receive =  " << encryptMessage(send_data) << "\n";
		std::cout << "Receive data after decrypt =  " << decryptMessage(encryptMessage(send_data)) << "\n";
	}

	MPI_Finalize();
	return 0;
}
