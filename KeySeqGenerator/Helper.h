#pragma once
#include <fstream>
#include <string>
#include <list>
#include <map>


class Helper
{
public:
#pragma region Singleton
    static Helper* instance();
    static void destroy();
#pragma endregion 

#pragma region Read write file
    static void readFileUTF16(const std::wstring& fn, std::list<std::wstring>& lstr);
	static void readFileUTF16(const std::wstring& fn, std::multimap<std::wstring, std::wstring>& mstr);
    static void writeFileUTF16(const std::wstring& fn, std::list<std::pair<std::wstring, std::wstring>>& lstr, std::ios_base::open_mode _Mode = 0);
#pragma endregion

#pragma region String manpulation
    static bool isNormalChar(const wchar_t c);
    
    static bool islower(const wchar_t c);
    static bool isupper(const wchar_t c);
    static std::wstring tolower(const std::wstring& str);
    static wchar_t tolower(const wchar_t c);
    static std::wstring toupper(const std::wstring& str);
    static wchar_t toupper(const wchar_t c);
#pragma endregion


protected:
	Helper(void);
	~Helper(void);

private:
    static std::map<wchar_t, wchar_t> _lower;
    static std::map<wchar_t, wchar_t> _upper;
};

