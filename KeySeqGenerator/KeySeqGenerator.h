#pragma once

#include "Helper.h"
#include <string>
#include <algorithm>
#include <deque>

class KeySeqGenerator
{
public:

    struct DialectData
	{
        std::wstring word;
        std::deque<std::wstring> dialect;
        std::deque<int> dPos;
        DialectData(const std::wstring& word, const std::deque<std::wstring>& dialect, const std::deque<int> dPos)
			: word(word)
            , dialect(dialect)
			, dPos(dPos){}
	};

	KeySeqGenerator(const Helper::InputMode& _mode = Helper::TELEX, const int scanDepth = 1, const int toneDepth = 1);
	~KeySeqGenerator(void);

    std::wstring getLeadConsonants(const std::wstring& w);
    std::wstring getLeadVowels(std::wstring& w);

    // Recursive function to get the key sequence from vowel
    std::list<DialectData> analyzeVowels(std::wstring& w, int depth = 0);

    // Generate key sequence
    std::list<std::pair<std::wstring, std::wstring>> generateSequence(const std::wstring& w);

private:
    Helper::InputMode _mode;
    const int _VOWEL_SCAN_MAX_DEPTH;
    const int _TONE_MAX_DEPTH;
    // Combine different part to form a mix of key sequence
    std::list<DialectData> _combineChar(std::list<std::list<DialectData>>& l);
    std::list<DialectData> _genTone(int tone);
};

