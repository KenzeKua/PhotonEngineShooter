#pragma once

#include "GameObject.h"

class MissileObject : public GameObject
{
protected:
	int OwnerNumber = 0;
	float Life;
	float Speed;

	int Target = 0;

public:
	MissileObject();
	~MissileObject();

	void SetOwnerNumber(int Number);
	int GetOwnerNumber();

	void SetMissileLife(float Duration);
	float GetMissileLife();

	void SetMissileSpeed(float Speed);
	float GetMissileSpeed();

	void SetTarget(int PlayerNumber);
	int GetTarget();
};