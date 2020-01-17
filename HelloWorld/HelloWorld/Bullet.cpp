#include "Bullet.h"

Bullet::Bullet()
{
}

void Bullet::SetDamage(float d)
{
	m_damage = d;
}

void Bullet::SetSpeed(float s)
{
	m_speed = s;
}

void Bullet::SetType(int t)
{
	m_type = t;
}

void Bullet::SetReflected(bool r)
{
	m_reflected = r;
}

float Bullet::GetDamage() const
{
	return m_damage;
}

float Bullet::GetSpeed() const
{
	return m_speed;
}

int Bullet::GetType() const
{
	return m_type;
}

bool Bullet::GetReflected() const
{
	return m_reflected;
}
