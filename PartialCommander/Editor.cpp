#include "Editor.h"
void Editor::init(std::filesystem::path path) {
	this->path = path;
	std::ifstream file(path.string());
	std::string s;
	while (std::getline(file, s)) {
		text.push_back(s);
	}
}

void Editor::draw() {
	size_t height = PANEL_HEIGHT;
	auto fonts = fontsHandler.getFonts();

	sf::Text line;
	line.setFillColor(sf::Color(255, 255, 255));
	line.setFont(fonts[CustomFonts::Font::ROBOTO]);
	line.setCharacterSize(CHARACTER_SIZE);

	for (size_t index = firstLine; index < std::min(text.size(), (size_t)firstLine + 32); ++index)
	{
		sf::Vector2f pos{ 10,0 };
		pos.y += 1.f * (index - firstLine + 1) * (1.f * height / TEXT_LINE_SPACING);

		line.setString(text[index]);
		line.setPosition(pos);

		window.draw(line);
	}
}

void Editor::update(sf::Event event) {
	//switch (event)
	//{
	//case sf::Keyboard::Scancode::Up: case sf::Keyboard::Scancode::Down: case sf::Keyboard::Scancode::Left: case sf::Keyboard::Scancode::Right:
	//	std::cout << "move\n";
	//	break;
	//default:
	//	
	//	break;
	//}

	int delta = static_cast<int>(event.mouseWheelScroll.delta);
	firstLine = std::max(0, std::min(firstLine - delta, (int)text.size() - 32));

	if (event.type == sf::Event::TextEntered)
	{
		if (event.text.unicode < 128)
			std::cout << "ASCII character typed: " << static_cast<char>(event.text.unicode) << std::endl;
	}
}