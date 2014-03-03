#include "Game.h"

Game::Game(int lvl) 
	: //mWindow(sf::VideoMode(640,480),"Tank Battle"), 
	mPlayer(), mTextureT(), mBase(), mTextureB(),mBullet(),pBullet(),eTanks(20),no_such_level(false)
{
	level=lvl;
	gameover=false;
	last_spawn=std::chrono::system_clock::now();
	scoreN=20*(level-1);livesN=3;
	bulletAlive=0;
	mPlayer.setOrigin(sf::Vector2f(16, 16));
	mBase.setOrigin(sf::Vector2f(16, 16));
	mBullet.setOrigin(sf::Vector2f(8,8));
	mUp=false,mDown=false,mLeft=false,mRight=false;
	pos=12;
	aiTargets.push_back(make_pair(7,14));
	aiTargets.push_back(make_pair(10,12));
	aiTargets.push_back(make_pair(12,14));
	//mPlayer.setRadius(40.f);
	if(!font.loadFromFile("Computerfont.ttf"))
	{
		std::cerr << "Error loading *.ttf" << std::endl;
		//return (-1);
	}
	score.setFont(font);
	score.setCharacterSize(20);
	//score.setOrigin(1,1);
	//score.setPosition(600,30);
	score.setColor(sf::Color(255,255,255,255));
	score.setString("Score: "+to_string(scoreN));
	lives.setFont(font);
	lives.setCharacterSize(20);
	lives.setColor(sf::Color(255,255,255,255));
	lives.setString("Lives: 3");

	if(!mTextureT.loadFromFile("tanks.png"))
	{
		printf("Error!");
	}
	if(!mTextureB.loadFromFile("gf.png"))
	{
		printf("Error!");
	}
	if(!pBullet.loadFromFile("bullet_red.png"))
	{
		printf("Error!");
	}
	playerRect.left=0;
	playerRect.top=0;
	playerRect.height=32;
	playerRect.width=32;
	mPlayer.setTexture(mTextureT);
	mPlayer.setTextureRect(playerRect);
	mPlayer.rotate(-90.0f);
	mPlayer.setPosition(204.f,450.f);
	mBase.setTexture(mTextureB);
	mBase.setPosition(320.f,464.f);
	score.setPosition(10,0);
	mBullet.setTexture(pBullet);
	mBullet.setPosition(700,700);
	bulletDir=1;
	//mPlayer.setFillColor(sf::Color::Cyan);
	loadTextures();
	loadMap(level);
}
int Game::run(sf::RenderWindow& App)
{
	lives.setPosition(App.getSize().x-score.getLocalBounds().width-10,0);
	//mWindow=App;
	sf::Time TimePerFrame =sf::seconds(1.f/60.f);
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	spawnEnemy();
	while(App.isOpen())
	{
		if(!processEvents(App)) break;
		if(no_such_level) return 999;
		//if(mapObjects.empty()) return 0;
		timeSinceLastUpdate+=clock.restart();
		while(timeSinceLastUpdate>TimePerFrame)
		{
			timeSinceLastUpdate-=TimePerFrame;
			if(!processEvents(App)) break;
			bool all_killed=true;
			for(auto& i:enemyTanks)
			{
				if(!i->killed){all_killed=false;break;}
			}
			if(all_killed) return 10+(++level);
			if(gameover) return 0;
			if(livesN<0) return 0;
			update(TimePerFrame);
		}
		render(App);
	}
	return 0;
}
int Game::processEvents(sf::RenderWindow& App)
{
	sf::Event event;
	while(App.pollEvent(event))
	{
		switch(event.type)
		{
		case sf::Event::KeyPressed:
			if(!handlePlayerInput(event.key.code,true)) return 0;
			break;
		case sf::Event::KeyReleased:
			handlePlayerInput(event.key.code,false);
			break;
		case sf::Event::Closed:
			return 0;
		}
		//if(event.type==sf::Event::Closed)
		//mWindow.close();
	}
	return 1;
}
int Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed)
{
	switch(key)
	{
	case sf::Keyboard::W:
	case sf::Keyboard::Up:
		mUp=isPressed;
		if(pos!=12)
		{
			if(pos==6) {mPlayer.rotate(180.0f);}
			if(pos==3) {mPlayer.rotate(-90.0f);}
			if(pos==9) {mPlayer.rotate(90.0f);}
		}
		pos=12;
		break;
	case sf::Keyboard::S:
	case sf::Keyboard::Down:
		mDown=isPressed;
		if(pos!=6)
		{
			if(pos==12){mPlayer.rotate(180.0f);}
			if(pos==3){mPlayer.rotate(90.0f);}
			if(pos==9){mPlayer.rotate(-90.0f);}
		}
		pos=6;
		break;
	case sf::Keyboard::A:
	case sf::Keyboard::Left:
		mLeft=isPressed;
		if(pos!=9)
		{
			if(pos==12){mPlayer.rotate(-90.0f);}
			if(pos==3){mPlayer.rotate(180.0f);}
			if(pos==6){mPlayer.rotate(90.0f);}
		}
		pos=9;
		break;
	case sf::Keyboard::D:
	case sf::Keyboard::Right:
		mRight=isPressed;
		if(pos!=3)
		{
			if(pos==12){mPlayer.rotate(90.0f);}
			if(pos==6){mPlayer.rotate(-90.0f);}
			if(pos==9){mPlayer.rotate(180.0f);}
		}
		pos=3;
		break;
	case sf::Keyboard::Escape:
		return 0;
	case sf::Keyboard::End:
		if(!isPressed==false)
		for(auto& i:enemyTanks)
			i->killed=true;
		break;
	case sf::Keyboard::Space:
		if(!bulletAlive)
		{
			mBullet.setPosition(mPlayer.getPosition());
			bulletAlive=1;
			//if(pos==12){bulletDir=1;}
			//if(pos==3){bulletDir=2;}
			//if(pos==6){bulletDir=3;}
			//if(pos==9){bulletDir=4;}
			bulletDir=pos;
		}
		break;
	}
	return 1;
}
void Game::update(sf::Time deltaTime)
{
	float PlayerSpeed=100.f;
	sf::Vector2f movement(0.f,0.f);
	if(mUp) 
	{
		movement.y-=PlayerSpeed;

		if(pos!=12)
		{
			if(pos==6) {mPlayer.rotate(180.0f);}
			if(pos==3) {mPlayer.rotate(-90.0f);}
			if(pos==9) {mPlayer.rotate(90.0f);}
			pos=12;
		}
	}
	else if(mDown)
	{
		movement.y+=PlayerSpeed;
		if(pos!=6)
		{
			if(pos==12){mPlayer.rotate(180.0f);}
			if(pos==3){mPlayer.rotate(90.0f);}
			if(pos==9){mPlayer.rotate(-90.0f);}
			pos=6;
		}
	}
	else if(mLeft)
	{
		movement.x-=PlayerSpeed;
		if(pos!=9)
		{
			if(pos==12){mPlayer.rotate(-90.0f);}
			if(pos==3){mPlayer.rotate(180.0f);}
			if(pos==6){mPlayer.rotate(90.0f);}
			pos=9;
		}
	}
	else if(mRight) 
	{
		movement.x+=PlayerSpeed;
		if(pos!=3)
		{
			if(pos==12){mPlayer.rotate(90.0f);}
			if(pos==6){mPlayer.rotate(-90.0f);}
			if(pos==9){mPlayer.rotate(180.0f);}
			pos=3;
		}

	}

	if(movement.x!=0.f || movement.y!=0.f)
	{
		if(playerRect.left!=224){playerRect.left+=32;}
		else playerRect.left=0;
		mPlayer.setTextureRect(playerRect);

		/*sf::Vector2f tmp=mPlayer.getPosition();*/
		//if(tmp.x+movement.x<0) movement.x=0;//tmp.x=16;
		//if(tmp.y+movement.y<0) movement.y=0;//tmp.y=16;
		//if(tmp.x+movement.x>=624) movement.x=0;//tmp.x=624;
		//if(tmp.y+movement.y>=464) movement.y=0;//tmp.y=464;
	}

	bool move1=true;
	for(auto& i: mapObjects)
	{
		sf::FloatRect temp=mPlayer.getGlobalBounds();
		temp.left+=movement.x*deltaTime.asSeconds();
		temp.top+=movement.y*deltaTime.asSeconds();
		temp.left+=8;
		temp.top+=8;
		temp.height=16;
		temp.width=16;
		if(temp.intersects(i->getGlobalBounds())) move1=false;
	}
	for(auto& i:enemyTanks)
	{
		sf::FloatRect temp=mPlayer.getGlobalBounds();
		temp.left+=movement.x*deltaTime.asSeconds();
		temp.top+=movement.y*deltaTime.asSeconds();
		temp.left+=8;
		temp.top+=8;
		temp.height=16;
		temp.width=16;
		bool canMove=true;
		if(temp.intersects(i->getGlobalBounds())) 
		{
			move1=false;
			//i->pause();
			canMove=false;
		}
		//i->canMove=canMove;
		for(auto& j:enemyTanks)
		{
			if(i==j) continue;
			//sf::FloatRect temp=i->getGlobalBounds();
			//temp.left+=movement.x*deltaTime.asSeconds();
			//temp.top+=movement.y*deltaTime.asSeconds();
			//temp.left+=8;
			//temp.top+=8;
			//temp.height=16;
			//temp.width=16;
			//bool canMove=true;
			if(i->answer.empty()) continue;;
			sf::FloatRect temp;
			temp.left=32*i->answer.top().x;//+16;
			temp.top=32*i->answer.top().y;//+16;
			temp.height=32;
			temp.width=32;

			//if(i->getGlobalBounds().intersects(j->getGlobalBounds())) 
			if(j->getGlobalBounds().intersects(temp))
			{
				//move1=false;
				//i->pause();
				//if(i->canMove && j->canMove)
					canMove=false;
			}
			//j->canMove=canMove;
		}
		i->canMove=canMove;
		//if(mPlayer.getGlobalBounds().intersects(i->getGlobalBounds())) i->pause();
	}
	sf::Vector2f tmp=mPlayer.getPosition();
	if(tmp.x+movement.x*deltaTime.asSeconds()<16) move1=false;//movement.x=0;//tmp.x=16;
	if(tmp.y+movement.y*deltaTime.asSeconds()<16) move1=false;//movement.y=0;//tmp.y=16;
	if(tmp.x+movement.x*deltaTime.asSeconds()>=624) move1=false;//movement.x=0;//tmp.x=624;
	if(tmp.y+movement.y*deltaTime.asSeconds()>=464) move1=false;//movement.y=0;//tmp.y=464;
	if(move1)
	{
		mPlayer.move(movement*deltaTime.asSeconds());
	}
	if(bulletAlive)
	{
		sf::Vector2f bulletMove(0.f,0.f);
		if(bulletDir==12) bulletMove.y-=160;
		else if(bulletDir==3) bulletMove.x+=160;
		else if(bulletDir==6) bulletMove.y+=160;
		else if(bulletDir==9) bulletMove.x-=160;

		sf::Vector2f tmp=mBullet.getPosition();
		if(tmp.x+movement.x*deltaTime.asSeconds()<8) bulletAlive=false;//movement.x=0;//tmp.x=16;
		else if(tmp.y+movement.y*deltaTime.asSeconds()<8) bulletAlive=false;//movement.y=0;//tmp.y=16;
		else if(tmp.x+movement.x*deltaTime.asSeconds()>=632) bulletAlive=false;//movement.x=0;//tmp.x=624;
		else if(tmp.y+movement.y*deltaTime.asSeconds()>=472) bulletAlive=false;//movement.y=0;//tmp.y=464;
		if(!bulletAlive) mBullet.setPosition(700,700);

		mBullet.move(bulletMove*deltaTime.asSeconds());
		//for(auto& i: mapObjects)
		for(int i=0; i<mapObjects.size(); i++)
		{
			if(mBullet.getGlobalBounds().intersects(mapObjects[i]->getGlobalBounds()))
			{
				if(objType[i]!='1')
				{
					mBullet.setPosition(700,700);
					bulletAlive=0;
					mapObjects[i]->setPosition(700,700);
				}
			}
		}

		for(auto& i: enemyTanks)
		{
			if(mBullet.getGlobalBounds().intersects(i->getGlobalBounds()) && i->alive)
			{
				mBullet.setPosition(700,700);
				bulletAlive=0;
				i->setPosition(700,700);
				i->alive=false;
				i->bullet.setPosition(700,700);
				i->bulletAlive=false;
				scoreN++;
				score.setString("Score: "+std::to_string(scoreN));
			}
		}
	}
	for(auto& i: enemyTanks)
	{
		if(i->alive)
		{
			if(i->answer.empty())
			{
				
			}
			i->drive(deltaTime);
			if(!i->bulletAlive)
			{
				i->bulletAlive=true;;
				i->bullet.setPosition(i->getPosition());
				i->bulletDir=i->direction;
			}
			else
			{

				sf::Vector2f bulletMove(0.f,0.f);
				if(i->bulletDir==12) bulletMove.y-=160;
				else if(i->bulletDir==3) bulletMove.x+=160;
				else if(i->bulletDir==6) bulletMove.y+=160;
				else if(i->bulletDir==9) bulletMove.x-=160;

				sf::Vector2f tmp=i->bullet.getPosition();
				if(tmp.x+movement.x*deltaTime.asSeconds()<8) i->bulletAlive=false;//movement.x=0;//tmp.x=16;
				else if(tmp.y+movement.y*deltaTime.asSeconds()<8) i->bulletAlive=false;//movement.y=0;//tmp.y=16;
				else if(tmp.x+movement.x*deltaTime.asSeconds()>=632) i->bulletAlive=false;//movement.x=0;//tmp.x=624;
				else if(tmp.y+movement.y*deltaTime.asSeconds()>=472) i->bulletAlive=false;//movement.y=0;//tmp.y=464;
				if(!i->bulletAlive) i->bullet.setPosition(700,700);

				i->bullet.move(bulletMove*deltaTime.asSeconds());
				//for(auto& i: mapObjects)
				for(int j=0; j<mapObjects.size(); j++)
				{
					if(i->bullet.getGlobalBounds().intersects(mapObjects[j]->getGlobalBounds()))
					{
						if(objType[j]!='1')
						{
							i->bullet.setPosition(700,700);
							i->bulletAlive=0;
							mapObjects[j]->setPosition(700,700);
						}
					}
				}
				/*for(auto& i: enemyTanks)
				{
					if(mBullet.getGlobalBounds().intersects(i->getGlobalBounds()) && i->alive)
					{
						mBullet.setPosition(700,700);
						bulletAlive=0;
						i->setPosition(700,700);
						i->alive=false;
						scoreN++;
						score.setString("Score: "+std::to_string(scoreN));
					}
				}*/
				if(i->bullet.getGlobalBounds().intersects(mPlayer.getGlobalBounds()))
				{
					i->bullet.setPosition(700,700);
					i->bulletAlive=false;
					mPlayer.setPosition(204.f,450.f);
					livesN--;
					lives.setString("Lives: "+std::to_string(livesN));
				}
				if(i->bullet.getGlobalBounds().intersects(mBase.getGlobalBounds())) gameover=true;
			}
		}
	}
	auto currentT=chrono::system_clock::now();
	double res=chrono::duration<double,milli>(currentT-last_spawn).count();
	if(res>5000.)
	{
		last_spawn=currentT;
		spawnEnemy();
	}
	//if(eTanks)
	//gameover=true;
	//for(auto& i: enemyTanks)
	//{
	//	if(!i->killed) gameover=false;
	//}

	//enemyTanks[0]->drive(deltaTime);

}
void Game::render(sf::RenderWindow& App)
{
	App.clear();
	for(auto& i: mapObjects)
	{
		App.draw(*i);
	}
	for(auto& i: enemyTanks)
	{
		App.draw(i->bullet);
		App.draw(*i);
	}
	App.draw(mBullet);
	App.draw(mPlayer);
	App.draw(mBase);
	App.draw(lives);
	App.draw(score);

	App.display();
}

void Game::loadMap(int lvl)
{
	std::stringstream ssf;
	ssf << "level" << lvl << ".mp";
	std::ifstream fs;
	char tmp;
	std::string filename;
	sf::Texture tx;
	fs.open(ssf.str());
	if(!fs.is_open()){no_such_level=true; return;}
	for(int i=0; i<15; i++)
	{
		for(int j=0; j<20; j++)
		{
			fs >> tmp;
			currentMap[i][j]=tmp-'0';
			if(tmp=='0') continue;
			//if(tmp=='1') filename="water.gif";
			//if(tmp=='3') filename="tree.png";
			if(!tx.loadFromFile(filename))
			{
				printf("Error!");
			}
			mapObjects.push_back(new sf::Sprite(*objTextures[tmp-'0'-1]));
			objType.push_back(tmp);
			//mapObjects.push_back(new Obstacle(
			mapObjects[mapObjects.size()-1]->setPosition(j*32,i*32);
		}
		//fs >> tmp;
	}
	fs.close();
}
void Game::loadTextures()
{
	objTextures.push_back(new sf::Texture());
	objTextures[objTextures.size()-1]->loadFromFile("water.gif");
	objTextures.push_back(new sf::Texture());
	objTextures[objTextures.size()-1]->loadFromFile("tree.png ");
	objTextures.push_back(new sf::Texture());
	objTextures[objTextures.size()-1]->loadFromFile("autumntree.png ");
	objTextures.push_back(new sf::Texture());
	objTextures[objTextures.size()-1]->loadFromFile("house.png ");
	objTextures.push_back(new sf::Texture());
	objTextures[objTextures.size()-1]->loadFromFile("brick.png ");

	//enemyTanks
	for(int i=0; i<eTanks; i++)
	{
		AITank* temp=new AITank(currentMap,mTextureT);
		enemyTanks.push_back(temp);
	}
}
void Game::spawnEnemy()
{
	if(eTanks==0) return;
	std::random_device rd;
	std::mt19937 gen(rd());
	uniform_int_distribution<> dis(0,19);
	uniform_int_distribution<> disT(0,aiTargets.size()-1);
	int res;
	do
	{res=dis(gen);}
	while(currentMap[0][res]!=0);

	int target=disT(gen);

	enemyTanks[eTanks-1]->setOrigin(16,16);
	enemyTanks[eTanks-1]->rotate(90);
	enemyTanks[eTanks-1]->setPosition(32*res+16,16);
	enemyTanks[eTanks-1]->think(res,0,aiTargets[target].first,aiTargets[target].second);
	enemyTanks[eTanks-1]->alive=true;
	eTanks--;
}

Game::~Game()
{
	for(int i=enemyTanks.size()-1; i>=0; i--)
	{
		delete enemyTanks[i];
		enemyTanks.pop_back();
	}
	for(int i=mapObjects.size()-1; i>=0; i--)
	{
		delete mapObjects[i];
		mapObjects.pop_back();
	}
	for(int i=objTextures.size()-1; i>=0; i--)
	{
		delete objTextures[i];
		objTextures.pop_back();
	}
}