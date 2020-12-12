#pragma once

#include "Test.h"

#include "glm/glm.hpp"

#include "Glados/VertexArray.h"
#include "Glados/Buffer.h"
#include "Glados/Object.h"

namespace test {

	class TestPhong : public Test
	{
	public:
		TestPhong();
		~TestPhong();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
		void OnEvent(Glados::Event& e) override;

		//bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		//bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
		bool OnMouseMovedEvent(Glados::MouseMovedEvent& e);
		bool OnMouseScrollEvent(Glados::MouseScrollEvent& e);
		//bool OnKeyPressedEvent(KeyPressedEvent& e);
		//bool OnKeyReleasedEvent(KeyReleasedEvent& e);
		//bool OnKeyTypedEvent(KeyTypedEvent& e);
		//bool OnWindowResizeEvent(WindowResizeEvent& e);

	private:
		int m_H, m_W;

		float m_Mousex, m_Mousey;
		bool m_Lmb, m_Rmb;

		std::unique_ptr<Glados::Object> m_ActiveCamera;

		std::shared_ptr<Glados::VertexArray> m_VAO;
		std::unique_ptr<Glados::Object> m_Teapot;
		std::unique_ptr<Glados::Object> m_Box;

		std::vector<std::unique_ptr<Glados::Object>> m_Lights;
		std::shared_ptr<Glados::UniformBuffer> m_LightBuffer;

		std::shared_ptr<Glados::Material> m_Mat1;
	};
}
