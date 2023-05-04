#pragma once

#include "BST.h"
#include <iostream>

// 平衡二叉搜索树
template <class T>
class BBST : public BST<T> {
  protected:
    BinNode<T>* leftRotation(BinNode<T>*);
    BinNode<T>* rightRotation(BinNode<T>*);
    BinNode<T>* leftRightRotation(BinNode<T>*);
    BinNode<T>* rightLeftRotation(BinNode<T>*);
    void        inOrder(BinNode<T>*) const;
    void        preOrder(BinNode<T>*) const;
    void        postOrder(BinNode<T>*) const;
    void        visitor(BinNode<T>*) const;
    BinNode<T>* insert(const T&);
    BinNode<T>* remove(BinNode<T>*);
    int         height(BinNode<T>*) const; // 结点高度
    void        updateHeight(BinNode<T>*); // 更新结点高度
  public:
    BinNode<T>* Max() const;
    BinNode<T>* Min() const;
    BinNode<T>* search(const T&);
    bool        erase(const T&);
    BinNode<T>* add(const T&);
    int         Size() const {
        return this->size;
    }; // 返回结点数
    void inOrder() const {
        if (this->root)
            inOrder(this->root);
        else
            std::cout << "Null";
    }; // 中序遍历
    void preOrder() const {
        if (this->root)
            preOrder(this->root);
        else
            std::cout << "Null";
    }; // 前序遍历
    void postOrder() const {
        if (this->root)
            postOrder(this->root);
        else
            std::cout << "Null";
    }; // 后序遍历
};

// 查找最大值结点
template <class T>
BinNode<T>* BBST<T>::Max() const {
    BinNode<T>*p = this->root, *pp = nullptr;
    while (p) {
        pp = p;
        p  = p->right;
    }
    return pp;
}

// 查找最小值结点
template <class T>
BinNode<T>* BBST<T>::Min() const {
    BinNode<T>*p = this->root, *pp = nullptr;
    while (p) {
        pp = p;
        p  = p->left;
    }
    return pp;
}

// 左旋
template <class T>
BinNode<T>* BBST<T>::leftRotation(BinNode<T>* node) {
    BinNode<T>*child = node->right, *parent = node->parent;

    node->right = child->left;
    // 结点不为空, 还要重新关联其父结点
    if (node->right) node->right->parent = node;

    child->left  = node;
    node->parent = child;

    child->parent = parent;

    // 更新结点高度
    updateHeight(node);
    updateHeight(child);

    return child;
}

// 右旋
template <class T>
BinNode<T>* BBST<T>::rightRotation(BinNode<T>* node) {
    BinNode<T>*child = node->left, *parent = node->parent;

    node->left = child->right;
    if (node->left) node->left->parent = node;

    child->right = node;
    node->parent = child;

    child->parent = parent;

    updateHeight(node);
    updateHeight(child);

    return child;
}

// 先左旋后右旋
template <class T>
BinNode<T>* BBST<T>::leftRightRotation(BinNode<T>* node) {
    node->left = leftRotation(node->left);
    return rightRotation(node);
}

// 先右旋后左旋
template <class T>
BinNode<T>* BBST<T>::rightLeftRotation(BinNode<T>* node) {
    node->right = rightRotation(node->right);
    return leftRotation(node);
}

// 中序遍历
template <class T>
void BBST<T>::inOrder(BinNode<T>* node) const {
    if (node) {
        inOrder(node->left);
        visitor(node);
        inOrder(node->right);
    }
}

// 前序遍历
template <class T>
void BBST<T>::preOrder(BinNode<T>* node) const {
    if (node) {
        visitor(node);
        preOrder(node->left);
        preOrder(node->right);
    }
}

// 后序遍历
template <class T>
void BBST<T>::postOrder(BinNode<T>* node) const {
    if (node) {
        postOrder(node->left);
        postOrder(node->right);
        visitor(node);
    }
}

// 查看结点信息
template <class T>
inline void BBST<T>::visitor(BinNode<T>* node) const {
    std::cout << node->data << "(" << node->height << ") ";
}

// 删除
template <class T>
BinNode<T>* BBST<T>::remove(BinNode<T>* x) {
    BinNode<T>*succ = nullptr, *w = x;

    if (w->left == nullptr || w->right == nullptr) { // 单分支结构
        if (w->left == nullptr)
            succ = w->right;
        else
            succ = x->left;

        if (w->isLeftChild()) {
            w->parent->left = succ;
        } else if (w->isRightChild()) {
            w->parent->right = succ;
        } else {
            this->root = succ;
        }
    } else { // 双分支结构
        w = w->right;

        // 遍历至右子树最小值结点
        while (w->left) {
            w = w->left;
        }

        // 交换被删结点与右子树最小值结点的数据
        std::swap(x->data, w->data);

        BinNode<T>* u = w->parent;

        // 实际被删结点的子树交给实际被删结点的父结点
        ((u == x) ? u->right : u->left) = succ = w->right;
    }

    this->hot = w->parent;

    if (succ) succ->parent = this->hot;

    delete w; // 删除最终结点

    return succ; // 返回代替者
}

template <class T>
bool BBST<T>::erase(const T& e) {
    BinNode<T>* x = search(e);
    if (!x) return false; // 目标不存在

    remove(x);
    if (--this->size <= 0) return true;

    // 往上更新父结点高度
    for (BinNode<T>* p = this->hot; p; p = p->parent) {
        updateHeight(p);
    }

    return true;
}

// 获取结点的高度
template <class T>
int BBST<T>::height(BinNode<T>* node) const {
    if (node)
        return node->height;
    else
        return 0;
}

// 更新结点高度
template <class T>
void BBST<T>::updateHeight(BinNode<T>* node) {
    int h1 = height(node->left), h2 = height(node->right);
    node->height = h1 > h2 ? h1 + 1 : h2 + 1;
};

// 搜索
template <class T>
BinNode<T>* BBST<T>::search(const T& e) {
    BinNode<T>* p = this->root;
    this->hot     = nullptr;

    while (p) {
        if (e == p->data) {
            return p;
        } else {
            this->hot = p;
            if (e < p->data) {
                p = p->left;
            } else if (e > p->data) {
                p = p->right;
            }
        }
    }
    // 无匹配对象
    return nullptr;
}

// 插入
template <class T>
BinNode<T>* BBST<T>::add(const T& e) {
    BinNode<T>* x = search(e);
    if (x) return x; // 如果结点存在, 返回该结点

    // 结点不存在, 插入新值
    x = insert(e);

    // 向上更新结点 x 的所有祖先的高度
    for (BinNode<T>* p = x->parent; p; p = p->parent) {
        updateHeight(p);
    }

    return x;
}

template <class T>
BinNode<T>* BBST<T>::insert(const T& e) {
    BinNode<T>* x = new BinNode<T>(e, this->hot);

    if (!x->isRoot()) {
        if (x->data > this->hot->data) {
            this->hot->right = x;
        } else {
            this->hot->left = x;
        }
    } else {
        this->root = x;
    }

    this->size++;

    return x;
}