#pragma once

namespace Glados {

	class EditorLayer : public Layer
	{
	private:
		Ref<Framebuffer> m_Framebuffer;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
	public:
		EditorLayer();
		~EditorLayer();

		void OnAttach() override;
		void OnDetach() override;

		void OnUpdate(Timestep timestep) override;
		void OnRender() override;
		void OnImGuiRender() override;

		void OnEvent(Event& e);

	private:
		void Dockspace();
	};

}