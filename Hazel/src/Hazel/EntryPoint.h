#pragma once
#include <new>
#include <src/Hazel/badAllocHandler.h>

#ifdef HZ_PLATFORM_WINDOWS

extern Hazel::Application* Hazel::createApplication();

int main(int argc, char** argv) {

	std::set_new_handler(Hazel::BadAllocHandler::noMoreMemory);

	Hazel::Log::init();
	HZ_CORE_WARN("Initialized Log!");
	int a = 1;
	HZ_INFO("Hello! Var = {}", a);


	auto app = Hazel::createApplication();
	app->run();
	app->~Application();

}

#endif // HZ_PLATFORM_WINDOWS
