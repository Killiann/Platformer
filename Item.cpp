#include "Item.h"
Item::Item(){
	id = -1;		
}

Item::Item(int i_id, std::string i_name, std::string i_description, sf::Sprite i_sprite) :
	id(i_id),
	name(i_name),
	description(i_description),
	sprite(i_sprite)
{}
