#pragma once

#include "JSON.h"
#include "Vector.h"

class Bullet {
public:
	//empty constructor
	Bullet();

	//setters
	void SetVelocity(vec2 s);
	void SetAcceleration(vec2 a);
	void SetType(int t);
	void SetReflected(bool r);
	void SetExtra(int e);

	//getters
	vec2 GetVelocity() const;
	vec2 GetAcceleration() const;
	int GetType() const;
	bool GetReflected() const;
	int GetExtra() const;

private:
	//variables
	vec2 m_velocity = vec2(0, 0); //current bullet velocity
	vec2 m_acceleration = vec2(0, 0); //current bullet acceleration
	int m_type = 1; // 1 = not reflectable, 2 = breakable, 3 = reflectable
	bool m_reflected = false; // whether or not the bullet was reflected (only reflected bullets can hit an enemy)
	int m_extra = 0; // any extra abilities the bullet has, 0 = nothing, 1 = explode, 2 = move?
};

//Sends bullet TO json file
inline void to_json(nlohmann::json& j, const Bullet& bullet)
{
	//Save speed
	j["BulletVelocityX"] = bullet.GetVelocity().x;
	j["BulletVelocityY"] = bullet.GetVelocity().y;
	//save acceleration
	j["BulletAccelerationX"] = bullet.GetAcceleration().x;
	j["BulletAccelerationY"] = bullet.GetAcceleration().y;
	//Save type
	j["BulletType"] = bullet.GetType();
	//Save reflected
	j["BulletReflected"] = bullet.GetReflected();
	//Save extra
	j["BulletExtra"] = bullet.GetExtra();
}

//Reads bullet in FROM json file
inline void from_json(const nlohmann::json& j, Bullet& bullet)
{
	//Set Speed
	bullet.SetVelocity(vec2(j["BulletVelocityX"], j["BulletVelocityY"]));
	//set Acceleration
	bullet.SetAcceleration(vec2(j["BulletAccelerationX"], j["BulletAccelerationY"]));
	//Set type
	bullet.SetType(j["BulletType"]);
	//Set reflected
	bullet.SetReflected(j["BulletReflected"]);
	//Set extra
	bullet.SetExtra(j["BulletExtra"]);
}