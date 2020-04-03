#pragma once

#include "JSON.h"

class Particle {
public:
	//empty constructor
	Particle() { };

	//update particle effects
	bool Update(float dt);

	//create particles
	void CreateParticle(int type, vec2 position, vec2 direction = vec2(0, 0));
	//delete particle
	void DeleteParticle();

	void SetParticle(int p);
	void SetParticlePos(vec2 pP);
	void SetParticleVelo(vec2 pV);
	void SetParticleAcc(vec2 pA);
	void SetParticleRotation(float pR);
	void SetParticleLife(float pL);

	//getters
	int GetParticle() const;
	vec2 GetParticlePos() const;
	vec2 GetParticleVelo() const;
	vec2 GetParticleAcc() const;
	float GetParticleRotation() const;
	float GetParticleLife() const;
	
private:
	int m_particle; //the particle entity number
	vec2 m_particlePos; //the posiiton of the particle
	vec2 m_particleVelo; //the velocity of the particle
	vec2 m_particleAcc; //the acceleration of the particle
	float m_particleRotation; //the rotation of the particle
	float m_particleLife; //how long the particle will be shown for
};