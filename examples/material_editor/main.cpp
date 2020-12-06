#include <application.h>
#include "material_editor_app.h"

class GlobalInterfaceImpl : public GlobalInterface
{
public:
	ImTextureID LoadTexture(const char* path) override
	{
		return Application_LoadTexture(path);
	}

	void DestroyTexture(ImTextureID id) override
	{
		Application_DestroyTexture(id);
	}

	int GetTextureWidth(ImTextureID id) override
	{
		return Application_GetTextureWidth(id);
	}

	int GetTextureHeight(ImTextureID id) override
	{
		return Application_GetTextureHeight(id);
	}
};

GlobalInterfaceImpl globalInterface;
MaterialEditorApp app(&globalInterface);

const char* Application_GetName()
{
	return app.GetName();
}

void Application_Initialize()
{
	app.Initialize();
}

void Application_Finalize()
{
	app.Finalize();
}

void Application_Frame()
{
	app.RunFrame();
}

