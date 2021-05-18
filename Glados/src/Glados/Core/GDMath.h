#pragma once
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/norm.hpp"
#include "glm/gtc/epsilon.hpp"
#include <random>
#include <memory>

// TODO: abstract math library

namespace Glados {

    namespace gdm {

        static const float PI = glm::pi<float>();
        static const float INFTY = std::numeric_limits<float>::infinity();

        inline bool NearZero(float e)
        {
            // Return true if the vector is close to zero in all dimensions.
            const auto s = 1e-8;
            return fabs(e) < s;
        }

        inline bool NearZero(const glm::vec2& e)
        {
            // Return true if the vector is close to zero in all dimensions.
            const auto s = 1e-8;
            return (fabs(e[0]) < s) && (fabs(e[1]) < s);
        }

        inline bool NearZero(const glm::vec3& e)
        {
            // Return true if the vector is close to zero in all dimensions.
            const auto s = 1e-8;
            return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
        }

        inline bool NearZero(const glm::vec4& e)
        {
            // Return true if the vector is close to zero in all dimensions.
            const auto s = 1e-8;
            return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s) && (fabs(e[3]) < s);
        }

        class Random
        {
        public:
            Random() = delete;
            ~Random() = default;

            static float RandomFloat();
            static float RandomFloat(float min, float max);

            static glm::vec3 RandomVec3();
            static glm::vec3 RandomVec3(const glm::vec3& min, const glm::vec3& max);
            static glm::vec4 RandomVec4();
            static glm::vec4 RandomVec4(const glm::vec4& min, const glm::vec4& max);

            static glm::vec3 RandomUnitSphere();
            static glm::vec3 RandomHemisphere(glm::vec3 normal);
            static glm::vec3 RandomUnitBall();
            static glm::vec3 RandomUnitDisk();

            static glm::vec3 RandomUnitCube();
            static glm::vec3 RandomUnitSquare();

        private:
            static std::random_device s_RD;
            static std::mt19937 s_Generator;
        };

    }

}