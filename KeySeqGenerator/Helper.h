#pragma once
#include <fstream>
#include <string>
#include <list>
#include <map>


class Helper
{
public:
	Helper(void);
	~Helper(void);

    static void readFileUTF16(const std::wstring& fn, std::list<std::wstring>& lstr);
	static void readFileUTF16(const std::wstring& fn, std::multimap<std::wstring, std::wstring>& mstr);

	static void writeFileUTF16(const std::wstring& fn, std::list<std::pair<std::wstring, std::wstring>>& mstr);
};

