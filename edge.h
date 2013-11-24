#ifndef __EDGE_H__
#define __EDGE_H__

class State;

class Edge {
 public:
  Edge(int c);
  ~Edge();

 private:
  int    c_;
  State *next_;
};

#endif  /* __EDGE_H__ */
