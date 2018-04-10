#include <windows.h>
#include <winsock.h>
#include <string>
#include <iostream>
using namespace std;
#pragma comment(lib,"wsock32.lib")
int main()
{
	char buf[256] = "";
	struct hostent *ph = 0;
	WSADATA w;
	WSAStartup(0x0101, &w);//这一行必须在使用任何SOCKET函数前写！
	gethostname(buf, 256);
		string hostNmae = buf;//此处获得本机名称
	ph = gethostbyname(buf);
	const char *IP = inet_ntoa(*((struct in_addr *)ph->h_addr_list[0]));//此处获得本机IP
	cout << IP << endl;
	WSACleanup();
	return 0;
}