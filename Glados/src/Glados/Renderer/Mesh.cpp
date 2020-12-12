#include "gladospch.h"
#include "Mesh.h"
#include "Buffer.h"

namespace Glados {

	Mesh::Mesh(const std::string& filepath, const Ref<VertexArray> vertexArray)
	{
		MeshData meshData = Parse(filepath);

		// vertex buffer
		{
			BufferLayout layout(
				{ Uniform(UniformType::Float3, "a_Position") }
			);
			Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(&meshData.Vertices[0][0], meshData.Vertices.size() * layout.GetStride());
			vertexBuffer->SetLayout(layout);
			vertexArray->AddVertexBuffer(vertexBuffer);
		}

		// texture coordinate buffer
		{
			BufferLayout layout(
				{ Uniform(UniformType::Float2, "a_Texture") }
			);
			Ref<VertexBuffer> texCoordBuffer = VertexBuffer::Create(&meshData.TexCoords[0][0], meshData.TexCoords.size() * layout.GetStride());
			texCoordBuffer->SetLayout(layout);
			vertexArray->AddVertexBuffer(texCoordBuffer);
		}

		// normal buffer
		{
			BufferLayout layout(
				{ Uniform(UniformType::Float3, "a_Normal") }
			);
			Ref<VertexBuffer> normalBuffer = VertexBuffer::Create(&meshData.Normals[0][0], meshData.Normals.size() * layout.GetStride());
			normalBuffer->SetLayout(layout);
			vertexArray->AddVertexBuffer(normalBuffer);
		}

		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(&meshData.Indices[0], meshData.Indices.size());
		vertexArray->SetIndexBuffer(indexBuffer);
	}

	Mesh::~Mesh()
	{

	}

	void Mesh::LoadMesh()
	{

	}

	void Mesh::FlatNormals()
	{
		
	}

	void Mesh::SmoothNormals()
	{

	}

	Ref<Mesh> Mesh::Create(const std::string& filepath, const Ref<VertexArray> vertexArray)
	{
		return CreateRef<Mesh>(filepath, vertexArray);
	}

	MeshData Mesh::Parse(const std::string& filepath)
	{
		MeshData meshData;

		FILE* fp;
		float x, y, z; // data buffers
		// indices //
		int va, vb, vc; 
		int ta, tb, tc; 
		int na, nb, nc;
		// /////// //

		char c1, c2;
		float minY = INFINITY, minZ = INFINITY;
		float maxY = -INFINITY, maxZ = -INFINITY;

		std::vector<glm::vec3> vertices;
		std::vector<glm::vec2> texCoords;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec3> indices;

		fopen_s(&fp, filepath.c_str(), "rb");

		GD_CORE_ASSERT(fp, "Error loading file: {0}", filepath);

		// feof returns end of file
		while (!feof(fp)) 
		{
			c1 = fgetc(fp); // returns character as int
			while (!(c1 == 'v' || c1 == 'f')) 
			{
				c1 = fgetc(fp);
				if (feof(fp))
					break;
			}
			c2 = fgetc(fp);

			// get vertices
			if ((c1 == 'v') && (c2 == ' ')) 
			{
				fscanf_s(fp, "%f %f %f", &x, &y, &z);
				vertices.push_back(glm::vec3(x, y, z));
				if (y < minY) minY = y;
				if (z < minZ) minZ = z;
				if (y > maxY) maxY = y;
				if (z > maxZ) maxZ = z;
			}

			else if ((c1 == 'v') && (c2 == 't'))
			{
				fscanf_s(fp, "%f %f", &x, &y);
				texCoords.push_back(glm::vec2(x, y));
			}

			// get normals
			else if ((c1 == 'v') && (c2 == 'n')) 
			{
				fscanf_s(fp, "%f %f %f", &x, &y, &z);
				normals.push_back(glm::normalize(glm::vec3(x, y, z)));
			}
			
			else if (c1 == 'f' && c2 == ' ')
			{
				fscanf_s(fp, "%d/%d/%d %d/%d/%d %d/%d/%d", 
					&va, &ta, &na, &vb, &tb, &nb, &vc, &tc, &nc);
				// triangle indices -- (vertex, normal)
				indices.push_back(glm::vec3(va - 1, ta - 1, na - 1));
				indices.push_back(glm::vec3(vb - 1, tb - 1, nb - 1));
				indices.push_back(glm::vec3(vc - 1, tc - 1, nc - 1));
			}

			else if (c1 == 's' && c2 == ' ')
			{
			}
		}

		fclose(fp); // Finished parsing

		// Recenter the teapot
		//float avgY = (minY + maxY) / 2.0f - 0.0234f;
		//float avgZ = (minZ + maxZ) / 2.0f;
		//for (unsigned int i = 0; i < m_Vertices.size(); ++i) {
		//	glm::vec3 shiftedVertex = (m_Vertices[i] - glm::vec3(0.0f, avgY, avgZ)) * glm::vec3(0.975f, 0.975f, 0.975f);
		//	m_Vertices[i] = shiftedVertex;
		//}

		for (uint32_t i = 0; i < indices.size(); i++)
		{
			meshData.Vertices.push_back(vertices[(uint32_t)indices[i].x]);
			meshData.TexCoords.push_back(texCoords[(uint32_t)indices[i].y]);
			meshData.Normals.push_back(normals[(uint32_t)indices[i].z]);
			meshData.Indices.push_back(i);
		}

		return meshData;
	}

}