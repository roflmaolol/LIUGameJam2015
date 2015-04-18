#include "EnemyRabbit.h"
#include "World.h"
#include "Player.h"
#include "Utility.h"
#include <stdlib.h>
#include <math.h>

EnemyRabbit::EnemyRabbit(World* pWorld) : Enemy(pWorld, et_rabbit) {
	_speed = 0.5;
    _rotation = -90;
}

void EnemyRabbit::Update() {
	if(aggro()) {
        _rotation = Utility::angle(_pWorld->_pPlayer->getPosition(), getPosition()) - 90;
        _speed = 1.0;
    }
    else {
        _speed = 0.5;
        
        if(_walkDuration <= 0) {
            int direction = rand() % 4 + 1;
            _rotation = direction * 90 - 180;
            _walkDuration = 400;
        }
        _walkDuration--;
    }
    
    collisionHandling();
    
    
    sf::Vector2f directionVector(cos(Utility::DtoR(_rotation+90)), sin(Utility::DtoR(_rotation+90)));
    Utility::Normalize(directionVector);
    _sprite.move(directionVector * _speed);
}

bool EnemyRabbit::aggro() {
    sf::Vector2f diff = _pWorld->_pPlayer->getPosition() - getPosition();
    float length = fabs(sqrt(pow(diff.x, 2) + pow(diff.y, 2)));
    
    if(length < 9001) {
        return true;
    }
    return false;
}

void EnemyRabbit::collisionHandling() {
    _sprite.setRotation(_rotation);
    
	sf::Vector2f direction(cos(Utility::DtoR(_rotation+90)), sin(Utility::DtoR(_rotation+90)));
	Utility::Normalize(direction);
    
    
	sf::Vector2f deltaSpeed(direction.x * _speed, direction.y * _speed);
	sf::Vector2f tempPosX = getPosition();
	sf::Vector2f tempPosY = getPosition();
	tempPosX.x += deltaSpeed.x;
	tempPosY.y += deltaSpeed.y;
    
    int short spacing = 2;
    for(Entity* e : _pWorld->_level->_entities)
	{
		//Colission in X
		if(Utility::SSCollision(tempPosX, getOrigin(), getSize(), e->getPosition(), e->getOrigin(), e->getSize()))
		{
			if(e->_entityType == et_wall)
			{
				if(deltaSpeed.x > 0) {
					setPositionX(e->getPosition().x - e->getOrigin().x - getSize() + getOrigin().x - spacing);
				}
				else if(deltaSpeed.x < 0) {
					setPositionX(e->getPosition().x - e->getOrigin().x + e->getSize() + getOrigin().x + spacing);
				}
				deltaSpeed.x = 0;
			}
		}
		//Colission in Y
		if(Utility::SSCollision(tempPosY, getOrigin(), getSize(), e->getPosition(), e->getOrigin(), e->getSize()))
		{
			if(e->_entityType == et_wall)
			{
				if(deltaSpeed.y > 0) {
					setPositionY(e->getPosition().y - e->getOrigin().y  - getSize() + getOrigin().y - spacing);
				}
				else if(deltaSpeed.y < 0) {
					setPositionY(e->getPosition().y - e->getOrigin().y + e->getSize() + getOrigin().y + spacing);
				}
				deltaSpeed.y = 0;
			}
		}
	}
}