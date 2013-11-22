#ifndef __REGEX_H__
#define __REGEX_H__

#include <stack>
#include <map>

using namespace std;
class Tree;
class Stream;
class State;

class Regex {
public:
    Regex();
    ~Regex();

    bool Compile(const char *str);
    bool Match(const char *str);

private:
    Tree* ConstructTree(const char *str);
    Tree* ProcessChar(int c, Stream *stream, stack<int> *ops, stack<Tree*> *nodes);
    Tree* ProcessAlter(int c, Stream *stream, stack<int> *ops, stack<Tree*> *nodes);
    Tree* ProcessGroup(int c, Stream *stream, stack<int> *ops, stack<Tree*> *nodes);
    Tree* ProcessStar(int c, Stream *stream, stack<int> *ops, stack<Tree*> *nodes);

    void AddTree(Tree *tree);
    
    bool ConstructDFA(Tree *root);

private:
    map<int, Tree*> tree_map_;
    map<int, bool>  chars_map_;
    map<int, State*> state_map_;
};

#endif  // __REGEX_H__
