/*
 * KdTree.cpp
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
#include "KdTree.h"
#include <algorithm>

inline float sqr(float scalar)
{
	return scalar * scalar;
}

namespace Glados {
	using Vector3 = glm::vec3;
	const size_t RVO3D_MAX_LEAF_SIZE = 10;

	KdTree::KdTree(std::vector<Agent*> agents) : agents_(agents) { }

	void KdTree::BuildAgentTree()
	{
		if (!agents_.empty()) {
			agentTree_.resize(2 * agents_.size() - 1);
			BuildAgentTreeRecursive(0, agents_.size(), 0);
		}
	}

	void KdTree::BuildAgentTreeRecursive(size_t begin, size_t end, size_t node)
	{
		agentTree_[node].begin = begin;
		agentTree_[node].end = end;
		agentTree_[node].minCoord = agents_[begin]->position;
		agentTree_[node].maxCoord = agents_[begin]->position;

		for (size_t i = begin + 1; i < end; ++i) {
			agentTree_[node].maxCoord[0] = std::max(agentTree_[node].maxCoord[0], agents_[i]->position.x);
			agentTree_[node].minCoord[0] = std::min(agentTree_[node].minCoord[0], agents_[i]->position.x);
			agentTree_[node].maxCoord[1] = std::max(agentTree_[node].maxCoord[1], agents_[i]->position.y);
			agentTree_[node].minCoord[1] = std::min(agentTree_[node].minCoord[1], agents_[i]->position.y);
			agentTree_[node].maxCoord[2] = std::max(agentTree_[node].maxCoord[2], agents_[i]->position.z);
			agentTree_[node].minCoord[2] = std::min(agentTree_[node].minCoord[2], agents_[i]->position.z);
		}

		if (end - begin > RVO3D_MAX_LEAF_SIZE) {
			/* No leaf node. */
			size_t coord;

			if (agentTree_[node].maxCoord[0] - agentTree_[node].minCoord[0] > agentTree_[node].maxCoord[1] - agentTree_[node].minCoord[1] && agentTree_[node].maxCoord[0] - agentTree_[node].minCoord[0] > agentTree_[node].maxCoord[2] - agentTree_[node].minCoord[2]) {
				coord = 0;
			}
			else if (agentTree_[node].maxCoord[1] - agentTree_[node].minCoord[1] > agentTree_[node].maxCoord[2] - agentTree_[node].minCoord[2]) {
				coord = 1;
			}
			else {
				coord = 2;
			}

			const float splitValue = 0.5f * (agentTree_[node].maxCoord[coord] + agentTree_[node].minCoord[coord]);

			size_t left = begin;

			size_t right = end;

			while (left < right) {
				while (left < right && agents_[left]->position[coord] < splitValue) {
					++left;
				}

				while (right > left && agents_[right - 1]->position[coord] >= splitValue) {
					--right;
				}

				if (left < right) {
					std::swap(agents_[left], agents_[right - 1]);
					++left;
					--right;
				}
			}

			size_t leftSize = left - begin;

			if (leftSize == 0) {
				++leftSize;
				++left;
				++right;
			}

			agentTree_[node].left = node + 1;
			agentTree_[node].right = node + 2 * leftSize;

			BuildAgentTreeRecursive(begin, left, agentTree_[node].left);
			BuildAgentTreeRecursive(left, end, agentTree_[node].right);
		}
	}

	void KdTree::ComputeAgentNeighbors(Agent *agent, float range) const
	{
		float rangeSq = pow(range, 2);
		agent->ClearNeighbors();
		QueryAgentTreeRecursive(agent, rangeSq, 0);
	}

	void KdTree::QueryAgentTreeRecursive(Agent *agent, float &rangeSq, size_t node) const
	{
		if (agentTree_[node].end - agentTree_[node].begin <= RVO3D_MAX_LEAF_SIZE) {
			for (size_t i = agentTree_[node].begin; i < agentTree_[node].end; ++i) {
				agent->InsertAgentNeighbor(agents_[i], rangeSq);
			}
		}
		else {
			const float distSqLeft = sqr(std::max(0.0f, agentTree_[agentTree_[node].left].minCoord[0] - agent->position.x)) + sqr(std::max(0.0f, agent->position.x - agentTree_[agentTree_[node].left].maxCoord[0])) + sqr(std::max(0.0f, agentTree_[agentTree_[node].left].minCoord[1] - agent->position.y)) + sqr(std::max(0.0f, agent->position.y - agentTree_[agentTree_[node].left].maxCoord[1])) + sqr(std::max(0.0f, agentTree_[agentTree_[node].left].minCoord[2] - agent->position.z)) + sqr(std::max(0.0f, agent->position.z - agentTree_[agentTree_[node].left].maxCoord[2]));

			const float distSqRight = sqr(std::max(0.0f, agentTree_[agentTree_[node].right].minCoord[0] - agent->position.x)) + sqr(std::max(0.0f, agent->position.x - agentTree_[agentTree_[node].right].maxCoord[0])) + sqr(std::max(0.0f, agentTree_[agentTree_[node].right].minCoord[1] - agent->position.y)) + sqr(std::max(0.0f, agent->position.y - agentTree_[agentTree_[node].right].maxCoord[1])) + sqr(std::max(0.0f, agentTree_[agentTree_[node].right].minCoord[2] - agent->position.z)) + sqr(std::max(0.0f, agent->position.z - agentTree_[agentTree_[node].right].maxCoord[2]));

			if (distSqLeft < distSqRight) {
				if (distSqLeft < rangeSq) {
					QueryAgentTreeRecursive(agent, rangeSq, agentTree_[node].left);

					if (distSqRight < rangeSq) {
						QueryAgentTreeRecursive(agent, rangeSq, agentTree_[node].right);
					}
				}
			}
			else {
				if (distSqRight < rangeSq) {
					QueryAgentTreeRecursive(agent, rangeSq, agentTree_[node].right);

					if (distSqLeft < rangeSq) {
						QueryAgentTreeRecursive(agent, rangeSq, agentTree_[node].left);
					}
				}
			}
		}
	}

	std::vector<std::pair<float, Agent*>> KdTree::OrderInDirection(glm::vec3 dir)
	{
		std::vector<std::pair<float,Agent*>> agents;
		OrderInDirectionRecursive(dir, 0, agents);
		return agents;
	}

	void KdTree::OrderInDirectionRecursive(glm::vec3 dir, size_t node, std::vector<std::pair<float, Agent*>>& agents)
	{
		if (agentTree_[node].end - agentTree_[node].begin <= RVO3D_MAX_LEAF_SIZE) {
			for (size_t i = agentTree_[node].begin; i < agentTree_[node].end; ++i) {

				float dist = glm::dot(agents_[i]->position, dir);
				bool pushed = false;
				for (uint32_t j = 0; j < agents.size(); j)
				{
				    if (dist <= agents[j].first)
				    {
						pushed = true;
						agents.insert(agents.begin() + j, std::make_pair(dist, agents_[i]));
				        break;
				    }
				}
				if (!pushed)
					agents.push_back(std::make_pair(dist, agents_[i]));
			}
		}
		else {
			//const float distSqLeft = sqr(std::max(0.0f, agentTree_[agentTree_[node].left].minCoord[0] - agent->position.x)) + sqr(std::max(0.0f, agent->position.x - agentTree_[agentTree_[node].left].maxCoord[0])) + sqr(std::max(0.0f, agentTree_[agentTree_[node].left].minCoord[1] - agent->position.y)) + sqr(std::max(0.0f, agent->position.y - agentTree_[agentTree_[node].left].maxCoord[1])) + sqr(std::max(0.0f, agentTree_[agentTree_[node].left].minCoord[2] - agent->position.z)) + sqr(std::max(0.0f, agent->position.z - agentTree_[agentTree_[node].left].maxCoord[2]));
			const float distSqLeft = std::max(glm::dot(agentTree_[agentTree_[node].left].minCoord, dir), glm::dot(agentTree_[agentTree_[node].left].maxCoord, dir));
			//const float distSqRight = sqr(std::max(0.0f, agentTree_[agentTree_[node].right].minCoord[0] - agent->position.x)) + sqr(std::max(0.0f, agent->position.x - agentTree_[agentTree_[node].right].maxCoord[0])) + sqr(std::max(0.0f, agentTree_[agentTree_[node].right].minCoord[1] - agent->position.y)) + sqr(std::max(0.0f, agent->position.y - agentTree_[agentTree_[node].right].maxCoord[1])) + sqr(std::max(0.0f, agentTree_[agentTree_[node].right].minCoord[2] - agent->position.z)) + sqr(std::max(0.0f, agent->position.z - agentTree_[agentTree_[node].right].maxCoord[2]));
			const float distSqRight = std::max(glm::dot(agentTree_[agentTree_[node].right].minCoord, dir), glm::dot(agentTree_[agentTree_[node].right].maxCoord, dir));
			if (distSqLeft < distSqRight) {
				OrderInDirectionRecursive(dir, agentTree_[node].left, agents);
				OrderInDirectionRecursive(dir, agentTree_[node].right, agents);
			}
			else {
				OrderInDirectionRecursive(dir, agentTree_[node].right, agents);
				OrderInDirectionRecursive(dir, agentTree_[node].left, agents);
			}
		}
	}
}
