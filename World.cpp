#include "World.h"

using nlohmann::json;

World::World(){

}

World::World(std::string fileName, sf::Texture texture): tilemapTexture(texture) {
	block.setFillColor(sf::Color::Green);
	block.setSize(sf::Vector2f((float)tileSize, (float)tileSize));

	std::ifstream file(fileName);
	json mapData = json::parse(file);

	tileSize = mapData["tileSize"];
	mapWidth = mapData["mapWidth"];
	mapHeight = mapData["mapHeight"];
	for (int i = 0; i < (int)mapData["layers"].size(); i++) {
		layer newLayer;
		newLayer.name = mapData["layers"][i]["name"];
		newLayer.collider = mapData["layers"][i]["collider"];
		for (int t = 0; t < (int)mapData["layers"][i]["tiles"].size(); t++) {
			tile newTile;
			
			std::string tileID = mapData["layers"][i]["tiles"][t]["id"];
			newTile.id = std::stoi(tileID);
		
			newTile.x = mapData["layers"][i]["tiles"][t]["x"];
			newTile.y = mapData["layers"][i]["tiles"][t]["y"];
			newLayer.tiles.push_back(newTile);
		}
		layers.push_back(newLayer);
	}	

	std::reverse(layers.begin(), layers.end());

	for (layer& layer : layers) {
		layer.tileArray = std::vector<int>(mapHeight * mapWidth, -1);
		layer.collisionArray = std::vector<int>(mapHeight * mapWidth, 0);

		for (const tile& tile : layer.tiles) {
			int arrayPosition = tile.y * mapWidth + tile.x;
			layer.tileArray[arrayPosition] = tile.id;
			layer.collisionArray[arrayPosition] = 1;
		}
	}

 	assert(layers.size() > 0);
}

void World::render(sf::RenderWindow& window) {

	for (const layer& layer : layers) {
		for (int y = 0; y < mapHeight; y++) {
			for (int x = 0; x < mapWidth; x++) {

				int tileId = layer.tileArray[y * mapWidth + x];
				if (tileId > -1) {
					int tileMapWidth = tilemapTexture.getSize().x / tileSize;
					int tileX = tileId % tileMapWidth;
					int tileY = tileId / tileMapWidth;

					sf::Sprite tileSprite;
					tileSprite.setTexture(tilemapTexture);
					tileSprite.setTextureRect({ tileX * tileSize, tileY * tileSize, tileSize, tileSize });

					tileSprite.setPosition((float)(x * tileSize), (float)(y * tileSize));

					window.draw(tileSprite);
				}
			}
		}
	}
}


std::vector<int> World::getWorld() {
	return tileMatrix;
}

std::vector<layer> World::getColliders() {
	std::vector<layer> result;
	for (layer l : layers) 
		if (l.collider)
			result.push_back(l);
	
	return result;
}

int World::getHeight() { return mapHeight; }
int World::getWidth() { return mapWidth; }
int World::getTileSize() { return tileSize; }