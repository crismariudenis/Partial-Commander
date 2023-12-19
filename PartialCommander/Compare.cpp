#include "Compare.h"

bool Compare::nameCompare(Folder a, Folder b) {
	std::string name1 = a.folderText.getString().toAnsiString();
	std::string name2 = b.folderText.getString().toAnsiString();
	return name1 < name2;
}

bool Compare::sizeCompare(Folder a, Folder b) {
	return a.getSize() < b.getSize();
}

bool Compare::timeCompare(Folder a, Folder b) {
	std::filesystem::file_time_type ftime1 = std::filesystem::last_write_time(a.path);
	std::filesystem::file_time_type ftime2 = std::filesystem::last_write_time(b.path);
	return ftime1 < ftime2;
}