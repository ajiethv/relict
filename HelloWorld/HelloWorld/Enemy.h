#pragma once

#include "JSON.h"

class Enemy {
public:
	//empty constructor
	Enemy();

	//setters
	void SetHealth(int h);
	void SetSpeed(float s);
	void SetAttackPattern(int ap);
	void SetAttackSpeed(int as);
	void SetType(int t);

	//getters
	int GetHealth() const;
	float GetSpeed() const;
	int GetAttackPattern() const;
	int GetAttackSpeed() const;
	int GetType() const;

private:
	//variables
	int m_health = 0; //enemy health
	float m_speed = 0.f; //enemy speed
	int m_attackPattern = 0; //1 = shoot straight at player, 2 = spiral, 3 grow
	int m_attackSpeed = 0; //enemy fire rate
	int m_type = 0; //sets the other variables to match the enemy type, 1 = basic enemy, 2 = boss 1, 3 = boss 2, etc.
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
	//save enemy type
	j["EnemyType"] = enemy.GetType();
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
	//Set enemy type
	enemy.SetType(j["EnemyType"]);
}