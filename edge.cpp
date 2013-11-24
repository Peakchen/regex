#include <stdio.h>
#include "edge.h"

Edge::Edge(int c)
  : c_(c),
    next_(NULL) {
}

Edge::~Edge() {
}


