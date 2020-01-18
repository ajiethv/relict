#pragma once

#include "JSON.h"
#include "Vector.h"

class Bullet {
public:
	//empty constructor
	Bullet();

	//setters
	void SetVelocity(vec2 s);
	void SetType(int t);
	void SetReflected(bool r);

	//getters
	vec2 GetVelocity() const;
	int GetType() const;
	bool GetReflected() const;

private:
	//variables
	vec2 m_velocity = vec2(0, 0); //current bullet velocity
	int m_type = 1; // 1 = not reflectable, 2 = breakable, 3 = reflectable
	bool m_reflected = false; // whether or not the bullet was reflected (only reflected bullets can hit an enemy)
};

//Sends bullet TO json file
inline void to_json(nlohmann::json& j, const Bullet& bullet)
{
	//Save speed
	j["BulletVelocityX"] = bullet.GetVelocity().x;
	j["BulletVelocityY"] = bullet.GetVelocity().y;
	//Save type
	j["BulletType"] = bullet.GetType();
	//Save reflected
	j["BulletReflected"] = bullet.GetReflected();
}

//Reads bullet in FROM json file
inline void from_json(const nlohmann::json& j, Bullet& bullet)
{
	//Set Speed
	bullet.SetVelocity(vec2(j["BulletVelocityX"], j["BulletVelocityY"]));
	//Set type
	bullet.SetType(j["BulletType"]);
	//Set reflected
	bullet.SetReflected(j["BulletReflected"]);
}