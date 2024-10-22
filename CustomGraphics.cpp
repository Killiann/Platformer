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

				lines.emplace_back(prevLine, *font, fontSize);
				lines.back().setLetterSpacing(letterSpacing);

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
				lines.emplace_back(prevLine, *font, fontSize);
				lines.back().setLetterSpacing(letterSpacing);

				newLine = newLine.substr(prevLine.size(), newLine.size() - prevLine.size());
				
				lines.emplace_back(newLine, *font, fontSize);
				lines.back().setLetterSpacing(letterSpacing);
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
		line.setPosition(sf::Vector2f((int)(position.x + padding), (int)(position.y + heightPadding)));
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


//********************FADING TEXT************************

void FadingText::init(sf::Font* font, sf::Vector2f& hotBarPos, sf::Vector2i& hotBarSize) {
	text.setFont(*font);
	text.setCharacterSize(20.f);
	text.setFillColor(sf::Color::Black);
	text.setPosition(sf::Vector2f((int)(hotBarPos.x + (hotBarSize.x / 2)), (int)(hotBarPos.y - 30)));
}

void FadingText::updateText(std::string t) {
	text.setString(t);
	text.setOrigin((int)(text.getGlobalBounds().width / 2), (int)(text.getGlobalBounds().height / 2));
}

void FadingText::update() {
	//let text display fade over time
	if (transparency > 0) {
		displayTimer += displayClock.restart();

		if (displayTimer.asSeconds() > displayTime) {
			//update transparency
			transparency -= fadeRate;
			if (transparency < 0) transparency = 0;
			text.setFillColor(sf::Color(0, 0, 0, (int)transparency));
		}
	}
}

void FadingText::resetFade() {
	transparency = 255;
	displayTimer = sf::Time::Zero;
	text.setFillColor(sf::Color(0, 0, 0, (int)transparency));
}


