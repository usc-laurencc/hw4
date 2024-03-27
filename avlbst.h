#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void insertFix(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* n);
    void removeFix(AVLNode<Key, Value>* n, int8_t diff);
    void rotateRight(AVLNode<Key, Value>* z);
    void rotateLeft(AVLNode<Key, Value>* x);
};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    // if tree is empty
    if (BinarySearchTree<Key, Value>::empty()) {
        BinarySearchTree<Key, Value>::root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);
        return;
    }
    
    AVLNode<Key, Value>* temp = (AVLNode<Key, Value>*) BinarySearchTree<Key, Value>::root_;
    AVLNode<Key, Value>* parent;

    while (temp != NULL) {
        // if key is already in the tree
        if (new_item.first == temp->getKey()) {
            temp->setValue(new_item.second);
            return;
        }

        parent = temp;
        if (new_item.first < temp->getKey()) 
            temp = temp->getLeft();

        else if (new_item.first > temp->getKey())
            temp = temp->getRight();
    }
    // if it gets this far, key is not in the AVL
    AVLNode<Key, Value>* curr = new AVLNode<Key, Value>(new_item.first, new_item.second, parent);
    int8_t oldBal = parent->getBalance();
    if (curr->getKey() < parent->getKey()) {
        parent->setLeft(curr);
    }
    else {
        parent->setRight(curr); 
    }
    if (oldBal == -1 || oldBal == 1){
        parent->setBalance(0);
        return;
    }
    if (oldBal == 0) {
        if (curr == parent->getLeft())
            parent->updateBalance(-1);
        else
            parent->updateBalance(1);
        insertFix(parent, curr);
    }
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
    // if tree is empty
    if (BinarySearchTree<Key, Value>::root_ == NULL) {
        return;
    }
    
    AVLNode<Key, Value>* temp = (AVLNode<Key, Value>*) BinarySearchTree<Key, Value>::root_;
    AVLNode<Key, Value>* node = NULL;

    while (temp != NULL) {
        // if key is already in the tree
        if (key == temp->getKey()) {
            node = temp;
            break;
        }

        if (key < temp->getKey()) 
            temp = temp->getLeft();

        else
            temp = temp->getRight();
    }
    // checking if the key is in the tree
    if (node == NULL) 
        return;
    // checking if node has 2 children
    if (node->getLeft() != NULL && node->getRight() != NULL) {
        nodeSwap(node, (AVLNode<Key, Value>*) BinarySearchTree<Key, Value>::predecessor(node));
    }
    AVLNode<Key, Value>* parent = node->getParent();
    int8_t diff;
    if (parent != NULL) {
        if (node == parent->getLeft())
            diff = 1;
        else
            diff = -1;
    }
    
    // node has 0 children
    if (node->getLeft() == NULL && node->getRight() == NULL) {
      if (node == BinarySearchTree<Key, Value>::root_) {
        BinarySearchTree<Key, Value>::root_ = NULL;
      }
      else if (parent->getLeft() == node)
        parent->setLeft(NULL);
      else
        parent->setRight(NULL);
    }
    // node has 1 child
    else if ((node->getLeft() == NULL && node->getRight() != NULL) || (node->getLeft() != NULL && node->getRight() == NULL)) {
      
      AVLNode<Key, Value>* child;
      if (node->getLeft() == NULL && node->getRight() != NULL)
        child = node->getRight();
      else
        child = node->getLeft();

      if (node == BinarySearchTree<Key, Value>::root_) {
        BinarySearchTree<Key, Value>::root_ = child;
        child->setParent(NULL);
      } 
      // if node is the left child of its parent
      else if (parent->getLeft() == node) {
        parent->setLeft(child);
        child->setParent(parent);
      }
      // if node is the right child of its parent 
      else {
        parent->setRight(child);
        child->setParent(parent);
      }
    }
    delete node;
    removeFix(parent, diff);
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* n)
{
    if (parent == NULL || parent->getParent() == NULL)
        return;
    AVLNode<Key, Value>* grandparent = parent->getParent();
    
    if (parent == grandparent->getLeft()) {
        grandparent->updateBalance(-1);
        if (grandparent->getBalance() == 0)
            return;
        else if (grandparent->getBalance() == -1) 
            insertFix(grandparent, parent);
        else if (grandparent->getBalance() == -2) {
            // if zig-zig
            if (parent->getLeft() == n) {
                rotateRight(grandparent);
                parent->setBalance(0);
                grandparent->setBalance(0);
            }
            // if zig-zag
            else {
                rotateLeft(parent);
                rotateRight(grandparent);
                if (n->getBalance() == -1) {
                    parent->setBalance(0);
                    grandparent->setBalance(1);
                }
                else if (n->getBalance() == 0) {
                    parent->setBalance(0);
                    grandparent->setBalance(0);
                }
                else if (n->getBalance() == 1) {
                    parent->setBalance(-1);
                    grandparent->setBalance(0);
                }
                n->setBalance(0);
            }
        }
    }
    // if parent is right child
    else {
        grandparent->updateBalance(1);
        if (grandparent->getBalance() == 0)
            return;
        else if (grandparent->getBalance() == 1) 
            insertFix(grandparent, parent);
        else if (grandparent->getBalance() == 2) {
            // if zig-zig
            if (parent->getRight() == n) {
                rotateLeft(grandparent);
                parent->setBalance(0);
                grandparent->setBalance(0);
            }
            // if zig-zag
            else {
                rotateRight(parent);
                rotateLeft(grandparent);
                if (n->getBalance() == 1) {
                    parent->setBalance(0);
                    grandparent->setBalance(-1);
                }
                else if (n->getBalance() == 0) {
                    parent->setBalance(0);
                    grandparent->setBalance(0);
                }
                else if (n->getBalance() == -1) {
                    parent->setBalance(1);
                    grandparent->setBalance(0);
                }
                n->setBalance(0);
            }
        }
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* n, int8_t diff) 
{
    if (n == NULL)
        return;
    AVLNode<Key, Value>* parent = n->getParent();
    int8_t ndiff = -1;
    if (parent != NULL && n == parent->getLeft())
        ndiff = 1;

    int8_t bal = n->getBalance();
    if (diff == 1) {
        if (bal + diff == 2) {
            AVLNode<Key, Value>* child = n->getRight();
            // zig-zig
            if (child->getBalance() == 1) {
                rotateLeft(n);
                n->setBalance(0);
                child->setBalance(0);
                removeFix(parent, ndiff);
            }
            else if (child->getBalance() == 0) {
                rotateLeft(n);
                n->setBalance(1);
                child->setBalance(-1);
            }
            // zig-zag
            else if (child->getBalance() == -1) {
                AVLNode<Key, Value>* g = child->getLeft();
                rotateRight(child);
                rotateLeft(n);
                if (g->getBalance() == -1) {
                    n->setBalance(0);
                    child->setBalance(1);
                }
                else if (g->getBalance() == 0) {
                    n->setBalance(0);
                    child->setBalance(0);
                }
                else if (g->getBalance() == 1) {
                    n->setBalance(-1);
                    child->setBalance(0);
                }
                g->setBalance(0);
                removeFix(parent, ndiff);
            }
        }
        else if (bal + diff == 1)
            n->setBalance(1);
        else if (bal + diff == 0) {
            n->setBalance(0);
            removeFix(parent, ndiff);
        }
    }
    // if diff = -1
    else {
        if (bal + diff == -2) {
            AVLNode<Key, Value>* child = n->getLeft();
            // zig-zig
            if (child->getBalance() == -1) {
                rotateRight(n);
                n->setBalance(0);
                child->setBalance(0);
                removeFix(parent, ndiff);
            }
            else if (child->getBalance() == 0) {
                rotateRight(n);
                n->setBalance(-1);
                child->setBalance(1);
            }
            // zig-zag
            else if (child->getBalance() == 1) {
                AVLNode<Key, Value>* g = child->getRight();
                rotateLeft(child);
                rotateRight(n);
                if (g->getBalance() == 1) {
                    n->setBalance(0);
                    child->setBalance(-1);
                }
                else if (g->getBalance() == 0) {
                    n->setBalance(0);
                    child->setBalance(0);
                }
                else if (g->getBalance() == -1) {
                    n->setBalance(1);
                    child->setBalance(0);
                }
                g->setBalance(0);
                removeFix(parent, ndiff);
            }
        }
        else if (bal + diff == -1)
            n->setBalance(-1);
        else if (bal + diff == 0) {
            n->setBalance(0);
            removeFix(parent, ndiff);
        }
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* z)
{
    if (z == NULL)
        return;
    AVLNode<Key, Value>* zParent = z->getParent();
    AVLNode<Key, Value>* y = z->getLeft();
    if (y == NULL)
        return;
    AVLNode<Key, Value>* c = y->getRight();
    
    // resetting z's parent
    if (zParent != NULL) {
        if (zParent->getLeft() == z)
            zParent->setLeft(y);
        else
            zParent->setRight(y);
        y->setParent(zParent);
    }
    // if z is root 
    else {
        BinarySearchTree<Key, Value>::root_ = y;
        y->setParent(NULL);
    }
    // resetting interaction between y and z 
    y->setRight(z);
    if (c != NULL) {
        z->setLeft(c);
        c->setParent(z);
    }
    else
        z->setLeft(NULL);
    z->setParent(y);
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* x)
{
    if (x == NULL)
        return;
    AVLNode<Key, Value>* xParent = x->getParent();
    AVLNode<Key, Value>* y = x->getRight();
    if (y == NULL)
        return;
    AVLNode<Key, Value>* b = y->getLeft();
    
    // resetting x's parent
    if (xParent != NULL) {
        if (xParent->getLeft() == x)
            xParent->setLeft(y);
        else
            xParent->setRight(y);
        y->setParent(xParent);
    }
    // if x is root 
    else {
        BinarySearchTree<Key, Value>::root_ = y;
        y->setParent(NULL);
    }
    
    // resetting interaction between x and y
    y->setLeft(x);
    if (b != NULL) {
        x->setRight(b);
        b->setParent(x);
    }
    else 
        x->setRight(NULL);
    x->setParent(y);
}

#endif
