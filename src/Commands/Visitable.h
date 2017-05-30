//
// Created by kamil on 26/05/17.
//

#ifndef SIMPLE_P2P_VISITABLE_H
#define SIMPLE_P2P_VISITABLE_H

class Visitor;

class Visitable
{
public:
    virtual void accept(Visitor* visitor) = 0;

    virtual ~Visitable() {}
};


#endif //SIMPLE_P2P_VISITABLE_H
