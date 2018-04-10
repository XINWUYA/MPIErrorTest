#include <iostream>
#include <mpi.h>
#include <Windows.h>
#include "MPI.h"

int main(int argc, char* argv[])
{
	MPICommon::initMPI(argc, argv);

	std::cout << "World Rank Size : " << MPICommon::getRankSize(MPI_COMM_WORLD) << std::endl;
	int Ranks[6] = { 0, 1, 3, 5, 7, 8 };
	MPI_Comm PrimeComm;
	MPICommon::createMPICommGroup(Ranks, sizeof(Ranks)/sizeof(int), PrimeComm);

	if (PrimeComm != MPI_COMM_NULL)
	{

		//std::cout << "MyComm Size : " << MPICommon::getRankSize(PrimeComm) << std::endl;
		int RankID = MPICommon::getRankID(PrimeComm);
		int n = 10;
		while (n--)
		{
			if (RankID == 0)
			{
				char HelloStr[] = "Hello,Guys!";
				int StrLength = _countof(HelloStr);
				std::cout << StrLength << std::endl;
				MPICommon::sendBroadcast(0, PrimeComm, StrLength, &HelloStr);
			}
			else
			{
				int RecvStrLength = 0;
				void *pRecvHelloStr = nullptr;
				MPICommon::recvBroadcast(0, PrimeComm, RecvStrLength, pRecvHelloStr);
				char* pData = (char*)pRecvHelloStr;
				std::cout << RankID << ": " << pData << n << std::endl;
				delete pData;
			}
			MPI_Barrier(PrimeComm);
		}
	}

	MPICommon::finalizeMPI();
	return 0;
}