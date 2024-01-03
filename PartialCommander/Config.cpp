#include "Config.h"
#include "Folder.h"
#include "Scrollbar.h"

CustomFonts fontsHandler;

std::filesystem::path leftPanelDirectoryPath("D:\\");
std::filesystem::path rightPanelDirectoryPath("C:\\");

std::unordered_map<int, bool> shortcutSelectedFolder, pressed;
std::vector<std::pair<int, char>> specialCharacters = { {41, '-'}, {50, '.'}, {40, ' '} , {46, '('}, {47, ')'} };

bool checkBoxLabel(float topLeftX, float topLeftY, float botRightX, float botRightY, float mouseX, float mouseY)
{
	return mouseX >= topLeftX && mouseX <= botRightX && mouseY >= topLeftY && mouseY <= botRightY;
}

bool nameCompare(const Folder& a, const Folder& b) {
	std::string name1 = a.folderText.getString().toAnsiString();
	std::string name2 = b.folderText.getString().toAnsiString();
	return name1 < name2;
}

bool sizeCompare(const Folder& a, const Folder& b) {
	return a.getSize() < b.getSize();
}

bool timeCompare(const Folder& a, const Folder& b) {
	std::filesystem::file_time_type ftime1 = std::filesystem::last_write_time(a.path);
	std::filesystem::file_time_type ftime2 = std::filesystem::last_write_time(b.path);
	return ftime1 < ftime2;
}

std::string getDate(std::filesystem::path path) {
	// Convert from fs::file_time_type to std::time_t
	auto to_time_t = [](auto tp) {
		namespace cs = std::chrono;
		namespace fs = std::filesystem;
		auto sctp = cs::time_point_cast<cs::system_clock::duration>(tp -
			fs::file_time_type::clock::now() + cs::system_clock::now());
		return cs::system_clock::to_time_t(sctp);
		};


	auto fileTime = std::filesystem::last_write_time(path);
	std::time_t tt = to_time_t(fileTime);

	// Convert the std::time_t value to a local time structure
	std::tm* localTime = std::localtime(&tt);
	std::stringstream buff;

	buff << std::put_time(localTime, "%d %b %H:%M");
	std::string formattedDate = buff.str();
	return formattedDate;
}