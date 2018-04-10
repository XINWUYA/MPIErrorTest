#pragma once

#include <iostream>
#include <mpi.h>
#include <vector>
#include <winsock.h>
#include <IPHlpApi.h>
#include <regex>

typedef HANDLE(WINAPI* ICMPCREATEFILE)(VOID);
typedef BOOL(WINAPI* ICMPCLOSEHANDLE)(HANDLE);
typedef DWORD(WINAPI* ICMPSENDECHO)(HANDLE, DWORD, LPVOID, WORD, PIP_OPTION_INFORMATION, LPVOID, DWORD, DWORD);

class CMPICommunicator
{
public:
	CMPICommunicator();
	~CMPICommunicator();

	bool initMPI(int vArgc, char **vArgv, std::vector<const char*> vIPSet);
	void finalizeMPI();

	bool createMPICommGroup(int *vRanks, int vGroupSize, MPI_Comm& voComm/*, int vRootRankID*/);
	int getRankID(const MPI_Comm& vComm);
	int getRankSize(const MPI_Comm& vComm);
	bool sendBroadcast(int vRootRankID, int vComm, int vSendSize, void* vSendData);
	bool recvBroadcast(int vRootRankID, int vComm, int& voRecvSize, void*& voRecvData);

private:
	bool __extractIPAddress(const char* vBuffer, char* vPattern, std::vector<const char*> voIPSet);
	bool __checkNetwokEnvironment(std::vector<const char*> vIPSet);
	bool __initICMP();
	bool __pingICMP(const char* vHostIP);
	char* __getLocalIP();

	ICMPCREATEFILE pIcmpCreateFile;
	ICMPCLOSEHANDLE pIcmpCloseHandle;
	ICMPSENDECHO pIcmpSendEcho;
};