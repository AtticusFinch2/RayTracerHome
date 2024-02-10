

#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Random.h"
#include "Walnut/Timer.h"

#include "Renderer.h"



using namespace Walnut;

class ExampleLayer : public Walnut::Layer
{
public:
	virtual void OnUIRender() override
	{
		ImGui::Begin("Settings"); //settings window
		if (ImGui::Button("Render")) {
			Render();
		}
		ImGui::Text("Time taken: %.3fms", renderTimeMs);
		ImGui::End(); //DONOTREMOVE




		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Viewport"); //Viewport Window

		view_Width = ImGui::GetContentRegionAvail().x;
		view_Height = ImGui::GetContentRegionAvail().y;

		auto f_image = skippy.GetFinalImage();
		if (f_image) {
			ImGui::Image(f_image->GetDescriptorSet(), { (float)f_image->GetWidth(), (float)f_image->GetHeight()},
			ImVec2(0,1), ImVec2(1,0));
		}

		ImGui::End();
		ImGui::PopStyleVar();
		//Render(); //TODO: UNCOMMENT TO MAKE REALTIME
	}

	void Render() { //What we call when we press render

		Timer timer;

		//renderer resize + render
		skippy.OnResize(view_Width, view_Height);
		skippy.Render();


		renderTimeMs = timer.ElapsedMillis();
	}

private:
	Renderer skippy;
	uint32_t* m_ImageData = nullptr;
	uint32_t view_Width = 0, view_Height=0;
	float renderTimeMs = 0.0f;
};


Walnut::Application* Walnut::CreateApplication(int argc, char** argv) // cant remove, unfortunately
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Walnut Example";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<ExampleLayer>();
	app->SetMenubarCallback([app]()
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Exit"))
				{
					app->Close();
				}
				ImGui::EndMenu();
			}
		});
	return app;
}