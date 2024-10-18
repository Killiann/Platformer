#pragma once
#include <vector>
#include "SFML/Graphics.hpp"
#include <iostream>

class MultiLineText {

private:
	std::vector<sf::Text> lines;

	float padding = 4.f;
	float fontSize;
	sf::Font* font;
	std::string text;	
	sf::Vector2f position;
	sf::Vector2f size;

	void resetHeights();

public:
	MultiLineText();
	MultiLineText(std::string text, int width, float fontSize, sf::Font* font);

	inline void setPosition(sf::Vector2f pos) {
		position = pos;
		resetHeights();
	}
	inline sf::Vector2f getPosition() { return position; }
	inline sf::Vector2f getSize() { return size; }
	void render(sf::RenderWindow& window);
};
