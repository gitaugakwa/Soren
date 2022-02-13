#pragma once

//Ifdef #SOREN_ENTRY_

#ifdef OL_PLATFORM_WINDOWS

int main(int argc, char** argv)
{
	argc;
	argv;
	Soren::Log::Init();
	OL_CORE_INFO("Initialized Log!");

	// Application should be a interface
	auto app = Soren::CreateApplication();

	app->Init();
	app->Run();
	app->Deinit();
	delete app;
}

#endif