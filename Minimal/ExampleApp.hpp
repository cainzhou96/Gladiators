#ifndef EXAMPLEAPP_H
#define EXAMPLEAPP_H

#include "RiftApp.hpp"
#include "shader.h"
#include "Scene.hpp"


// An example application that renders a simple cube
class ExampleApp : public RiftApp
{
	std::shared_ptr<Scene> scene;

public:
	ExampleApp()
	{
	}

protected:
	void initGl() override
	{
		RiftApp::initGl();
		glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
		glEnable(GL_DEPTH_TEST);
		ovr_RecenterTrackingOrigin(_session);
		scene = std::shared_ptr<Scene>(new Scene());
	}

	void shutdownGl() override
	{
		scene.reset();
	}

	void renderScene(const glm::mat4& projection, const glm::mat4& headPose, ovrEyeType eye, ovrPosef handPoses[]) override
	{
		scene->render(projection, headPose, eye, handPoses);
	}

	void handleButtonInput(int buttonState) {
		scene->handleButtonInput(buttonState); 
	}

	void handleLeftThumbstickInput(ovrVector2f tsState) {
		scene->handleLeftThumbstickInput(tsState);
	}

	void handleRightThumbstickInput(ovrVector2f tsState) {
		scene->handleRightThumbstickInput(tsState); 
	}

	void handleLeftIndTriggerInput(float value) {
		scene->handleLeftIndTriggerInput(value); 
	}

	void handleRightIndTriggerInput(float value) {
		scene->handleRightIndTriggerInput(value);
	}

	void handleLeftMidTriggerInput(float value) {
		scene->handleLeftMidTriggerInput(value);
	}

	void handleRightMidTriggerInput(float value) {
		scene->handleRightMidTriggerInput(value);
	}
};

#endif // EXAMPLEAPP_H