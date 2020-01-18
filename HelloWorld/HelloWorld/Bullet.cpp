#include "Bullet.h"

Bullet::Bullet()
{
}

void Bullet::SetVelocity(vec2 s)
{
	m_velocity = s;
}

void Bullet::SetType(int t)
{
	m_type = t;
}

void Bullet::SetReflected(bool r)
{
	m_reflected = r;
}

vec2 Bullet::GetVelocity() const
{
	return m_velocity;
}

int Bullet::GetType() const
{
	return m_type;
}

bool Bullet::GetReflected() const
{
	return m_reflected;
}
