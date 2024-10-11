#pragma once
#include <SFML/Graphics.hpp>
#include "World.h"
#include <iostream>

class Player
{
public:Player(sf::RenderWindow& window, World& world);
	  void update(sf::Time);
	  void processInput(sf::Keyboard::Key, bool);	  
	  void render(sf::RenderWindow &);

private:
	//world
	World& world;

	//graphics
	sf::RectangleShape shape;
	
	//physics
	sf::Vector2f pVelocity;
	const float movementSpeed{ 100.f };
	float movingUp{ false }, movingDown{ false }, movingLeft{ false }, movingRight{ false };	
	
	void handleCollisions(sf::Time&);	
};

