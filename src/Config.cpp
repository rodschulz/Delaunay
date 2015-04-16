/**
 * Author: rodrigo
 * 2015
 */
#include "Config.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>

Config::Config()
{
	randomize = false;
	debugLevel = NONE;
	walkingMethod = STANDARD;
}

Config::~Config()
{
}

void Config::load(const string &_filename)
{
	string line;
	ifstream inputFile;
	inputFile.open(_filename.c_str(), fstream::in);
	if (inputFile.is_open())
	{
		while (getline(inputFile, line))
		{
			// Parse string line
			vector<string> tokens;
			istringstream iss(line);
			copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter(tokens));

			parse(tokens[0], tokens[1]);
		}
		inputFile.close();
	}
	else
		cout << "Unable to open input: " << _filename;
}

DebugLevel Config::getDebugLevel()
{
	return getInstance()->debugLevel;
}

void Config::parse(const string _key, const string _value)
{
	if (_key.compare("debugLevel") == 0)
	{
		if (_value.compare("none") == 0)
			getInstance()->debugLevel = NONE;
		else if (_value.compare("low") == 0)
			getInstance()->debugLevel = LOW;
		else if (_value.compare("medium") == 0)
			getInstance()->debugLevel = MEDIUM;
		else if (_value.compare("high") == 0)
			getInstance()->debugLevel = HIGH;
	}
	else if (_key.compare("randomizeInput") == 0)
		getInstance()->randomize = _value.compare("true") == 0;
	else if (_key.compare("walkingMethod") == 0)
		getInstance()->walkingMethod = _value.compare("jumpandwalk") == 0 ? JUMP_AND_WALK : STANDARD;
}

bool Config::randomizeInput()
{
	return getInstance()->randomize;
}

WalkingMethod Config::getWalkingMethod()
{
	return getInstance()->walkingMethod;
}
