//#define _WINSOCK_DEPRECATED_NO_WARNINGS 1
//#include <Winsock2.h>
//#include "iphlpapi.h" 
//#include <iostream>
//
//
//#pragma comment(lib,"Iphlpapi.lib")
//#pragma comment(lib,"Ws2_32.lib")
//typedef HANDLE(WINAPI* ICMPCREATEFILE)(VOID);
//typedef BOOL(WINAPI* ICMPCLOSEHANDLE)(HANDLE);
//typedef DWORD(WINAPI* ICMPSENDECHO)(HANDLE, DWORD, LPVOID, WORD, PIP_OPTION_INFORMATION, LPVOID, DWORD, DWORD);
//
//
////定义三个指针函数   
//ICMPCREATEFILE pIcmpCreateFile;
//ICMPCLOSEHANDLE pIcmpCloseHandle;
//ICMPSENDECHO pIcmpSendEcho;
//// 函数功能：初始化ICMP函数：
//BOOL InitIcmp()
//{
//	HINSTANCE hIcmp = LoadLibrary(TEXT("ICMP.DLL"));
//	if (hIcmp == NULL)
//	{
//		return false;
//	}
//	pIcmpCreateFile = (ICMPCREATEFILE)GetProcAddress(hIcmp, "IcmpCreateFile");
//	pIcmpCloseHandle = (ICMPCLOSEHANDLE)GetProcAddress(hIcmp, "IcmpCloseHandle");
//	pIcmpSendEcho = (ICMPSENDECHO)GetProcAddress(hIcmp, "IcmpSendEcho");
//	if ((pIcmpCreateFile == NULL) || (pIcmpCloseHandle == NULL) || (pIcmpSendEcho == NULL))
//		return false;
//	return true;
//}
//
//// 函数功能：判断是否能ping通IP
//// 函数参数：IP地址或域名
//BOOL ICMPPing(char* host)
//{
//	DWORD timeOut = 1000;                                              //设置超时   
//	ULONG hAddr = inet_addr(host);                                     //如果是IP地址就直接转换   
//	if (hAddr == INADDR_NONE)
//	{
//		hostent* hp = gethostbyname(host);                             //如果是域名就用DNS解析出IP地址   
//		if (hp)
//			memcpy(&hAddr, hp->h_addr_list, hp->h_length);             //IP地址   
//		else
//		{
//			return false;
//		}
//	}
//	HANDLE hIp = pIcmpCreateFile();
//	IP_OPTION_INFORMATION ipoi;
//	memset(&ipoi, 0, sizeof(IP_OPTION_INFORMATION));
//	ipoi.Ttl = 128;                  //Time-To-Live   
//
//	unsigned char pSend[36];                                                                   //发送包   
//	memset(pSend, 'E', 32);
//
//	int repSize = sizeof(ICMP_ECHO_REPLY) + 32;
//	unsigned char pReply[100];                                                                 //接收包   
//	ICMP_ECHO_REPLY* pEchoReply = (ICMP_ECHO_REPLY*)pReply;
//
//	DWORD nPackets = pIcmpSendEcho(hIp, hAddr, pSend, 32, &ipoi, pReply, repSize, timeOut);             //发送ICMP数据报文   
//
//	if (pEchoReply->Status != 0)                                                                  //超时，可能是主机禁用了ICMP 或者目标主机不存在  
//	{
//		pIcmpCloseHandle(hIp);
//		return false;
//	}
//	std::cout << "ping tong" << std::endl;
//	pIcmpCloseHandle(hIp);
//	return true;
//}
//int main()
//{
//	InitIcmp();
//	ICMPPing("10.6.155.242");
//	return 0;
//}