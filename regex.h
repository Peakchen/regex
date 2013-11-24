#ifndef __REGEX_H__
#define __REGEX_H__

class ASTNode;
class State;

class Regex {
public:
    Regex();
    ~Regex();

    bool Compile(const char *str);
    bool Match(const char *str);

private:
    ASTNode *root_;
    State   *start_;
};

#endif  /* __REGEX_H__ */
