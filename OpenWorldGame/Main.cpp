#include "stdafx.h"
#include "App.h"
#include "GameRendererManager.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
	auto app = std::make_unique<App>();
	auto rendererManager = std::make_shared<GameRendererManager>();
	return app->Run(hInstance, nCmdShow, rendererManager);
}
