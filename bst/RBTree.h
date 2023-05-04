#pragma once

#include "BBST.h"
#include <iostream>

// 红黑树
template <class T>
class RedBlack : public BBST<T> {
  protected:
    void solveDoubleRed(BinNode<T>* const);   // 双红修正
    void solveDoubleBlack(BinNode<T>* const); // 双黑修正
    int  updateBlackHeight(BinNode<T>*);      // 更新黑节点高度
    bool blackHeightUpdated(BinNode<T>*);     // 是否需要更新黑高度
    bool isBlack(BinNode<T>* const) const;
    void visitor(BinNode<T>* const) const;
    int  blackHeight(BinNode<T>* const) const;

  public:
    bool        erase(const T&);
    BinNode<T>* add(const T&);
};

// 判断某个节点是否是黑色节点
template <class T>
bool RedBlack<T>::isBlack(BinNode<T>* const x) const {
    // 外部节点(nullptr)也是黑色节点
    if (x == nullptr || x->color == Color::BLACK) {
        return true;
    } else {
        return false;
    }
}

// 双红修正
template <class T>
void RedBlack<T>::solveDoubleRed(BinNode<T>* const x) {
    if (x->isRoot()) {                    // 递归至根, 树高增加
        this->root->color = Color::BLACK; // 双红修正到根时(此时根为红), 但根只能为黑
        this->root->blackHeight++;
        return;
    }

    BinNode<T>* p = x->parent;
    if (isBlack(p)) return; // 若 p 为黑色节点, 说明不是双红了, 可以退出

    // p 为红色, 说明 p 不可能是根, g 不可能是空
    BinNode<T>* g = p->parent;
    BinNode<T>* u = x->uncle(); // 获取 x 的叔节点, 可能为 null

    if (isBlack(u)) {
        // x 的叔节点是黑色, 考虑“双红修正-1”
        // 只有下面四种情况
        BinNode<T>*gg = g->parent, // 祖父节点(可能为 null)
            *root_    = nullptr;
        // 先判断 gg 是左孩子还是右孩子, 或者是根
        bool left  = g->isLeftChild();
        bool right = g->isRightChild();

        if (x->isLeftChild() && p->isLeftChild()) {
            // x 和 p 都是左孩子, 进行右旋
            p->blackHeight++; // 此时 p 会变成黑节点, 黑高度增加
            p->color = Color::BLACK;
            root_    = this->rightRotation(g); // 这会返回修正子树的根节点
        } else if (x->isRightChild() && p->isLeftChild()) {
            // p 是左孩子, x 是右孩子, 先左旋后右旋
            x->blackHeight++;
            x->color = Color::BLACK;
            root_    = this->leftRightRotation(g);
        } else if (x->isRightChild() && p->isRightChild()) {
            // x 和 p 都是右孩子, 进行左旋
            p->blackHeight++;
            p->color = Color::BLACK;
            root_    = this->leftRotation(g);
        } else {
            // x 是左孩子, p 是右孩子, 先右旋后左旋
            x->blackHeight++;
            x->color = Color::BLACK;
            root_    = this->rightLeftRotation(g);
        }

        // g 一定会变红
        g->blackHeight--;
        g->color = Color::RED;

        if (left) {
            gg->left = root_;
        } else if (right) {
            gg->right = root_;
        } else {
            // g 是根节点
            this->root = root_;
        }
    } else {
        // 叔节点是红色, 考虑“双红修正-2”
        p->color = Color::BLACK;
        p->blackHeight++;
        u->color = Color::BLACK;
        u->blackHeight++;
        if (!g->isRoot()) g->color = Color::RED; // g 若非根, 则转红
        solveDoubleRed(g);
    }
}

// 双黑修正
template <class T>
void RedBlack<T>::solveDoubleBlack(BinNode<T>* const x) {
    BinNode<T>* p = x ? x->parent : this->hot;
    if (!p) return; // 父节点不存在, 不需要修正

    // x 可能为空, 不能使用 x.isLeftChild()
    BinNode<T>* s = (x == p->left) ? p->right : p->left; // x 的兄弟

    BinNode<T>*t = nullptr, /*临时节点*/ *g = p->parent; // 祖父节点

    if (isBlack(s)) { // 兄弟为黑色节点
        if (!isBlack(s->left) || !isBlack(s->right)) {
            // 兄弟的红孩子存在, 进行“双黑修正-1”
            Color color = p->color; // 备份父节点颜色

            bool left  = p->isLeftChild();
            bool right = p->isRightChild();

            if (!isBlack(s->left)) {
                // 兄弟左孩子为红
                if (s->isLeftChild()) {
                    // 兄弟为左孩子, 进行右旋
                    t = this->rightRotation(p);
                } else {
                    // 兄弟为右孩子, 先右旋后左旋
                    t = this->rightLeftRotation(p);
                }
            } else {
                // 兄弟右孩子为红
                if (s->isLeftChild()) {
                    // 兄弟为左孩子, 先左旋后右旋
                    t = this->leftRightRotation(p);
                } else {
                    // 兄弟为右孩子, 进行左旋
                    t = this->leftRotation(p);
                }
            }

            if (left) {
                g->left = t;
            } else if (right) {
                g->right = t;
            } else {
                // p 原来是根节点, 旋转后还需要把根节点换成新的
                this->root = t;
            }

            // 孩子染黑
            t->left->color = Color::BLACK;
            updateBlackHeight(t->left);

            t->right->color = Color::BLACK;
            updateBlackHeight(t->right);

            // 恢复颜色
            t->color = color;
            updateBlackHeight(t);
        } else {
            // 兄弟的红孩子不存在
            s->color = Color::RED;
            s->blackHeight--;
            if (!isBlack(p)) {
                // 父节点为红色, 进行“双黑修正-2”
                p->color = Color::BLACK;
            } else {
                // 父节点为黑色, 进行“双黑修正-3”
                p->blackHeight--;
                solveDoubleBlack(p);
            }
        }
    } else {
        // 兄弟为红色节点, 即“双黑修复-4”
        // s 一定会存在父节点
        s->color = Color::BLACK;
        s->blackHeight++;
        p->color = Color::RED;
        p->blackHeight--;

        bool left  = p->isLeftChild();
        bool right = p->isRightChild();

        if (s->isLeftChild()) {
            // 兄弟节点是左孩子, 进行右旋
            t = this->rightRotation(p);
        } else {
            // 兄弟节点是右孩子, 进行左旋
            t = this->leftRotation(p);
        }

        // p 是根节点就不需要关联 g 的孩子
        if (left) {
            g->left = t;
        } else if (right) {
            g->right = t;
        } else {
            this->root = t;
        }

        solveDoubleBlack(x);
    }
}

// 更新黑节点高度
template <class T>
int RedBlack<T>::updateBlackHeight(BinNode<T>* const x) {
    int h1 = blackHeight(x->left), h2 = blackHeight(x->right);
    x->blackHeight = h1 > h2 ? h1 : h2;
    return isBlack(x) ? x->blackHeight++ : x->blackHeight;
}

// 黑高度更新条件
template <class T>
bool RedBlack<T>::blackHeightUpdated(BinNode<T>* const x) {
    // 左右子树黑高度相等, 且当前节点黑高度等于子树黑高度(当为黑节点时+1)
    return blackHeight(x->left) == blackHeight(x->right) &&
           x->blackHeight == (isBlack(x) ? blackHeight(x->left) + 1 : blackHeight(x->left));
}

// 插入
template <class T>
BinNode<T>* RedBlack<T>::add(const T& e) {
    BinNode<T>* x = this->search(e);
    if (x) return x; // 如果节点存在, 返回该节点

    x = this->insert(e);

    solveDoubleRed(x); // 双红修正

    return x;
}

// 重写查看结点信息的方法
template <class T>
void RedBlack<T>::visitor(BinNode<T>* node) const {
    std::cout << node->data << "(" << (node->color == Color::BLACK ? "B" : "R") << ", "
              << node->blackHeight << ")\n";
}

// 获取结点黑高度
template <class T>
inline int RedBlack<T>::blackHeight(BinNode<T>* node) const {
    if (node)
        return node->blackHeight;
    else
        return 0;
}

// 删除
template <class T>
bool RedBlack<T>::erase(const T& e) {
    BinNode<T>* x = this->search(e);
    if (!x) return false; // 目标不存在

    BinNode<T>* r = this->remove(x);

    // 节点都删完了, 啥都不用修复, 直接退出
    if (--this->size <= 0) return true;

    // 至少存在一个根节点...
    if (!this->hot) { // 被删节点为根节点
        this->root->color = Color::BLACK;
        this->root->blackHeight++;
        return true;
    }

    // 未出现双黑...
    if (blackHeightUpdated(this->hot)) return true;

    // 替换节点为红色
    if (!isBlack(r)) {
        r->color = Color::BLACK;
        r->blackHeight++;
        return true;
    }

    solveDoubleBlack(r); // 双黑修正

    return true;
}