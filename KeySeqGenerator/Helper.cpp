#include "stdafx.h"
#include "Helper.h"
#include <locale>
#include <codecvt>

using namespace std;

Helper::Helper(void)
{
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

void Helper::writeFileUTF16(const wstring& fn, list<pair<wstring, wstring>>& lstr)
{
    wofstream fout(fn.c_str(), ios::binary);
    fout.imbue(locale(fout.getloc(), new codecvt_utf16<wchar_t, 0x10ffff, (codecvt_mode)(generate_header|little_endian)>));

    for (pair<wstring, wstring>& p : lstr) {
        fout << p.first << L'\t' << p.second << L"\r\n";
	}

    fout.close();
}