#pragma once

#include "JSON.h"
#include "Vector.h"

class Bullet {
public:
	//empty constructor
	Bullet();

	//setters
	void SetPosition(vec2 p);
	void SetVelocity(vec2 s);
	void SetAcceleration(vec2 a);
	void SetScale(vec2 s);
	void SetType(int t);
	void SetReflected(bool r);
	void SetExtra(int e);

	//getters
	vec2 GetPosition() const;
	vec2 GetVelocity() const;
	vec2 GetAcceleration() const;
	vec2 GetScale() const;
	int GetType() const;
	bool GetReflected() const;
	int GetExtra() const;

private:
	//variables
	vec2 m_position = vec2(0, 0); //current bullet position
	vec2 m_velocity = vec2(0, 0); //current bullet velocity
	vec2 m_acceleration = vec2(0, 0); //current bullet acceleration
	vec2 m_scale = vec2(3.f, 3.f);
	int m_type = 1; // 1 = not reflectable, 2 = breakable, 3 = reflectable
	bool m_reflected = false; // whether or not the bullet was reflected (only reflected bullets can hit an enemy)
	int m_extra = 0; // any extra abilities the bullet has, 0 = nothing, 1 = explode, 2 = move?
};

//Sends bullet TO json file
inline void to_json(nlohmann::json& j, const Bullet& bullet)
{
	//Save position
	j["BulletPositionX"] = bullet.GetPosition().x;
	j["BulletPositionY"] = bullet.GetPosition().y;
	//Save speed
	j["BulletVelocityX"] = bullet.GetVelocity().x;
	j["BulletVelocityY"] = bullet.GetVelocity().y;
	//save acceleration
	j["BulletAccelerationX"] = bullet.GetAcceleration().x;
	j["BulletAccelerationY"] = bullet.GetAcceleration().y;
	//Save scale
	j["BulletScaleX"] = bullet.GetScale().x;
	j["BulletScaleY"] = bullet.GetScale().y;
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
	//Set position
	bullet.SetPosition(vec2(j["BulletPositionX"], j["BulletPositionY"]));
	//Set Speed
	bullet.SetVelocity(vec2(j["BulletVelocityX"], j["BulletVelocityY"]));
	//set Acceleration
	bullet.SetAcceleration(vec2(j["BulletAccelerationX"], j["BulletAccelerationY"]));
	//Set scale
	bullet.SetScale(vec2(j["BulletScaleX"], j["BulletScaleY"]));
	//Set type
	bullet.SetType(j["BulletType"]);
	//Set reflected
	bullet.SetReflected(j["BulletReflected"]);
	//Set extra
	bullet.SetExtra(j["BulletExtra"]);
}