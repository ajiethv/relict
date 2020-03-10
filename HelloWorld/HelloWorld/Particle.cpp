#include "ECS.h"

bool Particle::Update(float dt)
{
	m_particleLife -= dt;
	if (m_particleLife > 0.f) {
		m_particleVelo = m_particleVelo + (m_particleAcc * dt);
		m_particlePos = m_particlePos + (m_particleVelo * dt);
		m_particleRotation = tan(m_particleVelo.y / m_particleVelo.x);

		ECS::GetComponent<Transform>(m_particle).SetPosition(m_particlePos.x, m_particlePos.y, 90.f);
		ECS::GetComponent<Transform>(m_particle).SetRotationAngleZ(m_particleRotation);

		return true;
	}
	else {
		ECS::DestroyEntity(m_particle);
		return false;
	}
}

void Particle::CreateParticle(int type, vec2 position, vec2 direction)
{
	if (type == 1) {
		{
			auto spark = ECS::CreateEntity();

			//attach components
			ECS::AttachComponent<Transform>(spark);
			ECS::AttachComponent<Sprite>(spark);

			std::string fileName = "Spark.png";

			//set components
			ECS::GetComponent<Sprite>(spark).LoadSprite(fileName, 1, 1);
			ECS::GetComponent<Transform>(spark).SetRotationAngleZ(rand() % 360 * (180.f / PI));
			ECS::GetComponent<Transform>(spark).SetPosition(position.x, position.y, 50.f);

			//set player
			unsigned int bitHolder = EntityIdentifier::TransformBit() | EntityIdentifier::SpriteBit();
			ECS::SetUpIdentifier(spark, bitHolder, "Spark Particle");

			//add variables to vectors
			m_particle = spark;
			m_particlePos = vec2(ECS::GetComponent<Transform>(spark).GetPositionX(), ECS::GetComponent<Transform>(spark).GetPositionY());
			m_particleVelo = vec2(float(rand() % 5 + 8.f) * 10.f * cos(ECS::GetComponent<Transform>(spark).GetRotationAngleZ()), float(rand() % 5 + 8.f) * 10.f * sin(ECS::GetComponent<Transform>(spark).GetRotationAngleZ()));
			m_particleAcc = vec2(0, 0);
			m_particleRotation = ECS::GetComponent<Transform>(spark).GetRotationAngleZ();
			m_particleLife = 0.07f;
		}
	}

	//set the camera to focus on the main player
	ECS::GetComponent<HorizontalScroll>(EntityIdentifier::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()));
	ECS::GetComponent<VerticalScroll>(EntityIdentifier::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()));
}

int Particle::GetParticle() const
{
	return m_particle;
}

vec2 Particle::GetParticlePos() const
{
	return m_particlePos;
}

vec2 Particle::GetParticleVelo() const
{
	return m_particleVelo;
}

vec2 Particle::GetParticleAcc() const
{
	return m_particleAcc;
}

float Particle::GetParticleRotation() const
{
	return m_particleRotation;
}

float Particle::GetParticleLife() const
{
	return m_particleLife;
}

void Particle::SetParticle(int p)
{
	m_particle = p;
}

void Particle::SetParticlePos(vec2 pP)
{
	m_particlePos = pP;
}

void Particle::SetParticleVelo(vec2 pV)
{
	m_particleVelo = pV;
}

void Particle::SetParticleAcc(vec2 pA)
{
	m_particleAcc = pA;
}

void Particle::SetParticleRotation(float pR)
{
	m_particleRotation = pR;
}

void Particle::SetParticleLife(float pL)
{
	m_particleLife = pL;
}