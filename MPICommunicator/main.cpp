#include <iostream>
#include "MPICommunicator.h"

int main(int argc, char* argv[])
{
	std::vector<const char*> IPSet = { "111.13.100.92", "218.205.84.251", "211.152.123.110" };
	//std::cout << argc << ": " << argv[0] /*<< " " << argv[1]*/ << std::endl;
	CMPICommunicator MPIComm;
	if (!MPIComm.initMPI(argc, argv, IPSet))
	{
		std::cout << "Init Failed." << std::endl;
		return -1;
	}

	int RankID = MPIComm.getRankID(MPI_COMM_WORLD);
	if (0 == RankID)
	{
		char HelloStr[] = "Hello,Guys!";
		int StrLength = _countof(HelloStr);
		std::cout << StrLength << std::endl;
		MPIComm.sendBroadcast(0, MPI_COMM_WORLD, StrLength, &HelloStr);
	}
	else
	{
		int RecvStrLength = 0;
		void *pRecvHelloStr = nullptr;
		MPIComm.recvBroadcast(0, MPI_COMM_WORLD, RecvStrLength, pRecvHelloStr);
		char* pData = (char*)pRecvHelloStr;
		std::cout << RankID << ": " << pData << std::endl;
		delete pData;
	}

	MPIComm.finalizeMPI();
	return 0;
}