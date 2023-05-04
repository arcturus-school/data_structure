#pragma once

#include "BBST.h"

// AVL 树
template <class T>
class AVLTree : public BBST<T> {
    bool isBalence(BinNode<T>*) const;
    void reBalence(BinNode<T>*);
    int  bf(BinNode<T>* node) const;

  public:
    bool        erase(const T&);
    BinNode<T>* add(const T&);
};

// 结点是否平衡
template <class T>
bool AVLTree<T>::isBalence(BinNode<T>* node) const {
    int delta = bf(node);
    if (delta > -2 && delta < 2)
        return true;
    else
        return false;
}

// 结点重新平衡
template <class T>
void AVLTree<T>::reBalence(BinNode<T>* node) {
    // 先判断 node 是根, 左孩子还是右孩子
    bool left = false, right = false;
    if (node->isLeftChild())
        left = true;
    else if (node->isRightChild())
        right = true;

    BinNode<T>*n = nullptr, *p = node->parent;

    if (bf(node) == 2) { // 平衡因子为 2
        if (this->height(node->left->left) >= this->height(node->left->right)) {
            // node 左孩子的左子树更高, 进行右旋
            n = this->rightRotation(node);
        } else {
            // 先左旋后右旋
            n = this->leftRightRotation(node);
        }
    } else {
        if (this->height(node->right->left) <= this->height(node->right->right)) {
            // node 右孩子的右子树更高, 进行左旋
            n = this->leftRotation(node);
        } else {
            // 先右旋后左旋
            n = this->rightLeftRotation(node);
        }
    }

    if (left) {
        p->left = n;
    } else if (right) {
        p->right = n;
    } else {
        this->root = n;
    }
}

// 返回结点的平衡因子
template <class T>
int AVLTree<T>::bf(BinNode<T>* node) const {
    return this->height(node->left) - this->height(node->right);
}

// 删除
template <class T>
bool AVLTree<T>::erase(const T& e) {
    BinNode<T>* x = this->search(e);
    if (!x) return false; // 元素不存在, 删除失败

    this->remove(x);
    if (--this->size <= 0) return true;

    for (BinNode<T>* g = this->hot; g; g = g->parent) {
        if (!isBalence(g)) {
            reBalence(g);
        }
        this->updateHeight(g);
    }

    return true;
}

// 添加
template <class T>
BinNode<T>* AVLTree<T>::add(const T& e) {
    BinNode<T>* x = this->search(e);
    if (x) return x; // 如果结点存在, 返回该结点

    x = this->insert(e); // 插入新值

    for (BinNode<T>* p = x->parent; p; p = p->parent) {
        if (!isBalence(p)) {
            reBalence(p);
            // 只要某个不平衡结点平衡了
            // 其对应祖先都会平衡
            // 因此可以直接退出重平衡操作
            break;
        } else {
            // 因为插入结点的父亲一定是平衡的
            // 因此只需要更新其高度
            // 实际重平衡是从其祖父结点开始
            this->updateHeight(p);
        }
    }

    return x;
}