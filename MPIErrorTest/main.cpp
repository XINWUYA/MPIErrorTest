#include <iostream>
#include <mpi.h>
#include <Windows.h>

int main(int argc, char* argv[])
{
	MPI_Init(&argc, &argv);

	int RankID;
	MPI_Request Request;
	MPI_Status Status;
	MPI_Comm_rank(MPI_COMM_WORLD, &RankID);
	int n = 10;
	while (n--)
	{
		if (0 == RankID)
		{
			char HelloStr[] = "Hello,Guys!";
			int StrLength = _countof(HelloStr);
			std::cout << StrLength << std::endl;
			MPI_Ibcast(&StrLength, 1, MPI_INT, 0, MPI_COMM_WORLD, &Request);
			MPI_Wait(&Request, &Status);

			MPI_Bcast(HelloStr, StrLength, MPI_CHAR, 0, MPI_COMM_WORLD);
		}
		else
		{
			int RecvStrLength = 0;
			MPI_Ibcast(&RecvStrLength, 1, MPI_INT, 0, MPI_COMM_WORLD, &Request);
			MPI_Wait(&Request, &Status);
			//std::cout << RankID << ": " << RecvStrLength << std::endl;
			char *RecvHelloStr = new char[RecvStrLength];
			MPI_Bcast(RecvHelloStr, RecvStrLength, MPI_CHAR, 0, MPI_COMM_WORLD);
			std::cout << RankID << ": " << RecvHelloStr << n << std::endl;
			delete RecvHelloStr;
		}
		MPI_Barrier(MPI_COMM_WORLD);
	}

	MPI_Finalize();
	return 0;
}