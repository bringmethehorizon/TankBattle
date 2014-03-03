#include "AITank.h"

int calcH(int xs, int ys, int xe, int ye)
{
	return abs(xs-xe)+abs(ys-ye);
}

AITank::AITank(char (&_m)[15][20], sf::Texture& _tx):
	m(_m),tx(_tx),alive(false),canMove(true)
{
	killed=false;
	direction=6;
	last_movement=chrono::system_clock::now();
	ir.width=32;
	ir.height=32;
	ir.left=0;
	ir.top=32*3;
	this->setTexture(tx);
	this->setTextureRect(ir);
	this->setPosition(700,700);
	if(!bulletTex.loadFromFile("bullet_red.png"))
	{
		printf("Error!");
	}
	bullet.setTexture(bulletTex);
	bullet.setPosition(700,700);
	bulletAlive=false;
	bullet.setOrigin(8,8);
}

void AITank::think(int xs,int ys, int xe, int ye)
{
	while(!answer.empty())
	{
		answer.pop();
	}
	Node start(xs,ys,0,calcH(xs,ys,xe,ye),0,0,-1,-1);
	priority_queue<Node,vector<Node>,comp> q;
	vector<Node> closed;
	int ids=1;
	//Node s(start,0,calcH(start,end),0,0);//, e(end,0,calcH(end,end),100,0);
	//q.push(e);
	//stack<Node> answer;
	q.push(start);
	while(!q.empty())
	{
		Node* current=new Node(q.top());
		q.pop();
		if(current->x==xe && current->y==ye) 
		{answer.push(*current);break;}

		//int x,y;
		//for(int i=0; i<3; i++)
		//	for(int j=0; j<3; j++)
		//		if(current->matrix[i][j]==0) {x=i; y=j;}

		if(current->x-1>=0 && m[current->y][current->x-1]==0 
			&& current->px!=current->x-1)
		{
			//m[current->x-1][current->y]=9;
			Node* tmp=new Node();
			tmp->x=current->x-1;
			tmp->y=current->y;
			tmp->g=current->g+1;
			tmp->h=calcH(current->x-1,current->y,xe,ye);
			tmp->f=tmp->g+tmp->h;
			tmp->parent=current->id;
			tmp->id=ids++;
			tmp->px=current->x;
			tmp->py=current->y;
			bool found=false;
			for(auto& i: closed)
			{
				if(i.x==tmp->x && i.y==tmp->y) {found=true; break;}
			}
			if(!found) q.push(*tmp);
			delete tmp;
		}
		if(current->x+1<20 && m[current->y][current->x+1]==0
			&& current->px!=current->x+1)
		{
			//m[current->x+1][current->y]=9;
			Node* tmp=new Node();
			tmp->x=current->x+1;
			tmp->y=current->y;
			tmp->g=current->g+1;
			tmp->h=calcH(current->x+1,current->y,xe,ye);
			tmp->f=tmp->g+tmp->h;
			tmp->parent=current->id;
			tmp->id=ids++;
			tmp->px=current->x;
			tmp->py=current->y;
			bool found=false;
			for(auto& i: closed)
			{
				if(i.x==tmp->x && i.y==tmp->y) {found=true; break;}
			}
			if(!found) q.push(*tmp);
			delete tmp;
		}
		if(current->y-1>=0 && m[current->y-1][current->x]==0
			&& current->py!=current->y-1)
		{
			//m[current->x][current->y-1]=9;
			Node* tmp=new Node();
			tmp->x=current->x;
			tmp->y=current->y-1;
			tmp->g=current->g+1;
			tmp->h=calcH(current->x,current->y-1,xe,ye);
			tmp->f=tmp->g+tmp->h;
			tmp->parent=current->id;
			tmp->id=ids++;
			tmp->px=current->x;
			tmp->py=current->y;
			bool found=false;
			for(auto& i: closed)
			{
				if(i.x==tmp->x && i.y==tmp->y) {found=true; break;}
			}
			if(!found) q.push(*tmp);
			delete tmp;
		}
		if(current->y+1<15 && m[current->y+1][current->x]==0
			&& current->py!=current->y+1)
		{
			//m[current->x][current->y+1]=9;
			Node* tmp=new Node();
			tmp->x=current->x;
			tmp->y=current->y+1;
			tmp->g=current->g+1;
			tmp->h=calcH(current->x,current->y+1,xe,ye);
			tmp->f=tmp->g+tmp->h;
			tmp->parent=current->id;
			tmp->id=ids++;
			tmp->px=current->x;
			tmp->py=current->y;
			bool found=false;
			for(auto& i: closed)
			{
				if(i.x==tmp->x && i.y==tmp->y) {found=true; break;}
			}
			if(!found) q.push(*tmp);
			delete tmp;
		}


		closed.push_back(*current);
		delete current;
		//cout << q.top().f << endl;
		//q.pop();
	}
	while(1)
	{
		for(auto& i: closed) 
		{
			if(answer.top().parent==i.id) answer.push(i);
		}
		if(answer.top().id==0) break;

	}
}
void AITank::drive(sf::Time deltaTime)
{
	if(!canMove) return;
	sf::Vector2f movement(0,0);
	auto currentT=chrono::system_clock::now();
	double res=chrono::duration<double,milli>(currentT-last_movement).count();
	if(res>10.)
	{
		if(ir.left!=224){ir.left+=32;}
		else ir.left=0;
		this->setTextureRect(ir);
		if(!answer.empty())
		{
			if(this->getPosition().x!=32*answer.top().x+16)
			{
				//this->setPosition(32*answer.top().x+16, 32*answer.top().y+16);
				if(this->getPosition().x<32*answer.top().x+16)
				{
					if(direction!=3)
					{
						if(direction==12) this->rotate(90.);
						if(direction==6) this->rotate(-90.);
						if(direction==9) this->rotate(180.);
					}
					direction=3;
					this->setPosition(this->getPosition().x+1.,this->getPosition().y);
				}
				else
				{
					if(direction!=9)
					{
						if(direction==12) this->rotate(-90.);
						if(direction==6) this->rotate(90.);
						if(direction==3) this->rotate(180.);
					}
					direction=9;
					this->setPosition(this->getPosition().x-1.,this->getPosition().y);
				}
				last_movement=currentT;
			}
			else if(this->getPosition().y!=32*answer.top().y+16)
			{
				//this->setPosition(32*answer.top().x+16, 32*answer.top().y+16);
				if(this->getPosition().y<32*answer.top().y+16)
				{
					if(direction!=6)
					{
						if(direction==12) this->rotate(180.);
						if(direction==9) this->rotate(-90.);
						if(direction==3) this->rotate(90.);
					}
					direction=6;
					this->setPosition(this->getPosition().x,this->getPosition().y+1.);
				}
				else 
				{
					if(direction!=12)
					{
						if(direction==9) this->rotate(90.);
						if(direction==6) this->rotate(180.);
						if(direction==3) this->rotate(-90.);
					}
					direction=12;
					this->setPosition(this->getPosition().x,this->getPosition().y-1.);
				}
				last_movement=currentT;
			}
			else answer.pop();
		}
		else
		{
			//answer.push(Node((int)this->getPosition().x, (int)this->getPosition().y,0,0,0,0,0,0));

		auto currentT=chrono::system_clock::now();
		double res=chrono::duration<double,milli>(currentT-last_movement).count();
		if(res>100.)
		{
			this->rotate(90.);
			last_movement=currentT;
		}
		}
	}
}
void AITank::pause()
{
	last_movement=chrono::system_clock::now();
}
AITank::~AITank(void)
{
}
