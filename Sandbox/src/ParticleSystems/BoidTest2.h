#pragma once
#include "Glados/Core/Timestep.h"
#include "Glados/Features/ParticleSystem.h"
#include "Glados/DataStructures/Agent.h"
#include "Glados/DataStructures/KdTree.h"

class BoidTest2 : public Glados::ParticleSystem
{
protected:
    virtual void Update(float ts) override;
    virtual void ImGuiRender() override;
    virtual void CreateParticles(int size) override;
private:

    float m_MaxSpeed = 0.7f;
    float m_AttractionRange = 1.0f;
    float m_Attraction = 1.0f;
    float m_RepulsionRange = 0.1f;
    float m_Repulsion = 2.0f;
    float m_Alignment = 1.0f;
    glm::vec3 m_Bounds = { 4, 4, 4 };
};