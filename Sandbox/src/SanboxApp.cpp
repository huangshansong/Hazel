#include <Hazel.h>

class ExampleLayer : public Hazel::Layer {
public:
	ExampleLayer()
	{
		m_DebugName = "Example";
	}

	void onUpdate() override {
		HZ_INFO("ExampleLayer::Update");
	}

	virtual void onRender() override
	{

		

	}

	void onEvent(Hazel::Event& event) override {
		HZ_TRACE("{0}", event);
	}
};

class Sandbox : public Hazel::Application{
public:
	Sandbox() {
		//pushLayer(new ExampleLayer());

	}


};

Hazel::Application*  Hazel::createApplication() {
	return new Sandbox;
}