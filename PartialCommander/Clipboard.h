
#pragma once
#include "System.h"

class Clipboard
{
private:
	std::vector<Folder> folders;
	static Clipboard * clipboard;
	Clipboard();
	Panel* panel = NULL;
public:
	void copy(std::unordered_map<int, bool> &mark, std::vector<Folder>& folders);
	static Clipboard* getInstance();
	std::vector<Folder> getFolders();
	void paste(Panel & panel);
	void update(Panel& panel);
	void move(Panel& panel);
};