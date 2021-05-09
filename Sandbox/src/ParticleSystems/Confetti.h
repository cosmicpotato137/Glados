#pragma once
#include "Glados/Core/Timestep.h"
#include "Glados/Features/ParticleSystem.h"

class Confetti : public Glados::ParticleSystem
{
protected:
    virtual void Update(float ts) override;
    virtual void ImGuiRender() override;
    virtual void CreateParticles(int size) override;
};