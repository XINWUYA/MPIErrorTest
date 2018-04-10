#pragma once
#include <mpi.h>
#include <iostream>

namespace MPICommon
{
	bool initMPI(int vArgc, char **vArgv);
	bool finalizeMPI();
	bool createMPICommGroup(int *vpRanks, int vGroupSize, MPI_Comm& voComm/*, int vRootRankID*/);
	int getRankID(const MPI_Comm& vComm);
	int getRankSize(const MPI_Comm& vComm);
	bool sendBroadcast(int vRootRankID, int vComm, int vSendSize, void* vSendData);
	bool recvBroadcast(int vRootRankID, int vComm, int& voRecvSize, void*& voRecvData);
}