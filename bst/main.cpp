#include "AVLTree.h"
#include "BTree.h"
#include "RBTree.h"
#include "Random.h"
#include "funcTime.h"
#include <iostream>
#include <vector>

// AVL 树测试
void avlTreeTest(
    std::vector<int>& add_list, std::vector<int>& search_list, std::vector<int>& delete_list) {
    AVLTree<int>    avl;
    static FuncTime ft;
    ft.funcStart();

    // 插入
    for (int i = 0; i < add_list.size(); i++) {
        avl.add(add_list[i]);
    }

    ft.funcEnd();
    double t1 = ft.funcRunTime();

    ft.funcStart();

    // 查找
    for (int i = 0; i < search_list.size(); i++) {
        avl.search(search_list[i]);
    }

    ft.funcEnd();
    double t2 = ft.funcRunTime();

    ft.funcStart();

    // 删除
    for (int i = 0; i < delete_list.size(); i++) {
        avl.erase(delete_list[i]);
    }

    ft.funcEnd();
    double t3 = ft.funcRunTime();

    std::cout << t1 << "," << t2 << "," << t3 << ",";
}

// B-树测试
void bTreeTest(
    std::vector<int>& add_list, std::vector<int>& search_list, std::vector<int>& delete_list) {
    BTree<int>      b(512);
    static FuncTime ft;
    ft.funcStart();

    // 插入
    for (int i = 0; i < add_list.size(); i++) {
        b.add(add_list[i]);
    }

    ft.funcEnd();
    double t1 = ft.funcRunTime();

    ft.funcStart();

    // 查找
    for (int i = 0; i < search_list.size(); i++) {
        b.search(search_list[i]);
    }

    ft.funcEnd();
    double t2 = ft.funcRunTime();

    ft.funcStart();

    // 删除
    for (int i = 0; i < delete_list.size(); i++) {
        b.erase(delete_list[i]);
    }

    ft.funcEnd();
    double t3 = ft.funcRunTime();

    std::cout << t1 << "," << t2 << "," << t3 << ",";
}

// 红黑树测试
void rbTreeTest(
    std::vector<int>& add_list, std::vector<int>& search_list, std::vector<int>& delete_list) {
    RedBlack<int> rb;

    static FuncTime ft;
    ft.funcStart();

    // 插入
    for (int i = 0; i < add_list.size(); i++) {
        rb.add(add_list[i]);
    }

    ft.funcEnd();
    double t1 = ft.funcRunTime();

    ft.funcStart();

    // 查找
    for (int i = 0; i < search_list.size(); i++) {
        rb.search(search_list[i]);
    }

    ft.funcEnd();
    double t2 = ft.funcRunTime();

    ft.funcStart();

    // 删除
    for (int i = 0; i < delete_list.size(); i++) {
        rb.erase(delete_list[i]);
    }

    ft.funcEnd();
    double t3 = ft.funcRunTime();

    std::cout << t1 << "," << t2 << "," << t3 << " ";
}

void test() {
    std::cout << "1.按递增顺序插入N个整数, 随机查找1000个数, 按相同顺序删除\n";

    for (int N = 10000; N <= 500000; N += 10000) {
        Random           r(N);
        std::vector<int> add_list    = r.getIncreaseRandomList();
        std::vector<int> search_list = r.getRandomSearchList();

        // avl 树
        avlTreeTest(add_list, search_list, add_list);
        // B- 树
        bTreeTest(add_list, search_list, add_list);
        // 红黑树
        rbTreeTest(add_list, search_list, add_list);

        std::cout << "\n";
    }

    std::cout << "2.按递增顺序插入N个整数, 随机查找1000个数, 按相反顺序删除\n";

    for (int N = 10000; N <= 500000; N += 10000) {
        Random           r(N);
        std::vector<int> add_list    = r.getIncreaseRandomList();
        std::vector<int> search_list = r.getRandomSearchList();
        std::vector<int> delete_list = r.getDecreaseRandomList();

        // avl 树
        avlTreeTest(add_list, search_list, delete_list);
        // B- 树
        bTreeTest(add_list, search_list, delete_list);
        // 红黑树
        rbTreeTest(add_list, search_list, delete_list);

        std::cout << "\n";
    }

    std::cout << "3.按随机顺序插入N个整数, 随机查找1000个数, 按随机顺序删除\n";

    for (int N = 10000; N <= 500000; N += 10000) {
        Random           r(N);
        std::vector<int> add_list    = r.getRandomList();
        std::vector<int> search_list = r.getRandomSearchList();
        std::vector<int> delete_list = r.getRandomDeleteList();

        // avl 树
        avlTreeTest(add_list, search_list, delete_list);
        // B- 树
        bTreeTest(add_list, search_list, delete_list);
        // 红黑树
        rbTreeTest(add_list, search_list, delete_list);

        std::cout << "\n";
    }
}

int main() {
    test();
    return 0;
}