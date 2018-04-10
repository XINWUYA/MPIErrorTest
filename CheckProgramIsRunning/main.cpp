#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

#define BUFFERSIZE 256

bool checkProgramIsRunning(const TCHAR* vPrgramName)
{
	PROCESSENTRY32 ProcessInfo;
	ProcessInfo.dwSize = sizeof(ProcessInfo);

	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapShot == INVALID_HANDLE_VALUE)
	{
		MessageBox(GetForegroundWindow(), TEXT("检测当前进程失败"), TEXT("错误"), MB_ICONEXCLAMATION);
		return false;
	}


	BOOL Status = Process32First(hSnapShot, &ProcessInfo);
	bool IsExist = false;
	while (Status)
	{
		if (std::wcscmp(vPrgramName, ProcessInfo.szExeFile) == 0)
		{
			std::wcout << "Process \"" << vPrgramName << "\" is running." << std::endl;
			IsExist = true;
			break;
		}
		Status = Process32Next(hSnapShot, &ProcessInfo);
	}
	CloseHandle(hSnapShot);

	return IsExist;
}

bool runMPI()
{
	HKEY ThisHKey;
	TCHAR ProductType[BUFFERSIZE];
	memset(ProductType, 0, sizeof(ProductType));
	DWORD BufferLength = BUFFERSIZE;
	char AppPath[BUFFERSIZE];

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("SOFTWARE\\Microsoft\\MPI"), 0, KEY_QUERY_VALUE, &ThisHKey) != ERROR_SUCCESS)
	{
		MessageBox(GetForegroundWindow(), TEXT("注册表打开失败！"), TEXT("错误"), MB_ICONEXCLAMATION);
		return false;
	}

	if (RegQueryValueEx(ThisHKey, TEXT("InstallRoot"), NULL, NULL, (LPBYTE)ProductType, &BufferLength) != ERROR_SUCCESS)
	{
		MessageBox(GetForegroundWindow(), TEXT("注册表找不到该路径！"), TEXT("错误"), MB_ICONEXCLAMATION);
		return false;
	}

	RegCloseKey(ThisHKey);

	WideCharToMultiByte(CP_ACP, 0, ProductType, -1, AppPath, sizeof(AppPath), NULL, NULL);
	strcat_s(AppPath, "Bin\\smpd -d");
	std::cout << AppPath << std::endl;
	WinExec(AppPath, SW_SHOW);

	return true;
}

int main(int argc, char* argv[])
{
	if (!checkProgramIsRunning(L"smpd.exe"))
	{
		std::cout << "smpd.exe is not running." << std::endl;
		if (!runMPI())	std::cout << "[Error] smpd.exe restarted failed, please check if it has been installed." << std::endl;
		else std::cout << "smpd.exe restarted successful.\n\nPlease don't close this window." << std::endl;
	}

	return 0;
}