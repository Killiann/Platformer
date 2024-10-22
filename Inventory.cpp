#include "Inventory.h"

Inventory::Inventory(){}

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
	selectionIndicator = sf::Sprite(tex, sf::IntRect(177, 134, 16, 16));	
	selected = sf::Sprite(tex, sf::IntRect(1, 151, 15, 15));	

	background.setScale(viewScale, viewScale);
	hotbar_bg.setScale(viewScale, viewScale);
	bag_bg.setScale(viewScale, viewScale);
	selectionIndicator.setScale(viewScale, viewScale);
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

	items.resize(maxItems);

	resetHotbarPositions();
	resetSelected();

	heldItemText.init(font, hotBarPos, hotBarSize);
	if (!items[selectedItem].isEmpty()) heldItemText.updateText(items[selectedItem].item->getName());
}

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

void Inventory::resetSelected() {
	//set background graphic
	selected.setPosition(hotBarPos.x + selectedItem * ((hotBarSize.x / 12)), hotBarPos.y + viewScale);

	if (!items[selectedItem].isEmpty()) heldItemText.updateText(items[selectedItem].item->getName());
	else heldItemText.updateText("");
	heldItemText.resetFade();
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

void Inventory::setupInfoBoxTexture(Item* n_item, inventoryItem& newItem) {	
	unsigned int infoBoxW = 32 * viewScale - newItem.infoPadding * 2;
	
	//text boxes for item info
	MultiLineText itemName(n_item->getName(), infoBoxW, itemTitleFontSize, font);
	itemName.setLetterSpacing(1.f);
	newItem.itemName = itemName;

	MultiLineText itemDesc(n_item->getDescription(), infoBoxW, (float)itemDescriptionFontSize, font);
	itemDesc.setLetterSpacing(1.f);
	newItem.itemDescription = itemDesc;	

	unsigned int infoBoxH = newItem.infoPadding * 5 + itemName.getSize().y + itemDesc.getSize().y;

	//setup combined texture to draw to for the background of the info box (variable height)
	sf::RenderTexture combinedTexture;
	combinedTexture.create(32 * viewScale, infoBoxH);

	sf::Sprite topSection(*texture, sf::IntRect(16, 151, 32, 3));
	sf::Sprite middleSection(*texture, sf::IntRect(16, 154, 32, 3));
	sf::Sprite bottomSection(*texture, sf::IntRect(16, 162, 32, 3));

	float middleYScale = (infoBoxH - (topSection.getTextureRect().height + bottomSection.getTextureRect().height) * viewScale) / (middleSection.getTextureRect().height);

	topSection.setScale(viewScale, viewScale);
	middleSection.setScale(viewScale, middleYScale);
	bottomSection.setScale(viewScale, viewScale);

	topSection.setPosition(sf::Vector2f(0, 0));
	middleSection.setPosition(sf::Vector2f(0, 3 * viewScale));
	bottomSection.setPosition(sf::Vector2f(0, 3 * viewScale + middleSection.getTextureRect().height * middleYScale));

	//draw separate parts of the sprite to the RenderTexture
	combinedTexture.clear(sf::Color::Transparent);
	combinedTexture.draw(topSection);
	combinedTexture.draw(middleSection);
	combinedTexture.draw(bottomSection);
	combinedTexture.display();

	newItem.bgTex = sf::Texture(combinedTexture.getTexture());
	
}

void Inventory::addItem(Item* n_item) {	
	//check for max items
	if (itemCount < maxItems) {				
		n_item->setScale(viewScale);		 		
		inventoryItem empty;

		//if stackable, check if item of type already exists in vector
		bool hasStacked = false;

		if (n_item->canStack()) {
			for (size_t i = 0; i < maxItems;i++) {
				if (!items[i].isEmpty()) {
					if (items[i].item->getId() == n_item->getId()) {
						if (items[i].stackCount < items[i].maxStack) {
							items[i].stackCount++;
							items[i].stackInfo.setString(std::to_string(items[i].stackCount));
							hasStacked = true;
							break;
						}
					}
				}
			}
		}

		//if unique item or could not be added to stack
		if (!n_item->canStack() || !hasStacked) {
			//add item to first empty slot
			for (size_t i = 0; i < maxItems; i++) {
				if (items[i].isEmpty()) {
					inventoryItem newItem(i, n_item);
					setupInfoBoxTexture(n_item, newItem); //generates texture of custom height for info background					

					if (n_item->canStack()) {
						//setup stack text
						newItem.stackInfo.setFont(*font);
						newItem.stackInfo.setCharacterSize(20);
						newItem.stackInfo.setFillColor(sf::Color::Black);
						newItem.stackInfo.setOutlineColor(sf::Color::White);
						newItem.stackInfo.setOutlineThickness(2);					
						newItem.stackCount++;
						newItem.stackInfo.setString(std::to_string(newItem.stackCount));
					}

					items[i] = newItem;
					items[i].itemInfo_sprite = sf::Sprite(items[i].bgTex);
					itemCount++;
					break;
				}
			}
		}

		//maybe in future this could be changed for optimisation
		// if a lot of items flow into the inventory at once
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

//to be moved to custom graphics .h
bool pointInRect(sf::Vector2i& point, sf::Vector2f& rectPos, sf::Vector2i& rectSize) {	
	if (point.x > rectPos.x &&
		point.x < rectPos.x + rectSize.x &&
		point.y > rectPos.y &&
		point.y < rectPos.y + rectSize.y)
		return true;
	else return false;
}

void Inventory::update(sf::Vector2i& mousePos) {	
	isHovering = false;
	int currentHover = -1;

	//used later in handle click, could be better?
	mousePosition = mousePos;

	//toolbar hover check
	if (pointInRect(mousePos, hotBarPos, hotBarSize)) {
		isHovering = true;
		
		//ID of selected item
		int xPosition = (mousePos.x - ((int)hotBarPos.x)) / 16 / (int)viewScale;
		currentHover = xPosition;

		selectionIndicator.setPosition(
			hotBarPos.x + xPosition * ((hotBarSize.x / inv_width)),
			hotBarPos.y);
	}

	//for rest of inventory	
	if (isOpen) {
		if (pointInRect(mousePos, bagBgPos, bagBgSize)) {
			isHovering = true;

			//combine to make ID of hovered item
			int xPosition = (mousePos.x - ((int)bagBgPos.x)) / 16 / (int)viewScale;
			int yPosition = (mousePos.y - ((int)bagBgPos.y)) / 16 / (int)viewScale;

			currentHover = yPosition * inv_width + xPosition + inv_width;

			selectionIndicator.setPosition(
				bagBgPos.x + xPosition * ((bagBgSize.x / inv_width)),
				bagBgPos.y + yPosition * ((bagBgSize.y / 2))
			);
		}

		//item info timer
		if (hoveredItem != currentHover) hoverTime = sf::Time::Zero;
		else hoverTime += hoverClock.restart();
	}

	hoveredItem = currentHover;

	//update dragged sprite
	if (draggedItem != -1 && hoveredItem != draggedItem && !items[draggedItem].isEmpty()) {
		if(!isDragging)items[draggedItem].inv_sprite.setColor(sf::Color(255, 255, 255, 128));
		
		isDragging = true;
		draggedItemSprite.setPosition(mousePos.x, mousePos.y);
	}
	else if (isDragging && hoveredItem == draggedItem) {
		items[draggedItem].inv_sprite.setColor(sf::Color(255, 255, 255, 255));
		isDragging = false;
	}

	heldItemText.update();
}

void Inventory::handleClick(sf::Event& userInput) {
	
	if (userInput.mouseButton.button == sf::Mouse::Left) {
		if (isOpen) {
			if (hoveredItem != -1) {
				//if (!items[hoveredItem].isEmpty()) {
					draggedItem = hoveredItem;
					draggedItemSprite = items[draggedItem].inv_sprite;
					draggedItemSprite.setColor(sf::Color(255, 255, 255, 128));
				//}
			}
		}
	}
}



void Inventory::handleClickRelease(sf::Event& userInput) {
	if (userInput.mouseButton.button == sf::Mouse::Left) {
		if (!isOpen) {
			// select item in toolbar
			if (hoveredItem > -1 && selectedItem != hoveredItem) {
				selectedItem = hoveredItem;				

				if (isInToolbar(selectedItem))
					resetSelected();
			}			
		}
		else {
			//if clicked outside of the inventory
			if (!pointInRect(mousePosition, backgroundPosition, backgroundSize)) {
				if (isDragging) StopDragging();
				Close();
			}

			//drop dragged item
			if (isDragging && draggedItem != hoveredItem) {
				if (hoveredItem != -1) {
					items[draggedItem].inv_sprite.setColor(sf::Color(255, 255, 255, 255));
					std::swap(items[draggedItem], items[hoveredItem]);
					resetSpritePositions();					
				}
			}
			if (draggedItem != -1)StopDragging();
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
	else if(isOpen) {
		//swap hotkeyed items
		if (hoveredItem != -1) {
			switch (userInput.key.code) {
			case sf::Keyboard::Num1:
				std::swap(items[0], items[hoveredItem]);
				break;
			case sf::Keyboard::Num2:
				std::swap(items[1], items[hoveredItem]);
				break;
			case sf::Keyboard::Num3:
				std::swap(items[2], items[hoveredItem]);
				break;
			case sf::Keyboard::Num4:
				std::swap(items[3], items[hoveredItem]);
				break;
			case sf::Keyboard::Num5:
				std::swap(items[4], items[hoveredItem]);
				break;
			case sf::Keyboard::Num6:
				std::swap(items[4], items[hoveredItem]);
				break;
			case sf::Keyboard::Num7:
				std::swap(items[5], items[hoveredItem]);
				break;
			case sf::Keyboard::Num8:
				std::swap(items[6], items[hoveredItem]);
				break;
			case sf::Keyboard::Num9:
				std::swap(items[7], items[hoveredItem]);
				break;
			case sf::Keyboard::Num0:
				std::swap(items[8], items[hoveredItem]);
				break;
			
			}

			resetSpritePositions();
		}
	}
}

void Inventory::handleScroll(sf::Event& userInput) {
	//scroll select in toolbar
	if (!isOpen && selectedItem > -1 && isInToolbar(selectedItem)) {
		selectedItem += (int)userInput.mouseWheelScroll.delta;
		if (selectedItem >= inv_width) selectedItem = inv_width - 1;
		if (selectedItem <= -1) selectedItem = 0;
		resetSelected();
	}
}

void Inventory::Render(sf::RenderWindow& window) {

	//render inventory background
	if (isOpen) {		
		window.draw(background);		
		window.draw(hotbar_bg);
		window.draw(bag_bg);
	}	
	else {
		window.draw(hotbar_bg);
		window.draw(heldItemText.text);
		if (selectedItem > -1)window.draw(selected);
	}	
	if (isHovering)window.draw(selectionIndicator);

	//render items
	for (size_t i = 0; i < items.size(); i++) {
		if (isInToolbar(i))
			window.draw(items[i].inv_sprite);
		else {
			if (isOpen) {
				window.draw(items[i].inv_sprite);
			}
		}
		if (items[i].stackable) {
			window.draw(items[i].stackInfo);
		}
	}

	if (isOpen && isDragging) {
		window.draw(draggedItemSprite);
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


