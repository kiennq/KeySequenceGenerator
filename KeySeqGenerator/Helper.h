#pragma once
#include <fstream>
#include <string>
#include <list>
#include <map>
#include <set>


class Helper
{
public:

    static enum InputMode
	{
        TELEX = 0,
        VNI
	};
	static struct ToneDict
	{
        wchar_t _src;
		wchar_t _org;
        int _tone;
        ToneDict(wchar_t src, wchar_t org, int tone)
			: _src(src)
            , _org(org)
            , _tone(tone) {}
	};
    static struct DialectDict
	{
        std::wstring _dest;
        std::wstring _src;
        wchar_t _modifier;
        int _modPos;
        DialectDict(std::wstring& dest, std::wstring& src, wchar_t mod, int modPos)
			: _dest(dest)
            , _src(src)
            , _modifier(mod)
			, _modPos(modPos){}
	};
	const static wchar_t toneMark[][2];

#pragma region Singleton
#pragma endregion 

#pragma region Read write file
    static void readFileUTF16(const std::wstring& fn, std::list<std::wstring>& lstr);
	static void readFileUTF16(const std::wstring& fn, std::multimap<std::wstring, std::wstring>& mstr);
    static void writeFileUTF16(const std::wstring& fn, std::list<std::pair<std::wstring, std::wstring>>& lstr, std::ios_base::open_mode _Mode = 0);
#pragma endregion

#pragma region String manpulation
    static bool isNormalChar(const wchar_t c);
    static bool consistOfNormalChar(const std::wstring& s);
    
    static bool isConsonant(const wchar_t c);
    static bool isLeadCons(const std::wstring& s);

    static bool islower(const wchar_t c);
    static bool isupper(const wchar_t c);
    static std::wstring tolower(const std::wstring& str);
    static wchar_t tolower(const wchar_t c);
    static std::wstring toupper(const std::wstring& str);
    static wchar_t toupper(const wchar_t c);

    static int stripTonemark(std::wstring& w);

    static bool getDialectSeq(std::wstring& w, std::list<DialectDict>& l, InputMode _mode = TELEX);
#pragma endregion


protected:
	Helper(void);
	~Helper(void);

private:
    static std::map<wchar_t, wchar_t> _lower;
    static std::map<wchar_t, wchar_t> _upper;
    static std::set<wchar_t> _consonant;
    static std::set<std::wstring> _leadCons;
    static std::map<wchar_t, ToneDict> _tone; 
	static std::multimap<std::wstring, DialectDict> _telex;
	static std::multimap<std::wstring, DialectDict> _vni;
};

