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
	WSAStartup(0x0101, &w);//��һ�б�����ʹ���κ�SOCKET����ǰд��
	gethostname(buf, 256);
		string hostNmae = buf;//�˴���ñ�������
	ph = gethostbyname(buf);
	const char *IP = inet_ntoa(*((struct in_addr *)ph->h_addr_list[0]));//�˴���ñ���IP
	cout << IP << endl;
	WSACleanup();
	return 0;
}