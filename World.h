#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>
#include <iostream>
#include "json.hpp"

struct tile {
	int id;
	int x, y;
};

struct layer {
	bool collider;
	std::string name;
	std::vector<tile> tiles;
	std::vector<int> tileArray;
	std::vector<int> collisionArray;
};

class World
{
public:
	World();
	World(std::string, sf::Texture);
	void render(sf::RenderWindow&);

	std::vector<layer> getColliders();
	std::vector<int> getWorld();
	int getWidth();
	int getHeight();
	int getTileSize();

private:
	sf::Texture tilemapTexture;
	int tileSize{ 50 };
	int mapWidth{ 20 };
	int mapHeight{ 10 };
	std::vector<layer> layers;

	std::vector<int> tileMatrix;
	sf::RectangleShape block;
};

