#pragma once

#include "Helper.h"
#include <string>
#include <algorithm>
#include <deque>
#include <memory>

class CKeySeqGenerator
{
public:

    struct DialectData
    {
        std::wstring word;                  // The source word
        std::deque<std::wstring> dialect;   // List of non-swapable dialect sequences for source word
        std::deque<int> dPos;               // The positions allowed for each group of dialects
        DialectData(const std::wstring& word, const std::deque<std::wstring>& dialect, const std::deque<int> dPos)
            : word(word)
            , dialect(dialect)
            , dPos(dPos){}
    };

    CKeySeqGenerator(
            const CHelper::InputMode& _mode = CHelper::TELEX, // Input mode
            const int scanDepth = 1,                        // Scan depth for dialect
            const int toneDepth = 1);                       // Scan depth for tone
    ~CKeySeqGenerator(void);

    // Retuen the lead consonant
    std::wstring getLeadConsonants(const std::wstring& w);
    // Return the lead vowels part of the word (after being stripped of lead consonants)
    std::wstring getLeadVowels(std::wstring& w);

    // Recursive function to get the DialectData from vowel, return a list of DialectData ready to translate to key sequence
    std::list<DialectData> analyzeVowels(std::wstring& w, int depth = 0);

    // Generate key sequence from a given word
    std::list<std::pair<std::wstring, std::wstring>> generateSequence(const std::wstring& w);

private:
    CHelper::InputMode _mode;
    CHelper* _helper;
    const int _VOWEL_SCAN_MAX_DEPTH;
    const int _TONE_MAX_DEPTH;
    // Combine multi DialectData list to form a single list of DialectData
    std::list<DialectData> _combineChar(std::list<std::list<DialectData>>& l);
    // Generate sequences of tonemark
    std::list<DialectData> _genTone(int tone);
};
