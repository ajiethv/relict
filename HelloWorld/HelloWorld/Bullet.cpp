#include "Bullet.h"

Bullet::Bullet()
{
}

void Bullet::SetPosition(vec2 p)
{
	m_position = p;
}

void Bullet::SetVelocity(vec2 s)
{
	m_velocity = s;
}

void Bullet::SetAcceleration(vec2 a)
{
	m_acceleration = a;
}

void Bullet::SetScale(vec2 s)
{
	m_scale = s;
}

void Bullet::SetType(int t)
{
	m_type = t;
}

void Bullet::SetReflected(bool r)
{
	m_reflected = r;
}

void Bullet::SetExtra(int e)
{
	m_extra = e;
}

void Bullet::SetSpark(bool s)
{
	m_spark = s;
}

vec2 Bullet::GetPosition() const
{
	return m_position;
}

vec2 Bullet::GetVelocity() const
{
	return m_velocity;
}

vec2 Bullet::GetAcceleration() const
{
	return m_acceleration;
}

vec2 Bullet::GetScale() const
{
	return m_scale;
}

int Bullet::GetType() const
{
	return m_type;
}

bool Bullet::GetReflected() const
{
	return m_reflected;
}

int Bullet::GetExtra() const
{
	return m_extra;
}

bool Bullet::GetSpark() const
{
	return m_spark;
}
