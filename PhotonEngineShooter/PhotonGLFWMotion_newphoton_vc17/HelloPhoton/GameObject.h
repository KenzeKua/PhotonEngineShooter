#pragma once

#include "Sprite.h"

class GameObject
{
protected:
	Sprite*	m_sprite;
	BlendMode m_blendMode;

	std::string ObjectType;

	Vector m_position;	
	Vector m_velocity;
	Vector m_acceleration;
	Vector m_scale;
	float m_radian = -1;

	Color m_color;

public:	
	GameObject();
	GameObject(Sprite* sprite);
	GameObject(Sprite* sprite, float x, float y);
	~GameObject();

	void setSprite(Sprite* sprite);
	Sprite *GetSprite();

	void setBlendMode(BlendMode blendMode);
	BlendMode GetBlendMode();

	void SetObjectType(std::string ObjectName);
	std::string GetObjectType();

	void setPos(Vector position);
	void setPos(float x, float y);
	Vector getPos();	

	void setVelocity(Vector velocity);
	void setVelocity(float velocityX, float velocityY);
	Vector getVelocity();	

	void setAcceleration(Vector acceleration);
	Vector getAcceleration();

	void setColor(Color color);
	void setColor(float r, float g, float b, float a);
	Color getColor();

	void setScale(Vector scale);
	void setScale(float xScale, float yScale);
	Vector getScale();

	void setRotation(float radian);
	float getRotation();

	bool CheckCollision(GameObject &OtherObject);

	virtual void start();
	virtual void update(double elapsedTime);

	virtual void draw();
};

