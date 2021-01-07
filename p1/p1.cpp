#include <vector>
#include <unordered_map>
using namespace std;
typedef std::unordered_map<int, int> Map;
class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        Map map;
        int size = nums.size();
        for(int i = 0; i < size; ++i) {
            int comp = target - nums.at(i);
            if(map.find(comp) != map.end()) {
                return {i, map.at(comp)};
            }
            map.emplace(nums.at(i), i);
        }
        throw "failed";
    }
};