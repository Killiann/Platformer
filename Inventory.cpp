#include "Inventory.h"

Inventory::Inventory(){}

void Inventory::resetHotbarPositions() {
	hotBarPos = hotbar_bg.getPosition();
	hotBarSize = hotbar_bg.getTextureRect().getSize();
	hotBarSize.x *= (int)viewScale;
	hotBarSize.y *= (int)viewScale;

	hotBarSize.x -= (int)viewScale * 2;
	hotBarSize.y -= (int)viewScale * 2;

	hotBarPos.x += (int)viewScale;
	hotBarPos.y += (int)viewScale;	

	for (size_t i = 0; i < items.size(); i++) {
		if (isInToolbar(i)) {
			//top bar				
			items[i].inv_sprite.setPosition(sf::Vector2f(hotBarPos.x + i * 16 * viewScale, hotBarPos.y));
			items[i].updateInfoPosition(viewScale);
		}
		else {
			//rest
		}
	}
}

void Inventory::resetSpritePositions() {
	//loop through items and set positions depending on ID
	for (size_t i = 0; i < items.size(); i++) {
		//toolbar items
		if (isInToolbar(i)) {
			items[i].inv_sprite.setPosition(sf::Vector2f(hotBarPos.x + i * 16 * viewScale, hotBarPos.y));
		}
		else {
			//rest of inv.
			float xPos = bagBgPos.x + ((i - inv_width) % inv_width) * 16.f * viewScale;
			float yPos = bagBgPos.y + ((i - inv_width) / inv_width) * 16.f * viewScale;
			items[i].inv_sprite.setPosition(sf::Vector2f(xPos, yPos));
		}
		items[i].updateInfoPosition(viewScale);
	}
}

Inventory::Inventory(sf::Texture& tex, sf::RenderWindow& window, sf::Font& fon) : font(& fon), texture(& tex){
	//init background textures	

	background = sf::Sprite(tex, sf::IntRect(0, 0, 208, 80));
	background.setPosition(
		window.getSize().x / 2.f - (background.getTextureRect().width / 2) * viewScale,		
		window.getSize().y / 2.f - (background.getTextureRect().height /2) * viewScale);

	hotbar_bg= sf::Sprite(tex, sf::IntRect(0, 81, 194, 18));
	hotbar_bg.setPosition(
		background.getPosition().x + 7 * viewScale,
		background.getPosition().y + 120 * viewScale);

	bag_bg = sf::Sprite(tex, sf::IntRect(0, 100, 194, 34));
	bag_bg.setPosition(
		background.getPosition().x + 7 * viewScale,
		background.getPosition().y + 33 * viewScale);

	//selection
	selectionBox = sf::Sprite(tex, sf::IntRect(177, 134, 16, 16));	
	selected = sf::Sprite(tex, sf::IntRect(1, 151, 15, 15));	

	background.setScale(viewScale, viewScale);
	hotbar_bg.setScale(viewScale, viewScale);
	bag_bg.setScale(viewScale, viewScale);
	selectionBox.setScale(viewScale, viewScale);
	selected.setScale(viewScale, viewScale);	

	bagBgPos = bag_bg.getPosition();
	bagBgSize = bag_bg.getTextureRect().getSize();

	bagBgSize.x *= (int)viewScale;
	bagBgSize.y *= (int)viewScale;

	bagBgSize.x -= (int)viewScale * 2;
	bagBgSize.y -= (int)viewScale * 2;

	bagBgPos.x += (int)viewScale;
	bagBgPos.y += (int)viewScale;

	backgroundPosition = background.getPosition();
	backgroundSize = background.getTextureRect().getSize();
	backgroundSize.x *= (int)viewScale;
	backgroundSize.y *= (int)viewScale;

	resetHotbarPositions();
	resetSelected();

	//fill inventory with empty items
	for(size_t i = 0; i< maxItems; i++){
		inventoryItem empty;
		items.push_back(empty);
	}
}

void Inventory::addItem(Item* n_item) {
	
	//check for max items
	if (itemCount < maxItems) {				
		n_item->setScale(viewScale);		 		
		inventoryItem empty;

		//add item to first empty 
		for (size_t i = 0; i < maxItems;i++) {
			if (items[i].isEmpty()) {
				inventoryItem newItem(i, n_item);

				sf::Sprite itemInfoBg(*texture, sf::IntRect(16, 151, 28, 33));
				itemInfoBg.setScale(viewScale, viewScale);

				newItem.itemInfo_sprite = itemInfoBg;

				items[i] = newItem;
				itemCount++;
				break;
			}
		}		

		//maybe in future this could be changed if a lot of items flow into the inventory at once
		resetSpritePositions();
	}
	else {
		//error 
		std::cout << "Max items reached. Could not add " << n_item->getName() << ".\n";
	}
}

void Inventory::removeItem(int id) {
	//needs to be tested
	for (int i = 0; i < (int)items.size(); i++) {
		if (items[i].item->getId() == id) {
			inventoryItem empty;
			items[i] = empty;
			itemCount--;
		}		
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
	
	renderSelection = false;
	int currentHover = -1;

	//used later in handle click, could be better?
	mousePosition = mousePos;

	//toolbar hover check
	if (pointInRect(mousePos, hotBarPos, hotBarSize)) {
		renderSelection = true;
		
		//ID of selected item
		int xPosition = (mousePos.x - ((int)hotBarPos.x)) / 16 / (int)viewScale;
		currentHover = xPosition;

		selectionBox.setPosition(
			hotBarPos.x + xPosition * ((hotBarSize.x / inv_width)),
			hotBarPos.y);
	}

	//for rest of inventory	
	if (pointInRect(mousePos, bagBgPos, bagBgSize) && isOpen) {
		renderSelection = true;		

		//combine to make ID of hovered item
		int xPosition = (mousePos.x - ((int)bagBgPos.x)) / 16 / (int)viewScale;
		int yPosition = (mousePos.y - ((int)bagBgPos.y)) / 16 / (int)viewScale;

		currentHover = yPosition * inv_width + xPosition + inv_width;

		selectionBox.setPosition(
			bagBgPos.x + xPosition * ((bagBgSize.x / inv_width)),
			bagBgPos.y + yPosition * ((bagBgSize.y / 2))
		);
	}		
	if (isOpen) {
		if (hoveredItem != currentHover) hoverTime = sf::Time::Zero;
		else hoverTime += hoverClock.restart();
	}

	hoveredItem = currentHover;
}

void Inventory::handleClick(sf::Event& userInput) {
	if (userInput.mouseButton.button == sf::Mouse::Left) {
		if (!isOpen) {
			// select item in toolbar
			if (hoveredItem > -1)selectedItem = hoveredItem;
			
			if (isInToolbar(selectedItem)) {
				resetSelected();
			}
		}
		else {

			//if clicked outside of the inventory
			if (!pointInRect(mousePosition, backgroundPosition, backgroundSize)) {
				Close();
			}
		}
	}
}

void Inventory::handleKeyPress(sf::Event& userInput) {
	//num key select in toolbar
	if (!isOpen) {
		switch (userInput.key.code) {
		case sf::Keyboard::Num1:
			selectedItem = 0;
			break;
		case sf::Keyboard::Num2:
			selectedItem = 1;
			break;
		case sf::Keyboard::Num3:
			selectedItem = 2;
			break;
		case sf::Keyboard::Num4:
			selectedItem = 3;
			break;
		case sf::Keyboard::Num5:
			selectedItem = 4;
			break;
		case sf::Keyboard::Num6:
			selectedItem = 5;
			break;
		case sf::Keyboard::Num7:
			selectedItem = 6;
			break;
		case sf::Keyboard::Num8:
			selectedItem = 7;
			break;
		case sf::Keyboard::Num9:
			selectedItem = 8;
			break;
		case sf::Keyboard::Num0:
			selectedItem = 9;
			break;
		}
		resetSelected();
	}
}

void Inventory::handleScroll(sf::Event& userInput) {
	//scroll select in toolbar
	if (!isOpen && selectedItem > -1 && isInToolbar(selectedItem)) {
		selectedItem += (int)userInput.mouseWheelScroll.delta;
		if (selectedItem == inv_width) selectedItem = inv_width - 1;
		if (selectedItem == -1) selectedItem = 0;
		resetSelected();
	}
}

void Inventory::Render(sf::RenderWindow& window) {

	//render inventory background
	if (isOpen) {		
		window.draw(background);		
		window.draw(bag_bg);
	}	
	window.draw(hotbar_bg);
	if (renderSelection)window.draw(selectionBox);
	if (selectedItem > -1)window.draw(selected);

	//render items
	for (auto i : items) {
		if (isInToolbar(i.id)) 
			window.draw(i.inv_sprite);		
		else {
			if (isOpen) {
				window.draw(i.inv_sprite);				
			}
		}
	}

	//render info if needed
	if (hoveredItem != -1 && isOpen && hoverTime.asSeconds() > .4f) {
		for (size_t i = 0; i < items.size(); i++) {
			if (i == hoveredItem) {				
				items[i].RenderInfo(window);
			}
		}
	}
}
