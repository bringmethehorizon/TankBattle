#pragma once
#include <SFML/Graphics.hpp>

#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <stack>
#include <functional>
#include <chrono>

using namespace std;


class Node
{
public:
	Node():f(0),g(0),h(0),id(0),parent(0)
	{
	}
	Node(int _x, int _y,int _g,int _h,int _id,int _p,int _px, int _py):
		id(_id),parent(_p),f(_g+_h),g(_g),h(_h),x(_x),y(_y),px(_px),py(_py)
	{
	}
	int f;
	int g;
	int h;
	int id;
	int parent;
	int x,y,px,py;
	bool operator<(const Node& param) const
	{
		return (this->f<param.f);
	}
	bool operator>(const Node& param) const
	{
		return (this->f>param.f);
	}
	friend ostream& operator<<(ostream& os, Node&);
};
//ostream& operator<<(ostream& os, Node& n)
//{
//	cout << n.x << ":" << n.y << endl;
//	return os;
//}
class comp
{
	bool reverse;
public:
	comp(const bool& revparam=true)
	{reverse=revparam;}
	template<class T>
	bool operator()(const T& lhs, const T& rhs) const
	{
		if(reverse) return (lhs>rhs);
		else return (lhs<rhs);
	}
};

class AITank :
	public sf::Sprite
{
public:
	AITank(char (&_m)[15][20],sf::Texture&);
	void pause();
	void drive(sf::Time);
	virtual ~AITank(void);
	void think(int,int,int,int);
	sf::Texture& tx;
	sf::Sprite bullet;
	sf::Texture bulletTex;
	sf::IntRect ir;
	int bulletDir;
	int direction;
	//vector<sf::Vector2f> targets;
	stack<Node> answer;
	std::chrono::time_point<std::chrono::system_clock> last_movement;
	char (&m)[15][20];
	bool bulletAlive;
	bool alive;
	bool canMove;
	bool killed;
};

