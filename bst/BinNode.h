#pragma once

// 结点颜色
enum class Color { RED, BLACK };

// 结点类
template <class T>
struct BinNode {
    T           data;
    BinNode<T>* parent;      // 父结点
    BinNode<T>* left;        // 左结点
    BinNode<T>* right;       // 右结点
    int         height;      // 高度
    int         blackHeight; // 黑高度
    Color       color;       // 结点颜色

    // 构造函数
    BinNode()
        : parent(nullptr), left(nullptr), right(nullptr), height(1), blackHeight(0),
          color(Color::RED){};
    BinNode(T e, BinNode<T>* p = nullptr, int h = 1, BinNode<T>* l = nullptr,
        BinNode<T>* r = nullptr, Color c = Color::RED)
        : data(e), parent(p), left(l), right(r), height(h), color(c), blackHeight(0){};

    // 是否是根结点
    bool isRoot() const {
        return parent == nullptr;
    };

    // 是否是左孩子
    // 首先如果是根结点, 就不是什么左孩子
    // 其次不是其父结点的右孩子
    bool isLeftChild() {
        return !isRoot() && this == parent->left;
    };

    // 是否是右孩子
    bool isRightChild() {
        return !isRoot() && this == parent->right;
    };

    // 获取叔结点
    BinNode<T>* uncle() const;
};

template <class T>
BinNode<T>* BinNode<T>::uncle() const {
    if (isRoot()) {
        return nullptr;
    } // 根结点不会有叔结点
    else if (parent->isLeftChild()) {
        // 如果其父结点是其祖先的左孩子
        // 返回其祖先的右孩子
        return parent->parent->right;
    } else if (parent->isRightChild()) {
        return parent->parent->left;
    } else {
        // 父结点是根结点, 没有叔结点
        return nullptr;
    }
}