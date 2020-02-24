#include "Stats.h"
#include <iostream>
#include <string>

Stats::Stats()
{
}

Stats::Stats(float health, float stamina)
{
	m_health = health;
	m_stamina = stamina;
}

Stats::~Stats()
{
}

float Stats::GetHealth() const
{
	return m_health;
}

float Stats::GetStamina() const
{
	return m_stamina;
}

void Stats::SetHealth(float health)
{
	m_health = health;
}

void Stats::SetStamina(float stamina)
{
	m_stamina = stamina;
}
