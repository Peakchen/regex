#include <iostream>
#include <ctype.h>
#include <list>
#include "regex.h"
#include "state.h"
#include "stream.h"
#include "tree.h"

using namespace std;

Regex::Regex() {
}

Regex::~Regex() {
}

void Regex::AddTree(Tree *tree) {
  tree_map_[tree->get_index()] = tree;
}

Tree* Regex::ProcessChar(int c, Stream *stream, stack<int> *ops, stack<Tree*> *nodes) {
  Tree *right = new Tree(NORMAL, c);
  AddTree(right);

  chars_map_[c] = true;

  if (nodes->size() > 0) {
    Tree *left = nodes->top();nodes->pop();
    Tree *parent = new Tree(CAT);
    AddTree(parent);
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
      AddTree(right);
    }
    Tree *parent = new Tree(ALTER);
    AddTree(parent);
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
  AddTree(parent);
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

Tree* Regex::ConstructTree(const char *str) {
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
        return NULL;
      } else if (c == '|') {
        tree = ProcessAlter(c, &stream, &ops, &nodes);
      } else if (c == '*') {
        tree = ProcessStar(c, &stream, &ops, &nodes);
      }
    }
  }

  Tree *parent = new Tree(CAT);
  AddTree(parent);
  parent->set_left(tree);
  Tree *right = new Tree(END);
  AddTree(right);
  parent->set_right(right);

  return parent;
}

bool Regex::Compile(const char *str) {
  Tree *root = NULL;

  root = ConstructTree(str);
  return ConstructDFA(root);
}

bool Regex::ConstructDFA(Tree *root) {
  list<State *> unmarked_states;
  State *state;

  state = new State(root->get_firstpos());
  unmarked_states.push_back(state);
  while (!unmarked_states.empty()) {
    state = unmarked_states.front(); unmarked_states.pop_front();
    state_map_[state->get_index()] = state;
    state->set_marked();
    const set<Tree*> &tree_set = state->get_tree_set();

    map<int, bool>::iterator iter;
    for (iter = chars_map_.begin(); iter != chars_map_.end(); ++iter) {
      int c = iter->first;
      set<Tree*> followset;
      set<Tree*>::const_iterator tree_iter;
      for (tree_iter = tree_set.begin();
           tree_iter != tree_set.end(); ++tree_iter) {
        Tree *tree = *tree_iter;
        if (tree->get_type() == NORMAL && tree->get_char() == c) {
          followset.insert((*tree_iter)->get_follow_pos().begin(),
                           (*tree_iter)->get_follow_pos().end());
        }
      }
      // if this state already exist??
      State *follow_state = NULL;
      map<int, State*>::iterator state_iter;
      for (state_iter = state_map_.begin();
           state_iter != state_map_.end(); ++state_iter) {
        if (state_iter->second->get_tree_set() == followset) {
          follow_state = state_iter->second;
          break;
        }
      }
      if (follow_state == NULL) {
        follow_state = new State(followset);
        unmarked_states.push_back(follow_state);
      }
      state->AddTransferState(c, follow_state);
    }
  }

  return true;
}

bool Regex::Match(const char *str) {
  State *state;

  cout << "Try match: " << str << endl;
  state = state_map_[1];
  if (state == NULL) {
    return NULL;
  }
  while (str && *str) {
    state = state->get_transter_state(*str);
    if (state == NULL) {
      cout << "unmatch: " << *str << endl;
      return false;
    }
    ++str;
  }
  return true;
}
