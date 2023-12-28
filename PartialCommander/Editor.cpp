#include "Editor.h"

void Editor::draw() {

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
	if (event.type == sf::Event::TextEntered)
	{
		if (event.text.unicode < 128)
			std::cout << "ASCII character typed: " << static_cast<char>(event.text.unicode) << std::endl;
	}
}