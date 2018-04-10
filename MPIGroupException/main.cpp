#include <iostream>
#include <mpi.h>
#include <Windows.h>
#include "MPI.h"
#include <vector>


void checkGroupError(bool vRankState, int vCheckRankID, std::vector<int>& vRankSet, MPI_Comm& voGroupComm)
{
	int RankSetSize = vRankSet.size();
	if (!vRankState)
	{
		for (auto it = vRankSet.begin(); it != vRankSet.end();)
		{
			if (*it == vCheckRankID)
				it = vRankSet.erase(it);
			else
				++it;
		}
		for (int i = 0; i < vRankSet.size(); i++)
			std::cout << vRankSet[i] << std::endl;
		
	}
	if(vRankSet.size() != RankSetSize)
		MPICommon::createMPICommGroup(vRankSet, vRankSet.size(), voGroupComm);
}

int main(int argc, char* argv[])
{
	MPICommon::initMPI(argc, argv);

	std::cout << "World Rank Size : " << MPICommon::getRankSize(MPI_COMM_WORLD) << std::endl;
	std::vector<int> RanksSet = { 0, 1, 2, 3, 4, 5 };

	MPI_Comm PrimeComm;
	MPICommon::createMPICommGroup(RanksSet, RanksSet.size(), PrimeComm);

	if (PrimeComm != MPI_COMM_NULL)
	{
		int RankID = -1;
		int n = 10;
		bool RankState = true;
		while (n--)
		{
			checkGroupError(RankState, RankID, RanksSet, PrimeComm);
			RankID = MPICommon::getRankID(PrimeComm);

			if (RankID == 0)
			{
				char HelloStr[] = "Hello,Guys!";
				int StrLength = _countof(HelloStr);
				std::cout << StrLength << std::endl;
				MPICommon::sendBroadcast(0, PrimeComm, StrLength, &HelloStr);
			}
			else
			{
				if (n == 4 && RankID == 5)
				{
					RankState = false;
				}

				if (RankState)
				{
					int RecvStrLength = 0;
					void *pRecvHelloStr = nullptr;
					MPICommon::recvBroadcast(0, PrimeComm, RecvStrLength, pRecvHelloStr);
					char* pData = (char*)pRecvHelloStr;
					std::cout << RankID << ": " << pData << n << std::endl;
					delete pRecvHelloStr;
					//delete pData;
				}
			}
			MPI_Barrier(PrimeComm);
		}
	}

	MPICommon::finalizeMPI();
	return 0;
}