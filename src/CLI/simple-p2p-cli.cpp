//
// Created by erxyi on 23.05.17.
//
#include <iostream>
#include <boost/program_options.hpp>
#include "DaemonClient.h"

namespace po = boost::program_options;

int main(int argc, char** argv)
{
    std::cout << "Generic CLI app" << std::endl;
    po::options_description desc("Command line interface for p2p daemon. Allowed options");
    desc.add_options()
            ("help", "produce this help message")
            ("broadcast", "broadcast local resources to other nodes")
            ("add", po::value<std::string>(), "add the file to local resources")
            ("display", "display local and network resources");

    po::variables_map vm;

    try
    {
        po::store(po::parse_command_line(argc, argv, desc), vm);
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
        std::cout << "Unrecognized option(s). Try '--help'." << std::endl;
        return 0;
    }
    po::notify(vm);

    if (vm.count("help"))
    {
        std::cout << desc << std::endl;
        return 0;
    }

    try
    {
        DaemonClient client("127.0.0.1", 6000);

        if (vm.count("display"))
            std::cout << client.sendDisplay().getContent() << std::endl;

        if (vm.count("add"))
        {
            std::cout << client.sendAdd(vm["add"].as<std::string>()).getContent() << std::endl;
        }

        if (vm.count("broadcast"))
        {
            std::cout << client.sendBroadcast().getContent() << std::endl;
        }
    }
    catch (const std::exception exception)
    {
        std::cout << "Error: " << exception.what() << std::endl;
    }
}
