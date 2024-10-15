#include "Inventory.h"

Inventory::Inventory(){}

void Inventory::resetHotbarPositions() {
	hotBarPos = hotbar_bg.getPosition();
	hotBarSize = hotbar_bg.getTextureRect().getSize();
	hotBarSize.x *= viewScale;
	hotBarSize.y *= viewScale;

	hotBarSize.x -= viewScale * 2;
	hotBarSize.y -= viewScale * 2;

	hotBarPos.x += viewScale;
	hotBarPos.y += viewScale;			
}

Inventory::Inventory(sf::Texture& texture, sf::RenderWindow& window){
	//init background textures
	
	background = sf::Sprite(texture, sf::IntRect(0, 0, 208, 80));
	//background.setOrigin(background.getTextureRect().width / 2.f, background.getTextureRect().height / 2.f);
	background.setPosition(
		window.getSize().x / 2.f - (background.getTextureRect().width / 2) * viewScale,		
		window.getSize().y / 2.f - (background.getTextureRect().height /2) * viewScale);

	hotbar_bg= sf::Sprite(texture, sf::IntRect(0, 81, 194, 18));
	hotbar_bg.setPosition(
		background.getPosition().x + 7 * viewScale,
		background.getPosition().y + 120 * viewScale);

	bag_bg = sf::Sprite(texture, sf::IntRect(0, 100, 194, 34));
	bag_bg.setPosition(
		background.getPosition().x + 7 * viewScale,
		background.getPosition().y + 33 * viewScale);

	//selection
	selectionBox = sf::Sprite(texture, sf::IntRect(177, 134, 16, 16));	
	selected = sf::Sprite(texture, sf::IntRect(1, 151, 15, 15));	

	background.setScale(viewScale, viewScale);
	hotbar_bg.setScale(viewScale, viewScale);
	bag_bg.setScale(viewScale, viewScale);
	selectionBox.setScale(viewScale, viewScale);
	selected.setScale(viewScale, viewScale);

	resetHotbarPositions();

	bagBgPos = bag_bg.getPosition();
	bagBgSize = bag_bg.getTextureRect().getSize();

	bagBgSize.x *= viewScale;
	bagBgSize.y *= viewScale;

	bagBgSize.x -= viewScale * 2;
	bagBgSize.y -= viewScale * 2;

	bagBgPos.x += viewScale;
	bagBgPos.y += viewScale;
}

void Inventory::AddItem(Item newItem) {
	if ((int)items.size() < maxItems) {
		items.push_back(newItem);
	}
	else {
		//error 
		std::cout << "Max items reached. Could not add " << newItem.getName() << ".\n";
	}
}

void Inventory::RemoveItem(int id) {
	//needs to be tested
	for (int i = 0; i < (int)items.size(); i++) {
		if (items[i].getId() == id)
			items.erase(items.begin() + i);
	}
}

bool pointInRect(sf::Vector2i& point, sf::Vector2f& rectPos, sf::Vector2i& rectSize) {	
	if (point.x > rectPos.x &&
		point.x < rectPos.x + rectSize.x &&
		point.y > rectPos.y &&
		point.y < rectPos.y + rectSize.y)
		return true;
	else return false;
}

void Inventory::update(sf::Vector2i& mousePos) {
	
	//for toolbar	
	renderSelection = false;
	tempSelected = -1;

	if (pointInRect(mousePos, hotBarPos, hotBarSize)) {
		//mouse is within hotbar bounids
		renderSelection = true;
		
		//ID of selected item
		int xPosition = (mousePos.x - (hotBarPos.x)) / 16 / viewScale;
		tempSelected = xPosition;

		selectionBox.setPosition(
			hotBarPos.x + xPosition * ((hotBarSize.x / i_width)),
			hotBarPos.y);
	}

	//for rest of inventory	
	if (pointInRect(mousePos, bagBgPos, bagBgSize) && isOpen) {
		//mouse is in bg bounds
		renderSelection = true;

		//combined to make ID of selection
		int xPosition = (mousePos.x - (bagBgPos.x)) / 16 / viewScale;
		int yPosition = (mousePos.y - (bagBgPos.y)) / 16 / viewScale;

		tempSelected = yPosition * 16 + xPosition + i_width;

		selectionBox.setPosition(
			bagBgPos.x + xPosition * ((bagBgSize.x / i_width)),
			bagBgPos.y + yPosition * ((bagBgSize.y / 2))
		);
	}				
}

void Inventory::handleClick(sf::Event& userInput) {
	if (userInput.mouseButton.button == sf::Mouse::Left) {
		// selected 		
		if (!isOpen) {
			if (tempSelected > -1)itemSelected = tempSelected;

			if (itemSelected < i_width) {
				resetSelected();
			}
		}		
	}
}

void Inventory::handleKeyPress(sf::Event& userInput) {
	if (!isOpen) {
		switch (userInput.key.code) {
		case sf::Keyboard::Num1:
			itemSelected = 0;
			break;
		case sf::Keyboard::Num2:
			itemSelected = 1;
			break;
		case sf::Keyboard::Num3:
			itemSelected = 2;
			break;
		case sf::Keyboard::Num4:
			itemSelected = 3;
			break;
		case sf::Keyboard::Num5:
			itemSelected = 4;
			break;
		case sf::Keyboard::Num6:
			itemSelected = 5;
			break;
		case sf::Keyboard::Num7:
			itemSelected = 6;
			break;
		case sf::Keyboard::Num8:
			itemSelected = 7;
			break;
		case sf::Keyboard::Num9:
			itemSelected = 8;
			break;
		case sf::Keyboard::Num0:
			itemSelected = 9;
			break;
		}
		resetSelected();
	}
}

void Inventory::handleScroll(sf::Event& userInput) {
	if (!isOpen && itemSelected > -1 && itemSelected < i_width) {
		itemSelected += userInput.mouseWheelScroll.delta;
		if (itemSelected == i_width) itemSelected = i_width - 1;
		if (itemSelected == -1) itemSelected = 0;
		resetSelected();
	}
}

void Inventory::Render(sf::RenderWindow& window) {
	if (isOpen) {		
		window.draw(background);		
		window.draw(bag_bg);
	}	
	window.draw(hotbar_bg);
	if (renderSelection)window.draw(selectionBox);
	if (itemSelected > -1)window.draw(selected);
}
