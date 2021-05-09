#pragma once
#include "Glados/Core/Timestep.h"
#include "Glados/Renderer/Texture.h"

namespace Glados {

    struct Particle
    {
        // initial values
        float lifetime;
        glm::vec3 iposition;
        glm::vec3 irotation;
        glm::vec3 iscale;
        glm::vec3 ivelocity;
        glm::vec3 iacceleration;
        glm::vec4 icolor;
        float imass;

        // current values
        float age;
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;
        glm::vec3 velocity;
        glm::vec3 acceleration;
        glm::vec4 color;
        float mass;

        // initializer list constructor
        Particle(float lifetime, glm::vec3 pos, glm::vec3 rot, glm::vec3 scl, 
            glm::vec3 vel, glm::vec3 acc, glm::vec4 col, float mass)
            : lifetime{ lifetime }, iposition{ pos }, irotation{ rot }, iscale{ scl }, 
            ivelocity{ vel }, iacceleration{ acc }, icolor{ col }, imass{ mass }
        {
            age = 0;
            position = iposition;
            rotation = irotation;
            scale = iscale;
            velocity = ivelocity;
            acceleration = iacceleration;
            color = icolor;
            mass = imass;
        }
    };

    struct ParticleSystemSpecs
    {
        std::string Name;
        uint32_t MaxParticles;
        uint32_t StartingParticles;
        Ref<Texture2D> Texture;
        BlendMode Blend;
        bool FaceCamera;
    };

    class ParticleSystem {
    public:
        ParticleSystem();
        virtual ~ParticleSystem();

        void Init(ParticleSystemSpecs specs);
        void SetParticleBlend(BlendMode mode);

        virtual void OnUpdate(Timestep ts);
        virtual void OnImGuiRender();
        virtual void OnRender(const EditorCamera& camera);

    protected:
        virtual void Update(float dt) = 0;
        virtual void ImGuiRender() = 0;
        virtual void CreateParticles(int size) = 0;

    protected:
        ParticleSystemSpecs m_Specs;
        bool m_Restart = false;
        bool m_Running = true;
        std::vector<Particle> m_Particles;
        std::vector<uint32_t> m_DrawStack;
    };

}