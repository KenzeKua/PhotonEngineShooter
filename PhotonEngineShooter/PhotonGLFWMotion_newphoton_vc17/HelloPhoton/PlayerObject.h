#pragma once

#include "GameObject.h"
#include "BulletObject.h"
#include "MissileObject.h"

#include <list>

class PlayerObject : public GameObject
{
protected:
	float Life;
	int PlayerNumber = 0;
	Vector MousePosition;

	BulletObject UniqueBullet;
	std::list <BulletObject*> BulletList;

	MissileObject UniqueMissile;
	std::list <MissileObject*> MissileList;

public:
	PlayerObject();
	~PlayerObject();

	void SetLife(float Value);
	float GetLife();

	void SetPlayerNumber(int Number);
	int GetPlayerNumber();

	void SetMousePosition(float MouseX, float MouseY);
	void SetMousePosition(Vector Position);
	Vector GetMousePosition();

	void ShootBullet();
	BulletObject GetBullet();
	std::list <BulletObject*> GetBulletList();
	void SetBulletSprite(Sprite* BulletSprite);
	void SetBulletBlend(BlendMode BlendMode);
	void SetBulletSpeed(float Speed);
	void SetBulletVelocity(Vector BulletVelocity);
	void SetBulletVelocity(float VelocityX, float VelocityY);
	void SetBulletAcceleration(Vector BullectAcceleration);
	void SetBulletScale(Vector Scale);
	void SetBulletScale(float ScaleX, float ScaleY);

	void ShootMissile();
	MissileObject GetMissile();
	std::list <MissileObject*> GetMissileList();
	void SetMissileSprite(Sprite* MissileSprite);
	void SetMissileBlend(BlendMode BlendMode);
	void SetMissileSpeed(float Speed);
	void SetMissileVelocity(Vector MissileVelocity);
	void SetMissileVelocity(float VelocityX, float VelocityY);
	void SetMissileAcceleration(Vector MissileAcceleration);
	void SetMissileScale(Vector Scale);
	void SetMissileScale(float ScaleX, float ScaleY);

	void UpdateBullets(double ElapsedTime, PlayerObject *PlayerList, int TotalPlayers);
	void DrawBullets();

	void UpdateMissiles(double ElapsedTime, PlayerObject *PlayerList, int TotalPlayers);
	void DrawMissiles();
};