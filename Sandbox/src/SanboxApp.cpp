#include <Hazel.h>

class Sandbox : public Hazel::Application
{
public:
	Sandbox() {
	}
};

Hazel::Application*  Hazel::createApplication() {
	return new Sandbox;
}