#include "System.h"

System::System() {};

System* System::getInstance() {
	if (system == NULL)
		system = new System();
	return system;
}

std::string System::copy(std::filesystem::path startPath, std::filesystem::path endPath, Panel & panel)
{
	int copyIndex = panel.selectedFolderIndex;
	auto path = startPath;
	auto destPath = endPath;

	std::string extension = path.extension().string();
	destPath = destPath / (path.stem().string() + extension);

	for (int i = 1; std::filesystem::exists(destPath); i++) {
		destPath = destPath.parent_path() / (path.stem().string() + " (" + std::to_string(i) + ")" + extension);
	}

	try {
		std::filesystem::copy(path, destPath, std::filesystem::copy_options::recursive);
		std::cout << destPath.filename() << " coppied to " << destPath.parent_path() << '\n';
		return destPath.string();
	}
	catch (const std::filesystem::filesystem_error& e) {
		std::cerr << "Error copying file: " << e.what() << std::endl;
	}
}

void System::del(std::filesystem::path path, Panel & panel)
{
	if (std::filesystem::is_directory(path))
		std::cout << "Folder: " << path << " removed\n";
	else
		std::cout << "File: " << path << " removed\n";

	// remove from the filesystem
	// if it's a folder remove recursively 
	std::filesystem::remove_all(path);
}

void System::create(std::filesystem::path currentPath, Panel & panel) {
	int copyIndex = panel.selectedFolderIndex;
	std::string suffix = "New Folder";
	std::filesystem::path newPath = currentPath / "New Folder";
	for (int digit = 1; std::filesystem::exists(newPath); ++digit) 
		newPath = currentPath / (suffix + " (" + std::to_string(digit) + ")");
	std::filesystem::create_directory(newPath);
	panel.update(currentPath);
	std::vector<Folder> &folders = panel.getFolders();
	for (int index = 0; index < folders.size(); ++index) {
		if (folders[index].path == newPath) {
			panel.updateFolderSelectedFolder(index);
			panel.firstToDisplay = std::max(index - 3, 0);
			panel.lastToDisplay = std::min((int)folders.size() - 1, index + 14);
			if (panel.firstToDisplay - panel.lastToDisplay != 18) panel.firstToDisplay = 0;
			break;
		}
	}
	panel.updateShortcutSelectedFolder(3, -1);
}

void System::refresh(int code, Panel &panel)
{
	std::vector<Folder>& folders = panel.getFolders();
	std::filesystem::path currentPath = panel.getCurrentPath();
	switch (code)
	{
	case sf::Keyboard::Scancode::F8:
	{
		for (unsigned int index = 0; index < folders.size(); ++index) 
			if (index == panel.selectedFolderIndex || shortcutSelectedFolder[index]) 
				del(folders[index].path, panel);
		panel.update(currentPath);
		panel.updateShortcutSelectedFolder(3, -1);
		panel.updateFolderSelectedFolder(panel.lastToDisplay);
		break;
	}
	case sf::Keyboard::Scancode::F5: {
		std::vector<sf::Font> fonts = fontsHandler.getFonts();
		for (unsigned int index = 0; index < folders.size(); ++index)
			if (index == panel.selectedFolderIndex || shortcutSelectedFolder[index]) {
				copy(folders[index].path, folders[index].path.parent_path(), panel);
			}
		panel.update(currentPath);
		panel.updateShortcutSelectedFolder(3, -1);
		panel.updateFolderSelectedFolder(panel.lastToDisplay);
		break;
	}
	case sf::Keyboard::Scancode::F7:
	{
		create(currentPath, panel);
		break;
	}
	default:
		break;
	}
}

void System::rename(std::string oldName, Panel & panel)
{
	std::vector<Folder> &folders = panel.getFolders();
	std::string newName = folders[panel.selectedFolderIndex].folderText.getString().toAnsiString();
	if(oldName[0] == '\\') oldName.erase(oldName.begin() + 0);
	if (newName[0] == '\\') newName.erase(newName.begin() + 0);
	std::filesystem::path oldPath = panel.getCurrentPath() / oldName,
		newPath = panel.getCurrentPath() / newName;
	std::filesystem::rename(oldPath, newPath);
	folders[panel.selectedFolderIndex].path = newPath;
}

System* System::system = nullptr;