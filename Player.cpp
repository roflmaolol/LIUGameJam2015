#include "Player.h"
#include "Utility.h"
#include "World.h"
#include "EnemyRabbit.h"
#include "Textor.h"
#include "TextureHandler.h"
#include <math.h>
#include <iostream>
using namespace std;

#define Acceleration 0.8
#define TurnSpeed 3


Player::Player(World* pWorld) : Entity(pWorld, et_moving)
{
	setTexture(pWorld->_pTextureHandler->getTexture(tt_player));
    
	_rotation = 0;
	_speed = 0;
	_maxSpeed = 3;
	_sprite.setOrigin(32, 32);

	_light = new Light(pWorld, this);
}


Player::~Player()
{
	delete _light;
}


void Player::Draw()
{
	if(!_pWorld->_editorMode)
		_light->draw();
    // animation stuff
    int walk = _walkDuration % 40;
    _sprite.setTextureRect(sf::IntRect((walk-1)/10*64, 0, 64, 64));
	_pWorld->_pRenderTexture->draw(_sprite);
	

	//temp? :)

}


void Player::Update()
{
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		_speed += Acceleration;
	}
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		_speed -= Acceleration;
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		_rotation += TurnSpeed;
	}
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		_rotation -= TurnSpeed;
	}
	if(_speed > 0.1) _walkDuration++;
    if(_walkDuration > 4000000000) _walkDuration = 0;
	_sprite.setRotation(_rotation);
	_speed *= 0.8;

	if(_speed > _maxSpeed)
		_speed = _maxSpeed;

	sf::Vector2f direction(cos(Utility::DtoR(_rotation)), sin(Utility::DtoR(_rotation)));
	Utility::Normalize(direction);	


	sf::Vector2f deltaSpeed(direction.x * _speed, direction.y * _speed);
	sf::Vector2f tempPosX = getPosition();
	sf::Vector2f tempPosY = getPosition();
	tempPosX.x += deltaSpeed.x;
	tempPosY.y += deltaSpeed.y;
    
    sf::Vector2f spawnPosition(500, 700);
	for(Enemy* e : _pWorld->_pLevel->_enemies) {
		//Colission in X
        float angleLightA, angleLightB, angleToEnemy;
        angleToEnemy = Utility::angleComplete(e->getPosition(), getPosition());
        angleLightA = Utility::angleComplete(_light->getVertexArray()[1].position, getPosition());
        angleLightB = Utility::angleComplete(_light->getVertexArray()[2].position, getPosition());
        EnemyRabbit* rabbit = dynamic_cast<EnemyRabbit*>(e);
        if(angleLightA - 5 < angleToEnemy && angleToEnemy < angleLightB + 5) {
            rabbit->setInLight(true);
        }
        else {
            rabbit->setInLight(false);
        }
        
		if(Utility::SSCollision(tempPosX, getOrigin(), getSize(), e->getPosition(), e->getOrigin(), e->getSize()))
		{
			_pWorld->restart();
			return;
		}
		//Colission in Y
		if(Utility::SSCollision(tempPosY, getOrigin(), getSize(), e->getPosition(), e->getOrigin(), e->getSize()))
		{
			_pWorld->restart();
			return;
        }
    }
	for(Entity* e : _pWorld->_pLevel->_entities)
	{
		if(e != nullptr)
		{
			//Colission in X
			if(Utility::SSCollision(tempPosX, getOrigin(), getSize(), e->getPosition(), e->getOrigin(), e->getSize()))
			{
				if(e->_entityType == et_wall)
				{
					if(deltaSpeed.x > 0)
					{
						setPositionX(e->getPosition().x - e->getOrigin().x - getSize() + getOrigin().x - 1);

					}
					else if(deltaSpeed.x < 0)
					{
						setPositionX(e->getPosition().x - e->getOrigin().x + e->getSize() + getOrigin().x + 1);
					}
					deltaSpeed.x = 0;
				}
			}
			//Colission in Y
			if(Utility::SSCollision(tempPosY, getOrigin(), getSize(), e->getPosition(), e->getOrigin(), e->getSize()))
			{
				if(e->_entityType == et_wall)
				{
					if(deltaSpeed.y > 0)
					{
						setPositionY(e->getPosition().y - e->getOrigin().y  - getSize() + getOrigin().y - 1);
					}
					else if(deltaSpeed.y < 0)
					{
						setPositionY(e->getPosition().y - e->getOrigin().y + e->getSize() + getOrigin().y + 1);
					}
					deltaSpeed.y = 0;
				}
			}
		}
	}

	_sprite.move(deltaSpeed);
	//temp
	Utility::vMul(direction, 50);
	_light->update(direction, _rotation);

	sf::View v = _pWorld->_pRenderTexture->getView();
	v.setCenter(getPosition());
	_pWorld->_pRenderTexture->setView(v);

}

void Player::setRotation(float Rotation)
{
	_sprite.setRotation(Rotation);
}
