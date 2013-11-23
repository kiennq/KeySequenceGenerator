#pragma once
#include <fstream>
#include <string>
#include <list>
#include <map>
#include <deque>
#include <set>
#include <locale>
#include <codecvt>


class CHelper
{
public:

    static enum InputMode
    {
        TELEX = 0,
        VNI = 1
    };
    // Tone dictionary
    static struct ToneDict
    {
        wchar_t _src;   // The source (modified) character
        wchar_t _org;   // The base character
        int _tone;      // The tonemark
        ToneDict(wchar_t src, wchar_t org, int tone)
            : _src(src)
            , _org(org)
            , _tone(tone) {}
    };
    // Dialect dictionary
    static struct DialectDict
    {
        std::wstring _dest; // The output word
        std::wstring _src;  // The base key sequence
        wchar_t _modifier;  // The modifier come with key sequence
        int _modPos;        // The position allowed for the modifier
        DialectDict(std::wstring& dest, std::wstring& src, wchar_t mod, int modPos)
            : _dest(dest)
            , _src(src)
            , _modifier(mod)
            , _modPos(modPos){}
    };

#pragma region Constructor
    static CHelper* getInstance (InputMode _mode = TELEX);
    static void destroy();
    CHelper(const std::wstring& dialectDictName, const std::wstring& toneDictName);
    ~CHelper(void);
#pragma endregion

#pragma region Read write file
    static void readFileUTF16(const std::wstring& fn, std::list<std::wstring>& lstr);
    static void readFileUTF16(const std::wstring& fn, std::multimap<std::wstring, std::wstring>& mstr);
    static void writeFileUTF16(const std::wstring& fn, std::list<std::pair<std::wstring, std::wstring>>& lstr, std::ios_base::open_mode _Mode = 0);
#pragma endregion

#pragma region String manpulation
    // Check if a char is non-modified
    static bool isNormalChar(const wchar_t c);
    // Check if a string is contained of only non-modified character
    static bool consistOfNormalChar(const std::wstring& s);

    static bool isConsonant(const wchar_t c);
    static bool isLeadCons(const std::wstring& s);

    static bool islower(const wchar_t c);
    static bool isupper(const wchar_t c);
    static std::wstring tolower(const std::wstring& str);
    static wchar_t tolower(const wchar_t c);
    static std::wstring toupper(const std::wstring& str);
    static wchar_t toupper(const wchar_t c);

    // Strip the tone mark out of the word and return the tonemark
    static int stripTonemark(std::wstring& w);

    // Get the DialectDict for a component, return false if not found
    bool getDialectSeq(std::wstring& w, std::list<DialectDict>& l);
    // Return the character corresponding with the tone
    wchar_t getToneMark (int tone);
    // Return the number of available tone mark
    int getNumOfTone();
#pragma endregion


protected:

private:
    static std::map<wchar_t, wchar_t> _lower;
    static std::map<wchar_t, wchar_t> _upper;
    static std::set<wchar_t> _consonant;
    static std::set<std::wstring> _leadCons;
    static std::map<wchar_t, ToneDict> _tone;
    // There are large mapping different between each typing method, hence the _dialect dictionary is non-static
    std::multimap<std::wstring, DialectDict> _dialect;
    // Map to corresponding tone in each method
    std::deque<wchar_t> _toneMap;
    int _numOfTone;
};
