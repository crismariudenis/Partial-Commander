#include "System.h"


System::System() {};

System* System::getInstance() {
	if (system == NULL)
		system = new System();
	return system;
}

void System::copy(std::filesystem::path startPath, std::filesystem::path endPath)
{
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
	}
	catch (const std::filesystem::filesystem_error& e) {
		std::cerr << "Error copying file: " << e.what() << std::endl;
	}
}

void System::del(std::filesystem::path path)
{
	if (std::filesystem::is_directory(path))
		std::cout << "Folder: " << path << " removed\n";
	else
		std::cout << "File: " << path << " removed\n";

	// remove from the filesystem
	// if it's a folder remove recursively 
	std::filesystem::remove_all(path);
}

System* System::system = nullptr;