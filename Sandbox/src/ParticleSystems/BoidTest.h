#pragma once
#include "Glados/Core/Timestep.h"
#include "Glados/Features/ParticleSystem.h"

class BoidTest : public Glados::ParticleSystem
{
protected:
    virtual void Update(float ts) override;
    virtual void ImGuiRender() override;
    virtual void CreateParticles(int size) override;
private:
    float m_MaxSpeed = 1.0f;
    float m_AttractionRange = 0.5f;
    float m_Attraction = 1.0f;
    float m_RepulsionRange = 0.2f;
    float m_Repulsion = 1.0f;
    glm::vec3 m_Bounds = { 1, 1, 1 };
};