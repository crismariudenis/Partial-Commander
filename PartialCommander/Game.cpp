#include "Game.h"


void Game::initWindow(sf::RenderWindow& mainWindow, std::string name) {

    mainWindow.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), name);

    /// Calculating topLeft corner so that window is in the middle of the screen
    sf::VideoMode mode = sf::VideoMode::getDesktopMode();
    int desktopWidth = mode.width, desktopHeight = mode.height;
    sf::Vector2i topLeft(desktopWidth / 10, desktopHeight / 10);

    mainWindow.setPosition(topLeft);
}

void Game::initBackgroundColor(sf::RectangleShape& background) {

    background.setFillColor(backgroundColor);
    background.setPosition(0, 0);
    background.setSize(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
}

void Game::init() {
    initWindow(mainWindow, name);
    initBackgroundColor(background);
    initPanels();
}

Game::Game(std::string name) {
    this->name = name;
}

void Game::run() {
    while (mainWindow.isOpen()) {

        /// Event handling
        sf::Event event;
        while (mainWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                mainWindow.close();
        }

        mainWindow.setActive();

        /// Drawing operations
        mainWindow.draw(background);
        leftPanel.draw(mainWindow);
        rightPanel.draw(mainWindow);

        mainWindow.display();

    }
}

void Game::initPanels() {
    leftPanel.configure(PANEL_WIDTH, PANEL_HEIGHT, leftPanelDirectoryPath, leftPanelTopLeft, leftPanelBotRight);
    rightPanel.configure(PANEL_WIDTH, PANEL_HEIGHT, rightPanelDirectoryPath, rightPanelTopLeft, rightPanelBotRight);
}