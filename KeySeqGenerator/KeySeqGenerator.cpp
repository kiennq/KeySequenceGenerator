// KeySeqGenerator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <fstream>
#include <iostream>
#include <locale>
#include <codecvt>

using namespace std;

void readUTF16 (const wstring& filename)
{
    wofstream ofile(_T("outx.txt"), ios::binary);
	ofile.imbue(locale(ofile.getloc(), new codecvt_utf16<wchar_t, 0x10ffff, (codecvt_mode)(generate_header|little_endian)>));

    wifstream fin(filename.c_str());
    fin.imbue(locale(fin.getloc(), new codecvt_utf16<wchar_t, 0x10ffff, little_endian>));
    
    for (wstring str; fin >> str; ) {
        for (unsigned int i = 0; i < str.length(); i++)
            cout << showbase << hex << str[i] << ' ';
        cout << '\n'; 
        ofile << str << _T("\r\n"); 
	}
    
    ofile << _T('\x1ed1'); 

    wstring s;

    fin.close();
    ofile.close();
}


int _tmain(int argc, _TCHAR* argv[])
{
    wstring fn;
    wcout << L"File name: " ;
    wcin >> fn;

    readUTF16(fn);
    wcin >> fn;

	return 0;
}

