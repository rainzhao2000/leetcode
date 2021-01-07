#include <unordered_map>
#include <string>
using namespace std;
class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        int maxLen = s.size() == 1 ? 1 : 0;
        std::unordered_map<char, int> usedChars{};
        int n = s.size();
        int lo = 0, hi = 0;
        for(; hi < n; ++hi) {
            char c = s.at(hi);
            if(usedChars.find(c) != usedChars.end()) { // found duplicate
                int len = hi - lo;
                if(len > maxLen) {
                    maxLen = len;
                }
                for(;s.at(lo) != c; ++lo) {
                    usedChars.erase(s.at(lo));
                }
                ++lo;
            } else {
                usedChars.emplace(c, hi);
            }
        }
        int lastLen = hi - lo;
        if(lastLen > maxLen) {
            return lastLen;
        }
        return maxLen;
    }
};