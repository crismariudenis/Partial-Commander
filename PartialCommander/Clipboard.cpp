#include "Clipboard.h"


void Clipboard::copy(std::unordered_map<int, bool>& mark, std::vector<Folder>& folders) {
	this->folders.clear();
	for (int index = 0; index < folders.size(); ++index) {
		if (mark[index])
			this->folders.push_back(folders[index]);
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

void Clipboard::paste(Panel & panel) {
	System* sys = System::getInstance();
	std::vector<Folder>& folders = panel.getFolders();

	int copyIndex = panel.selectedFolderIndex;
	for (size_t index = 0; index < this->folders.size(); ++index)
		sys->copy(this->folders[index].path, panel.getCurrentPath(), panel);

	panel.update(panel.getCurrentPath());
	panel.selectedFolderIndex = copyIndex;

	folders[copyIndex].toggleIsSelected();
	folders[copyIndex].updateText();
	this->folders.clear();
}

void Clipboard::update(Panel & panel)
{
	std::vector<Folder>& folders = panel.getFolders();
	shortcutSelectedFolder[panel.selectedFolderIndex] = true;
	copy(shortcutSelectedFolder, folders);
	this->panel = &panel;
}

void Clipboard::move(Panel& panel)
{
	System* sys = System::getInstance();
	std::vector<Folder> folders = panel.getFolders();
	copy(shortcutSelectedFolder, folders);
	for (size_t index = 0; index < folders.size(); ++index) 
		sys->del(folders[index].path, panel);
}

Clipboard* Clipboard::clipboard = nullptr;