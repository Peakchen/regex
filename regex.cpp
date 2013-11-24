#include "astnode.h"
#include "regex.h"
#include "state.h"

Regex::Regex() 
  : root_(new AlternationNode())
  , start_(new State()) {
}

Regex::~Regex() {
  delete root_;
}

bool Regex::Compile(const char *str) {
  Stream stream(str);

  if (!root_->Parse(&stream)) {
    return false;
  }

  start_->Visit(root_);
}

bool Regex::Match(const char *str) {
  Stream stream(str);

  return start_->Match(&stream);
}
