#include "Glados.h"
#include "EditorLayer.h"
#include "imgui/imgui.h"

namespace Glados {

	EditorLayer::EditorLayer()
		: Layer("Editor Overlay")
	{
	}

	EditorLayer::~EditorLayer()
	{
	}

	void EditorLayer::OnAttach()
	{
		FramebufferSpecification spec;
		spec.Width = 1280;
		spec.Height = 720;
		m_Framebuffer = Framebuffer::Create(spec);

		Renderer::SetClearColor(glm::vec4(1, 1, 1, 1));
	}

	void EditorLayer::OnDetach()
	{
	}

	void EditorLayer::OnUpdate(Timestep timestep)
	{
	}

	void EditorLayer::OnRender()
	{
		m_Framebuffer->Bind();
		Renderer::Clear();
		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{
		Dockspace();
		
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("Viewport");
		ImVec2 viewportSize = ImGui::GetContentRegionAvail();
		if (m_ViewportSize != *(glm::vec2*)&viewportSize)
		{
			m_Framebuffer->Resize(viewportSize.x, viewportSize.y);
			m_ViewportSize = *(glm::vec2*)&viewportSize;
		}
		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)textureID, ImVec2(viewportSize.x, viewportSize.y), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::End();
		ImGui::PopStyleVar();

		static bool showDemo = true;
		if (showDemo)
			ImGui::ShowDemoWindow(&showDemo);
	}

	void EditorLayer::OnEvent(Glados::Event& e)
	{
	}

	void EditorLayer::Dockspace()
	{
		static bool p_open = true;
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->GetWorkPos());
		ImGui::SetNextWindowSize(viewport->GetWorkSize());
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::Begin("DockSpace Demo", &p_open, window_flags);
		ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Options"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows,
				// which we can't undo at the moment without finer window depth/z control.
				ImGui::MenuItem("Exit");

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		ImGui::End();
	}

}