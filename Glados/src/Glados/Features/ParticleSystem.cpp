#include "gladospch.h"
#include "Glados.h"
#include "ParticleSystem.h"
#include "imgui.h"

using namespace glm;

namespace Glados {

    ParticleSystem::ParticleSystem()
    {
    }

    ParticleSystem::~ParticleSystem()
    {
    }

    void ParticleSystem::Init(ParticleSystemSpecs specs)
    {
        m_Specs = specs;
        CreateParticles(m_Specs.StartingParticles);
        m_KdTree = KdTree(m_Particles);
    }

    void ParticleSystem::OnRender(const EditorCamera& camera)
    {
        mat4 frame(1);
        if (m_Specs.FaceCamera)
        {
            vec4 fa = vec4(glm::cross(camera.GetViewForward(), camera.GetViewUp()), 0);
            vec4 fb = vec4(camera.GetViewUp(), 0);
            vec4 fc = vec4(-camera.GetViewForward(), 0);
            frame = mat4(fa, fb, fc, vec4(0, 0, 0, 1));
        }

        //get draw order
        std::vector<uint32_t> drawStack;
        for (uint32_t j = 0; j < m_Particles.size(); j++)
        {
            bool pushed = false;
            float dist = glm::dot(m_Particles[j]->position - camera.GetViewPosition(), camera.GetViewForward());
            for (uint32_t i = 0; i < drawStack.size(); i++)
            {
                float dist1 = glm::dot(m_Particles[drawStack[i]]->position - camera.GetViewPosition(), camera.GetViewForward());
                if (dist <= dist1)
                {
                    pushed = true;
                    drawStack.insert(drawStack.begin() + i, j);
                    break;
                }
            }
            if (!pushed)
                drawStack.push_back(j);
        } 

        //auto particleOrder = m_KdTree.OrderInDirection(camera.GetViewForward());

        RenderCommand::SetBlend(m_Specs.Blend);
        for (int i = m_Particles.size()-1; i >= 0; i--)
        {
            Particle p = *(Particle*)m_Particles[drawStack[i]];
            glm::mat4 pos = glm::translate(glm::mat4(1), p.position);
            glm::mat4 scl = glm::scale(glm::mat4(1), p.scale);
            glm::mat4 transf = pos * scl * frame * glm::translate(mat4(1), vec3(-.5, -.5, 0));

            if (m_Specs.Texture)
                Renderer2D::DrawQuad(transf, m_Specs.Texture, 1.0f, p.color);
            else
                Renderer2D::DrawQuad(transf, p.color);
            }
    }

    void ParticleSystem::SetParticleBlend(BlendMode mode)
    {
        m_Specs.Blend = mode;
    }

    void ParticleSystem::OnUpdate(Timestep ts)
    {
        if (m_Restart)
        {
            m_Particles.clear();
            m_DrawStack.clear();
            CreateParticles(m_Specs.StartingParticles);
            m_KdTree = KdTree(m_Particles);
            m_Restart = false;
        }
        // for children
        if (m_Running)
        {
            m_KdTree.BuildAgentTree();
            Update(ts.GetSeconds());
        }
    }

    void ParticleSystem::OnImGuiRender()
    {
        ImGui::Text(m_Specs.Name.c_str());

        // pause particle system
        if (m_Running && ImGui::Button("Pause", ImVec2(100, 30)))
            m_Running = false;
        else if (!m_Running && ImGui::Button("Play", ImVec2(100, 30)))
            m_Running = true;

        // restart particle system
        if (!m_Restart && ImGui::Button("Restart", ImVec2(100, 30)))
            m_Restart = true;

        ImGui::DragInt("Max Particles", &(int&)m_Specs.MaxParticles, 1.0f, 0);
        ImGui::DragInt("Starting Particles", &(int&)m_Specs.StartingParticles, 1.0f, 0, m_Specs.MaxParticles);

        // for children
        ImGuiRender();
        // TODO: add texture selection
    }

}