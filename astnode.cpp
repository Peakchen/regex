#include <ctype.h>
#include <iostream>
#include "astnode.h"
#include "state.h"

using namespace std;

ASTNode::ASTNode() {
}

ASTNode::~ASTNode() {
}

CharNode::CharNode(int c)
  : c_(c) {
    cout << "new char node: " << c << endl;
}

CharNode::~CharNode() {
}

bool CharNode::Parse(Stream *stream) {
  return true;
}

State* CharNode::Visit(State *state) {
  return state->AddState(c_);
}

CharRangeNode::CharRangeNode() {
}

CharRangeNode::~CharRangeNode() {
}

bool CharRangeNode::Parse(Stream *stream) {
  return true;
}

State* CharRangeNode::Visit(State *state) {
  return NULL;
}

AlternationNode::AlternationNode() {
}

AlternationNode::~AlternationNode() {
  size_t i;

  for (i = 0; i < nodes_.size(); ++i) {
    delete nodes_[i];
  }
}

bool AlternationNode::Parse(Stream *stream) {
  ASTNode *node;
  int c;

  node = NULL;
  while (true) {
    c = stream->Next();

    if (c == '\0') {
      return true;
    } else if (isalpha(c)) {
      node = new ConcatenationNode();
      if (!node->Parse(stream)) {
        return false;
      }
    } else if (c == '[') {
      /*
      node = new CharRangeNode();
      if (!node->Parse(str, pos)) {
        return false;
      }
      */
    }

    nodes_.push_back(node);
  }
}

State* AlternationNode::Visit(State *state) {
  size_t i;
  State *new_state;

  cout << "AlternationNode node size: "
       << nodes_.size() << endl;
  for (i = 0; i < nodes_.size(); ++i) {
    new_state = nodes_[i]->Visit(state);
    state = new_state;
  }
  return new_state;
}

ConcatenationNode::ConcatenationNode() {
  cout << "new concat node\n";
}

ConcatenationNode::~ConcatenationNode() {
  size_t i;

  for (i = 0; i < nodes_.size(); ++i) {
    delete nodes_[i];
  }
}

bool ConcatenationNode::Parse(Stream *stream) {
  int c;

  while (true) {
    c = stream->Read();

    if (c == '\0') {
      stream->Back();
      return true;
    } else if (isalpha(c)) {
      nodes_.push_back(new CharNode(c));
    } else {
    }
  }

  return true;
}

State* ConcatenationNode::Visit(State *state) {
  size_t i;
  State *new_state;

  cout << "ConcatenationNode node size: "
       << nodes_.size() << endl;

  for (i = 0; i < nodes_.size(); ++i) {
    new_state = nodes_[i]->Visit(state);
    state = new_state;
  }
  return new_state;
}
