#ifndef __REGEX_H__
#define __REGEX_H__

#include <stack>
#include <map>

class Tree;

class Regex {
public:
  Regex();
  ~Regex();

  bool Compile(const char *str);
  bool Match(const char * str);

private:
  Tree* root_;
};

#endif  // __REGEX_H__
