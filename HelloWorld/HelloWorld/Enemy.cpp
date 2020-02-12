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
		m_attackSpeed = 100;
	}
	else if (t == 2) {
		m_health = 10;
		m_speed = 0.f;
		m_attackPattern = 2;
		m_attackSpeed = 100;
	}
	else if (t == 3) {
		m_health = 10;
		m_speed = 0;
		m_attackPattern = 3;
		m_attackSpeed = 100;
	}
	else if (t == 4) {
		m_health = 10;
		m_speed = 0;
		m_attackPattern = 4;
		m_attackSpeed = 100;
	}
	else if (t == 5) {
		m_health = 10;
		m_speed = 0;
		m_attackPattern = 5;
		m_attackSpeed = 100;
	}
	else if (t == 6) {
		m_health = 10;
		m_speed = 0;
		m_attackPattern = 6;
		m_attackSpeed = 100;
	}
	else if (t == 7) {
		m_health = 10;
		m_speed = 0;
		m_attackPattern = 7;
		m_attackSpeed = 100;
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
