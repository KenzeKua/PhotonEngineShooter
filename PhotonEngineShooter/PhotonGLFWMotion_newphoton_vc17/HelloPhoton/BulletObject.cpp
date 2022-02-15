#include "BulletObject.h"

BulletObject::BulletObject()
{
	Life = 0.0f;
	Speed = 500.0f;
}

BulletObject::~BulletObject()
{
}

void BulletObject::SetOwnerNumber(int Number)
{
	OwnerNumber = Number;
}
int BulletObject::GetOwnerNumber()
{
	return OwnerNumber;
}

void BulletObject::SetBulletLife(float Duration)
{
	Life = Duration;
}
float BulletObject::GetBulletLife()
{
	return Life;
}

void BulletObject::SetBulletSpeed(float Speed)
{
	this->Speed = Speed;
}
float BulletObject::GetBulletSpeed()
{
	return Speed;
}