#pragma once


#ifdef HZ_PLATFORM_WINDOWS

int main(int argc, char** argv) {

	Hazel::Log::init();
	HZ_CORE_WARN("Initialized Log!");
	int a = 1;
	HZ_INFO("Hello! Var = {}", a);


	auto app = Hazel::ApplicationInterface::create();
	Hazel::ApplicationInterface::run(app);
	Hazel::ApplicationInterface::destroy(app);

}

#endif // HZ_PLATFORM_WINDOWS
