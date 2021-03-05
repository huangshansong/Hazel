#pragma once


#ifdef HZ_PLATFORM_WINDOWS

extern Hazel::Application* Hazel::createApplication();

int main(int argc, char** argv) {

	Hazel::Log::init();
	HZ_CORE_WARN("Initialized Log!");
	int a = 1;
	HZ_INFO("Hello! Var = {}", a);


	auto app = Hazel::createApplication();
	app->run();
	delete app;

}

#endif // HZ_PLATFORM_WINDOWS
