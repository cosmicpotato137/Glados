#pragma once
#include <string>
#include <vector>
#include "glm/glm.hpp"
#include "VertexArray.h"

namespace Glados {

	struct MeshData
	{
		std::vector<glm::vec3> Vertices;
		std::vector<glm::vec3> Normals;
		std::vector<glm::vec2> TexCoords;
		std::vector<uint32_t> Indices;
	};

	class Mesh
	{
	public:
		Ref<VertexArray> m_VertexArray;
	private:
		std::string m_Filepath;
	public:
		Mesh(const std::string& filepath, const Ref<VertexArray>& vertexArray);
		~Mesh();

		void LoadMesh();
		
		inline unsigned int Size() { return m_VertexArray->GetIndexBuffer().GetCount(); }
		void FlatNormals();
		void SmoothNormals();

		static Ref<Mesh> Create(const std::string& filepath, const Ref<VertexArray>& vertexArray);

	private:
		MeshData Parse(const std::string& filepath);
	};

}