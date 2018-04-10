//#define _WINSOCK_DEPRECATED_NO_WARNINGS 1
//#include <Winsock2.h>
//#include <iphlpapi.h>
//#include <iostream>
//#include <vector>
//
//typedef HANDLE(WINAPI* ICMPCREATEFILE)(VOID);
//typedef BOOL(WINAPI* ICMPCLOSEHANDLE)(HANDLE);
//typedef DWORD(WINAPI* ICMPSENDECHO)(HANDLE, DWORD, LPVOID, WORD, PIP_OPTION_INFORMATION, LPVOID, DWORD, DWORD);
// 
//ICMPCREATEFILE pIcmpCreateFile;
//ICMPCLOSEHANDLE pIcmpCloseHandle;
//ICMPSENDECHO pIcmpSendEcho;
//
//BOOL initIcmp()
//{
//	HINSTANCE HinstIcmp = LoadLibrary(TEXT("ICMP.DLL"));
//	if (HinstIcmp != NULL)
//	{
//		pIcmpCreateFile = (ICMPCREATEFILE)GetProcAddress(HinstIcmp, "IcmpCreateFile");
//		pIcmpCloseHandle = (ICMPCLOSEHANDLE)GetProcAddress(HinstIcmp, "IcmpCloseHandle");
//		pIcmpSendEcho = (ICMPSENDECHO)GetProcAddress(HinstIcmp, "IcmpSendEcho");
//		if ((pIcmpCreateFile == NULL) || (pIcmpCloseHandle == NULL) || (pIcmpSendEcho == NULL))
//			return false;
//		return true;
//	}
//	return false;
//}
//
//BOOL pingICMP(const char* vHostIP)
//{
//	DWORD TimeOut = 1000;
//
//	ULONG Address = inet_addr(vHostIP);
//	if (Address == INADDR_NONE)
//	{
//		hostent* pHost = gethostbyname(vHostIP);
//		if (pHost)
//			memcpy(&Address, pHost->h_addr_list, pHost->h_length);
//		else
//			return false;
//	}
//
//	HANDLE IcmpFile = pIcmpCreateFile();
//	IP_OPTION_INFORMATION IPOption;
//	memset(&IPOption, 0, sizeof(IP_OPTION_INFORMATION));
//	IPOption.Ttl = 128;//Time-To-Live   
//
//	unsigned char SendPacket[36]; 
//	memset(SendPacket, 'E', 32);
//
//	int ReplySize = sizeof(ICMP_ECHO_REPLY) + 32;
//	unsigned char ReplyPacket[100];   
//	ICMP_ECHO_REPLY* pEchoReply = (ICMP_ECHO_REPLY*)ReplyPacket;
//
//	DWORD Packets = pIcmpSendEcho(IcmpFile, Address, SendPacket, 32, &IPOption, ReplyPacket, ReplySize, TimeOut); 
//
//	if (pEchoReply->Status != 0)//Time is out, maybe host disabled the ICMP, or host not exist.  
//	{
//		std::cout << "[Error]: Host " << Address << " is unaccessible." << std::endl;
//		pIcmpCloseHandle(IcmpFile);
//		return false;
//	}
//	std::cout << "Host " << vHostIP << " is accessible." << std::endl;
//	pIcmpCloseHandle(IcmpFile);
//	return true;
//}
//int main()
//{
//	initIcmp();
//	std::vector<char*> StrIP = { "111.13.100.92", "218.205.84.251", "211.152.123.110" };
//	for (int i = 0; i < 3; ++i)
//		pingICMP(StrIP[i]);
//	return 0;
//}