#include "Glados.h"
#include "BoidTest.h"
#include "imgui/imgui.h"

using namespace Glados;
using namespace glm;
using namespace gdm;

void BoidTest::CreateParticles(int size)
{
	for (int i = 0; i < size && m_Particles.size() < m_Specs.MaxParticles; i++)
	{
		vec3 vel = Random::RandomUnitSphere() * .5f * vec3(1,1,0);
		vec3 pos = Random::RandomUnitCube() * vec3(1,1,0);
		vec3 acc = vec3(0, -1, 0);
		float lifetime = Random::RandomFloat(5, 10);
		vec4 col = vec4(Random::RandomVec3(), 1.0f);
		m_Particles.push_back(new Particle{ 0, pos, vec3(0), vec3(.15), vel, vec3(0), col, 1 });
	}
}

void BoidTest::Update(float dt)
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
		vec3 newpos = p.position + glm::normalize(p.velocity) * dt * .1f;
		newpos = vec3(0);
		if (p.position.x + newpos.x > m_Bounds.x)
		{
			p.velocity += vec3(-1, 0, 0) * dt;
		}
		else if (p.position.x + newpos.x < -m_Bounds.x)
		{
			p.velocity += vec3(1, 0, 0) * dt;
		}
		if (p.position.y + newpos.y > m_Bounds.y)
		{
			p.velocity += vec3(0, -1, 0) * dt;
		}
		else if (p.position.y + newpos.y < -m_Bounds.y)
		{
			p.velocity += vec3(0, 1, 0) * dt;
		}
		if (p.position.z + newpos.z > m_Bounds.z)
		{
			p.velocity += vec3(0, 0, -1) * dt;
		}
		else if (p.position.z + newpos.z < -m_Bounds.z)
		{
			p.velocity += vec3(0, 0, 1) * dt;
		}

		m_KdTree.ComputeAgentNeighbors(m_Particles[i], m_AttractionRange);
		auto neighbors = p.GetNeighbors();
		vec3 avgdir = vec3(0);
		for (int j = 0; j < neighbors.size(); j++)
		{
			Particle& q = *(Particle*)neighbors[j].second;
			float dist = neighbors[j].first;
			if (dot(p.velocity, p.position - q.position) < 0)
				continue;

			avgdir = avgdir + normalize(q.velocity) * (1.0f - min((dist / m_AttractionRange), 0.0f)) * m_Alignment;

			if (dist > m_RepulsionRange)
			{
				glm::vec3 a = normalize(q.position - p.position);
				float b = (1.0f - max((m_RepulsionRange - dist) / (m_RepulsionRange - m_AttractionRange), 0.0f));
				avgdir = avgdir + a * b * m_Attraction;
			}
			else
			{
				avgdir = avgdir + normalize(p.position - q.position) * (1.0f - min((dist / m_RepulsionRange), 0.0f)) * m_Repulsion;
			}
		}
		if (length(avgdir) > 0)
			avgdir = normalize(avgdir);
		// movement
		//vec3 avgdir = vec3(0);
		//for (int j = 0; j < m_Particles.size(); j++)
		//{
		//	Particle& q = *(Particle*)m_Particles[j];
		//	if (i == j)
		//		continue;

		//	float dist = distance(p.position, q.position);
		//	if (dist < m_AttractionRange && dist >= m_RepulsionRange)
		//	{
		//		avgdir = normalize(avgdir + normalize(q.position - p.position) * m_Attraction);
		//	}
		//	else if (dist < m_RepulsionRange)
		//	{
		//		avgdir = normalize(avgdir + normalize(p.position - q.position) * m_Repulsion);
		//	}
		//}

		p.position += p.velocity * dt;
		p.velocity += avgdir * dt;
		if (length(p.velocity) > m_MaxSpeed)
			p.velocity = normalize(p.velocity) * m_MaxSpeed;
	}
}

void BoidTest::ImGuiRender()
{
	ImGui::DragFloat("Max Speed", &m_MaxSpeed);
	ImGui::DragFloat("Attraction Range", &m_AttractionRange, 0.01f, 0.0f);
	ImGui::DragFloat("Attraction", &m_Attraction, 0.01f, 0.0f);
	ImGui::DragFloat("Repulsion Range", &m_RepulsionRange, 0.01f, 0.0f);
	ImGui::DragFloat("Repuslion", &m_Repulsion, 0.01f, 0.0f);
	ImGui::DragFloat("Alignment", &m_Alignment, 0.01f, 0.0f);
	ImGui::DragFloat3("Bounds", &m_Bounds[0], 0.01f, 0.0f);
}

