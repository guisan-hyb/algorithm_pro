// 链表高频题模板：相交 / 入环 / 回文 / K 个一组翻转
// 全部为额外空间 O(1) 版本（面试推荐）
#define _CRT_SECURE_NO_WARNINGS
#include <cstdlib>
#include <iostream>
using namespace std;

struct ListNode {
    int val;
    ListNode* next;
    ListNode(int v = 0, ListNode* n = nullptr) : val(v), next(n) {}
};

// ---------- 1. 两个无环链表的第一个相交结点 ----------
ListNode* getIntersectionNode(ListNode* a, ListNode* b) {
    if (!a || !b) return nullptr;
    int diff = 0;
    ListNode* pa = a; ListNode* pb = b;
    while (pa->next) { pa = pa->next; diff++; }     // 走到尾并记长度差
    while (pb->next) { pb = pb->next; diff--; }
    if (pa != pb) return nullptr;                    // 尾不同 => 不相交
    ListNode* fast = diff >= 0 ? a : b;
    ListNode* slow = diff >= 0 ? b : a;
    for (int i = 0; i < abs(diff); i++) fast = fast->next;
    while (fast != slow) { fast = fast->next; slow = slow->next; }
    return fast;
}

// ---------- 2. 第一个入环节点（Floyd） ----------
ListNode* detectCycle(ListNode* head) {
    if (!head || !head->next) return nullptr;
    ListNode* slow = head->next;
    ListNode* fast = head->next->next;
    while (fast != slow) {
        if (!fast->next || !fast->next->next) return nullptr;
        fast = fast->next->next;
        slow = slow->next;
    }
    fast = head;                                     // 快指针回到头
    while (fast != slow) { fast = fast->next; slow = slow->next; }
    return fast;
}

// ---------- 3. 回文链表（O(1) 空间，比对后还原） ----------
bool isPalindrome(ListNode* head) {
    if (!head || !head->next) return true;
    ListNode *fast = head, *slow = head;
    while (fast->next && fast->next->next) {         // 慢指针到中点
        fast = fast->next->next;
        slow = slow->next;
    }
    ListNode* prev = nullptr;
    ListNode* cur = slow->next;
    while (cur) { ListNode* nxt = cur->next; cur->next = prev; prev = cur; cur = nxt; }
    bool ok = true;
    ListNode *l = head, *r = prev;
    while (r) {
        if (l->val != r->val) { ok = false; break; }
        l = l->next; r = r->next;
    }
    cur = prev; prev = nullptr;                      // 还原右半部分
    while (cur) { ListNode* nxt = cur->next; cur->next = prev; prev = cur; cur = nxt; }
    slow->next = prev;
    return ok;
}

// ---------- 4. 每 K 个一组翻转 ----------
ListNode* getEnd(ListNode* s, int k) {
    while (s && --k) s = s->next;
    return s;
}
void reverseRange(ListNode* start, ListNode* end) {
    ListNode* nxtGroup = end->next;
    ListNode* prev = nullptr;
    ListNode* cur = start;
    while (cur != nxtGroup) {
        ListNode* nxt = cur->next;
        cur->next = prev;
        prev = cur;
        cur = nxt;
    }
    start->next = nxtGroup;                          // start 已是本段尾
}
ListNode* reverseKGroup(ListNode* head, int k) {
    if (!head || k <= 1) return head;
    ListNode* start = head;
    ListNode* end = getEnd(start, k);
    if (!end) return head;
    head = end;                                      // 第一组决定新头
    reverseRange(start, end);
    ListNode* lastEnd = start;
    while (lastEnd->next) {
        start = lastEnd->next;
        end = getEnd(start, k);
        if (!end) return head;
        reverseRange(start, end);
        lastEnd->next = end;
        lastEnd = start;
    }
    return head;
}

int main() {
    int n, k;
    if (!(std::cin >> n >> k)) return 0;
    ListNode dummy, *tail = &dummy;
    for (int i = 0, x; i < n; i++) {
        (std::cin >> x);
        tail->next = new ListNode(x);
        tail = tail->next;
    }
    std::cout << "palindrome = " << isPalindrome(dummy.next) << "\n";
    ListNode* h = reverseKGroup(dummy.next, k);
    for (ListNode* p = h; p; p = p->next) std::cout << p->val << " ";
    std::cout << "\n";
    return 0;
}
