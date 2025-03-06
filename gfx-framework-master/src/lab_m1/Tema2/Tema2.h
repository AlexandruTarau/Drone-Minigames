#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema2/Drone.h"
#include "lab_m1/lab5/lab_camera.h"
#include "lab_m1/Tema2/Tree.h"
#include "lab_m1/Tema2/House.h"
#include "lab_m1/Tema2/Obstacle.h"
#include "lab_m1/Tema2/Checkpoint.h"
#include "lab_m1/Tema2/Timer.h"
#include "lab_m1/Tema2/Cloud.h"
#include "lab_m1/Tema2/PowerUp.h"
#include <random>
#include <cmath>


namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
    public:
        struct ViewportArea
        {
            ViewportArea() : x(0), y(0), width(1), height(1) {}
            ViewportArea(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}
            int x;
            int y;
            int width;
            int height;
        };

        Tema2();
        ~Tema2();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        void RenderScene(int playerID);
        void CreateTerrain(int gridWidth, int gridHeight, float squareSize);
        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec4& color, implemented::Camera* camera, float damageFactor = 0.f);
		void RestartGame();
    protected:
        glm::mat4 modelMatrix;
        GLenum polygonMode;

        // Player
        Drone player1, player2;
        glm::vec3 startPosition1, startPosition2;
		glm::vec3 prevPosition1, prevPosition2;
		glm::vec3 scale;
		glm::vec3 rotation;
        float colliderRadius;
        float thrust;
        float pitch;
        float yaw;
		float roll;
		float dashMultiplier;
		float dashDuration;
		float dashCooldown;
		float angularSpeed;
		float propellerAngularSpeed;

        // Camera
        bool firstPersonView1;
		bool firstPersonView2;

        implemented::Camera* camera1;
		implemented::Camera* camera2;
        glm::mat4 projectionMatrix;

        float distance;
        float angle;
        float fov;
        float fov_step;
        float ortho_left;
        float ortho_up;

		// Terrain
		float gridWidth;
		float gridHeight;
		float squareSize;
        float noiseScale;
		float noiseAmplitude;
		float heightThresholds[3];
		glm::vec3 heightColors[3];
        float marginArea;
        float groundLevel;
		float maxHeightLevel;

        // Obstacles
        std::vector<std::unique_ptr<Obstacle>> obstacles;
        int obstacleTypes;
		int obstacleCount;
        int minObstacleNumber;
		int maxObstacleNumber;
		float minObstacleDistance;

        std::vector<Cloud> clouds;
		int cloudCount;

        // Checkpoints
		std::vector<Checkpoint> checkpoints;
		Checkpoint *currentCheckpoint1, *currentCheckpoint2;
		int checkpointCount1, checkpointCount2;
        int checkpointNumber;

		// Power-ups
		std::vector<PowerUp> powerUps;
		int powerUpCount;
		int maxPowerUpCount;
		float powerUpRadius;
		float powerUpSpawnTimer;
		float powerUpSpawnCooldown;

		// World Properties
        float gravity;

        // UI
		Timer spGameTimer;

        // Others
        std::random_device rd;
        std::mt19937_64 rng;
        bool gameOver;
		bool startGame;
		bool multiplayer;
    };
}   // namespace m1

