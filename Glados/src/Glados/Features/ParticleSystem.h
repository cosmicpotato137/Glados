#pragma once
#include "Glados/Core/Timestep.h"
#include "Glados/Renderer/Texture.h"
#include "Glados/DataStructures/KdTree.h"

namespace Glados {

    // TODO: subclass Agent
    class Particle : public Agent
    {
    private:
        // initial values
        const float lifetime;
        const glm::vec3 iposition;
        const glm::vec3 irotation;
        const glm::vec3 iscale;
        const glm::vec3 ivelocity;
        const glm::vec3 iacceleration;
        const glm::vec4 icolor;
        const float imass;

    public:
        // current values
        float age;
        glm::vec3 rotation;
        glm::vec3 scale;
        glm::vec3 velocity;
        glm::vec3 acceleration;
        glm::vec4 color;
        float mass;

    public:
        // initializer list constructor
        Particle(float lifetime, glm::vec3 pos, glm::vec3 rot, glm::vec3 scl, 
            glm::vec3 vel, glm::vec3 acc, glm::vec4 col, float mass)
            : lifetime{ lifetime }, iposition{ pos }, irotation{ rot }, iscale{ scl }, 
            ivelocity{ vel }, iacceleration{ acc }, icolor{ col }, imass{ mass },
            Agent()
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

        inline glm::vec3 GetIPosition() const { return iposition; }
        inline glm::vec3 GetIrotation() const { return irotation; }
        inline glm::vec3 GetIScale() const { return iscale; }
        inline float GetLifetime() const { return lifetime; }
        inline glm::vec3 GetIVelicity() const { return ivelocity; }
        inline glm::vec3 GetIAcceleration() const { return iacceleration; }
        inline glm::vec4 GetIColor() const { return icolor; }
        inline float GetIMass() const { return imass; }
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
        std::vector<Agent*> m_Particles;
        KdTree m_KdTree;
        std::vector<uint32_t> m_DrawStack;
    };

}