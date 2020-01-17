#include "Enemy.h"

Enemy::Enemy()
{
}

void Enemy::SetHealth(float h)
{
	m_health = h;
}

void Enemy::SetSpeed(float s)
{
	m_speed = s;
}

void Enemy::SetAttackPattern(float ap)
{
	m_attackPattern = ap;
}

void Enemy::SetAttackSpeed(float as)
{
	m_attackSpeed = as;
}

float Enemy::GetHealth() const
{
	return m_health;
}

float Enemy::GetSpeed() const
{
	return m_speed;
}

float Enemy::GetAttackPattern() const
{
	return m_attackPattern;
}

float Enemy::GetAttackSpeed() const
{
	return m_attackSpeed;
}
