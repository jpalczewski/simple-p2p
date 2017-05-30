//
// Created by kamil on 26/05/17.
//

#ifndef SIMPLE_P2P_VISITOR_H
#define SIMPLE_P2P_VISITOR_H

class BroadcastCommand;
class DisplayCommand;
class AddCommand;
class DownloadCommand;
class UnknownCommand;

class Visitor
{
public:
    virtual void handle(BroadcastCommand* command) = 0;
    virtual void handle(DisplayCommand* command) = 0;
    virtual void handle(AddCommand* command) = 0;
    virtual void handle(DownloadCommand* command) = 0;
    virtual void handle(UnknownCommand* command) = 0;

    virtual ~Visitor() { }
};
#endif //SIMPLE_P2P_VISITOR_H
