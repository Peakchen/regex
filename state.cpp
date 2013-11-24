#include <iostream>
#include "astnode.h"
#include "state.h"
#include "edge.h"

State::State() {
}

State::~State() {
}

void State::AddEdge(int c) {
}

State* State::AddState(int c) {  
  State *state = new State();
  cout << "add edge " << (char)c << endl;
  out_[c] = state;

  return state;
}

bool State::Match(Stream *stream) {
  int c;
  State *state, *now;

  c = stream->Read();
  now = this;
  while (c != '\0') {
    state = now->Next(c);
    if (state == NULL) {
      return false;
    }
    now = state;
    c = stream->Read();
  }

  return true;
}

State* State::Next(int c) {
  return out_[c];
}

void State::Visit(ASTNode *node) {
  State *state;

  while ((state = node->Visit(this)) != NULL) {
  }
}
