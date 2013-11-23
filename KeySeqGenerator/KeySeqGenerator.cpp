#include "stdafx.h"
#include "KeySeqGenerator.h"
#include <ppl.h>

using namespace std;
using namespace concurrency;

CKeySeqGenerator::CKeySeqGenerator(const CHelper::InputMode& _mode, const int scanDepth, const int toneDepth)
    : _mode(_mode)
    , _VOWEL_SCAN_MAX_DEPTH(scanDepth)
    , _TONE_MAX_DEPTH(toneDepth)
    , _helper(CHelper::getInstance(_mode))
{
}


CKeySeqGenerator::~CKeySeqGenerator(void)
{
}

wstring CKeySeqGenerator::getLeadConsonants(const wstring& w)
{
    unsigned int size = w.length() < 3 ? w.length() : 3;
    wstring&& lower = CHelper::tolower(w);
    lower.erase(size);
    for (int k = size; k > 0; k--, lower.pop_back())
    {
        // Scan until encountered the first lead consonants composition
        if (CHelper::isLeadCons(lower))
        {
            return w.substr(0, k);
        }
    }
    return wstring();
}

wstring CKeySeqGenerator::getLeadVowels(std::wstring& w)
{
    wstring vowels;
    for (wchar_t c : w)
    {
        // The vowel part should be only consisted of vowels
        if (!CHelper::isConsonant(c))
        {
            vowels.push_back(c);
        }
        else
        {
            return vowels;
        }
    }
    return vowels;
}

list<CKeySeqGenerator::DialectData> CKeySeqGenerator::analyzeVowels(wstring& w, int depth)
{
    if (depth >= _VOWEL_SCAN_MAX_DEPTH)
    {
        // If exceed the limit depth, return imediately
        return list<CKeySeqGenerator::DialectData>();
    }

    list<list<DialectData>> componentSeq;
    list<wchar_t> remainStr;

    for (unsigned i = w.length(); i > 0; i--, remainStr.push_front(w.back()), w.pop_back())
    {
        list<CHelper::DialectDict> curSeq;
        if (_helper->getDialectSeq(w, curSeq))  // If can find the matching sequence in the dictionary
        {
            componentSeq.push_back(list<DialectData>());    // Add a new element
            list<DialectData>& i = componentSeq.back();     // Take back that element and use, not use any temporary object
            for (CHelper::DialectDict& d : curSeq)
            {
                if (CHelper::consistOfNormalChar(d._src))    // If consisted of normal char, add to the list imediately
                {
                    deque<int> dPos;
                    deque<wstring> dMod;
                    if (d._modifier != L'')
                    {
                        dPos.push_back(d._modPos);
                        dMod.push_back(wstring(1, d._modifier));
                    }
                    i.push_back(DialectData(d._src, dMod, dPos));
                }
                else    // else analyze again for modified character
                {
                    auto&& depthAnalyze = analyzeVowels(d._src, depth+1);
                    for (DialectData& dd : depthAnalyze)
                    {
                        if (dd.dialect.empty() && d._modifier != L'')
                        {
                            // If there isn't any modifier yet, add a new slot for modifier
                            dd.dialect.push_back(wstring(1, d._modifier));
                            dd.dPos.push_back(d._modPos);
                        }
                        else
                        {
                            // According to the Vietnamese syntax there should be only one non-swapable sequence of modifier
                            // Push back the previous level modifier in the current modifier sequence
                            dd.dialect.back().push_back(d._modifier);
                        }
                        i.push_back(dd);    // Add this DialectData to the list created earlier
                    }
                }
            }
            if (i.empty()) return list<DialectData>();
            break;
        }
        else if (w.length() == 1)   // If the word length is 1 and not appear in the Dialect Dictionary
        {
            componentSeq.push_back(list<DialectData>());
            list<DialectData>& i = componentSeq.back();
            i.push_back(DialectData(w, deque<wstring>(), deque<int>()));
            break;
        }
    }
    for (wchar_t& r : remainStr)    // Repeat analyze with all single character remain
    {
        componentSeq.push_back(list<DialectData>());
        list<DialectData>& rf = componentSeq.back();
        rf.splice(rf.end(), analyzeVowels(wstring(1, r), depth));
        if (rf.empty())
        {
            return list<DialectData>();
        }
    }
    return _combineChar(componentSeq);
}

list<CKeySeqGenerator::DialectData> CKeySeqGenerator::_combineChar(list<list<CKeySeqGenerator::DialectData>>& l)
{
    list<DialectData> possibleSeq;
    if (l.size() == 1)  // If there is only one sub-list, return imediately
    {
        return l.front();
    }
    for (list<DialectData>& lwr : l)
    {
        list<DialectData> _stack;   // Stack for saving temporary data, elimiting the need of recursive function
        if (!lwr.empty())
        {
            // Empty possibleSeq and add everything to a temporary _stack
            _stack.splice(_stack.begin(), possibleSeq);
        }
        for (DialectData& wr : lwr)
        {
            if (_stack.empty())
            {
                // Add imediately
                possibleSeq.push_back(wr);
            }
            else
            {
                // Only modify a copy of what inside _stack
                for (DialectData _prePart : _stack)
                {
                    deque<int> appDialectPos(wr.dPos);
                    for (int& p : appDialectPos)
                    {
                        // Recalculate the allowed position for modifier
                        p += (p <= 0 ? 1 : _prePart.word.length());
                    }
                    _prePart.dPos.insert(_prePart.dPos.end(), appDialectPos.begin(), appDialectPos.end());

                    _prePart.word.append(wr.word);

                    _prePart.dialect.insert(_prePart.dialect.end(), wr.dialect.begin(), wr.dialect.end());
                    possibleSeq.push_back(_prePart);
                }
            }
        }
    }
    return possibleSeq;
}

list<CKeySeqGenerator::DialectData> CKeySeqGenerator::_genTone(int tone)
{
    list<DialectData> possibleSeq;
    if (tone < 0 || tone > _helper->getNumOfTone())
    {
        return possibleSeq;
    }

    if (tone != 0 || _TONE_MAX_DEPTH > 1)    // Allow deleting tone
    {
        auto t = deque<wstring>();
        t.push_back(wstring(1, _helper->getToneMark(tone)));
        auto p = deque<int>();
        p.push_back(0);
        possibleSeq.push_back(DialectData(L"", t, p));
    }

    for (int i = 1; i < _TONE_MAX_DEPTH; i++)
    {
        list<DialectData> _stack;   // Using the same technique for elimiting recursive
        _stack.splice(_stack.end(), possibleSeq);
        for (int j = 0; j < _helper->getNumOfTone(); j++)
        {
            if (i == _TONE_MAX_DEPTH-1 && j == 0)   // Not allow the deleting tone be the first tone
            {
                continue;
            }
            for (DialectData _prePart : _stack)
            {
                if (_prePart.dialect.back().back() != _helper->getToneMark(j))
                {
                    _prePart.dialect.back().push_back(_helper->getToneMark(j));
                }
                possibleSeq.push_back(_prePart);
            }
        }
    }

    if (_TONE_MAX_DEPTH > 1)
    {
        for (DialectData& d : possibleSeq)
        {
            reverse(d.dialect.back().begin(), d.dialect.back().end());
        }
    }

    return possibleSeq;
}

list<pair<wstring, wstring>> CKeySeqGenerator::generateSequence(const wstring& cw)
{
    list<pair<wstring, wstring>> re;
    wstring w = cw;
    wstring leadCons = getLeadConsonants(w);
    wstring vowels = w.substr(leadCons.length());
    int tone = CHelper::stripTonemark(vowels);

    // combine the remaining part and tone first
    list<list<DialectData>> lVowelAndTone;
    lVowelAndTone.push_back(analyzeVowels(vowels));
    lVowelAndTone.push_back(_genTone(tone));

    // Then combine the leading consonants with the remaining
    list<list<DialectData>> lComponents;
    lComponents.push_back(analyzeVowels(leadCons));
    lComponents.push_back(_combineChar(lVowelAndTone));

    list<DialectData> lData = _combineChar(lComponents);

    for (auto& seq : lData)
    {
        // Create index array
        deque<int> ind;
        deque<int> dialectCount(seq.dialect.size(), 0);
        int mainWordCount = 0;

        for (unsigned int i = seq.word.length(); i > 0; --i)
        {
            ind.push_back(0);
        }
        for (unsigned int i = 1; i <= seq.dialect.size(); ++i)
        {
            for (unsigned int j = 0; j < seq.dialect[i-1].size(); ++j)
                ind.push_back(i);
        }
        do
        {
            if (re.empty() || !re.back().first.empty())
            {
                re.push_back(make_pair(wstring(), cw));
            }
            wstring& ws = re.back().first;
            ws.reserve(ind.size());
            fill(dialectCount.begin(), dialectCount.end(), 0);
            mainWordCount = 0;
            for (unsigned int i = 0; i < ind.size(); ++i)
            {
                if (ind[i] == 0)
                {
                    ws.push_back(seq.word[mainWordCount++]);
                }
                else
                {
                    if (mainWordCount >= seq.dPos[ind[i]-1])
                    {
                        ws.push_back(seq.dialect[ind[i]-1][dialectCount[ind[i]-1]++]);
                    }
                    else
                    {
                        ws.clear();
                        break;
                    }
                }
            }
        }
        while (next_permutation(ind.begin(), ind.end()));   // Using permutation to get the next sequence
    }
    if (!re.empty() && re.back().first.empty())
    {
        re.pop_back();
    }
    return re;
}
