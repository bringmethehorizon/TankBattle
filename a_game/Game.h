#include "screens.h"
#include "AITank.h"

class Game: cScreen
{
public:
	Game(int);
	~Game();
	virtual int run(sf::RenderWindow&);
private:
	int processEvents(sf::RenderWindow&);
	void update(sf::Time);
	void render(sf::RenderWindow&);
	int handlePlayerInput(sf::Keyboard::Key, bool);
	void loadMap(int);
	void loadTextures();
	void spawnEnemy();

	bool mUp,mDown,mLeft,mRight;
	int pos;
	//sf::RenderWindow mWindow;
	sf::Sprite mPlayer;
	sf::Sprite mBase;
	sf::Sprite mBullet;
	sf::Texture mTextureT;
	sf::Texture mTextureB;
	sf::IntRect playerRect;
	sf::Texture pBullet;
	sf::Text score;
	int scoreN;
	int bulletDir;
	bool bulletAlive;
	sf::Text lives;
	int livesN;
	sf::Font font;
	int level;
	std::vector<sf::Sprite*> mapObjects;
	std::vector<sf::Texture*> objTextures;
	std::vector<char> objType;
	std::vector<AITank*> enemyTanks;
	std::chrono::time_point<std::chrono::system_clock> last_spawn;
	int eTanks;
	char currentMap[15][20];
	bool gameover;
	vector<pair<int,int>> aiTargets;
	bool no_such_level;
};