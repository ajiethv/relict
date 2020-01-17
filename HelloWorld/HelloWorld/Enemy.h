#pragma once

#include "JSON.h"

class Enemy {
public:
	//empty constructor
	Enemy();

	//setters
	void SetHealth(float h);
	void SetSpeed(float s);
	void SetAttackPattern(float ap);
	void SetAttackSpeed(float as);

	//getters
	float GetHealth() const;
	float GetSpeed() const;
	float GetAttackPattern() const;
	float GetAttackSpeed() const;

private:
	//variables
	float m_health = 0.f;
	float m_speed = 0.f;
	float m_attackPattern = 0.f;
	float m_attackSpeed = 0.f;
};

//Sends enemy TO json file
inline void to_json(nlohmann::json& j, const Enemy& enemy)
{
	//Save health
	j["EnemyHealth"] = enemy.GetHealth();
	//Save speed
	j["EnemySpeed"] = enemy.GetSpeed();
	//Save attack pattern
	j["EnemyAttackPattern"] = enemy.GetAttackPattern();
	//Save attack speed
	j["EnemyAttackSpeed"] = enemy.GetAttackSpeed();
}

//Reads enemy in FROM json file
inline void from_json(const nlohmann::json& j, Enemy& enemy)
{
	//Set Health
	enemy.SetHealth(j["EnemyHealth"]);
	//Set Speed
	enemy.SetSpeed(j["EnemySpeed"]);
	//Set attack pattern
	enemy.SetAttackPattern(j["EnemyAttackPattern"]);
	//set attack speed
	enemy.SetAttackSpeed(j["EnemyAttackSpeed"]);
}