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
  Tree *right = new Tree(NORMAL, c);

  if (nodes->size() > 0) {
    Tree *left = nodes->top();nodes->pop();
    Tree *parent = new Tree(CAT);
    parent->set_left(left);
    parent->set_right(right);
    nodes->push(parent);
    // for nullable
    if (left->get_nullable() && right->get_nullable()) {
      parent->set_nullable(true);
    }
    // for first pos
    if (left->get_nullable()) {
      parent->add_firstpos(right->get_firstpos());
    } else {
      parent->add_firstpos(left->get_firstpos());
      parent->add_firstpos(right->get_firstpos());
    }
    // for last pos
    if (right->get_nullable()) {
      parent->add_lastpos(left->get_lastpos());
    } else {
      parent->add_lastpos(left->get_lastpos());
      parent->add_lastpos(right->get_lastpos());
    }

    // for follow pos
    // for cat node N=c1c2: follow_pos(c1) = first_pos(c2)
    left->add_followpos(right->get_lastpos());

    return parent;
  } else {
    nodes->push(right);
    return right;
  }
}

Tree* Regex::ProcessAlter(int c, Stream *stream, stack<int> *ops, stack<Tree*> *nodes) {
  int next;
  Tree *tree;

  next = stream->Read();
  if (isalpha(next) || next == '(') {
    // 取出节点栈顶的节点
    if (nodes->size() < 1) {
      return NULL;
    }
    Tree *left  = nodes->top(); nodes->pop();
    Tree *right = NULL;
    if (next == '(') {
      right = ProcessGroup(next, stream, ops, nodes);
    } else {
      right = new Tree(NORMAL, next);
    }
    Tree *parent = new Tree(ALTER);
    parent->set_left(left);
    parent->set_right(left);
    // for nullable
    if (left->get_nullable() || right->get_nullable()) {
      parent->set_nullable(true);
    }
    // for first pos
    parent->add_firstpos(left->get_firstpos());
    parent->add_firstpos(right->get_firstpos());
    // for last pos
    parent->add_lastpos(left->get_lastpos());
    parent->add_lastpos(right->get_lastpos());

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
        tree = ProcessStar(c, stream, &new_ops, &new_nodes);
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

Tree* Regex::ProcessStar(int c, Stream *stream, stack<int> *ops, stack<Tree*> *nodes) {
  if (nodes->size() < 1) {
    cout << "ProcessStar error\n";
    return NULL;
  }
  Tree *old_node = nodes->top(); nodes->pop();
  Tree *parent = new Tree(START);
  parent->set_left(old_node);
  // for nullable
  parent->set_nullable(true);
  // for first pos
  parent->add_firstpos(old_node->get_firstpos());
  // for last pos
  parent->add_lastpos(old_node->get_lastpos());
  // for follow pos
  // for star node N: follow_pos(last_pos(N)) = first_pos(N)
  set<Tree*> last_pos = parent->get_lastpos();
  set<Tree*>::iterator iter;
  for (iter = last_pos.begin(); iter != last_pos.end(); ++iter) {
    (*iter)->add_followpos(parent->get_firstpos());
  }
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
        tree = ProcessStar(c, &stream, &ops, &nodes);
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
