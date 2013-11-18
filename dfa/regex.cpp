#include <iostream>
#include <ctype.h>
#include "regex.h"
#include "stream.h"
#include "tree.h"

using namespace std;

Regex::Regex() {
}

Regex::~Regex() {
}

bool Regex::ProcessChar(int c, stack<int> *ops, stack<Tree*> *nodes) {
    Tree *tree = nodes->top();

    Tree *new_tree = new Tree(NORMAL, c);
    if (tree) {
        Tree *parent = new Tree(CAT);
        parent->set_left(tree);
        parent->set_right(new_tree);
        nodes->push(parent);
    } else {
        nodes->push(new_tree);
    }
    return true;
}

bool Regex::ProcessAlter(int c, stack<int> *ops, stack<Tree*> *nodes) {
    return true;
}

bool Regex::ProcessGroup(int c, stack<int> *ops, stack<Tree*> *nodes) {
    while (true) {
        int old_op = ops->top(); ops->pop();

        if (old_op == '|') {
            if (nodes->size() < 2) {
                cout << "error while process `|' operator\n";
                return false;
            }
            Tree *right = nodes->top(); nodes->pop();
            Tree *left = nodes->top(); nodes->pop();
            Tree *parent = new Tree(ALTER);
            parent->set_left(left);
            parent->set_right(right);
            nodes->push(parent);
        } else if (old_op == '(') {
            break;
        }
    }
    return true;
}

bool Regex::ConstructTree(const char *str) {
    int c;
    Stream stream(str);
    stack<int> ops;
    stack<Tree*> nodes;

    while ((c = stream.Read()) != '\0') {
        if (isalpha(c)) {
            cout << "read isalpha: " << (char)c << "\n";
            ProcessChar(c, &ops, &nodes);
        } else {
            cout << "read not isalpha: " << (char)c << "\n";
            if (c == '(') {
                ops.push(c);
            } else if (c == ')') {
                ProcessGroup(c, &ops, &nodes);
            } else if (c == '|') {
                ops.push(c);
            } else if (c == '*') {
                Tree *old_node = nodes.top(); nodes.pop();
                Tree *parent = new Tree(START);
                parent->set_left(old_node);
                nodes.push(parent);
            }
        }
    }

    return true;
}

bool Regex::Compile(const char *str) {
    ConstructTree(str);

    return true;
}

bool Regex::Match(const char *str) {
    return true;
}
