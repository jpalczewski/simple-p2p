#include <iostream>
#include <boost/property_tree/ptree.hpp>

using namespace std;

class ConfigHandler
{
private:
    static bool instanceFlag;
    static ConfigHandler *single;
    ConfigHandler()
    {
        //private constructor
    }
public:
    static ConfigHandler* getInstance();
    string get(const string&);
	void readDirectory(const string&);
    ~ConfigHandler()
    {
        instanceFlag = false;
    }
};
