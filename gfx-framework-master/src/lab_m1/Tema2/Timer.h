#pragma once

#include "utils/glm_utils.h"
#include "transform3D.h"
#include "lab_m1/lab5/lab_camera.h"
#include <vector>
#include <tuple>

class Timer
{
public:
	Timer(
		double totalTime,
		double timeElapsed,
		float barWidth,
		float barHeight
		);
	Timer();
	std::vector<std::tuple<glm::mat4, glm::vec3, std::string>> DrawTimer(implemented::Camera* camera, const glm::mat4& projectionMatrix, float posY);
	void UpdateTime(double deltaTime);
	void ResetTime();
	void Start();
	void Stop();
	bool GetActive();
	double GetTimeElapsed();
	double GetTotalTime();

protected:
	double totalTime;
	double timeElapsed;
	float barWidth;
	float barHeight;
	bool isActive;
};