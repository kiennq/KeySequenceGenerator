#include "stdafx.h"
#include "KeySeqGenerator.h"
#include <ppl.h>

using namespace std;
using namespace concurrency;

KeySeqGenerator::KeySeqGenerator(const Helper::InputMode& _mode, const int scanDepth, const int toneDepth)
	: _mode(_mode)
    , _VOWEL_SCAN_MAX_DEPTH(scanDepth)
    , _TONE_MAX_DEPTH(toneDepth)
{
}


KeySeqGenerator::~KeySeqGenerator(void)
{
}

wstring KeySeqGenerator::getLeadConsonants(const wstring& w)
{
    unsigned int size = w.length() < 3 ? w.length() : 3;
    wstring&& lower = Helper::tolower(w);
    lower.erase(size);
    for (int k = size; k > 0; k--, lower.pop_back()) {
        if (Helper::isLeadCons(lower)) return w.substr(0, k);
	}
    return wstring();
}

wstring KeySeqGenerator::getLeadVowels(std::wstring& w)
{
    wstring vowels;
    for (wchar_t c : w) {
        if (!Helper::isConsonant(c)) vowels.push_back(c);
		else return vowels;
	}
    return vowels;
}

list<KeySeqGenerator::DialectData> KeySeqGenerator::analyzeVowels(wstring& w, int depth)
{
    if (depth >= _VOWEL_SCAN_MAX_DEPTH) return list<KeySeqGenerator::DialectData>();
    list<list<DialectData>> componentSeq;
	list<wchar_t> remainStr;
    for (unsigned i = w.length(); i > 0; i--, remainStr.push_front(w.back()), w.pop_back()) {
		list<Helper::DialectDict> curSeq;
		if (Helper::getDialectSeq(w, curSeq, _mode)){
			componentSeq.push_back(list<DialectData>());
			list<DialectData>& i = componentSeq.back();
            for (Helper::DialectDict& d : curSeq) {
                // If consisted of normal char, add to the list imediately
                if (Helper::consistOfNormalChar(d._src)) {
                    deque<int> dPos;
                    deque<wstring> dMod;
                    if (d._modifier) {
                        dPos.push_back(d._modPos);
                        dMod.push_back(wstring(1, d._modifier));
					}
                    i.push_back(DialectData(d._src, dMod, dPos));
				} 
				// else analyze again
				else {
                    auto&& depthAnalyze = analyzeVowels(d._src, depth+1);
                    for (DialectData& dd : depthAnalyze) {
                        if (dd.dialect.empty() && d._modifier) {
							dd.dialect.push_back(wstring(1, d._modifier));
                            dd.dPos.push_back(d._modPos);
						} else {
                            dd.dialect.back().push_back(d._modifier);
						}
                        i.push_back(dd);
					}
				}
			}
            if (i.empty()) return list<DialectData>();
            break;
		} else if (w.length() == 1){
			componentSeq.push_back(list<DialectData>());
			list<DialectData>& i = componentSeq.back();
            i.push_back(DialectData(w, deque<wstring>(), deque<int>()));
            break;
		}
	}
	for (wchar_t& r : remainStr) {
		componentSeq.push_back(list<DialectData>());
		list<DialectData>& rf = componentSeq.back();
		rf.splice(rf.end(), analyzeVowels(wstring(1, r), depth));
		if (rf.empty()) return list<DialectData>();
	}
    return _combineChar(componentSeq);
}

list<KeySeqGenerator::DialectData> KeySeqGenerator::_combineChar(list<list<KeySeqGenerator::DialectData>>& l)
{
    list<DialectData> possibleSeq;
    if (l.size() == 1) return l.front();
    for (list<DialectData>& lwr : l) {
        list<DialectData> _stack;
        // Empty possibleSeq and add everything to a temporary _stack
        _stack.splice(_stack.begin(), possibleSeq);
        for (DialectData& wr : lwr) {
            if (_stack.empty()) {
                // Add imediately
                possibleSeq.push_back(wr);
			} else {
                // Only modify a copy of what inside _stack
				for (DialectData _prePart : _stack) {
					deque<int> appDialectPos(wr.dPos);
					for (int& p : appDialectPos) p += p<=0?1:_prePart.word.length();
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

list<KeySeqGenerator::DialectData> KeySeqGenerator::_genTone(int tone)
{
    list<DialectData> possibleSeq;
    if (tone < 0 || tone > 5) return possibleSeq;

	if (tone || _TONE_MAX_DEPTH > 1) {
		auto t = deque<wstring>();
		t.push_back(wstring(1, Helper::toneMark[tone][_mode]));
		auto p = deque<int>();
		p.push_back(0);
		possibleSeq.push_back(DialectData(L"", t, p)); 
	}

    for (int i = 1; i < _TONE_MAX_DEPTH; i++) {
        list<DialectData> _stack;
        _stack.splice(_stack.end(), possibleSeq);
        for (int j = 0; j < 6; j++) {
            if (i == _TONE_MAX_DEPTH-1 && !j) continue;
            for (DialectData _prePart : _stack) {
                if (_prePart.dialect.back().back() != Helper::toneMark[j][_mode])
					_prePart.dialect.back().push_back(Helper::toneMark[j][_mode]);
                possibleSeq.push_back(_prePart);
			}
		}
	}

    if (_TONE_MAX_DEPTH > 1)
		for (DialectData& d : possibleSeq) {
			reverse(d.dialect.back().begin(), d.dialect.back().end());
		}

    return possibleSeq;
}

list<pair<wstring, wstring>> KeySeqGenerator::generateSequence(const wstring& cw)
{
    list<pair<wstring, wstring>> re;
    wstring w = cw; 
    wstring leadCons = getLeadConsonants(w);
    wstring vowels = w.substr(leadCons.length());
    int tone = Helper::stripTonemark(vowels);

    // combine
    list<list<DialectData>> lVowelAndTone;
    lVowelAndTone.push_back(analyzeVowels(vowels));
    lVowelAndTone.push_back(_genTone(tone));

    list<list<DialectData>> lComponents;
    lComponents.push_back(analyzeVowels(leadCons));
    lComponents.push_back(_combineChar(lVowelAndTone));
    // add tone mark, anywhere after first vowel
    /*if (tone || _VOWEL_SCAN_MAX_DEPTH > 1) {
        for (DialectData& d : lComponents.back()) {
            d.dialect.push_back(wstring(1, Helper::toneMark[tone][_mode]));
            d.dPos.push_back(1);
		}
	}*/

    list<DialectData> lData = _combineChar(lComponents);

	for (auto& seq : lData) {
		// Create index array
		deque<int> ind;
        deque<int> dialectCount(seq.dialect.size(), 0);
		int mainWordCount = 0;

		for (unsigned int i = seq.word.length(); i > 0; --i)  ind.push_back(0);
		for (unsigned int i = 1; i <= seq.dialect.size(); ++i){
            for (unsigned int j = 0; j < seq.dialect[i-1].size(); ++j)
				ind.push_back(i);
		}
		do {
            if (re.empty() || !re.back().first.empty())
			    re.push_back(make_pair(wstring(), cw));
			// Test 
			wstring& ws = re.back().first;
			ws.reserve(ind.size());
            fill(dialectCount.begin(), dialectCount.end(), 0);
            mainWordCount = 0;
			for (unsigned int i = 0; i < ind.size(); ++i) {
				if (ind[i] == 0) {
					ws.push_back(seq.word[mainWordCount++]);
				} else {
                    if (mainWordCount >= seq.dPos[ind[i]-1]) {
					    ws.push_back(seq.dialect[ind[i]-1][dialectCount[ind[i]-1]++]);
					} else {
                        ws.clear();
                        break;
					}
				}
			}
		} while (next_permutation(ind.begin(), ind.end()));
	}
    if (!re.empty() && re.back().first.empty()) re.pop_back();
    return re;
}