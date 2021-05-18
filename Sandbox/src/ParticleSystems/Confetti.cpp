#include "Glados.h"
#include "Confetti.h"

using namespace Glados;
using namespace glm;
using namespace gdm;

void Confetti::CreateParticles(int size)
{
	for (int i = 0; i < size && m_Particles.size() < m_Specs.MaxParticles; i++)
	{
		vec3 vel = Random::RandomUnitSphere() * .5f;
		vec3 pos = Random::RandomUnitCube();
		vec3 acc = vec3(0, -1, 0);
		float lifetime = Random::RandomFloat(5, 10);
		vec4 col = vec4(Random::RandomVec3(), 1.0f);
		m_Particles.push_back(new Particle{ lifetime, pos, vec3(0), vec3(.25), vel, acc, col, 1 });
	}
}

void Confetti::Update(float dt)
{
	// update particles
	for (uint32_t i = 0; i < m_Particles.size(); i++)
	{
		Particle& p = *(Particle*)m_Particles[i];

		// death condition
		p.age += dt;
		if (p.age > p.GetLifetime() && p.GetLifetime() > 0)
		{
			p.color += glm::vec4(1, 1, 1, -1) * dt;
			p.color = glm::clamp(p.color, 0.0f, 1.0f);
			p.scale += glm::vec3(-1, -1, -1) * dt;
			p.scale = glm::clamp(p.scale, 0.0f, 1.0f);
			if (NearZero(p.color.w) && NearZero(p.scale))
			{
				m_Particles.erase(m_Particles.begin() + i);
				CreateParticles(1);
			}
		}

		// bounding box
		float box = 1;
		vec3 newpos = p.position + glm::normalize(p.velocity) * dt * .1f;
		newpos = vec3(0);
		if (p.position.x + newpos.x > box)
		{
			p.velocity = glm::reflect(p.velocity, vec3(-1, 0, 0));
		}
		else if (p.position.x + newpos.x < -box)
		{
			p.velocity = glm::reflect(p.velocity, vec3(1, 0, 0));
		}
		if (p.position.y + newpos.y > box)
		{
			p.velocity = glm::reflect(p.velocity, vec3(0, -1, 0));
		}
		else if (p.position.y + newpos.y < -box)
		{
			p.velocity = glm::reflect(p.velocity, vec3(0, 1, 0));
		}
		if (p.position.z + newpos.z > box)
		{
			p.velocity = glm::reflect(p.velocity, vec3(0, 0, -1));
		}
		else if (p.position.z + newpos.z < -box)
		{
			p.velocity = glm::reflect(p.velocity, vec3(0, 0, 1));
		}

		// movement
		p.position += p.velocity * dt;
		p.velocity += p.acceleration * dt;
	}
}

void Confetti::ImGuiRender()
{

}

