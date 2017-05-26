//
// Created by kamil on 26/05/17.
//

#include <cstdlib>
#include <iostream>
#include "SimpleCommandInterface.h"
#include "AddCommand.h"
#include "DisplayCommand.h"
#include "BroadcastCommand.h"
#include "UnknownCommand.h"

std::unique_ptr<Command> SimpleCommandInterface::getNextCommand()
{
    std::cout << "Available commands: add, display, broadcast, exit." << std::endl;
    std::string command;
    std::cin >> command;
    if (command == "exit")
        exit(0); // ...
    if (command == "add")
    {
        const std::string fileName = getFileName();
        return std::make_unique<AddCommand>(fileName);
    }
    if (command == "display")
        return std::make_unique<DisplayCommand>();
    if (command == "broadcast")
        return std::make_unique<BroadcastCommand>();
    return std::make_unique<UnknownCommand>(command);
}

std::string SimpleCommandInterface::getFileName()
{
    std::cout << "Please enter a file name." << std::endl;
    std::string fileName;
    std::cin >> fileName;
    return fileName;
}



