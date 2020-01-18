#include "Enemy.h"

Enemy::Enemy()
{
}

void Enemy::SetHealth(int h)
{
	m_health = h;
}

void Enemy::SetSpeed(float s)
{
	m_speed = s;
}

void Enemy::SetAttackPattern(int ap)
{
	m_attackPattern = ap;
}

void Enemy::SetAttackSpeed(int as)
{
	m_attackSpeed = as;
}

void Enemy::SetType(int t)
{
	m_type = t;
	if (t == 1) {
		m_health = 1;
		m_speed = 0.1f;
		m_attackPattern = 1;
		m_attackSpeed = 30;
	}
}

int Enemy::GetHealth() const
{
	return m_health;
}

float Enemy::GetSpeed() const
{
	return m_speed;
}

int Enemy::GetAttackPattern() const
{
	return m_attackPattern;
}

int Enemy::GetAttackSpeed() const
{
	return m_attackSpeed;
}

int Enemy::GetType() const
{
	return m_type;
}
