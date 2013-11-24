#ifndef __STATE_H__
#define __STATE_H__

#include <map>
#include "stream.h"

using namespace std;

class Edge;
class ASTNode;

enum StateType {
  START = 0,
  END,
  NORMAL,
};

class State {
 public:
  State();
  ~State();

  void Visit(ASTNode *node);
  void AddEdge(int c);
  State* AddState(int c);
  bool Match(Stream *stream);

  State* Next(int c);

 private:
  map<int, State*> out_;
};

#endif  /* __STATE_H__ */
