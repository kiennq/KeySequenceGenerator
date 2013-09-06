// KeySeqGenerator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <ppl.h>
#include <string>
#include <fstream>
#include <iostream>
#include <memory>
#include <Windows.h>
#include "Helper.h"
#include "KeySeqGenerator.h"

using namespace std;
using namespace concurrency;

class ProgMain
{
public:

	ProgMain(){}
	~ProgMain(){}

    static int main(int argc, _TCHAR* argv[])
	{

		wstring fin, fon;
        auto mode = Helper::TELEX;
        int depth = 1;
        int tdepth = 1;
        ARG_TYPE flag = NONE;
        bool displayHelp = false;

		if (argc == 1) {
			wcout << _T("Input file name: ") ;
			wcin >> fin;
			wcout << _T("Output file name: ");
			wcin >> fon;
            wcout << _T("Mode:");
            wstring m;
            wcin >> m;
			if (m == _T("telex")) {
				mode = Helper::TELEX;
			} else if (m == _T("vni")) {
				mode = Helper::VNI;
			}
            wcout << _T("Depth: ");;
            wcin >> depth;
            wcout << _T("Tone Depth: ");;
            wcin >> tdepth;
		} else {
			for (int i = 1; i < argc; i++) {
				switch (flag)
				{
				case ProgMain::IN_FILE_NAME:
                    fin = argv[i];
                    flag = NONE;
					break;
				case ProgMain::OUT_FILE_NAME:
                    fon = argv[i];
                    flag = NONE;
					break;
				case ProgMain::SCAN_DEPTH:
                    try {
                        depth = stoi(argv[i]);
					} catch (exception e) {
					}
					break;
				case ProgMain::TONE_DEPTH:
					try {
						tdepth = stoi(argv[i]);
					} catch (exception e) {
					}
					break;
				case ProgMain::MODE:
                    if (wstring(argv[i]) == _T("telex")) {
                        mode = Helper::TELEX;
					} else if (wstring(argv[i]) == _T("vni")) {
                        mode = Helper::VNI;
					}
                    flag = NONE;
					break;
				case ProgMain::NONE:
                    if (wstring(argv[i]) == _T("-i")) {
                        flag = IN_FILE_NAME;
					} else if (wstring(argv[i]) == _T("-o")) {
                        flag = OUT_FILE_NAME;
					} else if (wstring(argv[i]) == _T("-m")) {
                        flag = MODE;
					} else if (wstring(argv[i]) == _T("--dialect-depth")) {
                        flag = SCAN_DEPTH;
					} else if (wstring(argv[i]) == _T("--tone-depth")) {
                        flag = TONE_DEPTH;
					}
					else if (wstring(argv[i]) == _T("--help")) {
                        displayHelp = true;
					}
					break;
				default:
					break;
				}
			}
		}

        __int64 elapsed = 0;
        
        if (!fin.empty() && !fon.empty()){
			elapsed = time_call([&]{run(fin, fon, mode, depth, tdepth);});
			wcout << L"# Run in: " << elapsed << L"ms" << endl;
		}
		else if (!displayHelp) showHelp();

        if (displayHelp) showHelp();

		return 0;
	}

private:
	static enum ARG_TYPE {
		IN_FILE_NAME,
		OUT_FILE_NAME,
		MODE,
        SCAN_DEPTH,
        TONE_DEPTH,
		NONE,
	};

    static void showHelp() 
	{
		wcout << _T("-i INPUT_TEXT_FILE -o OUTPUT_TEXT_FILE [-m {telex|vni}] [--dialect-depth DEPTH] [--tone-depth TDEPTH] [--help]") << endl;
        wcout << endl;
        wcout << _T("--help\t\t\t: Show this message") << endl;
        wcout << _T("-i\t\t\t: Input file name") << endl;
        wcout << _T("-o\t\t\t: Output file name") << endl;
        wcout << _T("-m\t\t\t: input mode: telex or vni, default is telex") << endl;
        wcout << _T("--dialect-depth\t\t: dialect scan depth") << endl;
        wcout << _T("--tone-depth\t\t: tone scan depth") << endl;
        wcout << endl;
        wcout << _T("ex: -i test.txt -o out.txt") <<endl;
	}

    static void run(const wstring& fin, const wstring& fon, Helper::InputMode mode, int depth, int tdepth)
	{

        list<wstring> lWords;
		wcout << _T("# Loading ") << fin << endl;
        Helper::readFileUTF16(fin, lWords);
		wcout << _T("# Reading ") << lWords.size() << _T(" word(s)") << endl;
        unique_ptr<KeySeqGenerator> keySeq(new KeySeqGenerator(mode, depth, tdepth));

        combinable<list<pair<wstring, wstring>>> pout;
        list<pair<wstring, wstring>> out;

        // Thread safe
        parallel_for_each(lWords.begin(), lWords.end(), [&](wstring& w){
            pout.local().splice(pout.local().end(), keySeq->generateSequence(w));
		});
        
		pout.combine_each([&](list<pair<wstring, wstring>>& local){
            out.splice(out.end(), local);
		});

		wcout << _T("# Wrote out ") << out.size() << _T(" sequence(s) to file ") << fon << endl;
        Helper::writeFileUTF16(fon, out);
	}

	template <class Function>
	static	__int64 time_call(Function&& f)
	{
		__int64 begin = GetTickCount();
		f();
		return GetTickCount() - begin;
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
    return ProgMain::main(argc, argv);
}

