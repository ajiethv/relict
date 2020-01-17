#pragma once

#include "JSON.h"

class Bullet {
public:
	//empty constructor
	Bullet();

	//setters
	void SetDamage(float d);
	void SetSpeed(float s);
	void SetType(int t);
	void SetReflected(bool r);

	//getters
	float GetDamage() const;
	float GetSpeed() const;
	int GetType() const;
	bool GetReflected() const;

private:
	//variables
	float m_damage = 0.f;
	float m_speed = 0.f;
	int m_type = 0;
	bool m_reflected = false;
};

//Sends bullet TO json file
inline void to_json(nlohmann::json& j, const Bullet& bullet)
{
	//Save damage
	j["BulletDamage"] = bullet.GetDamage();
	//Save speed
	j["BulletSpeed"] = bullet.GetSpeed();
	//Save type
	j["BulletType"] = bullet.GetType();
	//Save reflected
	j["BulletReflected"] = bullet.GetReflected();
}

//Reads bullet in FROM json file
inline void from_json(const nlohmann::json& j, Bullet& bullet)
{
	//Set Damage
	bullet.SetDamage(j["BulletDamage"]);
	//Set Speed
	bullet.SetSpeed(j["BulletSpeed"]);
	//Set type
	bullet.SetType(j["BulletType"]);
	//Set reflected
	bullet.SetReflected(j["BulletReflected"]);
}