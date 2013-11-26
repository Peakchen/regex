#include <iostream>
#include <ctype.h>
#include "regex.h"
#include "stream.h"
#include "tree.h"

using namespace std;

Regex::Regex() {
}

Regex::~Regex() {
}

Tree* Regex::ProcessChar(int c, Stream *stream, stack<int> *ops, stack<Tree*> *nodes) {
  Tree *new_tree = new Tree(NORMAL, c);

  if (nodes->size() > 0) {
    Tree *tree = nodes->top();nodes->pop();
    Tree *parent = new Tree(CAT);
    parent->set_left(tree);
    parent->set_right(new_tree);
    nodes->push(parent);
    return parent;
  } else {
    nodes->push(new_tree);
    return new_tree;
  }
}

Tree* Regex::ProcessAlter(int c, Stream *stream, stack<int> *ops, stack<Tree*> *nodes) {
  int next;
  Tree *tree;

  next = stream->Read();
  if (isalpha(next)) {
    // 取出节点栈顶的节点
    if (nodes->size() < 1) {
      return NULL;
    }
    Tree *left  = nodes->top(); nodes->pop();
    Tree *right = new Tree(NORMAL, next);
    Tree *parent = new Tree(ALTER);
    parent->set_left(left);
    parent->set_right(left);
    nodes->push(parent);
    return parent;
  }
  if (next == '(') {
    // 取出节点栈顶的节点
    if (nodes->size() < 1) {
      return NULL;
    }
    Tree *left  = nodes->top(); nodes->pop();
    Tree *right = ProcessGroup(next, stream, ops, nodes);
    Tree *parent = new Tree(ALTER);
    parent->set_left(left);
    parent->set_right(left);
    nodes->push(parent);
    return parent;
  }

  cout << "ProcessAlter error\n";
  return NULL;
}

Tree* Regex::ProcessGroup(int c, Stream *stream, stack<int> *ops, stack<Tree*> *nodes) {
  Tree *tree;
  stack<int> new_ops;
  stack<Tree*> new_nodes;

  while ((c = stream->Read()) != '\0') {
    if (isalpha(c)) {
      tree = ProcessChar(c, stream, &new_ops, &new_nodes);
    } else {
      if (c == '(') {
        tree = ProcessGroup(c, stream, &new_ops, &new_nodes);
      } else if (c == ')') {
        break;
      } else if (c == '|') {
        tree = ProcessAlter(c, stream, &new_ops, &new_nodes);
      } else if (c == '*') {
        tree = ProcessStart(c, stream, &new_ops, &new_nodes);
      }
    }
  }

  if (c == '\0') {
    cout << "ProcessGroup error\n";
    return NULL;
  }

  nodes->push(tree);
  return tree;
}

Tree* Regex::ProcessStart(int c, Stream *stream, stack<int> *ops, stack<Tree*> *nodes) {
  if (nodes->size() < 1) {
    cout << "ProcessStart error\n";
    return NULL;
  }
  Tree *old_node = nodes->top(); nodes->pop();
  Tree *parent = new Tree(START);
  parent->set_left(old_node);
  nodes->push(parent);

  return parent;
}

bool Regex::ConstructTree(const char *str) {
  int c;
  Stream stream(str);
  stack<int> ops;
  stack<Tree*> nodes;
  Tree *tree;

  while ((c = stream.Read()) != '\0') {
    if (isalpha(c)) {
      tree = ProcessChar(c, &stream, &ops, &nodes);
    } else {
      if (c == '(') {
        tree = ProcessGroup(c, &stream, &ops, &nodes);
      } else if (c == ')') {
        cout << "error\n";
        return false;
      } else if (c == '|') {
        tree = ProcessAlter(c, &stream, &ops, &nodes);
      } else if (c == '*') {
        tree = ProcessStart(c, &stream, &ops, &nodes);
      }
    }
  }

  Tree *parent = new Tree(CAT);
  parent->set_left(tree);
  parent->set_right(new Tree(END));

  return true;
}

bool Regex::Compile(const char *str) {
  ConstructTree(str);

  return true;
}

bool Regex::Match(const char *str) {
  return true;
}
