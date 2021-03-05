//
// Created by Martin Voigt on 05.03.21.
//

#ifndef HELLOWORLD_NODE_H
#define HELLOWORLD_NODE_H

#include "BoundingBox.h"

class Node {

public:
    Node *fg, *fd;
    BoundingBox b;
    int beginning, end;
};


#endif //HELLOWORLD_NODE_H
