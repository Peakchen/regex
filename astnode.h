#ifndef _ASTNODE_H__
#define _ASTNODE_H__

#include <vector>
#include "stream.h"

using namespace std;

class State;

class ASTNode {
public:
    ASTNode();
    virtual ~ASTNode();

    virtual bool Parse(Stream *stream) = 0;

    virtual State* Visit(State *state) = 0;
};

class CharNode : public ASTNode {
public:
    CharNode(int c);
    virtual ~CharNode();

    virtual bool Parse(Stream *stream);
    virtual State* Visit(State *state);

private:    
    int c_;
};

class CharRangeNode : public ASTNode {
public:
    CharRangeNode();
    virtual ~CharRangeNode();

    virtual bool Parse(Stream *stream);
    virtual State* Visit(State *state);

private:    
    vector<int> range_;
};

class AlternationNode : public ASTNode {
public:
    AlternationNode();
    virtual ~AlternationNode();

    virtual bool Parse(Stream *stream);
    virtual State* Visit(State *state);
private:
    vector<ASTNode*> nodes_;
};

class ConcatenationNode : public ASTNode {
public:
    ConcatenationNode();
    virtual ~ConcatenationNode();

    virtual bool Parse(Stream *stream);
    virtual State* Visit(State *state);
private:
    vector<ASTNode*> nodes_;
};

#endif  /* _ASTNODE_H__ */
