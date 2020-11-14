#include "gladospch.h"

#include "ObjAttrib.h"

#include "imgui/imgui.h"
#include "GDMath.h"

#include "Object.h"
#include "Material.h"
#include "BufferLayout.h"

namespace Glados {

	//-----------------------------------
	//
	// Transform
	//
	//-----------------------------------

	Transform::Transform(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale)
		:position(pos), rotation(rot), scale(scale)
	{
		up = glm::vec3(0, 1, 0);
		forward = glm::vec3(0, 0, 1);
		UpdateTransform();
	}

	Transform::~Transform()
	{
	}

	void Transform::OnUpdate()
	{
		//UpdateRotation();
		UpdateTransform();
	}

	void Transform::OnImGuiRender()
	{
		if (!ImGui::TreeNode("Transform"))
			return;
		ImGui::InputFloat3("Position", &position[0], 3);
		ImGui::InputFloat3("Rotation", &rotation[0], 3);
		ImGui::InputFloat3("Scale", &scale[0], 3);
		ImGui::TreePop();
	}

	void Transform::UpdateTransform()
	{
		glm::vec4 u(glm::normalize(glm::cross(up, forward)), 0);
		glm::vec4 v(glm::normalize(up), 0);
		glm::vec4 w(glm::normalize(forward), 0);
		glm::mat4 frame(u, v, w, glm::vec4(0, 0, 0, 1));

		glm::mat4 sca = glm::scale(glm::mat4(1), scale);
		glm::mat4 pos = glm::translate(glm::mat4(1), position);
		transform = frame * pos * sca;
	}

	void Transform::UpdateBasis(glm::mat4 mat)
	{
		up = mat * glm::vec4(up, 1);
		forward = mat * glm::vec4(forward, 1);
		UpdateTransform();
	}

	void Transform::UpdatePosition(glm::mat4 mat)
	{
		position = mat * glm::vec4(position, 1);
		UpdateTransform();
	}

	void Transform::UpdateRotation(glm::vec3 rot)
	{
		rotation += rot;
		RotateVec3(up, rot);
		RotateVec3(forward, rot);
		UpdateTransform();
	}

	void Transform::RotateVec4(glm::vec4& v4, glm::vec3 rot)
	{
		v4 = glm::rotate(glm::mat4(1), rotation.x, glm::vec3(0, 1, 0)) * v4;
		v4 = glm::rotate(glm::mat4(1), rotation.y, glm::vec3(1, 0, 0)) * v4;
		v4 = glm::rotate(glm::mat4(1), rotation.z, glm::vec3(0, 0, 1)) * v4;
	}

	void Transform::RotateVec3(glm::vec3& v3, glm::vec3 rot)
	{
		v3 = glm::rotate(glm::mat4(1), glm::radians(rot.x), glm::vec3(0, 1, 0)) * glm::vec4(v3, 0);
		v3 = glm::rotate(glm::mat4(1), glm::radians(rot.y), glm::vec3(1, 0, 0)) * glm::vec4(v3, 0);
		v3 = glm::rotate(glm::mat4(1), glm::radians(rot.z), glm::vec3(0, 0, 1)) * glm::vec4(v3, 0);
	}


	//-----------------------------------
	//
	// Light
	//
	//-----------------------------------

	Light::Light(glm::vec4 lightdir, glm::vec4 lightcol, float lightint)
		: lightDir(lightdir), lightCol(lightcol), lightInt(lightint), active(true)
	{
	}

	Light::~Light()
	{
	}

	void Light::OnUpdate()
	{
	}

	void Light::OnImGuiRender()
	{
		if (!ImGui::TreeNode("Direction Light"))
			return;
		ImGui::Checkbox("Active", &active);
		ImGui::InputFloat3("Direction", &lightDir[0]);
		ImGui::ColorEdit4("Color", &lightCol[0]);
		ImGui::SliderFloat("Intensity", &lightInt, 0, 10);
		ImGui::TreePop();
	}

	//-----------------------------------
	//
	// Camera
	//
	//-----------------------------------

	Camera::Camera(glm::mat4 proj, std::shared_ptr<Transform> transf)
		: projection(proj), transf(transf), active(true),
		eye(transf->position), center(transf->forward), up(transf->up)
	{
		UpdateView();
	}

	Camera::~Camera()
	{
	}

	void Camera::OnUpdate()
	{
		UpdateView();
	}

	void Camera::UpdateView()
	{
		view = glm::lookAt(transf->position, transf->forward, transf->up);
	}

	//-----------------------------------
	//
	// Mesh
	//
	//-----------------------------------

	Mesh::Mesh(const std::string& fp, std::shared_ptr<VertexArray> vao)
		: filepath(fp), VAO(vao)
	{
		Parse();

		VertexBufferLayout vbl;
		vbl.Push<glm::vec3>(1); // vertex positions

		// vertex buffer
		VB = std::make_unique<VertexBuffer>(vbl, Size(), &vertices[0], GL_STATIC_DRAW);
		vao->AddBuffer(*VB, vbl);

		// normal buffer
		NB = std::make_unique<VertexBuffer>(vbl, Size(), &normals[0], GL_DYNAMIC_DRAW);
		vao->AddBuffer(*NB, vbl, 1);

		IndexBufferLayout ibl;
		ibl.Push(1);
		IB = std::make_unique<IndexBuffer>(ibl, Size(), &indices[0]);
	}

	Mesh::~Mesh()
	{
	}

	void Mesh::OnImGuiRender()
	{
		if (!ImGui::TreeNode("Mesh"))
			return;
		std::string file = std::string("File: ") + filepath;
		ImGui::Text(file.c_str());
		ImGui::Text("Vertices: %i", vertices.size());

		if (ImGui::Button("Calculate Normals"))
			FlatNormals();
		if (ImGui::Button("Smooth Normals"))
			SmoothNormals();
		ImGui::TreePop();
	}

	void Mesh::FlatNormals()
	{
		for (int i = 0; i < Size(); i += 3)
		{
			// calculate normal
			glm::vec3 v1 = vertices[i] - vertices[i + 1];
			glm::vec3 v2 = vertices[i] - vertices[i + 2];
			glm::vec3 norm = glm::normalize(glm::cross(v1, v2));

			// assign normal to each vertex
			glm::vec3 norms[] = { norm, norm, norm };
			NB->SetBufferSubData(i * sizeof(glm::vec3), sizeof(norms), &norms[0]);
		}
	}

	// SLOW normal calculating algorithm
	void Mesh::SmoothNormals()
	{
		std::unordered_map<glm::vec3, glm::vec3, GLMVecHash<glm::vec3>> temp;
		for (int i = 0; i < Size(); i++)
		{
			if (!&temp[vertices[i]])
				temp[vertices[i]] = normals[i];
			else
				temp[vertices[i]] += normals[i];
		}

		for (int i = 0; i < Size(); i++)
		{
			temp[vertices[i]] = glm::normalize(temp[vertices[i]]);
			NB->SetBufferSubData(i * sizeof(glm::vec3), sizeof(glm::vec3), &temp[vertices[i]]);
		}
	}

	void Mesh::Parse()
	{
		FILE* fp;
		float x, y, z;
		int fa, fb, fc, na, nb, nc, ignore;
		char c1, c2;
		float minY = INFINITY, minZ = INFINITY;
		float maxY = -INFINITY, maxZ = -INFINITY;

		std::vector<glm::vec3> verts;
		std::vector<glm::vec3> norms;
		std::vector<glm::vec2> inds;

		fopen_s(&fp, filepath.c_str(), "rb");

		if (fp == NULL) {
			std::cerr << "Error loading file: " << filepath << std::endl;
			std::getchar();
			exit(-1);
		}

		// feof returns end of file
		while (!feof(fp)) {
			c1 = fgetc(fp); // returns character as int
			while (!(c1 == 'v' || c1 == 'f')) {
				c1 = fgetc(fp);
				if (feof(fp))
					break;
			}
			c2 = fgetc(fp);

			// get vertices
			if ((c1 == 'v') && (c2 == ' ')) {
				fscanf_s(fp, "%f %f %f", &x, &y, &z);
				verts.push_back(glm::vec3(x, y, z));
				if (y < minY) minY = y;
				if (z < minZ) minZ = z;
				if (y > maxY) maxY = y;
				if (z > maxZ) maxZ = z;
			}
			// get normals
			else if ((c1 == 'v') && (c2 == 'n')) {
				fscanf_s(fp, "%f %f %f", &x, &y, &z);
				norms.push_back(glm::normalize(glm::vec3(x, y, z)));
			}
			else if (c1 == 'f' && c2 == ' ')
			{
				fscanf_s(fp, "%d//%d %d//%d %d//%d", &fa, &na, &fb, &nb, &fc, &nc);
				// triangle indices -- (vert, normal)
				inds.push_back(glm::vec2(fa - 1, na - 1));
				inds.push_back(glm::vec2(fb - 1, nb - 1));
				inds.push_back(glm::vec2(fc - 1, nc - 1));
			}
			else if (c1 == 's' && c2 == ' ')
			{
			}
		}

		fclose(fp); // Finished parsing

		// Recenter the teapot
		float avgY = (minY + maxY) / 2.0f - 0.0234f;
		float avgZ = (minZ + maxZ) / 2.0f;
		//for (unsigned int i = 0; i < m_Vertices.size(); ++i) {
		//	glm::vec3 shiftedVertex = (m_Vertices[i] - glm::vec3(0.0f, avgY, avgZ)) * glm::vec3(0.975f, 0.975f, 0.975f);
		//	m_Vertices[i] = shiftedVertex;
		//}

		for (unsigned int i = 0; i < inds.size(); i++)
		{
			vertices.push_back(verts[inds[i].x]);
			normals.push_back(norms[inds[i].y]);
			indices.push_back(i);
		}
	}

	//-----------------------------------
	//
	// Mesh Renderer
	//
	//-----------------------------------

	MeshRenderer::MeshRenderer(std::shared_ptr<Material> mat, Object* activecamera)
		: material(mat), activeCamera(activecamera), isLit(true)
	{
	}

	MeshRenderer::~MeshRenderer()
	{
	}

	void MeshRenderer::OnUpdate()
	{

	}

	void MeshRenderer::OnImGuiRender()
	{
		if (!ImGui::TreeNode("Mesh Renderer"))
			return;
		std::string namestr = std::string("Material: ") + material->name;
		ImGui::Text(namestr.c_str());
		ImGui::Checkbox("Lighting", &isLit);

		ImGui::TreePop();
	}

	void MeshRenderer::Clear() const
	{
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
	}

	void MeshRenderer::Draw(const Transform& transf, const Mesh& mesh)
	{
		if (!activeCamera)
			return;

		glm::mat4 view = activeCamera->GetAttrib<Camera>()->view;
		glm::mat4 proj = activeCamera->GetAttrib<Camera>()->projection;

		glm::mat4 modelview = view * transf.transform;
		material->shader->Bind();
		material->shader->SetUniformMat4f("u_ModelView", modelview);
		material->shader->SetUniformMat4f("u_Projection", proj);

		material->shader->Bind();
		mesh.VAO->Bind();
		mesh.IB->Bind();
		GLCall(glDrawElements(GL_TRIANGLES, mesh.IB->GetNumElements(), GL_UNSIGNED_INT, nullptr));
	}

}