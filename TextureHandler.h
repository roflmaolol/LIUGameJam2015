#include <SFML/Graphics.hpp>
#include <list>
#include <utility>

enum TextureType { tt_test, tt_player, tt_grater, tt_rabbit, tt_plant, tt_wall, tt_floor, tt_grass};

class TextureHandler
{
public:
	TextureHandler();
	~TextureHandler();
	sf::Texture* getTexture(TextureType type);
    std::string getResourcePath();

private:
	std::list<std::pair<TextureType, sf::Texture*> > _textures;
	std::string _resourcePath;
};
