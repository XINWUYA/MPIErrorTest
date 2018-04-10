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
////��������ָ�뺯��   
//ICMPCREATEFILE pIcmpCreateFile;
//ICMPCLOSEHANDLE pIcmpCloseHandle;
//ICMPSENDECHO pIcmpSendEcho;
//// �������ܣ���ʼ��ICMP������
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
//// �������ܣ��ж��Ƿ���pingͨIP
//// ����������IP��ַ������
//BOOL ICMPPing(char* host)
//{
//	DWORD timeOut = 1000;                                              //���ó�ʱ   
//	ULONG hAddr = inet_addr(host);                                     //�����IP��ַ��ֱ��ת��   
//	if (hAddr == INADDR_NONE)
//	{
//		hostent* hp = gethostbyname(host);                             //�������������DNS������IP��ַ   
//		if (hp)
//			memcpy(&hAddr, hp->h_addr_list, hp->h_length);             //IP��ַ   
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
//	unsigned char pSend[36];                                                                   //���Ͱ�   
//	memset(pSend, 'E', 32);
//
//	int repSize = sizeof(ICMP_ECHO_REPLY) + 32;
//	unsigned char pReply[100];                                                                 //���հ�   
//	ICMP_ECHO_REPLY* pEchoReply = (ICMP_ECHO_REPLY*)pReply;
//
//	DWORD nPackets = pIcmpSendEcho(hIp, hAddr, pSend, 32, &ipoi, pReply, repSize, timeOut);             //����ICMP���ݱ���   
//
//	if (pEchoReply->Status != 0)                                                                  //��ʱ������������������ICMP ����Ŀ������������  
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