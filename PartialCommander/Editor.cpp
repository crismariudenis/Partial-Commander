#include "Editor.h"
void Editor::init(std::filesystem::path path) {
	this->path = path;
	std::ifstream file(path.string());
	std::string s;
	while (std::getline(file, s)) {
		text.push_back(s);
	}
}
void Editor::save() {
	std::ofstream file(path.string());
	for (const auto& s : text)
		file << s << '\n';
}
void Editor::draw() {
	frames = (frames + 1) % 120;
	size_t height = PANEL_HEIGHT;
	auto fonts = fontsHandler.getFonts();

	sf::Text line;
	line.setFillColor(sf::Color(255, 255, 255));
	line.setFont(fonts[CustomFonts::Font::UBUNTU]);
	line.setCharacterSize(CHARACTER_SIZE - 1);

	//std::cout << text[0] << " " << text[0].size()<<'\n';
	for (size_t index = firstLine; index < std::min(text.size(), (size_t)firstLine + EDITOR_MAX_LINES); ++index)
	{
		sf::Vector2f pos{ EDITOR_PADDING,0 };
		pos.y += 1.f * (index - firstLine + 1) * (1.f * height / TEXT_LINE_SPACING);

		line.setString(text[index]);
		line.setPosition(pos);

		window.draw(line);

		if (index == cursorPos.y and frames > 30) {

			sf::RectangleShape curs;
			curs.setOutlineThickness(1);

			// For some reason event Mono font aren't consistent with their spacing
			// so, I'm using this slower aproach 
			line.setString(text[cursorPos.y].substr(0, cursorPos.x));
			float textWidth = line.getGlobalBounds().width;

			curs.setPosition(sf::Vector2f(textWidth + EDITOR_PADDING, pos.y));
			curs.setSize(sf::Vector2f(0, LINE_SPACING - 2));
			curs.setOutlineColor(sf::Color::White);
			window.draw(curs);

		}
	}
}

void Editor::update(sf::Event event) {
	switch (event.key.scancode)
	{
	case sf::Keyboard::Scancode::F9:
		save();
		return;
	case sf::Keyboard::Scan::Up:
		cursorPos.y = std::max(0, cursorPos.y - 1);
		cursorPos.x = std::min((int)text[cursorPos.y].size(), cursorPos.x);
		repairScrolling();
		break;
	case sf::Keyboard::Scan::Down:
		cursorPos.y = std::min((int)text.size() - 1, cursorPos.y + 1);
		cursorPos.x = std::min((int)text[cursorPos.y].size(), cursorPos.x);
		repairScrolling();
		break;
	case sf::Keyboard::Scan::Left:
		if (cursorPos.x == 0) {
			cursorPos.y = std::max(0, cursorPos.y - 1);
			cursorPos.x = text[cursorPos.y].size();
		}
		else
			cursorPos.x--;
		repairScrolling();
		break;
	case sf::Keyboard::Scan::Right:
		if (cursorPos.x == text[cursorPos.y].size()) {
			cursorPos.y = std::min((int)text.size() - 1, cursorPos.y + 1);
			cursorPos.x = 0;
		}
		else
			cursorPos.x++;
		repairScrolling();

		break;
	case sf::Keyboard::Scan::End:
		cursorPos.x = text[cursorPos.y].size();
		break;

	case sf::Keyboard::Scan::Home:
		cursorPos.x = 0;
		break;

	}

	switch (event.type)
	{
	case sf::Event::MouseWheelScrolled:
		int delta = static_cast<int>(event.mouseWheelScroll.delta);
		firstLine = std::max(0, std::min(firstLine - delta, (int)text.size() - EDITOR_MAX_LINES));
		break;
	}
	edit(event);
	draw();
}
void Editor::repairScrolling() {
	if (cursorPos.y < firstLine)firstLine--;
	if (cursorPos.y >= std::min(text.size(), (size_t)firstLine + EDITOR_MAX_LINES))firstLine++;
}
void Editor::edit(sf::Event event) {

	if (mode == Mode::VIEW) return;



	std::vector<std::string>newText;
	sf::Event e;

	switch (event.key.scancode)
	{
	case sf::Keyboard::Scan::Enter:
		for (int i = 0; i < cursorPos.y; i++)
			newText.push_back(text[i]);

		newText.push_back(text[cursorPos.y].substr(0, cursorPos.x));
		newText.push_back(text[cursorPos.y].substr(cursorPos.x, text[cursorPos.y].size()));

		for (int i = cursorPos.y + 1; i < text.size(); i++)
			newText.push_back(text[i]);
		text = newText;

		e.key.scancode = sf::Keyboard::Scan::Right;
		update(e);

		break;

	case sf::Keyboard::Scan::Backspace:
		if (cursorPos.x == 0) {
			if (cursorPos.y == 0)break;
			for (int i = 0; i < cursorPos.y - 1; i++)
				newText.push_back(text[i]);

			std::string newLine = text[cursorPos.y - 1];
			newLine += text[cursorPos.y];

			newText.push_back(newLine);

			for (int i = cursorPos.y + 1; i < text.size(); i++)
				newText.push_back(text[i]);
			text = newText;

		}
		else {
			text[cursorPos.y] = text[cursorPos.y].substr(0, cursorPos.x - 1) + text[cursorPos.y].substr(cursorPos.x, text[cursorPos.y].size());
		}
		e.key.scancode = sf::Keyboard::Scan::Left;
		update(e);
		break;

	

	}
	switch (event.type)
	{
	case sf::Event::TextEntered:
		if (event.text.unicode < 128 and event.text.unicode != 8 and event.text.unicode != 13)
		{
			//std::cout << "ASCII character typed: " << static_cast<char>(event.text.unicode) << event.text.unicode << " " << std::endl;
			char c = static_cast<char>(event.text.unicode);
			text[cursorPos.y] = text[cursorPos.y].substr(0, cursorPos.x) + c + text[cursorPos.y].substr(cursorPos.x, text[cursorPos.y].size());

			e.key.scancode = sf::Keyboard::Scan::Right;
			update(e);

		}

		break;
	}
}
