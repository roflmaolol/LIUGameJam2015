#pragma once
#include <SFML/Graphics.hpp>
#include <list>
class World;

enum EntityType { et_moving, et_wall, et_floor, et_plant, et_rabbit, et_grater, et_grass};

class Entity
{
public:
    Entity();
	Entity(World* pWorld, EntityType entityType);
    
	virtual void Update();
	virtual void Draw();
	virtual void setTexture(sf::Texture* texture);
	sf::Vector2f getPosition();
	void setPosition(const sf::Vector2f position);
	void setPositionX(float x);
	void setPositionY(float y);
	float getSize();
	void setSize(const float size);
	sf::Vector2f getOrigin();
	void setOrigin(sf::Vector2f origin);

	EntityType _entityType;
	World* _pWorld;
protected:
	sf::Sprite _sprite;
	float _size;
};
