#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

class Item
{
private:
	int id;
	bool isStackable{ true };
	std::string name;
	std::string description;
	sf::Sprite sprite;

public:
	Item();
	Item(int i_id, std::string i_name, std::string i_description, sf::Sprite sprite);

	inline const int getId() { return id; }
	inline const std::string getName() { return name; }
	inline const std::string getDescription() { return description; }
	inline const sf::Sprite getSprite() { return sprite; }
	inline const bool canStack() { return isStackable; }

	inline void setScale(float scale) { sprite.setScale(scale, scale); }
	inline void setPosition(sf::Vector2f position) { sprite.setPosition(position); }
	inline void setStackable(bool canStack) { isStackable = canStack; }
};

