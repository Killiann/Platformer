#pragma once
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <numeric>
#include "Item.h"
#include "CustomGraphics.h"

struct inventoryItem {
	Item* item;
	int id;
	const int maxStack = 99;

	bool stackable{ false };
	int stackCount{ 0 };

	//graphics
	float infoPadding = 11.f;
	sf::Texture bgTex;
	sf::Sprite inv_sprite;
	sf::Sprite itemInfo_sprite;
	sf::Text stackInfo;
	MultiLineText itemName;
	MultiLineText itemDescription;

	inventoryItem(int n_id, Item* n_item) {
		id = n_id;
		item = n_item;
		inv_sprite = n_item->getSprite();
		stackable = n_item->canStack();	
	}
	inventoryItem() {
		id = -1;
		item = nullptr;
	}

	inventoryItem& operator=(const inventoryItem& other) {

		if (this == &other) {
			return *this;
		}

		id = other.id;
		item = other.item;
		stackable = other.stackable;
		stackInfo = other.stackInfo;
		stackCount = other.stackCount;
		bgTex = other.bgTex;
		inv_sprite = other.inv_sprite;
		itemInfo_sprite = sf::Sprite(bgTex);
		itemInfo_sprite.setPosition(other.itemInfo_sprite.getPosition());
		itemInfo_sprite.setScale(other.itemInfo_sprite.getScale());
		itemName = other.itemName;
		itemDescription = other.itemDescription;
	}

	void updateInfoPosition(float viewScale) {
		itemInfo_sprite.setPosition(
			inv_sprite.getPosition().x + inv_sprite.getTextureRect().getSize().x * viewScale,
			inv_sprite.getPosition().y
		);

		itemName.setPosition(sf::Vector2f(
			itemInfo_sprite.getPosition().x + infoPadding,
			itemInfo_sprite.getPosition().y + infoPadding
		));

		itemDescription.setPosition(sf::Vector2f(
			itemInfo_sprite.getPosition().x + infoPadding,
			itemInfo_sprite.getPosition().y + infoPadding * 2 + itemName.getSize().y
		));

		stackInfo.setPosition(sf::Vector2f(
			(int)(inv_sprite.getPosition().x + inv_sprite.getTextureRect().getSize().x * viewScale) - infoPadding * 2,
			(int)(inv_sprite.getPosition().y + inv_sprite.getTextureRect().getSize().y * viewScale) - infoPadding * 2
		));
	}

	void Render(sf::RenderWindow& window) {
		window.draw(inv_sprite);
		
	}

	void RenderInfo(sf::RenderWindow& window) {
		window.draw(itemInfo_sprite);
		itemName.render(window);
		itemDescription.render(window);		
	}

	bool isEmpty() const { if (id == -1) return true; return false; }
};

//TODO: add to customGraphics

class Inventory
{
private:			
	//const variables
	static constexpr size_t maxItems = 36;	
	static const int inv_width = 12;
	static const int itemTitleFontSize = 30;
	static const int itemDescriptionFontSize = 20;
	float viewScale = 5.f;

	sf::Vector2i mousePosition;
	sf::Texture* texture;
	sf::Font* font;

	//graphics
	sf::Sprite background;
	sf::Sprite hotbar_bg;
	sf::Sprite bag_bg;	

	sf::Sprite selectionIndicator;
	sf::Sprite selected;
	sf::Sprite draggedItemSprite;

	FadingText heldItemText;

	//graphic info
	sf::Vector2f backgroundPosition;
	sf::Vector2i backgroundSize;

	sf::Vector2f hotBarPos;
	sf::Vector2i hotBarSize;

	sf::Vector2f bagBgPos;
	sf::Vector2i bagBgSize;

	//state info		
	bool isOpen{ false };
	bool isHovering{ false };
	bool isDragging{ false };
	//timers
	sf::Clock hoverClock;
	sf::Time hoverTime;	

	//items info
	std::vector<inventoryItem> items;
	int selectedItem{ 0 };
	int hoveredItem{ -1 };
	int draggedItem{ -1 };
	int itemCount = 0;

	//functions
	void resetHotbarPositions();
	void resetSpritePositions();
	void setupInfoBoxTexture(Item* n_item, inventoryItem &newItem);	
	void resetSelected();

	inline bool isInToolbar(int item_id) { if (item_id < inv_width) return true; return false; }
	inline void StopDragging() {
		items[draggedItem].inv_sprite.setColor(sf::Color(255, 255, 255, 255));
		draggedItem = -1;
		isDragging = false;
	}
public:
	Inventory();
	Inventory(sf::Texture& tex, sf::RenderWindow& window, sf::Font& fon);

	void addItem(Item* item);
	void removeItem(int id);

	void handleClick(sf::Event& userInput);
	void handleClickRelease(sf::Event& userInput);
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
		//resetSelected();
	}
	inline void Close() { 
		isOpen = false; 
		hotbar_bg.setPosition(
			background.getPosition().x + 7 * viewScale,
			background.getPosition().y + 120 * viewScale);
		resetHotbarPositions();
		//resetSelected();
	}
	inline const std::vector<inventoryItem> getItems() { return items; }
};

