#pragma once

#include "GameObject.h"

class BulletObject : public GameObject
{
protected:
	int OwnerNumber = 0;
	float Life;
	float Speed;

public:
	BulletObject();
	~BulletObject();

	void SetOwnerNumber(int Number);
	int GetOwnerNumber();

	void SetBulletLife(float Duration);
	float GetBulletLife();

	void SetBulletSpeed(float Speed);
	float GetBulletSpeed();
};