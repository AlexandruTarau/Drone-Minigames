#include "lab_m1/Tema2/Timer.h"
#include <iostream>

Timer::Timer(
	double totalTime,
	double timeElapsed,
	float barWidth,
	float barHeight
) {
	this->totalTime = totalTime;
	this->timeElapsed = timeElapsed;
	this->barWidth = barWidth;
	this->barHeight = barHeight;
	this->isActive = false;
}
Timer::Timer() {
	this->totalTime = 1;
	this->timeElapsed = 0;
	this->barWidth = 1;
	this->barHeight = 0.1;
	this->isActive = false;
}

std::vector<std::tuple<glm::mat4, glm::vec3, std::string>> Timer::DrawTimer(implemented::Camera* camera, const glm::mat4& projectionMatrix, float posY) {
	std::vector<std::tuple<glm::mat4, glm::vec3, std::string>> matrices;
	glm::mat4 modelMatrix = glm::mat4(1.0f);

	// Position the timer bar in front of the camera, slightly above the center
	glm::vec3 cameraPos = camera->position;
	glm::vec3 cameraForward = camera->forward;
	glm::vec3 barPosition = cameraPos + cameraForward * 2.0f + glm::vec3(0.0f, posY, 0.0f);  // Adjust for distance and vertical offset

	modelMatrix = glm::translate(glm::mat4(1.0f), barPosition);

	// Billboard effect: Align the timer to always face the camera
	glm::mat4 viewMatrix = camera->GetViewMatrix();
	viewMatrix[3] = glm::vec4(0, 0, 0, 1);  // Remove translation to isolate rotation
	modelMatrix *= glm::inverse(viewMatrix);  // Inverse to counteract camera rotation

	// Calculate the width of the timer bar based on remaining time
	float progress = static_cast<float>((totalTime - timeElapsed) / totalTime);
	float barWidthScale = barWidth * progress;  // Adjust width dynamically based on progress

	// Scale for bar dimensions (width is dynamic, height is fixed)
	modelMatrix = glm::scale(modelMatrix, glm::vec3(barWidthScale, barHeight, 0.01f));  // Thin bar depth

	// Add the green timer bar to the list of matrices for rendering
	matrices.push_back(std::tuple(modelMatrix, glm::vec3(0.0f, 1.0f, 0.0f), "box"));  // Green color

	return matrices;
}


void Timer::UpdateTime(double deltaTime) {
	timeElapsed += deltaTime;
}

void Timer::ResetTime() {
	timeElapsed = 0;
}

void Timer::Start() {
	timeElapsed = 0;
	isActive = true;
}

void Timer::Stop() {
	isActive = false;
}

bool Timer::GetActive() {
	return isActive;
}

double Timer::GetTimeElapsed() {
	return timeElapsed;
}

double Timer::GetTotalTime() {
	return totalTime;
}