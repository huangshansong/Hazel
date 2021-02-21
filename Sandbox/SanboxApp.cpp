#include <Hazel.h>

class Sandbox : public Hazel::Application{



};

Hazel::Application* Hazel::CreateApplication() {
	return new Sandbox;
}