#pragma once

#include "BinNode.h"

// 二叉搜索树
template <class T>
class BST {
  protected:
    BinNode<T>*         hot;
    BinNode<T>*         root; // 根节点
    int                 size; // 节点数
    virtual void        inOrder(BinNode<T>*) const   = 0;
    virtual void        preOrder(BinNode<T>*) const  = 0;
    virtual void        postOrder(BinNode<T>*) const = 0;
    virtual BinNode<T>* insert(const T&)             = 0;
    virtual int         height(BinNode<T>*) const    = 0;
    virtual void        updateHeight(BinNode<T>*)    = 0;
    virtual void        visitor(BinNode<T>*) const   = 0;

  public:
    BST() : root(nullptr), hot(nullptr), size(0){};
    virtual int         Size() const      = 0;
    virtual BinNode<T>* Max() const       = 0;
    virtual BinNode<T>* Min() const       = 0;
    virtual BinNode<T>* search(const T&)  = 0;
    virtual BinNode<T>* add(const T&)     = 0;
    virtual bool        erase(const T&)   = 0;
    virtual void        inOrder() const   = 0;
    virtual void        preOrder() const  = 0;
    virtual void        postOrder() const = 0;
};