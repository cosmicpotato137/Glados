/*
 * Agent.cpp
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

#include "gladospch.h"
#include "Agent.h"
#include "KdTree.h"

#include <cmath>
#include <algorithm>


namespace Glados {

	Agent::Agent(unsigned int maxNeighbors, float maxDistance) 
		: m_ID(0), m_MaxNeighbors(maxNeighbors), m_NeighborDist(maxDistance)
	{
	}

	//void Agent::computeNeighbors()
	//{
	//	agentNeighbors_.clear();

	//	if (maxNeighbors_ > 0) {
	//		sim_->kdTree_->computeAgentNeighbors(this, neighborDist_ * neighborDist_);
	//	}
	//}

	void Agent::InsertAgentNeighbor(const Agent *agent, float &rangeSq)
	{
		if (this != agent) {

			const float distSq = pow(position.x - agent->position.x, 2) + pow(position.y - agent->position.y, 2) + pow(position.z - agent->position.z, 2);

			if (distSq < rangeSq) {
				if (m_AgentNeighbors.size() < m_MaxNeighbors) {
					m_AgentNeighbors.push_back(std::make_pair(distSq, agent));
				}

				size_t i = m_AgentNeighbors.size() - 1;

				while (i != 0 && distSq < m_AgentNeighbors[i - 1].first) {
					m_AgentNeighbors[i] = m_AgentNeighbors[i - 1];
					--i;
				}

				m_AgentNeighbors[i] = std::make_pair(distSq, agent);

				if (m_AgentNeighbors.size() == m_MaxNeighbors) {
					rangeSq = m_AgentNeighbors.back().first;
				}
			}
		}
	}

}
