#include "ConfigHandler.h"

#include <boost/property_tree/ini_parser.hpp>

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
	boost::property_tree::ini_parser::read_ini(directoryPath + "config.ini", pt);
	
	pt.put("download.dir", directoryPath + "download/");
	pt.put("share.dir", directoryPath + "share/");
	pt.put("keys.dir", directoryPath + "keys/");
}