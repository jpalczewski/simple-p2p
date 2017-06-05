#include "ConfigHandler.h"

#include <boost/property_tree/ini_parser.hpp>
#include <boost/filesystem.hpp>

boost::property_tree::ptree pt;

bool ConfigHandler::instanceFlag = false;
ConfigHandler* ConfigHandler::single = NULL;
ConfigHandler* ConfigHandler::getInstance()
{
    if(! instanceFlag)
    {
        single = new ConfigHandler();
        instanceFlag = true;
        return single;
    }
    else
    {
        return single;
    }
}

string ConfigHandler::get(string const &parameter)
{	
	return pt.get<std::string>(parameter);
    //cout << "Method of the ConfigHandler class" << pt.get<std::string>(parameter) << endl;
}

void ConfigHandler::readDirectory(string const &directoryPath)
{
    using namespace boost::filesystem;
    path configPath = path(directoryPath) / path("config.ini");
	boost::property_tree::ini_parser::read_ini(configPath.string(), pt);

    path downloadDirectory = path(directoryPath + boost::filesystem::path::preferred_separator + "download/");
    path shareDirectory = path(directoryPath + boost::filesystem::path::preferred_separator + "share/");
    path keysDirectory = path(directoryPath + boost::filesystem::path::preferred_separator + "keys/");

    create_directories(downloadDirectory);
    create_directories(shareDirectory);
    create_directories(keysDirectory);

	pt.put("download.dir", downloadDirectory.string());
	pt.put("share.dir", shareDirectory.string());
	pt.put("keys.dir", keysDirectory.string());
}