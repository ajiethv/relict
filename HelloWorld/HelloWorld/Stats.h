#pragma once
#include "JSON.h"

class Stats
{
public:
	Stats();
	Stats(float health, float stamina);
	~Stats();

	float GetHealth() const;
	float GetStamina() const;
	void SetHealth(float health);
	void SetStamina(float stamina);
private:
	float m_health = 1.f;
	float m_stamina = 1.f;
};

inline void to_json(nlohmann::json& j, const Stats& bar)
{
	j["Health"] = bar.GetHealth();
	j["Stamina"] = bar.GetStamina();
}

inline void from_json(const nlohmann::json& j, Stats& bar)
{
	bar.SetHealth(j["Health"]);
	bar.SetStamina(j["Stamina"]);
}