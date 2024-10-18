#include "CustomGraphics.h"

MultiLineText::MultiLineText(){}

MultiLineText::MultiLineText(std::string t, int width, float fSize, sf::Font* fn) :
	text(t), fontSize(fSize), font(fn)
{
	position = sf::Vector2f(0, 0);
	size = sf::Vector2f(width, 0);

	std::string prevLine = "";
	std::string newLine = "";
	for (int i = 0; i < text.size(); i++) {
		if (text[i] != ' ') {
			newLine += text[i];
		}
		else if (text[i] == ' ') {
			sf::Text currentLine(newLine, *font, fontSize);						
			newLine += text[i];

			std::cout << currentLine.getGlobalBounds().width;

			if (currentLine.getGlobalBounds().width + padding * 2 > width) {
				
				sf::Text lastLine(prevLine, *font, fontSize);
				lines.push_back(lastLine);
				newLine = newLine.substr(prevLine.size(), newLine.size() - prevLine.size());
			}
			prevLine = newLine;
		}		

		//if reached end
		if (i + 1 == text.size()) {
			sf::Text currentLine(newLine, *font, fontSize);

			if (currentLine.getGlobalBounds().width + padding * 2 > width) {
				sf::Text lastLine(prevLine, *font, fontSize);
				lines.push_back(lastLine);
				newLine = newLine.substr(prevLine.size(), newLine.size() - prevLine.size());
				
				sf::Text finalLine(newLine, *font, fontSize);
				lines.push_back(finalLine);
			}
			else lines.push_back(currentLine);			
		}
	}		
	resetHeights();
}

void MultiLineText::resetHeights() {
	int heightPadding = padding;
	for (auto &line : lines) {
		line.setFillColor(sf::Color::Black);
		line.setPosition(sf::Vector2f(position.x + padding, position.y + heightPadding));
		heightPadding += padding + line.getGlobalBounds().height;
	}
	size.y = heightPadding;
}

void MultiLineText::render(sf::RenderWindow& window) {
	for (auto line : lines) {
		window.draw(line);
	}
}

