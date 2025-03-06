#include "lab_m1/Tema2/Tema2.h"

#include <vector>
#include <string>
#include <iostream>

#include "lab_m1/Tema2/transform3D.h"
#include "lab_m1/Tema2/primitives3D.h"
//#include "lab_m1/Tema2/noise.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema2::Tema2()
{
	firstPersonView1 = true;
	firstPersonView2 = true;

    // Player1
    startPosition1 = glm::vec3(10, 6, 10);
	prevPosition1 = startPosition1;
    scale = glm::vec3(1, 1, 1);
    rotation = glm::vec3(0, 0, 0);
	colliderRadius = 0.4f;
    thrust = 4.f;
	pitch = 1;
	yaw = 1;
	roll = 1;
	dashCooldown = 5;
	dashDuration = 1;
	dashMultiplier = 2.5;
    angularSpeed = 1.f;
    propellerAngularSpeed = 20.f;

    // Player2
	startPosition2 = glm::vec3(10, 7, 10);
	prevPosition2 = startPosition2;

    // Terrain
	gridHeight = 1000;
	gridWidth = 1000;
	squareSize = 0.05f;
	noiseScale = 0.2f;
	noiseAmplitude = 1.f;
    marginArea = 3;
    groundLevel = 0;
    maxHeightLevel = 10;
    {
		heightThresholds[0] = 0.35f * noiseAmplitude;
		heightThresholds[1] = 0.65f * noiseAmplitude;
		heightThresholds[2] = 1.f * noiseAmplitude;
	}

	{
        heightColors[0] = glm::vec3(0.1f, 0.5f, 0.1f); // Dark Grass (low elevation)
        heightColors[1] = glm::vec3(0.15f, 0.6f, 0.2f); // Lush Grass
        heightColors[2] = glm::vec3(0.2f, 0.7f, 0.3f); // Bright Grass (sunlit hilltops)
	}

    // Obstacles
    minObstacleNumber = 10;
	maxObstacleNumber = 30;

    rng = std::mt19937_64(rd());
    std::uniform_int_distribution<int> uni(minObstacleNumber, maxObstacleNumber);
    
    obstacleCount = uni(rng);
    obstacleTypes = 2;
    obstacles.reserve(obstacleCount);

	cloudCount = 15;

	// Checkpoints
    checkpointNumber = 5;
	checkpointCount1 = checkpointNumber;
	checkpointCount2 = checkpointNumber;
	checkpoints.reserve(checkpointNumber);

	// Power-ups
	powerUpCount = 0;
	maxPowerUpCount = 10;
	powerUps.reserve(maxPowerUpCount);
	powerUpRadius = 0.5f;
	powerUpSpawnCooldown = 5;
	powerUpSpawnTimer = 0;

    // World Properties
    gravity = 0.981f;
    gameOver = false;
	startGame = false;
	multiplayer = false;
}


Tema2::~Tema2()
{
}


void Tema2::Init()
{
    polygonMode = GL_FILL;

    Mesh* mesh = new Mesh("box");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
    meshes[mesh->GetMeshID()] = mesh;

	mesh = new Mesh("sphere");
	mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
	meshes[mesh->GetMeshID()] = mesh;

    // Initialize meshes
    mesh = primitives3D::CreateCylinder("cylinder", glm::vec3(0.f, 0.f, 0.f), 1.f, 1.f, true);
    meshes[mesh->GetMeshID()] = mesh;

    mesh = primitives3D::CreateCone("cone", glm::vec3(0.f, 0.f, 0.f), 1.f, 1.f, true);
    meshes[mesh->GetMeshID()] = mesh;

	mesh = primitives3D::CreatePrism("prism", glm::vec3(0.f, 0.f, 0.f), 1.f, 1.f, 1.f, true);
	meshes[mesh->GetMeshID()] = mesh;

	mesh = primitives3D::CreateArrow("arrow", glm::vec3(1.f, 0.f, 0.f));
	meshes[mesh->GetMeshID()] = mesh;

    // Initialize player drones
    player1 = Drone(startPosition1, scale, rotation, colliderRadius, thrust, pitch, yaw, roll, angularSpeed, propellerAngularSpeed, dashMultiplier, dashDuration, dashCooldown, 0);
	player2 = Drone(startPosition2, scale, rotation, colliderRadius, thrust, pitch, yaw, roll, angularSpeed, propellerAngularSpeed, dashMultiplier, dashDuration, dashCooldown, 0);

    // Initialize cameras
    camera1 = new implemented::Camera();
	camera2 = new implemented::Camera();

    // Initialize the game timer for singleplayer
	spGameTimer = Timer(60, 0, 1, 0.1);

    // Create terrain
	CreateTerrain(gridWidth, gridHeight, squareSize);

    // Create obstacles
    std::vector<glm::vec3> obstaclePositions;
    obstaclePositions.reserve(obstacleCount);

    for (int i = 0; i < obstacleCount; i++) {
        std::uniform_int_distribution<int> uni(0, obstacleTypes - 1);
		int type = uni(rng);
        glm::vec3 obstaclePosition;

        while (true) {
            uni = std::uniform_int_distribution<int>(marginArea, gridWidth * squareSize - marginArea);
            float posX = uni(rng);
            uni = std::uniform_int_distribution<int>(marginArea, gridHeight * squareSize - marginArea);
            float posZ = uni(rng);

            obstaclePosition = glm::vec3(posX, 0.f, posZ);
            bool invalidPosition = false;

            for (int j = 0; j < obstaclePositions.size(); j++) {
                if (std::pow(obstaclePositions[j].x - posX, 2) + std::pow(obstaclePositions[j].z - posZ, 2) < std::pow(obstacles[j]->GetOwnAreaRadius(), 2)) {
                    invalidPosition = true;
                    break;
                }
            }
            if (!invalidPosition) {
                obstaclePositions.push_back(obstaclePosition);
                break;
            }
        }
        
        switch (type) {
        case 0: {
			// Tree
            obstacles.push_back(std::make_unique<Tree>(obstaclePosition, glm::vec3(1, 1, 1), glm::vec3(0, 0, 0), 2, 0.2, 3, 2, 0.5, 2));
            break;
        }
        case 1: {
			// House
			obstacles.push_back(std::make_unique<House>(obstaclePosition, glm::vec3(1, 1, 1), glm::vec3(0, 0, 0), 4, 2, 2, 2, 1));
            break;
        }
        }
    }

	Cloud::GenerateClouds(clouds, cloudCount, glm::vec3(gridWidth * squareSize, maxHeightLevel, gridHeight * squareSize));

    // Create checkpoints
	for (int i = 0; i < checkpointNumber; i++) {
		glm::vec3 checkpointPosition;
		glm::vec3 checkpointRotation = glm::vec3(0, 0, 0);
        std::uniform_int_distribution<int> uni;
		std::uniform_real_distribution<float> uniFloat(0, 2 * M_PI);

		while (true) {
			uni = std::uniform_int_distribution<int>(marginArea, gridWidth * squareSize - marginArea);
			float posX = uni(rng);
			uni = std::uniform_int_distribution<int>(marginArea, gridHeight * squareSize - marginArea);
			float posZ = uni(rng);
            uni = std::uniform_int_distribution<int>(groundLevel + 2, maxHeightLevel - 5);
			float posY = uni(rng);

			checkpointPosition = glm::vec3(posX, posY, posZ);
			bool invalidPosition = false;

			// Check if the checkpoint is too close to an obstacle
			for (int j = 0; j < obstaclePositions.size(); j++) {
				if (std::pow(obstaclePositions[j].x - posX, 2) + std::pow(obstaclePositions[j].z - posZ, 2) < std::pow(obstacles[j]->GetOwnAreaRadius(), 2)) {
					invalidPosition = true;
					break;
				}
			}
            
			// Check if the checkpoint is too close to another checkpoint
			for (int j = 0; j < checkpoints.size(); j++) {
				if (glm::distance(checkpoints[j].GetPosition(), checkpointPosition) < checkpoints[j].GetOwnAreaRadius()) {
					invalidPosition = true;
					break;
				}
			}

			if (!invalidPosition) {
				break;
			}
		}

		checkpointRotation.y = uniFloat(rng);

		checkpoints.push_back(Checkpoint(checkpointPosition, glm::vec3(1, 1, 1), checkpointRotation, 1, 1, 0.05f, 2));
	}
	currentCheckpoint1 = &checkpoints[0];
	currentCheckpoint2 = &checkpoints[0];

	// Create shader for drone base
    {
        Shader* shader = new Shader("PlainColor");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VS_Plain.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FS_Plain.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // Create shader for terrain
    {
		Shader* shader = new Shader("Terrain");
		shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VS_Terrain.glsl"), GL_VERTEX_SHADER);
		shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FS_Terrain.glsl"), GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
    }

	// Camera settings
	float aspectRatio;
	if (multiplayer) {
		aspectRatio = (window->GetResolution().x / 2.f) / window->GetResolution().y;
	}
	else {
		aspectRatio = window->props.aspectRatio;
	}
    projectionMatrix = glm::perspective(RADIANS(70), aspectRatio, 0.01f, 200.0f);
}

void Tema2::CreateTerrain(int gridWidth, int gridHeight, float squareSize) {
    vector<VertexFormat> vertices;
    vector<unsigned int> indices;

    // Generate vertices for the grid
    for (int z = 0; z <= gridHeight; ++z) {
        for (int x = 0; x <= gridWidth; ++x) {
            // Each vertex has a position (x, 0, z), normal (0, 1, 0), and color (optional)
            vertices.push_back(VertexFormat(
                glm::vec3(x * squareSize, 0, z * squareSize),  // Position
                glm::vec3(0, 1, 0),                            // Normal (upward)
                glm::vec3(0.2f, 0.8f, 0.6f)                    // Color
            ));
        }
    }

    // Generate indices for the grid
    for (int z = 0; z < gridHeight; ++z) {
        for (int x = 0; x < gridWidth; ++x) {
            // Calculate the indices of the corners of the two triangles
            int topLeft = z * (gridWidth + 1) + x;
            int topRight = topLeft + 1;
            int bottomLeft = (z + 1) * (gridWidth + 1) + x;
            int bottomRight = bottomLeft + 1;

            // First triangle
            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);

            // Second triangle
            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }

    // Create the mesh and store it
    Mesh* terrain = new Mesh("terrain");
    terrain->InitFromData(vertices, indices);
    meshes[terrain->GetMeshID()] = terrain;
}

void Tema2::RestartGame() {
	player1 = Drone(startPosition1, scale, rotation, colliderRadius, thrust, pitch, yaw, roll, angularSpeed, propellerAngularSpeed, dashMultiplier, dashDuration, dashCooldown, 0);
	player2 = Drone(startPosition2, scale, rotation, colliderRadius, thrust, pitch, yaw, roll, angularSpeed, propellerAngularSpeed, dashMultiplier, dashDuration, dashCooldown, 0);

	// Reshuflle the obstacles and checkpoint
	rng = std::mt19937_64(rd());
	std::uniform_int_distribution<int> uni(minObstacleNumber, maxObstacleNumber);

	obstacleCount = uni(rng);
	obstacles.clear();
	obstacles.reserve(obstacleCount);

	std::vector<glm::vec3> obstaclePositions;
	obstaclePositions.reserve(obstacleCount);

	// Create obstacles
	for (int i = 0; i < obstacleCount; i++) {
		std::uniform_int_distribution<int> uni(0, obstacleTypes - 1);
		int type = uni(rng);
		glm::vec3 obstaclePosition;

		while (true) {
			uni = std::uniform_int_distribution<int>(marginArea, gridWidth * squareSize - marginArea);
			float posX = uni(rng);
			uni = std::uniform_int_distribution<int>(marginArea, gridHeight * squareSize - marginArea);
			float posZ = uni(rng);

			obstaclePosition = glm::vec3(posX, 0.f, posZ);
			bool invalidPosition = false;

			for (int j = 0; j < obstaclePositions.size(); j++) {
				if (std::pow(obstaclePositions[j].x - posX, 2) + std::pow(obstaclePositions[j].z - posZ, 2) < std::pow(obstacles[j]->GetOwnAreaRadius(), 2)) {
					invalidPosition = true;
					break;
				}
			}
			if (!invalidPosition) {
				obstaclePositions.push_back(obstaclePosition);
				break;
			}
		}

		switch (type) {
		case 0: {
			// Tree
			obstacles.push_back(std::make_unique<Tree>(obstaclePosition, glm::vec3(1, 1, 1), glm::vec3(0, 0, 0), 2, 0.2, 3, 2, 0.5, 2));
			break;
		}
		case 1: {
			// House
			obstacles.push_back(std::make_unique<House>(obstaclePosition, glm::vec3(1, 1, 1), glm::vec3(0, 0, 0), 4, 2, 2, 2, 1));
			break;
		}
		}
	}

	// Create checkpoints
	checkpoints.clear();
	checkpoints.reserve(checkpointNumber);

	for (int i = 0; i < checkpointNumber; i++) {
		glm::vec3 checkpointPosition;
		glm::vec3 checkpointRotation = glm::vec3(0, 0, 0);
		std::uniform_int_distribution<int> uni;
		std::uniform_real_distribution<float> uniFloat(0, 2 * M_PI);

		while (true) {
			uni = std::uniform_int_distribution<int>(marginArea, gridWidth * squareSize - marginArea);
			float posX = uni(rng);
			uni = std::uniform_int_distribution<int>(marginArea, gridHeight * squareSize - marginArea);
			float posZ = uni(rng);
			uni = std::uniform_int_distribution<int>(groundLevel + 2, maxHeightLevel);
			float posY = uni(rng);

			checkpointPosition = glm::vec3(posX, posY, posZ);
			bool invalidPosition = false;

			for (int j = 0; j < obstaclePositions.size(); j++) {
				if (std::pow(obstaclePositions[j].x - posX, 2) + std::pow(obstaclePositions[j].z - posZ, 2) < std::pow(obstacles[j]->GetOwnAreaRadius(), 2)) {
					invalidPosition = true;
					break;
				}
			}

			for (int j = 0; j < checkpoints.size(); j++) {
				if (glm::distance(checkpoints[j].GetPosition(), checkpointPosition) < checkpoints[j].GetOwnAreaRadius()) {
					invalidPosition = true;
					break;
				}
			}

			if (!invalidPosition) {
				break;
			}
		}

		checkpointRotation.y = uniFloat(rng);

		checkpoints.push_back(Checkpoint(checkpointPosition, glm::vec3(1, 1, 1), checkpointRotation, 1, 1, 0.05f, 2));
	}

	checkpointCount1 = checkpointNumber;
	currentCheckpoint1 = &checkpoints[0];

	checkpointCount2 = checkpointNumber;
	currentCheckpoint2 = &checkpoints[0];

	// Restart power-ups
	powerUps.clear();
	powerUpCount = 0;
	powerUps.reserve(maxPowerUpCount);

	spGameTimer = Timer(60, 0, 1, 0.1);
	gameOver = false;
	startGame = false;
	firstPersonView1 = true;
	firstPersonView2 = true;
}

void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Tema2::RenderScene(int playerID)
{
    Drone* player;
	Timer dashTimer;
	Checkpoint* currentCheckpoint;
	implemented::Camera* camera;
    if (playerID == 1) {
        player = &player1;
		currentCheckpoint = currentCheckpoint1;
		camera = camera1;
	}
    else {
        player = &player2;
		currentCheckpoint = currentCheckpoint2;
		camera = camera2;
    }

	// Render terrain
    RenderSimpleMesh(meshes["terrain"], shaders["Terrain"], glm::mat4(1), glm::vec4(0.2, 0.8, 0.6, 1), camera);

    // Render player 1
	{
		std::vector<std::pair<glm::mat4, DronePart>> droneParts = player1.DrawDrone();
		for (auto& part : droneParts) {
			if (part.second == BASE) {
				RenderSimpleMesh(meshes["box"], shaders["PlainColor"], part.first, glm::vec4(0.5, 0.5, 0.5, 1), camera);
			}
			else if (part.second == PROPELLERS) {
				RenderSimpleMesh(meshes["box"], shaders["PlainColor"], part.first, glm::vec4(0, 0, 0, 1), camera);
			}
		}
	}

	// Render player 2
	{
		if (multiplayer) {
			std::vector<std::pair<glm::mat4, DronePart>> droneParts = player2.DrawDrone();
			for (auto& part : droneParts) {
				if (part.second == BASE) {
					RenderSimpleMesh(meshes["box"], shaders["PlainColor"], part.first, glm::vec4(0.5, 0.5, 0.5, 1), camera);
				}
				else if (part.second == PROPELLERS) {
					RenderSimpleMesh(meshes["box"], shaders["PlainColor"], part.first, glm::vec4(0, 0, 0, 1), camera);
				}
			}
		}
	}

	// Render obstacles
    for (auto& obstacle : obstacles) {
        std::vector<std::tuple<glm::mat4, glm::vec3, std::string>> obstacleParts = obstacle->DrawObstacle();
		
        for (auto& part : obstacleParts) {
            RenderSimpleMesh(meshes[std::get<2>(part)], shaders["PlainColor"], std::get<0>(part), glm::vec4(std::get<1>(part), 1), camera);
        }
    }

	// Render clouds
	for (auto& cloud : clouds) {
		std::vector<std::tuple<glm::mat4, glm::vec3, std::string>> cloudParts = cloud.DrawCloud();

		for (auto& part : cloudParts) {
			RenderSimpleMesh(meshes[std::get<2>(part)], shaders["PlainColor"], std::get<0>(part), glm::vec4(std::get<1>(part), 0.7), camera);
		}
	}

    if (startGame) {
        // Render checkpoints
        for (auto& checkpoint : checkpoints) {
            std::vector<std::tuple<glm::mat4, glm::vec3, std::string>> checkpointParts = checkpoint.DrawCheckpoint();
			int chooseColor = 0;

            for (auto& part : checkpointParts) {
				glm::vec3 color = std::get<1>(part);
				if (!gameOver && &checkpoint == currentCheckpoint) {
					if (chooseColor % 2 == 0) {
						if (checkpoint.IsBlocked()) {
							color = glm::vec3(1, 0, 0);
						}
						else {
							color = glm::vec3(0, 1, 0);
						}
					}
					else {
						if (checkpoint.IsBlocked()) {
							color = glm::vec3(1, 0, 0);
						}
						else {
							color = glm::vec3(1, 1, 0);
						}
					}
					chooseColor++;
				}
                RenderSimpleMesh(meshes[std::get<2>(part)], shaders["PlainColor"], std::get<0>(part), glm::vec4(color, 1), camera);
            }
        }

        // Render game timer
		if (!multiplayer && !gameOver) {
			std::vector<std::tuple<glm::mat4, glm::vec3, std::string>> timerParts = spGameTimer.DrawTimer(camera1, projectionMatrix, 0.9);
			for (auto& part : timerParts) {
				RenderSimpleMesh(meshes[std::get<2>(part)], shaders["PlainColor"], std::get<0>(part), glm::vec4(std::get<1>(part), 1), camera);
			}
		}

        if (!gameOver) {
            glm::vec3 playerPos = player->GetPosition();
            glm::vec3 playerForward = player->GetForward();
            glm::vec3 playerUp = player->GetUp();

            glm::vec3 direction = glm::normalize(currentCheckpoint->GetPosition() - playerPos);

            glm::quat rotationQuat = glm::rotation(glm::vec3(0, 0, 1), direction);
            glm::mat4 rotationMatrix = glm::toMat4(rotationQuat);

            float droneForwardOffset = 0.5f;
            float droneUpOffset = 0.1f;
            glm::vec3 localOffset = playerForward * droneForwardOffset + playerUp * droneUpOffset;

            glm::mat4 modelMatrix = glm::mat4(1.0f);
            modelMatrix = glm::translate(modelMatrix, playerPos + localOffset);
            modelMatrix *= rotationMatrix;
            modelMatrix *= glm::scale(glm::mat4(1.0f), glm::vec3(0.2, 0.2, 0.5));

            // Render the arrow
            RenderSimpleMesh(meshes["arrow"], shaders["PlainColor"], modelMatrix, glm::vec4(1.0f, 0.0f, 0.0f, 1.f), camera);
        }
    }

	// Render dash timer
	std::vector<std::tuple<glm::mat4, glm::vec3, std::string>> dashTimerParts = player->DrawDashCD(camera, projectionMatrix, -1.4);
	for (auto& part : dashTimerParts) {
		RenderSimpleMesh(meshes[std::get<2>(part)], shaders["PlainColor"], std::get<0>(part), glm::vec4(std::get<1>(part), 1), camera);
	}

	// Render power-ups
	if (multiplayer && startGame && !gameOver) {
		for (auto& powerUp : powerUps) {
			if (!powerUp.IsActive()) {
				std::vector<std::tuple<glm::mat4, glm::vec3, std::string>> powerUpParts = powerUp.DrawPowerUp();
				for (auto& part : powerUpParts) {
					RenderSimpleMesh(meshes[std::get<2>(part)], shaders["PlainColor"], std::get<0>(part), glm::vec4(std::get<1>(part), 1), camera);
				}
			}
		}
	}
}

void Tema2::Update(float deltaTimeSeconds)
{
    glLineWidth(3);
    glPointSize(5);
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

    // Sets the screen area where to draw
    glm::ivec2 resolution = window->GetResolution();

	// Player 1
	player1.RotatePropellers(deltaTimeSeconds);
	player1.ApplyGravity(deltaTimeSeconds, gravity, gridHeight * squareSize, gridWidth * squareSize, obstacles);
	player1.UpdateDash(deltaTimeSeconds, gridHeight * squareSize, gridWidth * squareSize, obstacles);

    // Set the camera position based on the current view mode
    if (firstPersonView1) {
        // First-person view: slightly above and in front of the drone's position
        glm::vec3 dronePos = player1.GetPosition();
		glm::vec3 offset = player1.GetUp() * 0.35f + player1.GetForward() * 0.1f;
        camera1->Set(dronePos + offset, dronePos + player1.GetForward(), player1.GetUp());
    }
    else {
        // Third-person view: behind the drone
        glm::vec3 dronePos = player1.GetPosition();
        glm::vec3 backwardOffset = -player1.GetForward() * camera1->distanceToTarget;
		glm::vec3 thirdPersonPos = dronePos + backwardOffset + player1.GetUp();
        camera1->Set(thirdPersonPos, dronePos, player1.GetUp());
    }

    // Update timer properties
	if (!gameOver && !multiplayer && spGameTimer.GetActive()) {
		spGameTimer.UpdateTime(deltaTimeSeconds);
		if (spGameTimer.GetTimeElapsed() >= spGameTimer.GetTotalTime()) {
			spGameTimer.Stop();
			gameOver = true;
			cout << "Game Over!" << endl;
		}
	}

	// Check if the drone passed the active checkpoint
    if (startGame && !gameOver && currentCheckpoint1->Passed(player1.GetPosition(), prevPosition1)) {
		int nextIndex = (currentCheckpoint1 - &checkpoints[0] + 1) % checkpointNumber;
		currentCheckpoint1 = &checkpoints[nextIndex];
		checkpointCount1--;

		// Print the number of checkpoints left
		cout << "Checkpoints left player 1: " << checkpointCount1 << endl;

        if (checkpointCount1 == 0) {
            gameOver = true;
			cout << "Game Over!";
			if (multiplayer) {
				cout << " Player 1 wins!";
			}
			cout << endl;
        }
    }

	if (multiplayer) {
		player2.RotatePropellers(deltaTimeSeconds);
		player2.ApplyGravity(deltaTimeSeconds, gravity, gridHeight * squareSize, gridWidth * squareSize, obstacles);
		player2.UpdateDash(deltaTimeSeconds, gridHeight * squareSize, gridWidth * squareSize, obstacles);

		// Set the camera position based on the current view mode
		if (firstPersonView2) {
			// First-person view: slightly above and in front of the drone's position
			glm::vec3 dronePos = player2.GetPosition();
			glm::vec3 offset = player2.GetUp() * 0.35f + player2.GetForward() * 0.1f;
			camera2->Set(dronePos + offset, dronePos + player2.GetForward(), player2.GetUp());
		}
		else {
			// Third-person view: behind the drone
			glm::vec3 dronePos = player2.GetPosition();
			glm::vec3 backwardOffset = -player2.GetForward() * camera2->distanceToTarget;
			glm::vec3 thirdPersonPos = dronePos + backwardOffset + player2.GetUp();
			camera2->Set(thirdPersonPos, dronePos, player2.GetUp());
		}

		// Check if the drone passed the active checkpoint
		if (startGame && !gameOver && currentCheckpoint2->Passed(player2.GetPosition(), prevPosition2)) {
			int nextIndex = (currentCheckpoint2 - &checkpoints[0] + 1) % checkpointNumber;
			currentCheckpoint2 = &checkpoints[nextIndex];
			checkpointCount2--;
			
			// Print the number of checkpoints left
			cout << "Checkpoints left player 2: " << checkpointCount2 << endl;

			if (checkpointCount2 == 0) {
				gameOver = true;
				cout << "Game Over! Player 2 wins!" << endl;
			}
		}
	
		if (startGame && !gameOver) {
			// Generate power-ups
			if (powerUpCount < maxPowerUpCount) {
				powerUpSpawnTimer += deltaTimeSeconds;
				if (powerUpSpawnTimer >= powerUpSpawnCooldown) {
					powerUpSpawnTimer = 0;
					glm::vec3 powerUpPosition;
					std::uniform_int_distribution<int> uni = std::uniform_int_distribution<int>(0, 2);
					int powerUpType = uni(rng);
					PowerUpType type;
					switch (powerUpType) {
					case 0:
						type = PowerUpType::SPEED;
						break;
					case 1:
						type = PowerUpType::STUN;
						break;
					case 2:
						type = PowerUpType::BLOCK;
						break;
					}

					while (true) {
						uni = std::uniform_int_distribution<int>(marginArea, gridWidth * squareSize - marginArea);
						float posX = uni(rng);
						uni = std::uniform_int_distribution<int>(marginArea, gridHeight * squareSize - marginArea);
						float posZ = uni(rng);
						uni = std::uniform_int_distribution<int>(groundLevel + 1, maxHeightLevel - 2);
						float posY = uni(rng);

						powerUpPosition = glm::vec3(posX, posY, posZ);
						bool invalidPosition = false;

						for (int j = 0; j < obstacleCount; j++) {
							if (std::pow(obstacles[j]->GetPosition().x - posX, 2) + std::pow(obstacles[j]->GetPosition().z - posZ, 2) < std::pow(obstacles[j]->GetOwnAreaRadius(), 2)) {
								invalidPosition = true;
								break;
							}
						}

						for (int j = 0; j < checkpoints.size(); j++) {
							if (glm::distance(checkpoints[j].GetPosition(), powerUpPosition) < checkpoints[j].GetOwnAreaRadius()) {
								invalidPosition = true;
								break;
							}
						}

						if (!invalidPosition) {
							break;
						}
					}

					powerUps.push_back(PowerUp(powerUpPosition, glm::vec3(1, 1, 1), glm::vec3(0, 0, 0), 2, 1, type));
					powerUpCount++;
				}
			}

			// Check Power-Up collision
			for (auto& powerUp : powerUps) {
				if (!powerUp.IsActive()) {
					if (powerUp.IsColliding(player1.GetPosition(), player1.GetColliderRadius())) {
						powerUp.SetActive(true);
						powerUp.ApplyPowerUp(player1, player2, &currentCheckpoint2);
					}
					else if (powerUp.IsColliding(player2.GetPosition(), player2.GetColliderRadius())) {
						powerUp.SetActive(true);
						powerUp.ApplyPowerUp(player2, player1, &currentCheckpoint1);
					}
				}
			}

			// Update power-ups
			for (auto it = powerUps.begin(); it != powerUps.end(); ) {
				if (it->UpdatePowerUp(deltaTimeSeconds)) {  // Returns true if the power-up ends
					it = powerUps.erase(it);
					powerUpCount--;
					cout << "Power-Up ended" << endl;
				}
				else {
					++it;
				}
			}
		}
	}

	// Update clouds
	for (auto& cloud : clouds) {
		cloud.UpdateCloud(deltaTimeSeconds, glm::vec3(gridWidth * squareSize, maxHeightLevel, gridHeight * squareSize));
	}

    // Render Scene
	if (!multiplayer) {
		glViewport(0, 0, resolution.x, resolution.y);
		RenderScene(1);
	}
	else {
		glViewport(0, 0, resolution.x / 2, resolution.y);
		RenderScene(1);

		glViewport(resolution.x / 2, 0, resolution.x / 2, resolution.y);
		RenderScene(2);
	}
}

void Tema2::FrameEnd()
{
	prevPosition1 = player1.GetPosition();
	prevPosition2 = player2.GetPosition();
}

void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec4& color, implemented::Camera* camera, float damageFactor)
{
	if (!mesh || !shader || !shader->GetProgramID()) {
		return;
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(shader->program);

	// Set Model matrix
	int location = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Set View matrix
	location = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));

	// Set Projection matrix
	location = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	// Set Color uniform
	location = glGetUniformLocation(shader->program, "color");
	glUniform4fv(location, 1, glm::value_ptr(color));

	// Set damage factor
	location = glGetUniformLocation(shader->program, "damageFactor");
	glUniform1f(location, damageFactor);

	// Set Noise Scale uniform
	location = glGetUniformLocation(shader->program, "noiseScale");
	glUniform1f(location, noiseScale);

	// Set Noise Amplitude uniform
	location = glGetUniformLocation(shader->program, "noiseAmplitude");
	glUniform1f(location, noiseAmplitude);

	location = glGetUniformLocation(shader->program, "heightThresholds");
	glUniform1fv(location, 3, heightThresholds);

	location = glGetUniformLocation(shader->program, "terrainColors");
	glUniform3fv(location, 3, glm::value_ptr(heightColors[0]));

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->m_VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);

	glDisable(GL_BLEND);
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
	// Move the player drone

    // Var 1
	/*if (window->KeyHold(GLFW_KEY_W)) {
        player.MoveForward(deltaTime, gridHeight * squareSize, gridWidth * squareSize, obstacles);
	}
	if (window->KeyHold(GLFW_KEY_S)) {
        player.MoveBackward(deltaTime, gridHeight * squareSize, gridWidth * squareSize, obstacles);
	}
	if (window->KeyHold(GLFW_KEY_A)) {
        player.MoveLeft(deltaTime, gridHeight * squareSize, gridWidth * squareSize, obstacles);
	}
	if (window->KeyHold(GLFW_KEY_D)) {
        player.MoveRight(deltaTime, gridHeight * squareSize, gridWidth * squareSize, obstacles);
	}
	if (window->KeyHold(GLFW_KEY_Q)) {
        player.MoveDown(deltaTime, gridHeight * squareSize, gridWidth * squareSize, obstacles);
	}
	if (window->KeyHold(GLFW_KEY_E)) {
        player.MoveUp(deltaTime, gridHeight * squareSize, gridWidth * squareSize, obstacles);
	}*/

    // Var 2
    /*
    if (window->KeyHold(GLFW_KEY_W)) {
        player.MoveUp(deltaTime, gridHeight * squareSize, gridWidth * squareSize, obstacles);
    }
	if (window->KeyHold(GLFW_KEY_S)) {
		player.MoveDown(deltaTime, gridHeight * squareSize, gridWidth * squareSize, obstacles);
	}
    if (window->KeyHold(GLFW_KEY_UP)) {
        player.RotateOX(-deltaTime);
    }
    if (window->KeyHold(GLFW_KEY_DOWN)) {
        player.RotateOX(deltaTime);
    }
    if (window->KeyHold(GLFW_KEY_A)) {
        player.RotateOY(deltaTime);
    }
    if (window->KeyHold(GLFW_KEY_D)) {
        player.RotateOY(-deltaTime);
    }
	if (window->KeyHold(GLFW_KEY_LEFT)) {
		player.RotateOZ(deltaTime);
	}
	if (window->KeyHold(GLFW_KEY_RIGHT)) {
		player.RotateOZ(-deltaTime);
	}
    */

    // Var 3
    // Player 1
	if (!player1.IsDashing()) {
		if (window->KeyHold(GLFW_KEY_W)) {
			player1.MoveForward(deltaTime, gridHeight * squareSize, gridWidth * squareSize, obstacles);
		}
		if (window->KeyHold(GLFW_KEY_S)) {
			player1.MoveBackward(deltaTime, gridHeight * squareSize, gridWidth * squareSize, obstacles);
		}
		if (window->KeyHold(GLFW_KEY_A)) {
			player1.MoveLeft(deltaTime, gridHeight * squareSize, gridWidth * squareSize, obstacles);
		}
		if (window->KeyHold(GLFW_KEY_D)) {
			player1.MoveRight(deltaTime, gridHeight * squareSize, gridWidth * squareSize, obstacles);
		}
		if (window->KeyHold(GLFW_KEY_Z)) {
			player1.MoveDown(deltaTime, gridHeight * squareSize, gridWidth * squareSize, obstacles);
		}
		if (window->KeyHold(GLFW_KEY_X)) {
			player1.MoveUp(deltaTime, gridHeight * squareSize, gridWidth * squareSize, obstacles);
		}
		if (window->KeyHold(GLFW_KEY_E)) {
			player1.RotateOY(-deltaTime);
		}
		if (window->KeyHold(GLFW_KEY_Q)) {
			player1.RotateOY(deltaTime);
		}
	}
	// Player 2
    if (multiplayer) {
		if (!player2.IsDashing()) {
			if (window->KeyHold(GLFW_KEY_U)) {
				player2.MoveForward(deltaTime, gridHeight * squareSize, gridWidth * squareSize, obstacles);
			}
			if (window->KeyHold(GLFW_KEY_J)) {
				player2.MoveBackward(deltaTime, gridHeight * squareSize, gridWidth * squareSize, obstacles);
			}
			if (window->KeyHold(GLFW_KEY_H)) {
				player2.MoveLeft(deltaTime, gridHeight * squareSize, gridWidth * squareSize, obstacles);
			}
			if (window->KeyHold(GLFW_KEY_K)) {
				player2.MoveRight(deltaTime, gridHeight * squareSize, gridWidth * squareSize, obstacles);
			}
			if (window->KeyHold(GLFW_KEY_N)) {
				player2.MoveDown(deltaTime, gridHeight * squareSize, gridWidth * squareSize, obstacles);
			}
			if (window->KeyHold(GLFW_KEY_M)) {
				player2.MoveUp(deltaTime, gridHeight * squareSize, gridWidth * squareSize, obstacles);
			}
			if (window->KeyHold(GLFW_KEY_I)) {
				player2.RotateOY(-deltaTime);
			}
			if (window->KeyHold(GLFW_KEY_Y)) {
				player2.RotateOY(deltaTime);
			}
		}
    }
}


void Tema2::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_V) {
        firstPersonView1 = !firstPersonView1;
    }
	if (key == GLFW_KEY_B) {
		firstPersonView2 = !firstPersonView2;
	}
	if (key == GLFW_KEY_R) {
		RestartGame();
	}
	if (key == GLFW_KEY_TAB) {
        if (!startGame) {
            startGame = true;
            spGameTimer.Start();
        }
	}
	if (key == GLFW_KEY_GRAVE_ACCENT) {
		multiplayer = !multiplayer;
		RestartGame();
	}
	if (key == GLFW_KEY_SPACE) {
		player1.StartDash();
	}
	if (key == GLFW_KEY_ENTER) {
		if (multiplayer) {
			player2.StartDash();
		}
	}
}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// Rotate the player drone. If I move my mouse to the left, the drone should rotate to the left.
	// If I move my mouse to the right, the drone should rotate to the right.
	// The player only rotates around the OY axis.
	//if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
 //       float sensivityOX = 0.001f;
 //       float sensivityOY = 0.001f;

	//	player.RotateOY(-sensivityOX * deltaX);
	//}
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}
