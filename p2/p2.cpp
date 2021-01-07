/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(nullptr) {}
 * };
 */
struct ListNode {
  int val;
  ListNode *next;
  ListNode(int x) : val(x), next(nullptr) {}
};
class Solution {
public:    
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        ListNode *curr = l2;
        ListNode *prev = nullptr;
        int carry = 0;
        while((l1 && curr) || carry) {
            if(l1 && curr) {
                int sum = curr->val + l1->val + carry;
                carry = sum / 10;
                curr->val = sum % 10;
                l1 = l1->next;
                prev = curr;
                curr = curr->next;
            } else if(l1 && !curr) { // carry
                int sum = l1->val + carry;
                carry = sum / 10;
                prev->next = new ListNode{sum % 10};
                prev = prev->next;
                l1 = l1->next;
            } else if(!l1 && curr) { // carry
                int sum = curr->val + carry;
                carry = sum / 10;
                curr->val = sum % 10;
                prev = curr;
                curr = curr->next;
            } else { // carry
                int sum = carry;
                carry = sum / 10;
                prev->next = new ListNode{sum % 10};
                prev = prev->next;
            }
        }
        while(l1) {
            prev->next = new ListNode{l1->val};
            prev = prev->next;
            l1 = l1->next;
        }
        return l2;
    }
};