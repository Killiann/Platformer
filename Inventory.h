#pragma once
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <numeric>
#include "Item.h"


class Inventory
{
private:	
	std::vector<Item> items;
	static const int maxItems = 36;	
	static const int i_width = 12;
	float viewScale = 5.0f;

	//drawable
	//sf::Texture texture;
	sf::Sprite background;
	sf::Sprite hotbar_bg;
	sf::Sprite bag_bg;

	sf::Sprite selectionBox;
	sf::Sprite selected;

	sf::Vector2f hotBarPos;
	sf::Vector2i hotBarSize;

	sf::Vector2f bagBgPos;
	sf::Vector2i bagBgSize;

	bool isOpen{ false };
	bool renderSelection{ false };
	int itemSelected{ -1 };
	int tempSelected{ -1 };

	void resetHotbarPositions();
	inline void resetSelected() {
		selected.setPosition(hotBarPos.x + itemSelected * ((hotBarSize.x / 12)), hotBarPos.y + viewScale);
	}
public:
	Inventory();
	Inventory(sf::Texture& tex, sf::RenderWindow& window);

	void AddItem(Item item);
	void RemoveItem(int id);

	void handleClick(sf::Event& userInput);
	void handleScroll(sf::Event& userInput);
	void handleKeyPress(sf::Event& userInput);
		
	void update(sf::Vector2i &mousePos);
	void Render(sf::RenderWindow& window);

	inline const bool IsOpen() { return isOpen; }
	inline void Open() { 
		isOpen = true; 
		hotbar_bg.setPosition(
			background.getPosition().x + 7 * viewScale,
			background.getPosition().y + 12 * viewScale);
		resetHotbarPositions();
		resetSelected();
	}
	inline void Close() { 
		isOpen = false; 
		hotbar_bg.setPosition(
			background.getPosition().x + 7 * viewScale,
			background.getPosition().y + 120 * viewScale);
		resetHotbarPositions();
		resetSelected();
	}
	inline const std::vector<Item> getItems() { return items; }
};
