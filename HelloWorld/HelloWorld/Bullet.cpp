#include "Bullet.h"

Bullet::Bullet()
{
}

void Bullet::SetVelocity(vec2 s)
{
	m_velocity = s;
}

void Bullet::SetAcceleration(vec2 a)
{
	m_acceleration = a;
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

vec2 Bullet::GetVelocity() const
{
	return m_velocity;
}

vec2 Bullet::GetAcceleration() const
{
	return m_acceleration;
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
