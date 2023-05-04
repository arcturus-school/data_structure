#pragma once

#include <algorithm>
#include <random>
#include <vector>

// 生成随机序列
class Random {
    int              N;
    std::vector<int> random_list;        // 随机序列
    std::vector<int> increase_list;      // 随机序列的递增序列
    std::vector<int> decrease_list;      // 随机序列的递减序列
    std::vector<int> random_search_list; // 随机查找序列
    std::vector<int> random_delete_list; // 随机删除序列
  public:
    Random(int n = 1000) : N(n) {
        createRandomList();
        createIncreaseRandomList();
        createDecreaseRandomList();
    };

    std::vector<int>& getRandomList() {
        return random_list;
    }

    std::vector<int>& getIncreaseRandomList() {
        return increase_list;
    }

    std::vector<int>& getDecreaseRandomList() {
        return decrease_list;
    }

    // 生成范围为 0~1000000 的随机序列
    void createRandomList() {
        random_list.resize(N);
        std::default_random_engine         e;
        std::uniform_int_distribution<int> u(0, 1000000);
        for (int i = 0; i < random_list.size(); i++) {
            random_list[i] = u(e);
        }
    }

    // 递增序列
    void createIncreaseRandomList() {
        increase_list = random_list;
        // 对 increase_list 进行递增排序
        sort(increase_list.begin(), increase_list.end());
    }

    // 递减序列
    void createDecreaseRandomList() {
        decrease_list = increase_list;
        reverse(decrease_list.begin(), decrease_list.end());
    }

    // 随机获取 1000 个元素的查找序列
    std::vector<int>& getRandomSearchList(int num = 1000) {
        random_search_list.resize(num);

        std::default_random_engine         e;
        std::uniform_int_distribution<int> u(0, N - 1);
        for (int i = 0; i < random_search_list.size(); i++) {
            // 读取随机的下标
            random_search_list[i] = random_list[u(e)];
        }
        return random_search_list;
    }

    // 随机删除序列
    std::vector<int>& getRandomDeleteList() {
        random_delete_list = random_list;

        std::default_random_engine         e;
        std::uniform_int_distribution<int> u(0, random_delete_list.size() - 1);
        for (int i = 0; i < random_delete_list.size(); i++) {
            // 与随机下标进行交换
            std::swap(random_delete_list[i], random_delete_list[u(e)]);
        }
        return random_delete_list;
    }
};