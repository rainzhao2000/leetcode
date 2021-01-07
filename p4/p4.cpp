#include <vector>
using namespace std;
class Solution {
public:
    double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
        int len = nums1.size() + nums2.size();
        int n = len / 2;
        int prev;
        int i = 0, j = 0;
        for(int k = 0; k < n; ++k) {
            if(i >= nums1.size()) {
                prev = nums2.at(j++);
            } else if(j >= nums2.size()) {
                prev = nums1.at(i++);
            } else if(nums1.at(i) > nums2.at(j)) {
                prev = nums2.at(j++);
            } else {
                prev = nums1.at(i++);
            }
        }
        int curr;
        if(i >= nums1.size()) {
            curr = nums2.at(j);
        } else if(j >= nums2.size()) {
            curr = nums1.at(i);
        } else {
            curr = nums1.at(i) > nums2.at(j) ? nums2.at(j) : nums1.at(i);
        }
        return len % 2 == 0 ? (curr + prev) / 2.0 : curr;
    }
};