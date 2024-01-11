#include "Editor.h"
void Editor::init(std::filesystem::path path) {
	this->path = path;
	fonts = fontsHandler.getFonts();

	std::ifstream file(path.string());
	std::string s;

	sf::Text text;
	text.setFillColor(sf::Color(255, 255, 255));
	text.setFont(fonts[CustomFonts::Font::UBUNTU]);
	text.setCharacterSize(CHARACTER_SIZE - 1);
	text.setString("A");
	maxLines = PANEL_HEIGHT/(TEXT_LINE_SPACING + 2);

	TextLine t;
	while (std::getline(file, s)) {
		text.setString(s);

		t.string = s;
		t.text = text;

		lines.push_back(t);
	}
	if (lines.empty())
	{
		s.clear();
		text.setString(s);

		t.string = s;
		t.text = text;

		lines.push_back(t);
	}
}
void Editor::save() {
	std::ofstream file(path.string());
	for (const auto& x : lines)
		file << x.string << '\n';
}
void Editor::draw() {
	// Possible alternative is to draw all the lines in a single sf::Text component 

	frames = (frames + 1) % 180;
	size_t height = PANEL_HEIGHT;
	size_t end = std::min(lines.size(), (size_t)firstLine + maxLines);

	sf::Vector2f pos{ EDITOR_PADDING,0 };
	for (size_t index = firstLine; index < end && pos.y < height; ++index)
	{
		pos.y = 1.f * (index - firstLine + 1) * (1.f * height / TEXT_LINE_SPACING);

		lines[index].text.setPosition(pos);

		window.draw(lines[index].text);

		if (index == cursorPos.y and frames > 30) {
			sf::Text line = lines[index].text;
			sf::RectangleShape curs;
			curs.setOutlineThickness(1);

			// For some reason event Mono font aren't consistent with their spacing
			// so, I'm using this slower aproach 
			line.setString(lines[cursorPos.y].string.substr(0, cursorPos.x));
			float textWidth = line.getGlobalBounds().width;

			curs.setPosition(sf::Vector2f(textWidth + EDITOR_PADDING, pos.y));
			curs.setSize(sf::Vector2f(0, LINE_SPACING - 2));
			curs.setOutlineColor(sf::Color::White);
			window.draw(curs);

		}
	}

}

void Editor::update(sf::Event event) {
	std::string currString = lines[cursorPos.y].string;
	switch (event.key.scancode)
	{
	case sf::Keyboard::Scancode::F9:
		save();
		return;
	case sf::Keyboard::Scan::Up:
		cursorPos.y = std::max(0, cursorPos.y - 1);
		cursorPos.x = std::min((int)currString.size(), cursorPos.x);
		repairScrolling();
		break;
	case sf::Keyboard::Scan::Down:
		cursorPos.y = std::min((int)lines.size() - 1, cursorPos.y + 1);
		cursorPos.x = std::min((int)currString.size(), cursorPos.x);
		repairScrolling();
		break;
	case sf::Keyboard::Scan::Left:
		if (cursorPos.x == 0) {
			cursorPos.y = std::max(0, cursorPos.y - 1);
			cursorPos.x = currString.size();
		}
		else
			cursorPos.x--;
		repairScrolling();
		break;
	case sf::Keyboard::Scan::Right:
		if (cursorPos.x == currString.size()) {
			cursorPos.y = std::min((int)lines.size() - 1, cursorPos.y + 1);
			cursorPos.x = 0;
		}
		else
			cursorPos.x++;
		repairScrolling();

		break;
	case sf::Keyboard::Scan::End:
		cursorPos.x = currString.size();
		break;

	case sf::Keyboard::Scan::Home:
		cursorPos.x = 0;
		break;
	}

	switch (event.type)
	{
	case sf::Event::MouseWheelScrolled:
		int delta = static_cast<int>(event.mouseWheelScroll.delta);
		firstLine = std::max(0, std::min(firstLine - delta, (int)lines.size() - maxLines));
		break;
	}
	draw();
	edit(event);
}
void Editor::repairScrolling() {
	if (cursorPos.y < firstLine)firstLine--;
	if (cursorPos.y >= std::min(lines.size(), (size_t)firstLine + maxLines))firstLine++;
}
void Editor::edit(sf::Event event) {

	if (mode == Mode::VIEW) return;

	std::vector<TextLine>newLines;
	sf::Event e;
	TextLine line;
	TextLine& element = lines[cursorPos.y];
	switch (event.key.scancode)
	{
	case sf::Keyboard::Scan::Enter:
		for (int i = 0; i < cursorPos.y; i++)
			newLines.push_back(lines[i]);

		line.string = element.string.substr(0, cursorPos.x);
		line.text = element.text;
		line.text.setString(line.string);
		newLines.push_back(line);

		line.string = element.string.substr(cursorPos.x, element.string.size());
		line.text.setString(line.string);
		newLines.push_back(line);

		for (int i = cursorPos.y + 1; i < lines.size(); i++)
			newLines.push_back(lines[i]);
		lines = newLines;

		e.key.scancode = sf::Keyboard::Scan::Right;
		update(e);

		return;

	case sf::Keyboard::Scan::Backspace:
		if (cursorPos.x == 0) {
			if (cursorPos.y == 0)return;
			for (int i = 0; i < cursorPos.y - 1; i++)
				newLines.push_back(lines[i]);

			line.string = lines[cursorPos.y - 1].string + element.string;
			line.text = element.text;
			line.text.setString(line.string);

			newLines.push_back(line);

			for (int i = cursorPos.y + 1; i < lines.size(); i++)
				newLines.push_back(lines[i]);
			lines = newLines;

			if (cursorPos.y != 0)firstLine = std::max(0, firstLine - 1);
			cursorPos.y = std::max(0, cursorPos.y - 1);
			cursorPos.x = lines[cursorPos.y].string.size();
		}
		else {

			try {
				element.string = element.string.substr(0, cursorPos.x - 1) + element.string.substr(cursorPos.x, element.string.size());
				element.text.setString(element.string);

			}
			catch (const std::exception& e) {
				std::cerr << "Exception caught: " << e.what() << std::endl;
			}

			e.key.scancode = sf::Keyboard::Scan::Left;
			update(e);
		}

		return;
	}
	switch (event.type)
	{
	case sf::Event::TextEntered:
		if (event.text.unicode < 128 and event.text.unicode != 8 and event.text.unicode != 13)
		{
			char c = static_cast<char>(event.text.unicode);

			element.string = element.string.substr(0, cursorPos.x) + c + element.string.substr(cursorPos.x, element.string.size());
			element.text.setString(element.string);

			e.key.scancode = sf::Keyboard::Scan::Right;
			update(e);

		}

		return;
	}
}
