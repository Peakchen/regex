#ifndef __REGEX_H__
#define __REGEX_H__

#include <stack>

using namespace std;
class Tree;
class Stream;

class Regex {
public:
    Regex();
    ~Regex();

    bool Compile(const char *str);
    bool Match(const char *str);

private:
    bool ConstructTree(const char *str);
    bool ProcessChar(int c, stack<int> *ops, stack<Tree*> *nodes);
    bool ProcessAlter(int c, stack<int> *ops, stack<Tree*> *nodes);
    bool ProcessGroup(int c, stack<int> *ops, stack<Tree*> *nodes);

private:
};

#endif  // __REGEX_H__
