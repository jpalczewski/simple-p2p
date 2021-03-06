//
// Created by erxyi on 23.05.17.
//
#include <iostream>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include "DaemonClient.h"

namespace po = boost::program_options;

int main(int argc, char** argv)
{
    std::cout << "simple-p2p client application" << std::endl;
    po::options_description desc("Command line interface for p2p daemon. Allowed options");
    desc.add_options()
            ("help", "produce this help message")
            ("broadcast", "broadcast local resources to other nodes")
            ("add", po::value<std::string>(), "add the file to local resources")
            ("download", po::value<uint64_t>(), "download a file with a given local id")
            ("block", po::value<uint64_t>(), "block a file with a given local id")
            ("unblock", po::value<uint64_t>(), "unblock a file with a given local id")
            ("delete", po::value<uint64_t>(), "delete a file with a given local id")
            ("invalidate", po::value<uint64_t>(), "invalidate a file with a given local id")
            ("display", "display local and network resources")
            ("port", po::value<int>(), "specify daemon port");

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
        int daemonPort = vm.count("port") ? vm["port"].as<int>() : 6000;
        DaemonClient client("127.0.0.1", daemonPort);

        if (vm.count("display"))
            std::cout << client.sendNoParam<DisplayCommand>().getContent() << std::endl;

        if (vm.count("add")) {
            const std::string &path = vm["add"].as<std::string>();
            boost::filesystem::path p{path};
            boost::filesystem::path absolutePath = boost::filesystem::absolute(p);
            if(!boost::filesystem::exists(p) || !boost::filesystem::is_regular_file(p))
            {
                std::cerr << "File doesn't exist! Do you really wanted to add " << absolutePath.native() << "?" << std::endl;
                return 1;
            }
            std::cout << client.sendAdd(absolutePath.native()).getContent() << std::endl;
        }
        if (vm.count("broadcast"))
            std::cout << client.sendNoParam<BroadcastCommand>().getContent() << std::endl;

        if (vm.count("download"))
            std::cout << client.sendOneParam<DownloadCommand>(vm["download"].as<uint64_t>()).getContent() << std::endl;

        if (vm.count("block"))
            std::cout << client.sendOneParam<BlockCommand>(vm["block"].as<uint64_t>()).getContent() << std::endl;

        if (vm.count("unblock"))
            std::cout << client.sendOneParam<UnblockCommand>(vm["unblock"].as<uint64_t>()).getContent() << std::endl;

        if (vm.count("invalidate"))
            std::cout << client.sendOneParam<InvalidateCommand>(vm["invalidate"].as<uint64_t>()).getContent() << std::endl;

        if (vm.count("delete"))
            std::cout << client.sendOneParam<DeleteCommand>(vm["delete"].as<uint64_t>()).getContent() << std::endl;
    }
    catch (const std::exception exception)
    {
        std::cout << "Error: " << exception.what() << std::endl;
    }
}
