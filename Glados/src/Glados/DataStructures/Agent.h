/*
 * Agent.h
 * RVO2-3D Library
 *
 * Copyright 2008 University of North Carolina at Chapel Hill
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Please send all bug reports to <geom@cs.unc.edu>.
 *
 * The authors may be contacted via:
 *
 * Jur van den Berg, Stephen J. Guy, Jamie Snape, Ming C. Lin, Dinesh Manocha
 * Dept. of Computer Science
 * 201 S. Columbia St.
 * Frederick P. Brooks, Jr. Computer Science Bldg.
 * Chapel Hill, N.C. 27599-3175
 * United States of America
 *
 * <https://gamma.cs.unc.edu/RVO2/>
 */

/**
 * \file    Agent.h
 * \brief   Contains the Agent class.
 */
#pragma once

#include <cstddef>
#include <utility>
#include <vector>
#include "glm/glm.hpp"

namespace Glados {
	/**
	 * \brief   Defines an agent in the simulation.
	 */
	class Agent {
	public:
		/**
		 * \brief   Constructs an agent instance.
		 * \param   sim  The simulator instance.
		 */
		Agent(unsigned int maxNeighbors = 10, float maxDistance = 1.0f);

		/**
		 * \brief   Computes the neighbors of this agent.
		 */
		//void computeNeighbors();

		inline std::vector<std::pair<float, const Agent*>> GetNeighbors() const { return m_AgentNeighbors; }

	private:
		/**
		 * \brief   Inserts an agent neighbor into the set of neighbors of this agent.
		 * \param   agent    A pointer to the agent to be inserted.
		 * \param   rangeSq  The squared range around this agent.
		 */
		void InsertAgentNeighbor(const Agent *agent, float &rangeSq);
		void ClearNeighbors() { m_AgentNeighbors.clear(); }
	public:
		glm::vec3 position;

	private:
		int m_ID;
		float m_NeighborDist;
		unsigned int m_MaxNeighbors;
		std::vector<std::pair<float, const Agent *> > m_AgentNeighbors;

		friend class KdTree;
	};
}
