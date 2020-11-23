#pragma once

#include "Core/Core.h"
#include "glm/glm.hpp"

namespace Glados {

	// hash function for glm::vec3
	glm::vec3 vec(0);
	unsigned int s = vec.length();

	template <typename T>
	class GLMVecHash
	{
	public:
		std::size_t operator()(T const& vec) const {
			std::size_t seed = vec.length();
			for (int i = 0; i < vec.length(); i++) {
				seed = (std::size_t)pow(seed, vec[i] + 0x9e3779b9 + (seed << 6) + (seed >> 2));
			}
			return seed;
		}
	};

}