#pragma once
#include "Sprite.h"
#include "GameObject.h"
#include "PlayerObject.h"

#include <list>

class Application
{
protected:
	int m_method;

	Sprite*		m_arrow;
	Sprite*		Arrow[8];
	Sprite*		BulletSprite;
	Sprite*		MissileSprite;

	int MyPlayerNumber = 0;
	int TotalPlayers = 0;
	PlayerObject m_player[9];

	Vector m_lastReceivedPos[9];
	Vector LastMousePosition[9];
	double m_prevReceivedTime;
	bool m_gameStarted;

	void networkUpdate();

	void limitVelAndPos(GameObject* go);

public:
	Application();
	~Application();

	static Application& getInstance()
	{
		static Application app;
		return app;
	}

	void SetMyPlayerNumber(int PlayerNumber);
	void SetTotalPlayer(int NumberOfPlayers);

	void sendMyData(void);
	void SendBulletData(void);

	void OnReceivedOpponentData(int PlayerNumber, float* data);
	void ReceiveEnemyShoot(int PlayerNumber, int EventCode);

	virtual void start();	
	virtual void update(double elapsedTime);
	virtual void draw();

	virtual void onKeyPressed(int key);
	virtual void onKeyReleased(int key);
	virtual void onKeyHold(int key);

	virtual void onMousePressed(GLFWwindow *window, int button);
	virtual void onMouseReleased(int button);
	virtual void onMouseHold(int button);
	virtual void onMouseMoved(double mousePosX, double mousePosY);
};

