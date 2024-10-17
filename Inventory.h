#pragma once
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <numeric>
#include "Item.h"


struct inventoryItem {

	int id;
	Item* item;
	sf::Sprite inv_sprite;

	inventoryItem(int n_id, Item* n_item) {
		id = n_id;
		item = n_item;
		inv_sprite = n_item->getSprite();
	}
	inventoryItem() {
		id = -1;
		item = nullptr;
	}

	bool isEmpty() { if (id == -1) return true; return false; }
};

class Inventory
{
private:		
	std::vector<inventoryItem> items;
	static const int maxItems = 36;	
	static const int inv_width = 12;
	float viewScale = 5.0f;

	//drawable
	//sf::Texture texture;
	sf::Sprite background;
	sf::Sprite hotbar_bg;
	sf::Sprite bag_bg;

	sf::Sprite selectionBox;
	sf::Sprite selected;

	sf::Vector2f backgroundPosition;
	sf::Vector2i backgroundSize;

	sf::Vector2f hotBarPos;
	sf::Vector2i hotBarSize;

	sf::Vector2f bagBgPos;
	sf::Vector2i bagBgSize;

	sf::Vector2i mousePosition;

	bool isOpen{ false };
	bool renderSelection{ false };
	
	int selectedItem{ 0 };
	int hoveredItem{ -1 };
	int itemCount = 0;

	void resetHotbarPositions();
	void resetSpritePositions();
	inline void resetSelected() {
		selected.setPosition(hotBarPos.x + selectedItem * ((hotBarSize.x / 12)), hotBarPos.y + viewScale);
	}

	inline bool isInToolbar(int item_id) { if (item_id < inv_width) return true; return false; }
public:
	Inventory();
	Inventory(sf::Texture& tex, sf::RenderWindow& window);

	void addItem(Item* item);
	void removeItem(int id);

	void handleClick(sf::Event& userInput);
	void handleScroll(sf::Event& userInput);
	void handleKeyPress(sf::Event& userInput);
		
	void update(sf::Vector2i &mousePos);
	void Render(sf::RenderWindow& window);

	inline const bool getOpen() { return isOpen; }
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
	inline const std::vector<inventoryItem> getItems() { return items; }
};
