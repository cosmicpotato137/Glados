#include "Glados.h"
#include "Glados/Core/EntryPoint.h"
#include "EditorLayer.h"

namespace Glados {

	class GladosEditorApp : public Application
	{
	public:
		GladosEditorApp()
			: Application()
		{
			PushLayer(new EditorLayer());

		}

		~GladosEditorApp()
		{
		}
	};

	Glados::Application* Glados::CreateApplication()
	{
		return new GladosEditorApp();
	}

}