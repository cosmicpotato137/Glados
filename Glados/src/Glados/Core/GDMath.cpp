#include "gladospch.h"
#include "GDMath.h"
#include "GDLog.h"

namespace Glados {

	namespace gdm {

        std::random_device Random::s_RD = std::random_device();
        std::mt19937 Random::s_Generator = std::mt19937(s_RD());

        float Random::RandomFloat()
        {
            std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
            return distribution(s_Generator);
        }

        float Random::RandomFloat(float min, float max)
        {
            GD_CORE_ASSERT(min < max, "min must be less than max!");
            std::uniform_real_distribution<float> distribution(min, max);
            return distribution(s_Generator);
        }

        glm::vec3 Random::RandomVec3()
        {
            float x = RandomFloat();
            float y = RandomFloat();
            float z = RandomFloat();
            return glm::vec3(x, y, z);
        }

        glm::vec3 Random::RandomVec3(const glm::vec3& min, const glm::vec3& max)
        {
            float x = RandomFloat(min.x, max.x);
            float y = RandomFloat(min.y, max.y);
            float z = RandomFloat(min.z, max.z);
            return glm::vec3(x, y, z);
        }
        glm::vec4 Random::RandomVec4()
        {
            return glm::vec4(RandomVec3(), RandomFloat());
        }

        glm::vec4 Random::RandomVec4(const glm::vec4& min, const glm::vec4& max)
        {
            return glm::vec4(RandomVec3((glm::vec3)min, (glm::vec3)max), RandomFloat(min.w, max.w));
        }

        glm::vec3 Random::RandomUnitSphere()
        {
            glm::mat4 quat = glm::rotate(glm::mat4(1), RandomFloat(0, 2.0f * 3.142f), glm::vec3(1, 0, 0));
            quat = glm::rotate(quat, RandomFloat(0, 2.0f * 3.142f), glm::vec3(0, 1, 0));
            quat = glm::rotate(quat, RandomFloat(0, 2.0f * 3.142f), glm::vec3(0, 0, 1));
            return glm::vec3(quat * glm::vec4(0, 1, 0, 1));
        }

        glm::vec3 Random::RandomHemisphere(glm::vec3 normal)
        {
            glm::vec3 in_unit_sphere = RandomUnitSphere();
            if (glm::dot(in_unit_sphere, normal) > 0.0f) // In the same hemisphere as the normal
            {
                return in_unit_sphere;
            }
            else
            {
                return -in_unit_sphere;
            }
        }

        glm::vec3 Random::RandomUnitBall()
        {
            glm::mat4 quat = glm::rotate(glm::mat4(1), RandomFloat(0, 2.0f * 3.142f), glm::vec3(1, 0, 0));
            quat = glm::rotate(quat, RandomFloat(0, 2.0f * 3.142f), glm::vec3(0, 1, 0));
            quat = glm::rotate(quat, RandomFloat(0, 2.0f * 3.142f), glm::vec3(0, 0, 1));
            return glm::vec3(quat * glm::vec4(1, 0, 0, 1)) * RandomFloat(0.0f, 1.0f);
        }

        glm::vec3 Random::RandomUnitDisk()
        {
            glm::mat4 quat = glm::rotate(quat, RandomFloat(0, 2.0f * 3.142f), glm::vec3(0, 0, 1));
            return glm::vec3(quat * glm::vec4(0, 1, 0, 1)) * RandomFloat(0.0f, 1.0f);
        }

        glm::vec3 Random::RandomUnitCube()
        {
            return RandomVec3(glm::vec3(-.5, -.5, -.5), glm::vec3(.5, .5, .5));
        }

        glm::vec3 Random::RandomUnitSquare()
        {
            float x = RandomFloat(-0.5, 0.5);
            float y = RandomFloat(-0.5, 0.5);
            return glm::vec3(x, y, 0);
        }

	}

}