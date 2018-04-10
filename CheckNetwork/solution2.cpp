//#include <iostream>
//#include <string>
//
//int main()
//{
//	std::string StrPing = "ping ";
//	std::string StrIP[3] = { "111.13.100.92", "218.205.84.251", "211.152.123.110" };
//
//	for (int i = 0; i < 3; ++i)
//	{
//		std::string StrTmp = StrPing + StrIP[i];
//
//		if (!std::system(StrTmp.c_str()))
//			std::cout << "ping " << StrIP[i] << " success!" << std::endl;
//		else
//			std::cout << "ping " << StrIP[i] << " failed!" << std::endl;
//	}
//
//	return 0;
//}