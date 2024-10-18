#pragma once
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <numeric>
#include "Item.h"
#include "CustomGraphics.h"

struct inventoryItem {
	float infoPadding = 11.f;
	int id;
	Item* item;

	sf::Texture bgTex;
	sf::Sprite inv_sprite;
	sf::Sprite itemInfo_sprite;
	sf::Text itemName;
	MultiLineText itemDescription;

	inventoryItem(int n_id, Item* n_item) {
		id = n_id;
		item = n_item;
		inv_sprite = n_item->getSprite();
	}
	inventoryItem() {
		id = -1;
		item = nullptr;
	}

	inventoryItem(const inventoryItem& other) {
		id = other.id;
		item = other.item;		
		bgTex = other.bgTex;
		inv_sprite = other.inv_sprite;
		itemInfo_sprite = other.itemInfo_sprite;
		itemName = other.itemName;
		itemDescription = other.itemDescription;
	}

	void updateInfoPosition(float viewScale) {
		itemInfo_sprite.setPosition(
			inv_sprite.getPosition().x + inv_sprite.getTextureRect().getSize().x * viewScale,
			inv_sprite.getPosition().y
		);

		itemName.setPosition(
			itemInfo_sprite.getPosition().x + infoPadding,
			itemInfo_sprite.getPosition().y + infoPadding
		);

		itemDescription.setPosition(sf::Vector2f(
			itemInfo_sprite.getPosition().x + infoPadding,
			itemInfo_sprite.getPosition().y + infoPadding* 2 + itemName.getCharacterSize()
		));
	}

	void Render(sf::RenderWindow& window) {
		window.draw(inv_sprite);
	}

	void RenderInfo(sf::RenderWindow& window) {
		window.draw(itemInfo_sprite);
		window.draw(itemName);
		itemDescription.render(window);
	}

	bool isEmpty() { if (id == -1) return true; return false; }
};

class Inventory
{
private:		
	std::vector<inventoryItem> items;
	static const int maxItems = 36;	
	static const int inv_width = 12;
	static const int itemTitleFontSize = 30;
	static const int itemDescriptionFontSize = 20;

	float viewScale = 5.f;

	sf::Vector2i mousePosition;
	sf::Texture* texture;
	sf::Font* font;

	//drawable
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

	//item info	

	bool isOpen{ false };
	bool renderSelection{ false };
	
	sf::Clock hoverClock;
	sf::Time hoverTime;
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
	Inventory(sf::Texture& tex, sf::RenderWindow& window, sf::Font& fon);

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

