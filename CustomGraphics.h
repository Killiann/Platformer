#pragma once
#include <vector>
#include "SFML/Graphics.hpp"
#include <iostream>

class MultiLineText {

private:
	std::vector<sf::Text> lines;

	int width;
	float padding = 4.f;
	float letterSpacing;
	float fontSize;
	sf::Font* font;
	std::string text;	
	sf::Vector2f position;
	sf::Vector2f size;

	void resetHeights();	
	void generate();
public:
	MultiLineText();
	MultiLineText(std::string text, int width, float fontSize, sf::Font* font);

	inline void setPosition(sf::Vector2f pos) {
		position = pos;
		resetHeights();
	}
	inline sf::Vector2f getPosition() { return position; }
	inline sf::Vector2f getSize() { return size; }
	inline std::string getString() { return text; }
	void setLetterSpacing(float spacing);
	void render(sf::RenderWindow& window);
};



//temp as struct, can be refactored to be safer in future
struct FadingText {
	sf::Text text;
	sf::Clock displayClock;
	sf::Time displayTimer;

	float displayTime = 1.5f;
	float transparency = 255;
	float fadeRate = 2.f;

	void init(sf::Font* font, sf::Vector2f& hotBarPos, sf::Vector2i& hotBarSize);
	void updateText(std::string t);
	void update();
	void resetFade();
};