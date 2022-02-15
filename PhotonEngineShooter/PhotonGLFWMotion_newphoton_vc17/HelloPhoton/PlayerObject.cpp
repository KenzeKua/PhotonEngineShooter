#include "PlayerObject.h"

PlayerObject::PlayerObject()
{
	Life = 1.0f;
}

PlayerObject::~PlayerObject()
{
}

void PlayerObject::SetLife(float Value)
{
	Life = Value;
}
float PlayerObject::GetLife()
{
	return Life;
}

void PlayerObject::SetPlayerNumber(int Number)
{
	PlayerNumber = Number;
}
int PlayerObject::GetPlayerNumber()
{
	return PlayerNumber;
}

void PlayerObject::SetMousePosition(float MouseX, float MouseY)
{
	MousePosition.set(MouseX, MouseY, 0);
}
void PlayerObject::SetMousePosition(Vector Position)
{
	MousePosition = Position;
}
Vector PlayerObject::GetMousePosition()
{
	return MousePosition;
}

void PlayerObject::ShootBullet()
{
	BulletObject* NewBullet = new BulletObject();
	NewBullet->SetOwnerNumber(PlayerNumber);
	NewBullet->SetBulletLife(1.0);
	NewBullet->setSprite(UniqueBullet.GetSprite());
	NewBullet->setBlendMode(UniqueBullet.GetBlendMode());
	NewBullet->SetObjectType("Bullet");
	NewBullet->setPos(this->m_position);
	NewBullet->setVelocity(cos(this->m_radian - acos(0.0f)) * UniqueBullet.GetBulletSpeed(),
		sin(this->m_radian - acos(0.0f)) * UniqueBullet.GetBulletSpeed());
	NewBullet->setRotation(this->m_radian);

	BulletList.push_back(NewBullet);
}

BulletObject PlayerObject::GetBullet()
{
	return UniqueBullet;
}
std::list <BulletObject*> PlayerObject::GetBulletList()
{
	return BulletList;
}

void PlayerObject::SetBulletSprite(Sprite* BulletSprite)
{
	UniqueBullet.setSprite(BulletSprite);
}
void PlayerObject::SetBulletBlend(BlendMode BlendMode)
{
	UniqueBullet.setBlendMode(BlendMode);
}

void PlayerObject::SetBulletSpeed(float Speed)
{
	UniqueBullet.SetBulletSpeed(Speed);
}

void PlayerObject::SetBulletVelocity(Vector BulletVelocity)
{
	UniqueBullet.setVelocity(BulletVelocity);
}
void PlayerObject::SetBulletVelocity(float VelocityX, float VelocityY)
{
	UniqueBullet.setVelocity(VelocityX, VelocityY);
}

void PlayerObject::SetBulletAcceleration(Vector BulletAcceleration)
{
	UniqueBullet.setAcceleration(BulletAcceleration);
}

void PlayerObject::SetBulletScale(Vector Scale)
{
	UniqueBullet.setScale(Scale);
}
void PlayerObject::SetBulletScale(float ScaleX, float ScaleY)
{
	UniqueBullet.setScale(ScaleX, ScaleY);
}
//====================================================================
void PlayerObject::ShootMissile()
{
	MissileObject* NewMissile = new MissileObject();
	NewMissile->SetOwnerNumber(PlayerNumber);
	NewMissile->SetMissileLife(10.0);
	NewMissile->setSprite(UniqueMissile.GetSprite());
	NewMissile->setBlendMode(UniqueMissile.GetBlendMode());
	NewMissile->SetObjectType("Missile");
	NewMissile->setPos(this->m_position);
	NewMissile->setVelocity(cos(this->m_radian - acos(0.0f)) * UniqueMissile.GetMissileSpeed(),
		sin(this->m_radian - acos(0.0f)) * UniqueMissile.GetMissileSpeed());
	NewMissile->setRotation(this->m_radian);

	MissileList.push_back(NewMissile);
}

MissileObject PlayerObject::GetMissile()
{
	return UniqueMissile;
}
std::list<MissileObject*> PlayerObject::GetMissileList()
{
	return MissileList;
}

void PlayerObject::SetMissileSprite(Sprite* MissileSprite)
{
	UniqueMissile.setSprite(MissileSprite);
}
void PlayerObject::SetMissileBlend(BlendMode BlendMode)
{
	UniqueMissile.setBlendMode(BlendMode);
}

void PlayerObject::SetMissileSpeed(float Speed)
{
	UniqueMissile.SetMissileSpeed(Speed);
}

void PlayerObject::SetMissileVelocity(Vector MissileVelotiy)
{
	UniqueMissile.setVelocity(MissileVelotiy);
}
void PlayerObject::SetMissileVelocity(float VelocityX, float VelocityY)
{
	UniqueMissile.setVelocity(VelocityX, VelocityY);
}

void PlayerObject::SetMissileAcceleration(Vector MissileAcceleration)
{
	UniqueMissile.setAcceleration(MissileAcceleration);
}

void PlayerObject::SetMissileScale(Vector Scale)
{
	UniqueMissile.setScale(Scale);
}
void PlayerObject::SetMissileScale(float ScaleX, float ScaleY)
{
	UniqueMissile.setScale(ScaleX, ScaleY);
}

void PlayerObject::UpdateBullets(double ElapsedTime, PlayerObject *PlayerList, int TotalPlayers)
{
	if (!BulletList.empty())
	{
		std::list <BulletObject*>::iterator Iterator = BulletList.begin();
		float TempLife = 0.0f;
		while (Iterator != BulletList.end())
		{
			if ((*Iterator)->GetBulletLife() <= 0.0f)
			{
				Iterator = BulletList.erase(Iterator);
			}
			else
			{
				// Update
				(*Iterator)->update(ElapsedTime);

				TempLife = (*Iterator)->GetBulletLife() - (float)ElapsedTime;
				(*Iterator)->SetBulletLife(TempLife);

				// Check collision
				for (int i = 1; i < TotalPlayers + 1; i++)
				{
					if (i == (*Iterator)->GetOwnerNumber())
						continue;
					else if ((*Iterator)->CheckCollision(PlayerList[i]))
					{
						(*Iterator)->SetBulletLife(0.0f);
						PlayerList[i].setColor(1.0f, 0.0f, 0.0f, 1.0f);
						std::cout << "Hit Player " << i << std::endl;
					}
				}
				Iterator++;
			}
		}
	}
}

void PlayerObject::DrawBullets()
{
	std::list <BulletObject*>::iterator Iterator = BulletList.begin();
	while (Iterator != BulletList.end())
	{
		(*Iterator)->draw();
		Iterator++;
	}
}

void PlayerObject::UpdateMissiles(double ElapsedTime, PlayerObject *PlayerList, int TotalPlayers)
{
	if (!MissileList.empty())
	{
		std::list <MissileObject*>::iterator Iterator = MissileList.begin();
		float TempLife = 0.0f;
		while (Iterator != MissileList.end())
		{
			if ((*Iterator)->GetMissileLife() <= 0.0f)
			{
				Iterator = MissileList.erase(Iterator);
			}
			else
			{
				// Find nearest player
				static float TempDistanceA = 0;
				static float TempDistanceB = 0;
				static int TempTarget1 = 0;
				static int TempTarget2 = 0;
				if (TotalPlayers <= 2)
				{
					if (PlayerNumber == 1)
						(*Iterator)->SetTarget(2);
					else
						(*Iterator)->SetTarget(1);
				}
				else
				{
					for (int i = 1; i < TotalPlayers + 1; i++)
					{
						// Initializer filter
						if (i == 1)
						{
							for (int i = 1; i < 9; i++)
							{
								if (i != PlayerNumber)
								{
									TempTarget1 = i;
									TempDistanceA = (float)sqrt(pow(PlayerList[TempTarget1].getPos().mVal[0] - (*Iterator)->getPos().mVal[0], 2) +
										pow(PlayerList[i].getPos().mVal[0] - (*Iterator)->getPos().mVal[1], 2) *
										1.0f);
									break;
								}
							}
							for (int i = 1; i < 9; i++)
							{
								if (i != PlayerNumber || i != TempTarget1)
								{
									TempTarget2 = i;
									TempDistanceB = (float)sqrt(pow(PlayerList[i].getPos().mVal[0] - (*Iterator)->getPos().mVal[0], 2) +
										pow(PlayerList[i].getPos().mVal[1] - (*Iterator)->getPos().mVal[1], 2) *
										1.0f);
								}
							}
							if (TempDistanceA <= TempDistanceB)
								(*Iterator)->SetTarget(TempTarget1);
							else
							{
								(*Iterator)->SetTarget(TempTarget2);
								TempDistanceA = TempDistanceB;
							}
						}
						else if (i == PlayerNumber || i == (*Iterator)->GetTarget())
						{
						}
						else
						{
							TempTarget1 = (*Iterator)->GetTarget();

							TempDistanceB = (float)sqrt(pow(PlayerList[i].getPos().mVal[0] - (*Iterator)->getPos().mVal[0], 2) +
								pow(PlayerList[i].getPos().mVal[1] - (*Iterator)->getPos().mVal[1], 2) *
								1.0f);

							if (TempDistanceA <= TempDistanceB)
								(*Iterator)->SetTarget(TempTarget1);
							else
							{
								(*Iterator)->SetTarget(i);
								TempDistanceA = TempDistanceB;
							}
						}
					}
				}

				// Home the missile
				Vector Direction = PlayerList[(*Iterator)->GetTarget()].getPos() - (*Iterator)->getPos();
				(*Iterator)->setVelocity(Direction * 2);

				// Rotate the missile
				float RadY = ( PlayerList[(*Iterator)->GetTarget()].getPos().mVal[1]) - (*Iterator)->getPos().mVal[1];
				float RadX = PlayerList[(*Iterator)->GetTarget()].getPos().mVal[0] - (*Iterator)->getPos().mVal[0];
				float Radian = atan2(RadY, RadX);
				(*Iterator)->setRotation(Radian);

				// Update
				(*Iterator)->update(ElapsedTime);
				TempLife = (*Iterator)->GetMissileLife() - (float)ElapsedTime;
				(*Iterator)->SetMissileLife(TempLife);

				// Check collision
				for (int i = 1; i < TotalPlayers + 1; i++)
				{
					if (i == (*Iterator)->GetOwnerNumber())
						continue;
					else if ((*Iterator)->CheckCollision(PlayerList[i]))
					{
						(*Iterator)->SetMissileLife(0.0f);
						PlayerList[i].setColor(1.0f, 0.0f, 0.0f, 1.0f);
						std::cout << "Hit Player " << i << std::endl;
					}
				}
				Iterator++;
			}
		}
	}
}

void PlayerObject::DrawMissiles()
{
	std::list <MissileObject*>::iterator Iterator = MissileList.begin();
	while (Iterator != MissileList.end())
	{
		(*Iterator)->draw();
		Iterator++;
	}
}