#include "state.h"

int State::gIndex_ = 1;

State::State(const set<Tree*> &tree_set)
  : marked_(false) {
  index_ = gIndex_;
  ++gIndex_;
  tree_set_ = tree_set;
}

State::~State() {
}

void State::AddTransferState(int c, State* state) {
  transfer_map_[c] = state;
}
