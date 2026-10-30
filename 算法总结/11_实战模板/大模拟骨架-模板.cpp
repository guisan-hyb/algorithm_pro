#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
// 大模拟（CSP T3）骨架模板
// 目标：65 分钟内写出"能编译、能过样例、规则不漏"的版本；先拿 60 分，再谈满分。
//
// 使用流程（读题时就照这个顺序做）：
// 1. 把题面规则抄成编号 checklist，每条规则后面留一个空括号；
// 2. 先定实体（struct / class）和容器（vector / map / queue / set），再写逻辑；
// 3. 分层实现：输入解析 -> 状态维护 -> 单条规则 -> 输出；
// 4. 手算两组边界数据（空输入、单元素、极值、多组数据）；
// 5. 用暴力版对拍（见 11_实战模板/暴力保底-模板.cpp）。
//
// 边界表（每题至少过一遍）：
// [ ] n = 0 / 1        [ ] 同一时刻多个事件   [ ] 时间边界（闭/开区间）
// [ ] 空行 / 多空格     [ ] 输出末尾空格/换行  [ ] 多组数据是否要清空全局状态
// [ ] 数值溢出          [ ] 队列/栈为空时操作  [ ] 规则优先级
using namespace std;

struct Entity {
    int id = 0;
    int hp = 0;
    int attack = 0;
    bool alive = true;
};

class Simulator {
public:
    void run() {
        int queryCount = 0;
        cin >> queryCount;
        for (int i = 0; i < queryCount; i++) {
            string command;
            cin >> command;
            if (command == "ADD") {
                add();
            } else if (command == "HIT") {
                hit();
            } else if (command == "QUERY") {
                query();
            }
        }
    }

private:
    vector<Entity> entities;
    unordered_map<int, int> idToIndex;
    int nextId = 1;

    void add() {
        int hp = 0;
        int attack = 0;
        cin >> hp >> attack;

        Entity entity;
        entity.id = nextId++;
        entity.hp = hp;
        entity.attack = attack;
        idToIndex[entity.id] = static_cast<int>(entities.size());
        entities.push_back(entity);
    }

    void hit() {
        int attackerId = 0;
        int targetId = 0;
        cin >> attackerId >> targetId;

        auto attackerIt = idToIndex.find(attackerId);
        auto targetIt = idToIndex.find(targetId);
        if (attackerIt == idToIndex.end() || targetIt == idToIndex.end()) {
            return;
        }

        Entity& attacker = entities[attackerIt->second];
        Entity& target = entities[targetIt->second];
        if (!attacker.alive || !target.alive) {
            return;
        }

        target.hp -= attacker.attack;
        if (target.hp <= 0) {
            target.alive = false;
        }
    }

    void query() {
        int aliveCount = 0;
        for (const Entity& entity : entities) {
            if (entity.alive) {
                aliveCount++;
            }
        }
        cout << aliveCount << '\n';
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int testCount = 1;
    cin >> testCount;
    while (testCount--) {
        Simulator simulator;
        simulator.run();
    }
    return 0;
}
