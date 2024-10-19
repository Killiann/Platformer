#include "CustomGraphics.h"

MultiLineText::MultiLineText(){}

MultiLineText::MultiLineText(std::string t, int w, float fSize, sf::Font* fn) :
	text(t), fontSize(fSize), font(fn), width(w)
{
	letterSpacing = 1.f;

	position = sf::Vector2f(0, 0);
	size = sf::Vector2f(width, 0);
			
	generate();
}

void MultiLineText::generate() {

	lines.clear();

	std::string prevLine = "";
	std::string newLine = "";

	for (int i = 0; i < text.size(); i++) {
		//continue until end of word
		if (text[i] != ' ') {
			newLine += text[i];
		}
		else if (text[i] == ' ') {
			//when end of new word reached, check width against given width
			sf::Text currentLine(newLine, *font, fontSize);
			currentLine.setLetterSpacing(letterSpacing);

			newLine += text[i];

			//if line has become too long, create a new line and continue
			if (currentLine.getGlobalBounds().width + padding * 2 > width) {

				//remove trailing spaces before text
				if (prevLine[0] == ' ' && prevLine.size() > 1) prevLine = prevLine.substr(1, prevLine.size() - 1);
				sf::Text lastLine(prevLine, *font, fontSize);
				lastLine.setLetterSpacing(letterSpacing);
				
				lines.push_back(lastLine);
				newLine = newLine.substr(prevLine.size(), newLine.size() - prevLine.size());
			}
			prevLine = newLine;
		}

		//if reached end
		if (i + 1 == text.size()) {
			if (newLine[0] == ' ' && newLine.size() > 1) newLine = newLine.substr(1, newLine.size() - 1);
			sf::Text currentLine(newLine, *font, fontSize);
			currentLine.setLetterSpacing(letterSpacing);

			if (currentLine.getGlobalBounds().width + padding * 2 > width) {
				sf::Text lastLine(prevLine, *font, fontSize);
				lastLine.setLetterSpacing(letterSpacing);

				lines.push_back(lastLine);
				newLine = newLine.substr(prevLine.size(), newLine.size() - prevLine.size());

				sf::Text finalLine(newLine, *font, fontSize);				
				finalLine.setLetterSpacing(letterSpacing);
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

void MultiLineText::setLetterSpacing(float spacing) {
	letterSpacing = spacing;
	generate();
}
void MultiLineText::render(sf::RenderWindow& window) {
	for (auto line : lines) {
		window.draw(line);
	}
}

