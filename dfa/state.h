#ifndef __STATE_H__
#define __STATE_H__

#include <set>
#include <map>

using namespace std;

class Tree;

class State {
public:
  State(const set<Tree*> &tree_set);  
  ~State();

  const set<Tree*>& get_tree_set() const {
    return tree_set_;
  }

  void set_marked() {
    marked_ = true;
  }

  bool get_acceptable() const {
    return acceptable_;
  }

  int get_index() const {
    return index_;
  }

  void AddTransferState(int c, State* state);
  State* get_transter_state(int c) {
    return transfer_map_[c];
  }

private:
  map<int, State*> transfer_map_;
  set<Tree*> tree_set_;
  bool marked_;
  bool acceptable_;
  int  index_;
  static int gIndex_;
};

#endif  /* __STATE_H__ */
