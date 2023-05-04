#pragma once

#include <iostream>
#include <vector>

// B 树结点
template <class T>
struct BTNode {
    BTNode<T>*              parent;
    std::vector<T>          key; // 关键码
    std::vector<BTNode<T>*> child;
    BTNode() {
        parent = nullptr;
        child.push_back(nullptr);
    };
    BTNode(T e, BTNode<T>* left = nullptr, BTNode<T>* right = nullptr);
    int search(const T& e) const;
};

template <class T>
inline BTNode<T>::BTNode(T e, BTNode<T>* left, BTNode<T>* right) {
    parent = nullptr;
    key.push_back(e);
    child.push_back(left);
    child.push_back(right);
    if (left) left->parent = this;
    if (right) right->parent = this;
}

// 在关键码向量中顺序查找元素
template <class T>
int BTNode<T>::search(const T& e) const {
    for (int i = 0; i < key.size(); i++) {
        // 匹配就返回其索引
        // 不匹配返回最后一个小于 e 的索引
        if (e == key[i])
            return i;
        else if (e < key[i])
            return i - 1;
    }

    // e 比所有关键码都大
    // 返回最后一个值的索引
    return key.size() - 1;
}

// B 树
template <class T>
class BTree {
    int        size;  // 关键码数
    int        order; // 阶
    BTNode<T>* root;
    BTNode<T>* hot;
    void       solveOverFlow(BTNode<T>*);
    void       solveUnderFlow(BTNode<T>*);
    void       ASC(BTNode<T>* node) const;
    void       inOrder(BTNode<T>*) const;
    void       preOrder(BTNode<T>*) const;
    void       postOrder(BTNode<T>*) const;

  public:
    BTree(int o) : order(o), size(0), hot(nullptr) {
        root = new BTNode<T>();
    };
    BTNode<T>* search(const T&);
    bool       add(const T&);
    bool       erase(const T&);
    void       ASC() const {
        if (root->key.size())
            ASC(root);
        else
            std::cout << "Null";
    };
    void inOrder() const {
        if (root->key.size())
            inOrder(root);
        else
            std::cout << "Null";
    };
    void preOrder() const {
        if (root->key.size())
            preOrder(root);
        else
            std::cout << "Null";
    };
    void postOrder() const {
        if (root->key.size())
            postOrder(root);
        else
            std::cout << "Null";
    };
};

// 上溢分裂
template <class T>
void BTree<T>::solveOverFlow(BTNode<T>* node) {
    if (node->child.size() > order) { // 发生上溢
        int mid = node->key.size() / 2;

        // 分裂出右子树
        BTNode<T>* right = new BTNode<T>();
        int        len   = node->key.size() - mid - 1; // 右子树 key 的个数
        for (int i = 0; i < len; i++) {
            right->key.insert(right->key.begin() + i, node->key[mid + 1]);
            node->key.erase(node->key.begin() + (mid + 1)); // 插入右子树的同时删掉这个元素
            right->child.insert(right->child.begin() + i, node->child[mid + 1]);
            node->child.erase(node->child.begin() + (mid + 1));
        }
        // child 比 key 多一个(注意这里不是插入, 而是替换, new 的时候已经有一个子结点)
        right->child[len] = node->child[mid + 1];
        node->child.erase(node->child.begin() + (mid + 1));

        // 统一右子树的父结点
        if (right->child[0]) {
            // right 非叶结点其子结点才有父结点
            for (int j = 0; j < right->child.size(); j++) {
                right->child[j]->parent = right;
            }
        }

        BTNode<T>* p = node->parent;
        if (!p) {
            // 如果当前结点是根结点
            // 新建一个新结点作为新根结点
            root = p     = new BTNode<T>();
            p->child[0]  = node;
            node->parent = p;
        }

        int r = p->search(node->key[mid]);
        p->key.insert(p->key.begin() + (r + 1), node->key[mid]);
        // 不要忘了删除 mid 元素
        node->key.erase(node->key.begin() + mid);
        p->child.insert(p->child.begin() + (r + 2), right);
        // 关联右子树
        right->parent = p;

        // 迭代上溢检测父结点
        solveOverFlow(p);
    }
}

// 下溢合并
template <class T>
void BTree<T>::solveUnderFlow(BTNode<T>* node) {
    if ((order + 1) / 2 > node->child.size()) { // 发生下溢

        BTNode<T>* p = node->parent;
        if (!p) { // 若 node 是根结点
            if (!node->key.size() && node->child[0]) {
                // 并且结点数为空, 但是其存在非空孩子
                // root 指向其非空孩子
                root         = node->child[0];
                root->parent = nullptr;
                delete node;
            }
            return;
        }

        // node 是 p 的第 r 个孩子
        int r = 0;
        while (p->child[r] != node) {
            r++;
        }

        // 旋转
        if (r > 0) {                         // node 非第一个孩子
            BTNode<T>* ls = p->child[r - 1]; // 先看左兄弟

            if ((order + 1) / 2 < ls->child.size()) { // 左兄弟少一个不会下溢
                // 向父结点借一个
                node->key.insert(node->key.begin(), p->key[r - 1]);
                // 兄弟结点最大值给父结点
                p->key[r - 1] = ls->key.back();
                ls->key.pop_back();
                // 兄弟最后一个孩子过继给 node
                node->child.insert(node->child.begin(), ls->child.back());
                ls->child.pop_back(); // 不要忘了删除左兄弟最后一个孩子
                // 如果孩子不为 null, 还要修改其父亲
                if (node->child[0]) node->child[0]->parent = node;
                return;
            }
        }

        if (r < p->child.size() - 1) {
            BTNode<T>* rs = p->child[r + 1]; // 看看右兄弟

            if ((order + 1) / 2 < rs->child.size()) {
                // 往 node 的最后位置插入父结点值
                node->key.push_back(p->key[r]);
                // 右兄弟将第一个值给父结点
                p->key[r] = rs->key[0];
                // 删除右兄弟第一个结点
                rs->key.erase(rs->key.begin());
                // 将右兄弟的第一个孩子过继给 node
                node->child.push_back(rs->child[0]);
                rs->child.erase(rs->child.begin()); // 不要忘了把右兄弟第一个孩子删除
                if (node->child.back()) node->child.back()->parent = node;
                return;
            }
        }

        // 合并
        if (r > 0) {
            BTNode<T>* ls = p->child[r - 1];
            // 左兄弟加入粘合剂
            ls->key.push_back(p->key[r - 1]);
            // 将作为粘合剂的父结点的关键码从父结点中移除
            p->key.erase(p->key.begin() + (r - 1));
            // 将 node 从 p.child 中移除
            p->child.erase(p->child.begin() + r);

            // 因为 child 总是比 key 多一个, 为配合 while 写出来一个
            ls->child.push_back(node->child[0]);
            node->child.erase(node->child.begin());
            if (ls->child.back()) {
                ls->child.back()->parent = ls;
            }
            // 将 node 剩余孩子逐步过继给左兄弟
            while (!node->key.empty()) {
                ls->key.push_back(node->key[0]);
                node->key.erase(node->key.begin());
                ls->child.push_back(node->child[0]);
                node->child.erase(node->child.begin());
                // 如果孩子不为 null, 其父亲改成左兄弟
                if (ls->child.back()) {
                    ls->child.back()->parent = ls;
                }
            }
            delete node;
        } else {
            BTNode<T>* rs = p->child[r + 1];
            node->key.push_back(p->key[r]);
            p->key.erase(p->key.begin() + r);
            // 将右兄弟从父结点中删除
            p->child.erase(p->child.begin() + (r + 1));

            // 将右兄弟的孩子过继给 node
            node->child.push_back(rs->child[0]);
            rs->child.erase(rs->child.begin());
            if (node->child.back()) node->child.back()->parent = node;
            while (!rs->key.empty()) {
                node->key.push_back(rs->key[0]);
                rs->key.erase(rs->key.begin());
                node->child.push_back(rs->child[0]);
                rs->child.erase(rs->child.begin());
                if (node->child.back()) node->child.back()->parent = node;
            }
            delete rs;
        }
        // p 删掉一个结点后可能出现下溢
        solveUnderFlow(p);
    }
}

// 升序输出结点
template <class T>
void BTree<T>::ASC(BTNode<T>* node) const {
    if (node) {
        int i = 0;
        for (; i < node->key.size(); i++) {
            ASC(node->child[i]);
            std::cout << node->key[i] << " ";
        }
        ASC(node->child[i]);
    }
}

// 中序遍历
template <class T>
void BTree<T>::inOrder(BTNode<T>* node) const {
    if (node) {
        inOrder(node->child[0]);
        std::cout << "[";
        int i = 0;
        for (; i < node->key.size() - 1; i++) {
            std::cout << node->key[i] << " ";
        }
        std::cout << node->key[i] << "] ";
        for (int j = 1; j < node->child.size(); j++) {
            inOrder(node->child[j]);
        }
    }
}

// 前序遍历
template <class T>
void BTree<T>::preOrder(BTNode<T>* node) const {
    if (node) {
        std::cout << "[";
        int i = 0;
        for (; i < node->key.size() - 1; i++) {
            std::cout << node->key[i] << " ";
        }
        std::cout << node->key[i] << "] ";
        for (int j = 0; j < node->child.size(); j++) {
            preOrder(node->child[j]);
        }
    }
}

// 后序遍历
template <class T>
void BTree<T>::postOrder(BTNode<T>* node) const {
    if (node) {
        for (int j = 0; j < node->child.size(); j++) {
            preOrder(node->child[j]);
        }
        std::cout << "[";
        int i = 0;
        for (; i < node->key.size() - 1; i++) {
            std::cout << node->key[i] << " ";
        }
        std::cout << node->key[i] << "] ";
    }
}

// 搜索元素
template <class T>
BTNode<T>* BTree<T>::search(const T& e) {
    BTNode<T>* v = root;
    hot          = nullptr;

    while (v) {
        int r = v->search(e);
        if (r >= 0 && e == v->key[r]) return v; // 找到元素

        // hot 保存 e 的父结点(当 e 在树中)
        // 或者 e 所应该在的结点(当 e 不在树中)
        // 即 v->child[r] == null, hot 指向叶结点
        hot = v;
        v   = v->child[r + 1];
    }

    return nullptr;
}

// 加入结点
template <class T>
bool BTree<T>::add(const T& e) {
    BTNode<T>* v = search(e);
    if (v) return false; // 结点重复, 插入失败

    int r = hot->search(e);
    hot->key.insert(hot->key.begin() + (r + 1), e);
    hot->child.insert(hot->child.begin() + (r + 2), nullptr);

    size++;
    solveOverFlow(hot); // 上溢分裂
    return true;
}

// 删除结点
template <class T>
bool BTree<T>::erase(const T& e) {
    BTNode<T>* v = search(e); // 查找被删元素
    if (!v) return false;

    int r = v->search(e); // 这个位置肯定是精确的
    // std::cout << "找到被删元素" << e << "位置" << r << "元素父节点" << v->parent << "\n";

    if (v->child[0]) {
        // v 非叶结点
        BTNode<T>* u = v->child[r + 1];
        while (u->child[0]) {
            u = u->child[0];
        }
        v->key[r] = u->key[0];
        v         = u;
        r         = 0;
    }

    v->key.erase(v->key.begin() + r);
    // 删哪个都可以, 叶结点的 child 都是 nullptr
    v->child.erase(v->child.begin() + r);
    size--;

    solveUnderFlow(v); // 下溢合并

    return true;
}