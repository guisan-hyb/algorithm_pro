// 链表四件套模板：反转 / 合并 / 相加 / 划分
// 统一使用"哨兵节点 dummy"，避免头结点特判
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;

struct ListNode {
    int val;
    ListNode* next;
    ListNode(int v = 0) : val(v), next(nullptr) {}
};

// ---------- 1. 反转单链表 ----------
ListNode* reverseList(ListNode* head) {
    ListNode* prev = nullptr;
    while (head) {
        ListNode* nxt = head->next;    // 先存后继！
        head->next = prev;
        prev = head;
        head = nxt;
    }
    return prev;
}

// ---------- 2. 合并两个有序链表 ----------
ListNode* mergeTwoLists(ListNode* a, ListNode* b) {
    ListNode dummy, *tail = &dummy;
    while (a && b) {
        if (a->val <= b->val) { tail->next = a; a = a->next; }
        else                  { tail->next = b; b = b->next; }
        tail = tail->next;
    }
    tail->next = a ? a : b;
    return dummy.next;
}

// ---------- 3. 两数相加（个位在链头） ----------
ListNode* addTwoNumbers(ListNode* a, ListNode* b) {
    ListNode dummy, *tail = &dummy;
    int carry = 0;
    while (a || b || carry) {                    // carry 也要考虑
        int sum = carry + (a ? a->val : 0) + (b ? b->val : 0);
        tail->next = new ListNode(sum % 10);
        tail = tail->next;
        carry = sum / 10;
        a = a ? a->next : nullptr;
        b = b ? b->next : nullptr;
    }
    return dummy.next;
}

// ---------- 4. 划分链表：< x 在前，>= x 在后 ----------
ListNode* partition(ListNode* head, int x) {
    ListNode lh, rh, *lt = &lh, *rt = &rh;
    while (head) {
        ListNode* nxt = head->next;
        head->next = nullptr;                    // 先断链
        if (head->val < x) { lt->next = head; lt = head; }
        else               { rt->next = head; rt = head; }
        head = nxt;
    }
    lt->next = rh.next;                          // 拼接两段
    return lh.next;
}

// ---------- 简单自测：建链表 -> 反转 -> 打印 ----------
int main() {
    int n;
    if (!(std::cin >> n)) return 0;
    ListNode dummy, *tail = &dummy;
    for (int i = 0, x; i < n; i++) {
        (std::cin >> x);
        tail->next = new ListNode(x);
        tail = tail->next;
    }
    ListNode* h = reverseList(dummy.next);
    for (ListNode* p = h; p; p = p->next) std::cout << p->val << " ";
    std::cout << "\n";
    return 0;
}
