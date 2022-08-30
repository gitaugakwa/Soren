#pragma once

//Ifdef #SOREN_ENTRY_

#ifdef MATTHREADS
#else
#define MATTHREADS 0
#endif // MATTHREADS


#ifdef SOREN_PLATFORM_WINDOWS

int main(int argc, char** argv)
{
	argc;
	argv;
	Soren::Log::Init();
	SOREN_CORE_INFO("Initialized Log!");

	Eigen::initParallel();
	SOREN_CORE_INFO("Initialized Eigen Parallel!");
	Eigen::setNbThreads(MATTHREADS);
	SOREN_CORE_INFO("Eigen Threads: {0}", Eigen::nbThreads());


	// Application should be a interface
	auto app = Soren::CreateApplication();

		app->Init();
		app->Run();
		app->Deinit();
	
	delete app;
}

#endif