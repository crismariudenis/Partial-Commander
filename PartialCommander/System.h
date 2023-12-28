#pragma once

#include "Config.h"

class System
{
private:
	static System* system;
	System();

public:
	void copy(std::filesystem::path startPath, std::filesystem::path endPath);
	void del(std::filesystem::path path);
	void rename();
	void create();
	static System* getInstance();

};