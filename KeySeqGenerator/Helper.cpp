#include "stdafx.h"
#include "Helper.h"
#include <locale>
#include <codecvt>

using namespace std;

static Helper* helper = NULL;
map<wchar_t, wchar_t> Helper::_upper = []()->map<wchar_t, wchar_t>
{
    map<wchar_t, wchar_t> m;
    m[0xe0] = 0xc0;
    m[0xe1] = 0xc1;
    m[0xe2] = 0xc2;
    m[0xe3] = 0xc3;
    m[0xe8] = 0xc8;
    m[0xe9] = 0xc9;
    m[0xea] = 0xca;
    m[0xec] = 0xcc;
    m[0xed] = 0xcd;
    m[0xf2] = 0xd2;
    m[0xf3] = 0xd3;
    m[0xf4] = 0xd4;
    m[0xf5] = 0xd5;
    m[0xf9] = 0xd9;
    m[0xfa] = 0xda;
    m[0xfd] = 0xdd;
    m[0x103] = 0x102;
    m[0x111] = 0x110;
    m[0x129] = 0x128;
    m[0x169] = 0x168;
    m[0x1a1] = 0x1a0;
    m[0x1b0] = 0x1af;
    m[0x1ea1] = 0x1ea0;
    m[0x1ea3] = 0x1ea2;
    m[0x1ea5] = 0x1ea4;
    m[0x1ea7] = 0x1ea6;
    m[0x1ea9] = 0x1ea8;
    m[0x1eab] = 0x1eaa;
    m[0x1ead] = 0x1eac;
    m[0x1eaf] = 0x1eae;
    m[0x1eb1] = 0x1eb0;
    m[0x1eb3] = 0x1eb2;
    m[0x1eb5] = 0x1eb4;
    m[0x1eb7] = 0x1eb6;
    m[0x1eb9] = 0x1eb8;
    m[0x1ebb] = 0x1eba;
    m[0x1ebd] = 0x1ebc;
    m[0x1ebf] = 0x1ebe;
    m[0x1ec1] = 0x1ec0;
    m[0x1ec3] = 0x1ec2;
    m[0x1ec5] = 0x1ec4;
    m[0x1ec7] = 0x1ec6;
    m[0x1ec9] = 0x1ec8;
    m[0x1ecb] = 0x1eca;
    m[0x1ecd] = 0x1ecc;
    m[0x1ecf] = 0x1ece;
    m[0x1ed1] = 0x1ed0;
    m[0x1ed3] = 0x1ed2;
    m[0x1ed5] = 0x1ed4;
    m[0x1ed7] = 0x1ed6;
    m[0x1ed9] = 0x1ed8;
    m[0x1edb] = 0x1eda;
    m[0x1edd] = 0x1edc;
    m[0x1edf] = 0x1ede;
    m[0x1ee1] = 0x1ee0;
    m[0x1ee3] = 0x1ee2;
    m[0x1ee5] = 0x1ee4;
    m[0x1ee7] = 0x1ee6;
    m[0x1ee9] = 0x1ee8;
    m[0x1eeb] = 0x1eea;
    m[0x1eed] = 0x1eec;
    m[0x1eef] = 0x1eee;
    m[0x1ef1] = 0x1ef0;
    m[0x1ef3] = 0x1ef2;
    m[0x1ef5] = 0x1ef4;
    m[0x1ef7] = 0x1ef6;
    m[0x1ef9] = 0x1ef8;
    return m;
}();
map<wchar_t, wchar_t> Helper::_lower = []()->map<wchar_t, wchar_t>
{
    map<wchar_t, wchar_t> m;
    m[0xc0] = 0xe0;
    m[0xc1] = 0xe1;
    m[0xc2] = 0xe2;
    m[0xc3] = 0xe3;
    m[0xc8] = 0xe8;
    m[0xc9] = 0xe9;
    m[0xca] = 0xea;
    m[0xcc] = 0xec;
    m[0xcd] = 0xed;
    m[0xd2] = 0xf2;
    m[0xd3] = 0xf3;
    m[0xd4] = 0xf4;
    m[0xd5] = 0xf5;
    m[0xd9] = 0xf9;
    m[0xda] = 0xfa;
    m[0xdd] = 0xfd;
    m[0x102] = 0x103;
    m[0x110] = 0x111;
    m[0x128] = 0x129;
    m[0x168] = 0x169;
    m[0x1a0] = 0x1a1;
    m[0x1af] = 0x1b0;
    m[0x1ea0] = 0x1ea1;
    m[0x1ea2] = 0x1ea3;
    m[0x1ea4] = 0x1ea5;
    m[0x1ea6] = 0x1ea7;
    m[0x1ea8] = 0x1ea9;
    m[0x1eaa] = 0x1eab;
    m[0x1eac] = 0x1ead;
    m[0x1eae] = 0x1eaf;
    m[0x1eb0] = 0x1eb1;
    m[0x1eb2] = 0x1eb3;
    m[0x1eb4] = 0x1eb5;
    m[0x1eb6] = 0x1eb7;
    m[0x1eb8] = 0x1eb9;
    m[0x1eba] = 0x1ebb;
    m[0x1ebc] = 0x1ebd;
    m[0x1ebe] = 0x1ebf;
    m[0x1ec0] = 0x1ec1;
    m[0x1ec2] = 0x1ec3;
    m[0x1ec4] = 0x1ec5;
    m[0x1ec6] = 0x1ec7;
    m[0x1ec8] = 0x1ec9;
    m[0x1eca] = 0x1ecb;
    m[0x1ecc] = 0x1ecd;
    m[0x1ece] = 0x1ecf;
    m[0x1ed0] = 0x1ed1;
    m[0x1ed2] = 0x1ed3;
    m[0x1ed4] = 0x1ed5;
    m[0x1ed6] = 0x1ed7;
    m[0x1ed8] = 0x1ed9;
    m[0x1eda] = 0x1edb;
    m[0x1edc] = 0x1edd;
    m[0x1ede] = 0x1edf;
    m[0x1ee0] = 0x1ee1;
    m[0x1ee2] = 0x1ee3;
    m[0x1ee4] = 0x1ee5;
    m[0x1ee6] = 0x1ee7;
    m[0x1ee8] = 0x1ee9;
    m[0x1eea] = 0x1eeb;
    m[0x1eec] = 0x1eed;
    m[0x1eee] = 0x1eef;
    m[0x1ef0] = 0x1ef1;
    m[0x1ef2] = 0x1ef3;
    m[0x1ef4] = 0x1ef5;
    m[0x1ef6] = 0x1ef7;
    m[0x1ef8] = 0x1ef9;
    return m;
}();

Helper* Helper::instance()
{
    if (helper) return helper;
	else return helper = new Helper();
}

void Helper::destroy()
{
    if (!helper) return;
    delete helper;
    helper = NULL;
}

Helper::Helper(void)
{
    wifstream fin(L"lowerUpperCase.txt");
    fin.imbue(locale(fin.getloc(), new codecvt_utf16<wchar_t, 0x10ffff, little_endian>));

    for (wchar_t lo, up; fin >> lo >> up;){
        _lower.insert(make_pair(up, lo));
        _upper.insert(make_pair(lo, up));
	}

    fin.close();
}


Helper::~Helper(void)
{
}

void Helper::readFileUTF16(const wstring& fn, list<wstring>& lstr)
{
    wifstream fin(fn.c_str());
    fin.imbue(locale(fin.getloc(), new codecvt_utf16<wchar_t, 0x10ffff, little_endian>));

    for (wstring str; fin >> str; lstr.push_back(str));

    fin.close();
}

void Helper::readFileUTF16(const wstring& fn, multimap<wstring, wstring>& mstr)
{
    wifstream fin(fn.c_str());
    fin.imbue(locale(fin.getloc(), new codecvt_utf16<wchar_t, 0x10ffff, little_endian>));

	for (wstring strKey, strVal; fin >> strVal >> strKey; mstr.insert(make_pair(strKey, strVal)));

    fin.close();
}

void Helper::writeFileUTF16(const wstring& fn, list<pair<wstring, wstring>>& lstr, std::ios_base::open_mode _Mode)
{
    wofstream fout(fn.c_str(), ios::binary|_Mode);
    fout.imbue(locale(fout.getloc(), new codecvt_utf16<wchar_t, 0x10ffff, (codecvt_mode)(generate_header|little_endian)>));

    for (pair<wstring, wstring>& p : lstr) {
        fout << p.first << L'\t' << p.second << L"\r\n";
	}

    fout.close();
}

bool Helper::isNormalChar(const wchar_t c)
{
    return c < 0xC0;
}

bool Helper::islower(const wchar_t c)
{
    if (isNormalChar(c)) return ::iswlower(c) != 0;
	return _lower.find(c) != _lower.end();
}

bool Helper::isupper(const wchar_t c)
{
    if (isNormalChar(c)) return ::iswupper(c) != 0;
	return _upper.find(c) != _upper.end();
}

wstring Helper::tolower(const wstring& str)
{
    wstring re;
    re.reserve(str.length());
    for (const wchar_t c : str) {
        if (isNormalChar(c)) re.push_back(::towlower(c));
		else {
            map<wchar_t, wchar_t>::iterator lo = _lower.find(c);
            if (lo != _lower.end()) re.push_back(lo->second);
            else re.push_back(c);
		}
	}
    return re;
}

wchar_t Helper::tolower(const wchar_t c)
{
    if (isNormalChar(c)) return ::towlower(c);
    map<wchar_t, wchar_t>::iterator lo = _lower.find(c);
	if (lo != _lower.end()) return lo->second;
    else return c;
}

wstring Helper::toupper(const wstring& str)
{
    wstring re;
    re.reserve(str.length());
    for (const wchar_t c : str) {
        if (isNormalChar(c)) re.push_back(::towupper(c));
		else {
            map<wchar_t, wchar_t>::iterator up = _upper.find(c);
            if (up!= _upper.end()) re.push_back(up->second);
            else re.push_back(c);
		}
	}
    return re;
}

wchar_t Helper::toupper(const wchar_t c)
{
    if (isNormalChar(c)) return ::toupper(c);
    map<wchar_t, wchar_t>::iterator lo = _upper.find(c);
	if (lo != _upper.end()) return lo->second;
    else return c;
}