#ifndef __TREE_H__
#define __TREE_H__

#include <iostream>

using namespace std;

enum TreeType {
    NORMAL = 0,
    CAT,
    START,
    ALTER,
    GROUP
};

class Tree {
public:
  Tree(TreeType type, int c = -1)
    : type_(type),
      c_(c),
      left_(NULL),
      right_(NULL),
      parent_(NULL) {
      cout << "new tree type: " << type << ", c: " << c << "\n";
  }

  TreeType get_type() const { return type_; }
  int      get_char() const { return c_; }

  void set_left(Tree* left) {
      left_ = left;
      left_->set_parent(this);
  }

  void set_right(Tree *right) {
      right_ = right;
      left_->set_parent(this);
  }

  void set_parent(Tree *parent) {
      parent_ = parent;
  }

  Tree* get_left() {
      return left_;
  }

  Tree* get_right() {
      return right_;
  }

private:
    TreeType type_;
    int      c_;
    Tree    *left_, *right_;
    Tree    *parent_;
};

#endif  // __TREE_H__
