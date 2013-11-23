#include "stdafx.h"
#include "Helper.h"
#include <algorithm>

using namespace std;

map<wchar_t, wchar_t> CHelper::_upper = []()->map<wchar_t, wchar_t>
{
    map<wchar_t, wchar_t> m;
    m[0xe0] = 0xc0; // à -> À
    m[0xe1] = 0xc1; // á -> Á
    m[0xe2] = 0xc2; // â -> Â
    m[0xe3] = 0xc3; // ã -> Ã
    m[0xe8] = 0xc8; // è -> È
    m[0xe9] = 0xc9; // é -> É
    m[0xea] = 0xca; // ê -> Ê
    m[0xec] = 0xcc; // ì -> Ì
    m[0xed] = 0xcd; // í -> Í
    m[0xf2] = 0xd2; // ò -> Ò
    m[0xf3] = 0xd3; // ó -> Ó
    m[0xf4] = 0xd4; // ô -> Ô
    m[0xf5] = 0xd5; // õ -> Õ
    m[0xf9] = 0xd9; // ù -> Ù
    m[0xfa] = 0xda; // ú -> Ú
    m[0xfd] = 0xdd; // ý -> Ý
    m[0x103] = 0x102;   // ă -> Ă
    m[0x111] = 0x110;   // đ -> Đ
    m[0x129] = 0x128;   // ĩ -> Ĩ
    m[0x169] = 0x168;   // ũ -> Ũ
    m[0x1a1] = 0x1a0;   // ơ -> Ơ
    m[0x1b0] = 0x1af;   // ư -> Ư
    m[0x1ea1] = 0x1ea0; // ạ -> Ạ
    m[0x1ea3] = 0x1ea2; // ả -> Ả
    m[0x1ea5] = 0x1ea4; // ấ -> Ấ
    m[0x1ea7] = 0x1ea6; // ầ -> Ầ
    m[0x1ea9] = 0x1ea8; // ẩ -> Ẩ
    m[0x1eab] = 0x1eaa; // ẫ -> Ẫ
    m[0x1ead] = 0x1eac; // ậ -> Ậ
    m[0x1eaf] = 0x1eae; // ắ -> Ắ
    m[0x1eb1] = 0x1eb0; // ằ -> Ằ
    m[0x1eb3] = 0x1eb2; // ẳ -> Ẳ
    m[0x1eb5] = 0x1eb4; // ẵ -> Ẵ
    m[0x1eb7] = 0x1eb6; // ặ -> Ặ
    m[0x1eb9] = 0x1eb8; // ẹ -> Ẹ
    m[0x1ebb] = 0x1eba; // ẻ -> Ẻ
    m[0x1ebd] = 0x1ebc; // ẽ -> Ẽ
    m[0x1ebf] = 0x1ebe; // ế -> Ế
    m[0x1ec1] = 0x1ec0; // ề -> Ề
    m[0x1ec3] = 0x1ec2; // ể -> Ể
    m[0x1ec5] = 0x1ec4; // ễ -> Ễ
    m[0x1ec7] = 0x1ec6; // ệ -> Ệ
    m[0x1ec9] = 0x1ec8; // ỉ -> Ỉ
    m[0x1ecb] = 0x1eca; // ị -> Ị
    m[0x1ecd] = 0x1ecc; // ọ -> Ọ
    m[0x1ecf] = 0x1ece; // ỏ -> Ỏ
    m[0x1ed1] = 0x1ed0; // ố -> Ố
    m[0x1ed3] = 0x1ed2; // ồ -> Ồ
    m[0x1ed5] = 0x1ed4; // ổ -> Ổ
    m[0x1ed7] = 0x1ed6; // ỗ -> Ỗ
    m[0x1ed9] = 0x1ed8; // ộ -> Ộ
    m[0x1edb] = 0x1eda; // ớ -> Ớ
    m[0x1edd] = 0x1edc; // ờ -> Ờ
    m[0x1edf] = 0x1ede; // ở -> Ở
    m[0x1ee1] = 0x1ee0; // ỡ -> Ỡ
    m[0x1ee3] = 0x1ee2; // ợ -> Ợ
    m[0x1ee5] = 0x1ee4; // ụ -> Ụ
    m[0x1ee7] = 0x1ee6; // ủ -> Ủ
    m[0x1ee9] = 0x1ee8; // ứ -> Ứ
    m[0x1eeb] = 0x1eea; // ừ -> Ừ
    m[0x1eed] = 0x1eec; // ử -> Ử
    m[0x1eef] = 0x1eee; // ữ -> Ữ
    m[0x1ef1] = 0x1ef0; // ự -> Ự
    m[0x1ef3] = 0x1ef2; // ỳ -> Ỳ
    m[0x1ef5] = 0x1ef4; // ỵ -> Ỵ
    m[0x1ef7] = 0x1ef6; // ỷ -> Ỷ
    m[0x1ef9] = 0x1ef8; // ỹ -> Ỹ
    return m;
}();
map<wchar_t, wchar_t> CHelper::_lower ([]()->map<wchar_t, wchar_t>
{
    map<wchar_t, wchar_t> m;
    m[0xc0] = 0xe0; // À -> à
    m[0xc1] = 0xe1; // Á -> á
    m[0xc2] = 0xe2; // Â -> â
    m[0xc3] = 0xe3; // Ã -> ã
    m[0xc8] = 0xe8; // È -> è
    m[0xc9] = 0xe9; // É -> é
    m[0xca] = 0xea; // Ê -> ê
    m[0xcc] = 0xec; // Ì -> ì
    m[0xcd] = 0xed; // Í -> í
    m[0xd2] = 0xf2; // Ò -> ò
    m[0xd3] = 0xf3; // Ó -> ó
    m[0xd4] = 0xf4; // Ô -> ô
    m[0xd5] = 0xf5; // Õ -> õ
    m[0xd9] = 0xf9; // Ù -> ù
    m[0xda] = 0xfa; // Ú -> ú
    m[0xdd] = 0xfd; // Ý -> ý
    m[0x102] = 0x103;   // Ă -> ă
    m[0x110] = 0x111;   // Đ -> đ
    m[0x128] = 0x129;   // Ĩ -> ĩ
    m[0x168] = 0x169;   // Ũ -> ũ
    m[0x1a0] = 0x1a1;   // Ơ -> ơ
    m[0x1af] = 0x1b0;   // Ư -> ư
    m[0x1ea0] = 0x1ea1; // Ạ -> ạ
    m[0x1ea2] = 0x1ea3; // Ả -> ả
    m[0x1ea4] = 0x1ea5; // Ấ -> ấ
    m[0x1ea6] = 0x1ea7; // Ầ -> ầ
    m[0x1ea8] = 0x1ea9; // Ẩ -> ẩ
    m[0x1eaa] = 0x1eab; // Ẫ -> ẫ
    m[0x1eac] = 0x1ead; // Ậ -> ậ
    m[0x1eae] = 0x1eaf; // Ắ -> ắ
    m[0x1eb0] = 0x1eb1; // Ằ -> ằ
    m[0x1eb2] = 0x1eb3; // Ẳ -> ẳ
    m[0x1eb4] = 0x1eb5; // Ẵ -> ẵ
    m[0x1eb6] = 0x1eb7; // Ặ -> ặ
    m[0x1eb8] = 0x1eb9; // Ẹ -> ẹ
    m[0x1eba] = 0x1ebb; // Ẻ -> ẻ
    m[0x1ebc] = 0x1ebd; // Ẽ -> ẽ
    m[0x1ebe] = 0x1ebf; // Ế -> ế
    m[0x1ec0] = 0x1ec1; // Ề -> ề
    m[0x1ec2] = 0x1ec3; // Ể -> ể
    m[0x1ec4] = 0x1ec5; // Ễ -> ễ
    m[0x1ec6] = 0x1ec7; // Ệ -> ệ
    m[0x1ec8] = 0x1ec9; // Ỉ -> ỉ
    m[0x1eca] = 0x1ecb; // Ị -> ị
    m[0x1ecc] = 0x1ecd; // Ọ -> ọ
    m[0x1ece] = 0x1ecf; // Ỏ -> ỏ
    m[0x1ed0] = 0x1ed1; // Ố -> ố
    m[0x1ed2] = 0x1ed3; // Ồ -> ồ
    m[0x1ed4] = 0x1ed5; // Ổ -> ổ
    m[0x1ed6] = 0x1ed7; // Ỗ -> ỗ
    m[0x1ed8] = 0x1ed9; // Ộ -> ộ
    m[0x1eda] = 0x1edb; // Ớ -> ớ
    m[0x1edc] = 0x1edd; // Ờ -> ờ
    m[0x1ede] = 0x1edf; // Ở -> ở
    m[0x1ee0] = 0x1ee1; // Ỡ -> ỡ
    m[0x1ee2] = 0x1ee3; // Ợ -> ợ
    m[0x1ee4] = 0x1ee5; // Ụ -> ụ
    m[0x1ee6] = 0x1ee7; // Ủ -> ủ
    m[0x1ee8] = 0x1ee9; // Ứ -> ứ
    m[0x1eea] = 0x1eeb; // Ừ -> ừ
    m[0x1eec] = 0x1eed; // Ử -> ử
    m[0x1eee] = 0x1eef; // Ữ -> ữ
    m[0x1ef0] = 0x1ef1; // Ự -> ự
    m[0x1ef2] = 0x1ef3; // Ỳ -> ỳ
    m[0x1ef4] = 0x1ef5; // Ỵ -> ỵ
    m[0x1ef6] = 0x1ef7; // Ỷ -> ỷ
    m[0x1ef8] = 0x1ef9; // Ỹ -> ỹ
    return m;
}());
set<wchar_t> CHelper::_consonant ([]()->set<wchar_t>
{
    set<wchar_t> s;
    s.insert(0x62); // b
    s.insert(0x63); // c
    s.insert(0x64); // d
    s.insert(0x111);    // đ
    s.insert(0x67); // g
    s.insert(0x68); // h
    s.insert(0x6b); // k
    s.insert(0x6c); // l
    s.insert(0x6d); // m
    s.insert(0x6e); // n
    s.insert(0x70); // p
    s.insert(0x71); // q
    s.insert(0x72); // r
    s.insert(0x73); // s
    s.insert(0x74); // t
    s.insert(0x75); // u
    s.insert(0x76); // v
    s.insert(0x78); // x
    s.insert(0x42); // B
    s.insert(0x43); // C
    s.insert(0x44); // D
    s.insert(0x110);    // Đ
    s.insert(0x47); // G
    s.insert(0x48); // H
    s.insert(0x4b); // K
    s.insert(0x4c); // L
    s.insert(0x4d); // M
    s.insert(0x4e); // N
    s.insert(0x50); // P
    s.insert(0x51); // Q
    s.insert(0x52); // R
    s.insert(0x53); // S
    s.insert(0x54); // T
    s.insert(0x55); // U
    s.insert(0x56); // V
    s.insert(0x58); // X
    return s;
}());
set<wstring> CHelper::_leadCons ([]()->set<wstring>  // Lead consonants set
{
    wifstream fin(L"_leadConsonant.txt");
    fin.imbue(locale(fin.getloc(), new codecvt_utf16<wchar_t, 0x10ffff, little_endian>));

    set<wstring> l;

    for (wstring ls; (fin >> ls) != 0; l.insert(ls));
    fin.close();
    return l;
}());
map<wchar_t, CHelper::ToneDict> CHelper::_tone ([]()->map<wchar_t, CHelper::ToneDict> // Tone Dictionary
{
    wifstream fin(L"_toneDict.txt");
    fin.imbue(locale(fin.getloc(), new codecvt_utf16<wchar_t, 0x10ffff, little_endian>));

    map<wchar_t, CHelper::ToneDict> m;

    int tone;

    for (wchar_t src, org; (fin >> src >> org >> tone) != 0; m.insert(make_pair(src, CHelper::ToneDict(src, org, tone))));
    fin.close();
    return m;
}());

static CHelper* _helper = NULL; // CHelper singleton

CHelper* CHelper::getInstance (InputMode _mode)
{
    switch (_mode)
    {
    case CHelper::TELEX:
        if (_helper) return _helper;
        else return _helper = new CHelper(L"_telexDict.txt", L"_telexTone.txt");
    case CHelper::VNI:
        if (_helper) return _helper;
        else return _helper = new CHelper(L"_vniDict.txt", L"_vniTone.txt");
    default:
        break;
    }
    return NULL;
}
void CHelper::destroy()
{
    if (_helper) delete(_helper);
}

CHelper::CHelper(const wstring& dialectDictName, const wstring& toneDictName)
{
    // Read the dialect dict
    wifstream fin(dialectDictName);
    fin.imbue(locale(fin.getloc(), new codecvt_utf16<wchar_t, 0x10ffff, little_endian>));
    
    wstring dest, src;
    wchar_t mod;
    int pos;
    
    for (; fin >>  dest >> src >> mod >> pos; _dialect.insert(make_pair(dest, CHelper::DialectDict(dest, src, mod==L'='?0:mod, pos))));
    fin.close();

    // Read the tone map
    fin.open(toneDictName);
    fin.imbue(locale(fin.getloc(), new codecvt_utf16<wchar_t, 0x10ffff, little_endian>));
    wchar_t tone;
    for (; fin >> tone; )
    {
        if (tone != '#')
        {
            _toneMap.push_back(tone);
        }
    }
    fin.close();
}


CHelper::~CHelper(void)
{
}

void CHelper::readFileUTF16(const wstring& fn, list<wstring>& lstr)
{
    wifstream fin(fn.c_str());
    fin.imbue(locale(fin.getloc(), new codecvt_utf16<wchar_t, 0x10ffff, little_endian>));

    for (wstring str; fin >> str; lstr.push_back(str));

    fin.close();
}

void CHelper::readFileUTF16(const wstring& fn, multimap<wstring, wstring>& mstr)
{
    wifstream fin(fn.c_str());
    fin.imbue(locale(fin.getloc(), new codecvt_utf16<wchar_t, 0x10ffff, little_endian>));

    for (wstring strKey, strVal; fin >> strVal >> strKey; mstr.insert(make_pair(strKey, strVal)));

    fin.close();
}

void CHelper::writeFileUTF16(const wstring& fn, list<pair<wstring, wstring>>& lstr, std::ios_base::open_mode _Mode)
{
    //wofstream fout(fn.c_str(), ios::binary|_Mode);
    //fout.imbue(locale(fout.getloc(), new codecvt_utf16<wchar_t, 0x10ffff, (codecvt_mode)(generate_header|little_endian)>));
    //for (pair<wstring, wstring>& p : lstr)
    //{
    //fout << p.first << L'\t' << p.second << L"\r\n";
    //}
    //fout.close();
    // Faster than fstream
    FILE* fout;
    if (!_wfopen_s(&fout, fn.c_str(), L"w+, ccs=UTF-16LE"))
    {
        for (pair<wstring, wstring>& p : lstr)
        {
            fwprintf(fout, L"%s\t%s\n", p.first.c_str(), p.second.c_str());
        }
        fclose(fout);
    }
}

bool CHelper::isNormalChar(const wchar_t c)
{
    return c < 0xC0;    // All character greater than xC0 is modified char.
}

bool CHelper::consistOfNormalChar(const std::wstring& s)
{
    for (const wchar_t& c : s)
    {
        if (!isNormalChar(c))
        {
            return false;
        }
    }
    return true;
}

bool CHelper::isConsonant(const wchar_t c)
{
    return _consonant.find(c) != _consonant.end();
}

bool CHelper::isLeadCons(const wstring& s)
{
    return _leadCons.find(tolower(s)) != _leadCons.end();
}

bool CHelper::islower(const wchar_t c)
{
    if (isNormalChar(c))
    {
        return ::iswlower(c) != 0;
    }
    return _lower.find(c) != _lower.end();
}

bool CHelper::isupper(const wchar_t c)
{
    if (isNormalChar(c))
    {
        return ::iswupper(c) != 0;
    }
    return _upper.find(c) != _upper.end();
}

wstring CHelper::tolower(const wstring& str)
{
    wstring re;
    re.reserve(str.length());
    for (const wchar_t c : str)
    {
        if (isNormalChar(c))
        {
            re.push_back(::towlower(c));
        }
        else
        {
            auto lo = _lower.find(c);
            if (lo != _lower.end()) re.push_back(lo->second);
            else re.push_back(c);
        }
    }
    return re;
    // The method of using locale run much more slower than using defined table
    //wstring re = str;
    //locale loc("vi");
    //std::transform(re.begin(), re.end(), re.begin(), [&](wchar_t t)->wchar_t {return std::tolower(t,loc);});
    //return re;
}

wchar_t CHelper::tolower(const wchar_t c)
{
    if (isNormalChar(c))
    {
        return ::towlower(c);
    }
    auto lo = _lower.find(c);
    if (lo != _lower.end())
    {
        return lo->second;
    }
    return c;
}

wstring CHelper::toupper(const wstring& str)
{
    wstring re;
    re.reserve(str.length());
    for (const wchar_t c : str)
    {
        if (isNormalChar(c))
        {
            re.push_back(::towupper(c));
        }
        else
        {
            auto up = _upper.find(c);
            if (up!= _upper.end()) re.push_back(up->second);
            else re.push_back(c);
        }
    }
    return re;
}

wchar_t CHelper::toupper(const wchar_t c)
{
    if (isNormalChar(c))
    {
        return ::toupper(c);
    }
    auto lo = _upper.find(c);
    if (lo != _upper.end())
    {
        return lo->second;
    }
    return c;
}

int CHelper::stripTonemark(std::wstring& w)
{
    for (wchar_t& c : w)
    {
        if (!isNormalChar(c))
        {
            auto ic = _tone.find(c);
            if (ic != _tone.end())
            {
                c = ic->second._org;
                return ic->second._tone;
            }
        }
    }
    return 0;
}

bool CHelper::getDialectSeq(wstring& w, list<DialectDict>& l)
{
    auto finder = _dialect.equal_range(w);
    for (auto i = finder.first; i != finder.second; i++)
    {
        l.push_back(i->second);
    }
    return !l.empty();
}

wchar_t CHelper::getToneMark (int tone)
{
    if (tone >= 0 && tone < (int)_toneMap.size())
    {
        return _toneMap[tone];
    }
    return _toneMap[0];
}

int CHelper::getNumOfTone()
{
    return _toneMap.size();
}