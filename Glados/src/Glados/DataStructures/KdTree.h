/*
 * KdTree.h
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
 * \file    KdTree.h
 * \brief   Contains the KdTree class.
 */
#pragma once

#include <cstddef>
#include <vector>
#include "glm/glm.hpp"
#include "Agent.h"

namespace Glados {

	/**
	 * \brief   Defines <i>k</i>d-trees for agents in the simulation.
	 */
	class KdTree {
	public:
		/**
		 * \brief   Defines an agent <i>k</i>d-tree node.
		 */
		class AgentTreeNode {
		public:
			/**
			 * \brief   The beginning node number.
			 */
			size_t begin;

			/**
			 * \brief   The ending node number.
			 */
			size_t end;

			/**
			 * \brief   The left node number.
			 */
			size_t left;

			/**
			 * \brief   The right node number.
			 */
			size_t right;

			/**
			 * \brief   The maximum coordinates.
			 */
			glm::vec3 maxCoord;

			/**
			 * \brief   The minimum coordinates.
			 */
			glm::vec3 minCoord;
		};

		/**
		 * \brief   Constructs a <i>k</i>d-tree instance.
		 * \param   sim  The simulator instance.
		 */
		KdTree(std::vector<Agent*> agents = {});

		/**
		 * \brief   Builds an agent <i>k</i>d-tree.
		 */
		void BuildAgentTree();


		/**
		 * \brief   Computes the agent neighbors of the specified agent.
		 * \param   agent    A pointer to the agent for which agent neighbors are to be computed.
		 * \param   rangeSq  The squared range around the agent.
		 */
		void ComputeAgentNeighbors(Agent *agent, float rangeSq) const;

		std::vector<std::pair<float, Agent*>> OrderInDirection(glm::vec3 dir);

	private:
		void BuildAgentTreeRecursive(size_t begin, size_t end, size_t node);
		void QueryAgentTreeRecursive(Agent *agent, float &rangeSq, size_t node) const;

		void OrderInDirectionRecursive(glm::vec3 dir, size_t node, std::vector<std::pair<float, Agent*>>& agents);

		std::vector<Agent *> agents_;
		std::vector<AgentTreeNode> agentTree_;

		friend class Agent;
	};
}
