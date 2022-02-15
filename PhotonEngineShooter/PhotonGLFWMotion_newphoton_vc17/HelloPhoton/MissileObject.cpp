#include "MissileObject.h"

MissileObject::MissileObject()
{
	Life = 0.0f;
	Speed = 200.0f;
}

MissileObject::~MissileObject()
{
}

void MissileObject::SetOwnerNumber(int Number)
{
	OwnerNumber = Number;
}
int MissileObject::GetOwnerNumber()
{
	return OwnerNumber;
}

void MissileObject::SetMissileLife(float Duration)
{
	Life = Duration;
}
float MissileObject::GetMissileLife()
{
	return Life;
}

void MissileObject::SetMissileSpeed(float Speed)
{
	this->Speed = Speed;
}
float MissileObject::GetMissileSpeed()
{
	return Speed;
}

void MissileObject::SetTarget(int PlayerNumber)
{
	Target = PlayerNumber;
}
int MissileObject::GetTarget()
{
	return Target;
}