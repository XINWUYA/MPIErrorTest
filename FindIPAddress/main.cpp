#include <iostream>
#include <vector> 
#include <string>
#include <regex>

bool findIPAddress(const char* vBuffer, char* vPattern, std::vector<const char*> voResult)
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
			voResult.push_back(MatchResult[0].str().c_str());
			vBuffer = MatchResult.suffix().first;
		}
		return true;
	}	
}

int main(int argc, char* argv[])
{
	char* Pattern = "((25[0-5]|2[0-4]\\d|((1\\d{2})|([1-9]?\\d)))\\.){3}(25[0-5]|2[0-4]\\d|((1\\d{2})|([1-9]?\\d)))";
	std::vector<const char*> IPSet;
	findIPAddress(argv[2], Pattern, IPSet);
	return 0;
}