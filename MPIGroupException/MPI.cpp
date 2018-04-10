#include "MPI.h"

//**********************************************************************************
//FUNCTION:
bool MPICommon::initMPI(int vArgc, char ** vArgv)
{
	if(MPI_Init(&vArgc, &vArgv) != MPI_SUCCESS) return false;
	return true;
}

//**********************************************************************************
//FUNCTION:
bool MPICommon::finalizeMPI()
{
	MPI_Finalize();
	return true;
}

//**********************************************************************************
//FUNCTION:
bool MPICommon::createMPICommGroup(const std::vector<int>& vpRanks, int vGroupSize, MPI_Comm& voComm/*, int vRootRankID*/)
{
	if (vpRanks.empty() || vGroupSize <= 0)
	{
		std::cout << "Error: Create MPI communicate group failed! Group should has at least one rank." << std::endl;
		return false;
	}

	MPI_Group WorldGroup;
	MPI_Comm_group(MPI_COMM_WORLD, &WorldGroup);
	MPI_Group PrimeGroup;
	MPI_Group_incl(WorldGroup, vGroupSize, vpRanks.data(), &PrimeGroup);
	MPI_Comm PrimeComm;
	MPI_Comm_create(MPI_COMM_WORLD, PrimeGroup, &PrimeComm);

	voComm = PrimeComm;

	/*if (WorldGroup != MPI_GROUP_NULL) MPI_Group_free(&WorldGroup);
	if (PrimeGroup != MPI_GROUP_NULL) MPI_Group_free(&PrimeGroup);
	if (PrimeComm != MPI_COMM_NULL) MPI_Comm_free(&PrimeComm);*/
	return true;
}

//**********************************************************************************
//FUNCTION:
int MPICommon::getRankID(const MPI_Comm & vComm)
{
	int RankID = -1;
	MPI_Comm_rank(vComm, &RankID);
	return RankID;
}

//**********************************************************************************
//FUNCTION:
int MPICommon::getRankSize(const MPI_Comm & vComm)
{
	int RankSize = -1;
	MPI_Comm_size(vComm, &RankSize);
	return RankSize;
}

//**********************************************************************************
//FUNCTION:
bool MPICommon::sendBroadcast(int vRootRankID, int vComm, int vSendSize, void * vSendData)
{
	if (!vSendData || vRootRankID < 0 || vComm == MPI_COMM_NULL || vSendSize <= 0)
	{
		std::cout << "Error: MPI sending broadcast failed! " << std::endl;
		return false;
	}

	MPI_Request Request;
	MPI_Status Status;
	MPI_Ibcast(&vSendSize, 1, MPI_INT, vRootRankID, vComm, &Request);
	MPI_Wait(&Request, &Status);

	MPI_Bcast(vSendData, vSendSize, MPI_BYTE, vRootRankID, vComm);
	return true;
}

//**********************************************************************************
//FUNCTION:
bool MPICommon::recvBroadcast(int vRootRankID, int vComm, int & voRecvSize, void *& voRecvData)
{
	if (vRootRankID < 0 || vComm == MPI_COMM_NULL)
	{
		std::cout << "Error: MPI receiving broadcast failed! " << std::endl;
		return false;
	}

	MPI_Request Request;
	MPI_Status Status;
	MPI_Ibcast(&voRecvSize, 1, MPI_INT, vRootRankID, vComm, &Request);
	MPI_Wait(&Request, &Status);

	voRecvData = new unsigned char[voRecvSize];

	MPI_Bcast(voRecvData, voRecvSize, MPI_BYTE, vRootRankID, vComm);
	return true;
}

