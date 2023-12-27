#include "Clipboard.h"


void Clipboard::copy(std::unordered_map<int, bool>& mark, std::vector<Folder>& folders) {
	this->folders.clear();
	for (int index = 0; index < folders.size(); ++index) {
		if (mark[index])
			this->folders.push_back(folders[index]), std::cout << "HERE\n";
	}
}

Clipboard::Clipboard() {};

Clipboard* Clipboard::getInstance() {
	if (clipboard == NULL)
		clipboard = new Clipboard();
	return clipboard;
}

std::vector<Folder> Clipboard::getFolders() {
	return folders;
}

Clipboard* Clipboard::clipboard = nullptr;