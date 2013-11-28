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
    Tree* ProcessChar(int c, Stream *stream, stack<int> *ops, stack<Tree*> *nodes);
    Tree* ProcessAlter(int c, Stream *stream, stack<int> *ops, stack<Tree*> *nodes);
    Tree* ProcessGroup(int c, Stream *stream, stack<int> *ops, stack<Tree*> *nodes);
    Tree* ProcessStar(int c, Stream *stream, stack<int> *ops, stack<Tree*> *nodes);

private:
};

#endif  // __REGEX_H__
