#pragma once

#include "Config.h"
#include "Panel.h"

class System
{
private:
	static System* system;
	System();

public:
	std::string copy(std::filesystem::path startPath, std::filesystem::path endPath, Panel & panel);
	void del(std::filesystem::path path, Panel & panel);
	void rename(std::string oldPath, Panel &panel);
	void create(std::filesystem::path path, Panel & panel);
	static System* getInstance();
	void refresh(int code, Panel &panel);
};