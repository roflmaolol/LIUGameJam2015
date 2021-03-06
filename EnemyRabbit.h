#include "Enemy.h"
class World;

class EnemyRabbit : public Enemy
{
public:
	EnemyRabbit(World* pWorld);
    ~EnemyRabbit();
    void Update();
    void Draw();
    bool aggro();
    void collisionHandling();
    void setInLight(bool inLight);

private:
    int _walkDuration;
    bool _inLight;
    float _speed;
    float _rotation;
    unsigned long int _drawWalkDuration;
};