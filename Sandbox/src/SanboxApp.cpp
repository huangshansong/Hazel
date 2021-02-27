#include <Hazel.h>

class ExampleLayer : public Hazel::Layer {
public:
	ExampleLayer()
	{
		m_DebugName = "Example";
	}

	void OnUpdate() override {
		HZ_INFO("ExampleLayer::Update");
	}

	virtual void OnRender() override
	{

		

	}

	void OnEvent(Hazel::Event& event) override {
		HZ_TRACE("{0}", event);
	}
};

class Sandbox : public Hazel::Application{
public:
	Sandbox() {
		//PushLayer(new ExampleLayer());

	}


};

Hazel::Application*  Hazel::CreateApplication() {
	return new Sandbox;
}