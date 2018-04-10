#include "MPICommunicator.h"
#define TimeOut 1000
//#define _WINSOCK_DEPRECATED_NO_WARNINGS 1

CMPICommunicator::CMPICommunicator()
{
}

CMPICommunicator::~CMPICommunicator()
{
}

//**********************************************************************************
//FUNCTION:
bool CMPICommunicator::initMPI(int vArgc, char ** vArgv, std::vector<const char*> vIPSet)
{
	if (vIPSet.empty() || !__checkNetwokEnvironment(vIPSet)) return false;
	else
	{
		if (MPI_Init(&vArgc, &vArgv) != ERROR_SUCCESS)
		{
			std::cout << "[Error]: MPI init failed." << std::endl;
			return false;
		}
	}

	return true;
}

//**********************************************************************************
//FUNCTION:
void CMPICommunicator::finalizeMPI()
{
	MPI_Finalize();
}

//**********************************************************************************
//FUNCTION:
bool CMPICommunicator::createMPICommGroup(int * vRanks, int vGroupSize, MPI_Comm & voComm)
{
	if (!vRanks || vGroupSize <= 0)
	{
		std::cout << "Error: Create MPI communicate group failed! Group should has at least one rank." << std::endl;
		return false;
	}

	MPI_Group WorldGroup;
	MPI_Comm_group(MPI_COMM_WORLD, &WorldGroup);
	MPI_Group PrimeGroup;
	MPI_Group_incl(WorldGroup, vGroupSize, vRanks, &PrimeGroup);
	MPI_Comm PrimeComm;
	MPI_Comm_create(MPI_COMM_WORLD, PrimeGroup, &PrimeComm);

	voComm = PrimeComm;

	return true;
}

//**********************************************************************************
//FUNCTION:
int CMPICommunicator::getRankID(const MPI_Comm & vComm)
{
	int RankID = -1;
	MPI_Comm_rank(vComm, &RankID);
	return RankID;
}

//**********************************************************************************
//FUNCTION:
int CMPICommunicator::getRankSize(const MPI_Comm & vComm)
{
	int RankSize = -1;
	MPI_Comm_size(vComm, &RankSize);
	return RankSize;
}

//**********************************************************************************
//FUNCTION:
bool CMPICommunicator::sendBroadcast(int vRootRankID, int vComm, int vSendSize, void * vSendData)
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
bool CMPICommunicator::recvBroadcast(int vRootRankID, int vComm, int & voRecvSize, void *& voRecvData)
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

//**********************************************************************************
//FUNCTION:本来想从argv指令中直接提取IP，发现提取不了，所以暂时用不到该函数
bool CMPICommunicator::__extractIPAddress(const char * vBuffer, char * vPattern, std::vector<const char*> voIPSet)
{
	std::regex RegexExpression(vPattern);
	std::cmatch MatchResult;
	if (!std::regex_search(vBuffer, MatchResult, RegexExpression))
	{
		std::cout << "No IP match!" << std::endl;
		return false;
	}
	else
	{
		while (std::regex_search(vBuffer, MatchResult, RegexExpression))
		{
			std::cout << MatchResult[0] << std::endl;
			voIPSet.push_back(MatchResult[0].str().c_str());
			vBuffer = MatchResult.suffix().first;
		}
		return true;
	}
}

//**********************************************************************************
//FUNCTION:检查网络中需要用到的IP是否能ping通
bool CMPICommunicator::__checkNetwokEnvironment(std::vector<const char*> vIPSet)
{
	if (!__initICMP()) return false;
	else
		for (auto i : vIPSet)
			if (!__pingICMP(i))	return false;

	return true;
}

//**********************************************************************************
//FUNCTION:
bool CMPICommunicator::__initICMP()
{
	HINSTANCE HinstIcmp = LoadLibrary(TEXT("ICMP.DLL"));
	if (HinstIcmp != NULL)
	{
		pIcmpCreateFile = (ICMPCREATEFILE)GetProcAddress(HinstIcmp, "IcmpCreateFile");
		pIcmpCloseHandle = (ICMPCLOSEHANDLE)GetProcAddress(HinstIcmp, "IcmpCloseHandle");
		pIcmpSendEcho = (ICMPSENDECHO)GetProcAddress(HinstIcmp, "IcmpSendEcho");
		if ((pIcmpCreateFile == NULL) || (pIcmpCloseHandle == NULL) || (pIcmpSendEcho == NULL))
			return false;
		return true;
	}
	return false;
}

//**********************************************************************************
//FUNCTION:
bool CMPICommunicator::__pingICMP(const char* vHostIP)
{
	ULONG Address = inet_addr(vHostIP);
	if (Address == INADDR_NONE)
	{
		hostent* pHost = gethostbyname(vHostIP);
		if (pHost)
			memcpy(&Address, pHost->h_addr_list, pHost->h_length);
		else
			return false;
	}

	HANDLE IcmpFile = pIcmpCreateFile();
	IP_OPTION_INFORMATION IPOption;
	memset(&IPOption, 0, sizeof(IP_OPTION_INFORMATION));
	IPOption.Ttl = 128;//Time-To-Live   

	unsigned char SendPacket[36];
	memset(SendPacket, 'E', 32);

	int ReplySize = sizeof(ICMP_ECHO_REPLY) + 32;
	unsigned char ReplyPacket[100];
	ICMP_ECHO_REPLY* pEchoReply = (ICMP_ECHO_REPLY*)ReplyPacket;

	int PingCnt = 4;//as long as once success
	while (PingCnt--)
	{
		DWORD Packets = pIcmpSendEcho(IcmpFile, Address, SendPacket, 32, &IPOption, ReplyPacket, ReplySize, TimeOut);

		if (pEchoReply->Status == 0) 
		{
			std::cout << __getLocalIP() << ": Host " << vHostIP << " is accessible." << std::endl;
			pIcmpCloseHandle(IcmpFile);
			return true;		
		}	
	}
	//4 times sending all have the error: Time is out, maybe host disabled the ICMP, or host not exist. 
	std::cout << __getLocalIP() << ": [Error] Host " << vHostIP << " is unaccessible." << std::endl;
	pIcmpCloseHandle(IcmpFile);
	return false;
}

//**********************************************************************************
//FUNCTION:
char* CMPICommunicator::__getLocalIP()
{
	char HostName[256] = "";
	struct hostent *pHost = 0;

	WSADATA WSData;
	WSAStartup(0x0101, &WSData);
	gethostname(HostName, 256);
	//std::cout << HostName << std::endl;
	pHost = gethostbyname(HostName);
	char* pIP = inet_ntoa(*((struct in_addr *)pHost->h_addr_list[0]));
	//std::cout << pIP << std::endl;
	WSACleanup();

	return pIP;
}
