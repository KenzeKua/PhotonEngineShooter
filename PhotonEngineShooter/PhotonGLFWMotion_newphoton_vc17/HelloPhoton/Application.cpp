#include "Application.h"
#include "MyPhoton.h"

#include <iostream>

#include <GLFW/glfw3.h>

using namespace std;

// The higher networkFPS, the higher bandwidth requirement of your game.
// ** about 30FPS is common data sync rate for real time games
// ** slower-paced action games could use as low as 10 FPS

const float networkFPS = 30.0f;
const float gNetworkFrameTime = 1.0f / networkFPS;

static float RadX;
static float RadY;
static float Radian;

Application::Application ()
{
	/*
	method 0 : without any interpolation or prediction (send/receive position only)
	method 1 : simple interpolation (send/receive position only)
	method 2 : prediction + interpolation 1 (send/receive position only, data saving hybrid method, 
											 the result is like in between method 1 and method 3)
	method 3 : prediction + interpolation 2 (send/receive position, velocity and acceleration information for more precise prediction, 
  											 3 times higher data bandwidth requirement compare with method 0-2, more responsive than method 2)
	*/

	m_method = 3;
}

Application::~Application ()
{	
}

void Application::networkUpdate()
{
	static double prevTime = glfwGetTime();
	double time = glfwGetTime();

	if (time - prevTime >= gNetworkFrameTime)
	{
		sendMyData();
		prevTime = time;
	}
}

void Application::limitVelAndPos(GameObject* go)
{
	if (go->getVelocity().length() > 200.0f)
	{
		Vector vec = go->getVelocity();
		vec.normalize();
		vec *= 200.0f;
		go->setVelocity(vec);
	}
}

void Application::SetMyPlayerNumber(int PlayerNumber)
{
	if (MyPlayerNumber <= 0)
	{
		MyPlayerNumber = PlayerNumber;
	}
	else {}
}

void Application::SetTotalPlayer(int NumberOfPlayers)
{
	TotalPlayers = NumberOfPlayers;
}

void Application::sendMyData(void)
{
	Vector pos = m_player[MyPlayerNumber].getPos();
	Vector vel = m_player[MyPlayerNumber].getVelocity();
	Vector acc = m_player[MyPlayerNumber].getAcceleration();
	Vector MousePos = m_player[MyPlayerNumber].GetMousePosition();

	MyPhoton::getInstance().sendMyData 
	(
		pos.mVal[0], pos.mVal[1],
		vel.mVal[0], vel.mVal[1],
		acc.mVal[0], acc.mVal[1], 
		MousePos.mVal[0], MousePos.mVal[1] 
	);
}

void Application::SendBulletData(void)
{
	if (!m_player[MyPlayerNumber].GetBulletList().empty())
	{
		std::list <BulletObject*> TempBulletList = m_player[MyPlayerNumber].GetBulletList();
		std::list <BulletObject*>::iterator Iterator = TempBulletList.begin();
		while (Iterator != TempBulletList.end())
		{
			Iterator++;
		}
	}
}

void Application::OnReceivedOpponentData(int PlayerNumber, float* data)
{
	if (m_gameStarted == false)
	{
		m_gameStarted = true;
		m_player[PlayerNumber].setPos(Vector(data[0], data[1], 0));

		m_lastReceivedPos[PlayerNumber] = m_player[PlayerNumber].getPos();
		LastMousePosition[PlayerNumber] = m_player[PlayerNumber].GetMousePosition();
		m_prevReceivedTime = glfwGetTime();

		return;
	}

	LastMousePosition[PlayerNumber] = Vector(data[6], data[7], 0);

	if (m_method == 0 || m_method == 1)
	{
		m_lastReceivedPos[PlayerNumber] = Vector(data[0], data[1], 0);
	}
	else if (m_method == 2)
	{
		double currentTime = glfwGetTime();
		double timeSinceLastReceive = currentTime - m_prevReceivedTime;

		if (timeSinceLastReceive >= gNetworkFrameTime * 0.5) //filter the noise
		{
			Vector targetPos = Vector(data[0], data[1], 0.0f);

			float length = (m_lastReceivedPos[PlayerNumber] - targetPos).length() / (float)timeSinceLastReceive;
			Vector goVec = targetPos - m_player[PlayerNumber].getPos();
			goVec.normalize();

			goVec = goVec * length;

			Vector finalVel = m_player[PlayerNumber].getVelocity() * 0.6f + goVec * 0.4f;
			m_player[PlayerNumber].setVelocity(finalVel);

			m_lastReceivedPos[PlayerNumber] = targetPos;
			m_prevReceivedTime = currentTime;
		}
	}
	else if (m_method == 3)
	{
		m_lastReceivedPos[PlayerNumber] = Vector(data[0], data[1], 0);
		m_player[PlayerNumber].setVelocity(Vector(data[2], data[3], 0));
		m_player[PlayerNumber].setAcceleration(Vector(data[4], data[5], 0));
	}
}

void Application::ReceiveEnemyShoot(int PlayerNumber, int EventCode)
{
	// Recalibrate enemy position for accuracy
	m_player[PlayerNumber].setPos(m_lastReceivedPos[PlayerNumber]);
	// Recalibrate enemy mouse position
	m_player[PlayerNumber].SetMousePosition(LastMousePosition[PlayerNumber]);
	// Recalibrate enemy rotation
	RadY = (600 - m_player[PlayerNumber].GetMousePosition().mVal[1]) - m_player[PlayerNumber].getPos().mVal[1];
	RadX = m_player[PlayerNumber].GetMousePosition().mVal[0] - m_player[PlayerNumber].getPos().mVal[0];
	Radian = atan2(RadY, RadX);
	m_player[PlayerNumber].setRotation(Radian);

	if (EventCode == 2)
	{
		m_player[PlayerNumber].ShootBullet();
	}
	else if (EventCode == 3)
	{
		m_player[PlayerNumber].ShootMissile();
	}
}

void Application::start ()
{
	srand((unsigned int)time(0));

	m_gameStarted = false;
	
	MyPhoton::getInstance().connect();

	m_arrow = new Sprite ("../media/Arrow.bmp");
	Arrow[0] = new Sprite ("../media/Arrow1.bmp");
	Arrow[1] = new Sprite ("../media/Arrow2.bmp");
	Arrow[2] = new Sprite ("../media/Arrow3.bmp");
	Arrow[3] = new Sprite ("../media/Arrow4.bmp");
	Arrow[4] = new Sprite ("../media/Arrow5.bmp");
	Arrow[5] = new Sprite ("../media/Arrow6.bmp");
	Arrow[6] = new Sprite ("../media/Arrow7.bmp");
	Arrow[7] = new Sprite ("../media/Arrow8.bmp");
	BulletSprite = new Sprite ("../media/Bullet.bmp");
	MissileSprite = new Sprite("../media/Missile.bmp");

	for (int i = 1; i < 9; i++)
	{
		m_player[i].SetPlayerNumber(i);
		m_player[i].setSprite(Arrow[i - 1]);
		m_player[i].setBlendMode(ADDITIVE);
		m_player[i].SetObjectType("Player");
		m_player[i].setPos(Vector(rand() % 752 + 24.0f, rand() % 552 + 24.0f, 0));
		m_player[i].setRotation(0);
		// Bullet settings
		m_player[i].SetBulletSprite(BulletSprite);
		m_player[i].SetBulletBlend(ADDITIVE);
		// Missile settings
		m_player[i].SetMissileSprite(m_arrow);
		m_player[i].SetMissileBlend(ADDITIVE);

		m_lastReceivedPos[i] = m_player[i].getPos();
	}
}

void Application::update (double elapsedTime)
{
	MyPhoton::getInstance().run();

	if(!m_gameStarted)
		return;

	// Time lock to update color
	static double PrevTime = glfwGetTime();
	double Time = glfwGetTime();
	if (Time - PrevTime >= 1.0)
	{
		for (int i = 1; i < 9; i++)
		{
			m_player[i].setColor(1.0f, 1.0f, 1.0f, 1.0f);
		}
		PrevTime = Time;
	}

	for (int i = 1; i < 9; i++)
	{
		// Update Players' bullets
		m_player[i].UpdateBullets(elapsedTime, m_player, TotalPlayers);
		// Update Players' missiles
		m_player[i].UpdateMissiles(elapsedTime, m_player, TotalPlayers);

		// Update Players' rotation base on mouse cursors		
		RadY = (600 - m_player[i].GetMousePosition().mVal[1]) - m_player[i].getPos().mVal[1];
		RadX = m_player[i].GetMousePosition().mVal[0] - m_player[i].getPos().mVal[0];
		Radian = atan2(RadY, RadX);
		m_player[i].setRotation(Radian);
	}
	
	// Update MyPlayer
	m_player[MyPlayerNumber].update(elapsedTime);
	m_player[MyPlayerNumber].setAcceleration(Vector(0.0f, 0.0f, 0.0f));
	limitVelAndPos(&m_player[MyPlayerNumber]);
	
	networkUpdate();

	if(m_method == 0)
	{
		for (int i = 1; i < 9; i++)
		{
			if (i == MyPlayerNumber)
				continue;

			m_player[i].setPos(m_lastReceivedPos[i]); 
			m_player[i].SetMousePosition(LastMousePosition[i]);
		}
	}
	else if(m_method == 1)
	{
		for (int i = 1; i < 9; i++)
		{
			if (i == MyPlayerNumber)
				continue;

			m_player[i].setPos(m_player[i].getPos() * 0.96f + m_lastReceivedPos[i] * 0.04f);
			m_player[i].SetMousePosition(m_player[i].GetMousePosition() * 0.7f + LastMousePosition[i] * 0.3f);
		}
	}
	else if(m_method == 2)
	{
		for (int i = 1; i < 9; i++)
		{
			if (i == MyPlayerNumber)
				continue;

			m_player[i].update(elapsedTime);
			m_player[i].setAcceleration(Vector(0.0f, 0.0f, 0.0f));

			m_player[i].SetMousePosition(m_player[i].GetMousePosition() * 0.6f + LastMousePosition[i] * 0.4f);

			limitVelAndPos(&m_player[i]);
		}
	}
	else if(m_method == 3)
	{
		for (int i = 1; i < 9; i++)
		{
			if (i == MyPlayerNumber)
				continue;

			m_player[i].update(elapsedTime);

			// Very slowly interpolate from on-going predicting pos to last received pos. Without this interpolation, 
			// the offset of opponent position will keep being accumulated.
			m_player[i].setPos(m_player[i].getPos() * 0.995f + m_lastReceivedPos[i] * 0.005f);
			m_player[i].SetMousePosition(m_player[i].GetMousePosition() * 0.5f + LastMousePosition[i] * 0.5f);

			limitVelAndPos(&m_player[i]);
		}
	}
}

void Application::draw()
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(!m_gameStarted)
		return;

	for (int i = 1; i < (TotalPlayers + 1); i++)
	{
		m_player[i].draw();
		m_player[i].DrawBullets();
		m_player[i].DrawMissiles();
	}
}

void Application::onKeyPressed (int key)
{
}
void Application::onKeyReleased (int key)
{
}
void Application::onKeyHold (int key)
{
	if(!m_gameStarted)
		return;
	
	if (key == GLFW_KEY_W)
	{
		m_player[MyPlayerNumber].setAcceleration(m_player[MyPlayerNumber].getAcceleration() + Vector(0.0f, 500.0f, 0.0f));
	}
	if(key == GLFW_KEY_A)									    
	{
		m_player[MyPlayerNumber].setAcceleration(m_player[MyPlayerNumber].getAcceleration() + Vector(-500.0f, 0.0f, 0.0f));
	}
	if(key == GLFW_KEY_S)									    
	{
		m_player[MyPlayerNumber].setAcceleration(m_player[MyPlayerNumber].getAcceleration() + Vector(0.0f, -500.0f, 0.0f));
	}
	if(key == GLFW_KEY_D)									    
	{
		m_player[MyPlayerNumber].setAcceleration(m_player[MyPlayerNumber].getAcceleration() + Vector(500.0f, 0.0f, 0.0f));
	}
	
	/*if(key == GLFW_KEY_W)
	{
		m_object1.setVelocity(Vector(0.0f, 400.0f, 0.0f));
	}
	if(key == GLFW_KEY_A)
	{
		m_object1.setVelocity(Vector(-400.0f, 0.0f, 0.0f));
	}
	if(key == GLFW_KEY_S)
	{
		m_object1.setVelocity(Vector(0.0f, -400.0f, 0.0f));
	}
	if(key == GLFW_KEY_D)
	{
		m_object1.setVelocity(Vector(400.0f, 0.0f, 0.0f));
	}
	*/
}

void Application::onMousePressed (GLFWwindow *window, int button)
{
	// Shoot bullet
	if (button == GLFW_MOUSE_BUTTON_1)
	{
		m_player[MyPlayerNumber].ShootBullet();

		MyPhoton::getInstance().SendShootBullet(true);
	}
	// Shoot missile
	else if (button == GLFW_MOUSE_BUTTON_2)
	{
		m_player[MyPlayerNumber].ShootMissile();

		MyPhoton::getInstance().SendShootMissile(true);
	}
}
void Application::onMouseReleased (int button)
{
}
void Application::onMouseHold (int button)
{
}
void Application::onMouseMoved (double mousePosX, double mousePosY)
{
	//std::cout << "Mouse X: " << mousePosX << " | Mouse Y: " << mousePosY << std::endl;

	m_player[MyPlayerNumber].SetMousePosition((float)mousePosX, (float)mousePosY);
}