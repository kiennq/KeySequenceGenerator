// KeySeqGenerator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <fstream>
#include <iostream>
#include <locale>
#include <codecvt>
#include <algorithm>
#include <vector>
#include <list>
#include "Helper.h"

using namespace std;

void readUTF16 (const wstring& filename)
{
    wofstream ofile(_T("outx.txt"), ios::binary);
	ofile.imbue(locale(ofile.getloc(), new codecvt_utf16<wchar_t, 0x10ffff, (codecvt_mode)(generate_header|little_endian)>));

    wifstream fin(filename.c_str());
    fin.imbue(locale(fin.getloc(), new codecvt_utf16<wchar_t, 0x10ffff, little_endian>));
    
    for (wstring str; fin >> str; ) {
        for (unsigned int i = 0; i < str.length(); i++) {
            cout << showbase << hex << str[i] << '\t';
			cout << (Helper::isNormalChar(str[i])? 't' : 'f' )<< ' ';
		}
        cout << '\n'; 
        ofile << str << _T("\r\n"); 
	}
    
    ofile << _T('\x1ed1'); 

    wstring s;

    fin.close();
    ofile.close();
}

void test (const wstring& fn)
{
    //Helper* helper = Helper::instance();
    multimap<wstring, wstring> t;
    list<pair<wstring, wstring>> l;
    Helper::readFileUTF16(fn, t);

    wofstream lfile(_T("lowerCode.txt"), ios::binary);
    wofstream ufile(_T("upperCode.txt"), ios::binary);
	lfile.imbue(locale(lfile.getloc(), new codecvt_utf16<wchar_t, 0x10ffff, (codecvt_mode)(generate_header|little_endian)>));
	ufile.imbue(locale(ufile.getloc(), new codecvt_utf16<wchar_t, 0x10ffff, (codecvt_mode)(generate_header|little_endian)>));

    for (multimap<wstring, wstring>::iterator j = t.begin(); j != t.end(); j++){
        wstring str = j->first;
        for (unsigned int i = 0; i < str.length(); i++) {
            cout << showbase << hex << str[i] << '\t';
			cout << (Helper::isNormalChar(str[i])? 't' : 'f' )<< '\t';
		}
        str = j->second;
        for (unsigned int i = 0; i < str.length(); i++) {
            cout << showbase << hex << str[i] << '\t';
			cout << (Helper::isNormalChar(str[i])? 't' : 'f' );
		}
        cout << '\n'; 
        //ufile << showbase << hex << L"m[" << (wint_t)j->first[0] << L"] = " << (wint_t)j->second[0] << ";\r\n";
        //lfile << showbase << hex << L"m[" << (wint_t)j->second[0] << L"] = " << (wint_t)j->first[0] << ";\r\n";
		l.push_back(make_pair(j->first, Helper::toupper(j->second)));
	}
    Helper::writeFileUTF16(L"out.txt", l);
    
}


int _tmain(int argc, _TCHAR* argv[])
{
    wstring fn;
    if (argc == 1) {
        wcout << L"File name: " ;
        wcin >> fn;
	} else {
		fn = argv[1];
	}

    test(fn);

	int myints[] = {1,2,3,3};
    vector<int> tl(myints, myints + sizeof(myints)/sizeof(int));

	cout << "The 3! possible permutations with 3 elements:\n";
	do {
		std::cout << noshowbase << tl[0] << ' ' << tl[1] << ' ' << tl[2] << ' ' << tl[3] << '\n';
	} while ( std::next_permutation(tl.begin(),tl.end()) );

	std::cout << "After loop: " << tl[0] << ' ' << tl[1] << ' ' << tl[2] << ' ' << tl[3] << '\n';

	return 0;
}

