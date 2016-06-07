#define GLEW_STATIC
#include <GL/glew.h>
#include "shprogram.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include "models.h"

#include "Model.h"
#include "TexturedModel.h"
#include "Transfromation.h"
#include "AnimationController.h"
#include "RotationController.h"
#include "DrawableGroup.h"
#include "TimelineController.h"
#include "InvertedAnimation.h"

using namespace std;

const GLuint WIDTH = 800;
const GLuint HEIGHT = 600;

const float FoV = 90.f;

glm::mat4 viewMatrix;
glm::mat4 projectionMatrix;

bool freeFloating = false;

glm::vec3 lightPosition = glm::vec3(0, 7, 7);
float lightIntensity = 50.f;

glm::vec3 position = glm::vec3(0, 0, 5);

float orbitingAngle = 3.14f / 4;

float horizontalAngle = 3.14f;
float verticalAngle = 0.0f;

bool triggered = false;
bool running = false;

GLFWwindow* window;

void computeMatricesFromInputs() {
	const float FoV = 45.0f;
	const float speed = 3.0f; 
	const float mouseSpeed = 0.005f;

	const float orbitingDistance = 15.f;
	const float orbitingHeight = 8.f;

	static double lastTime = glfwGetTime();

	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	double xpos;
	double ypos;

	glfwGetCursorPos(window, &xpos, &ypos);
	if(freeFloating)
		glfwSetCursorPos(window, WIDTH / 2, HEIGHT / 2);

	horizontalAngle += mouseSpeed * float(WIDTH / 2 - xpos);
	verticalAngle += mouseSpeed * float(HEIGHT / 2 - ypos);

	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);

	glm::vec3 right = glm::vec3(sin(horizontalAngle - 3.14f / 2.0f), 0, cos(horizontalAngle - 3.14f / 2.0f));

	glm::vec3 up = glm::cross(right, direction);

	if (glfwGetKey(window, GLFW_KEY_LEFT)) {
		orbitingAngle += 1.f * deltaTime * speed;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT)) {
		orbitingAngle -= 1.f * deltaTime * speed;
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		position += direction * deltaTime * speed;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		position -= direction * deltaTime * speed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		position += right * deltaTime * speed;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		position -= right * deltaTime * speed;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		position += up * deltaTime * speed;
	}
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
		position -= up * deltaTime * speed;
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		lightIntensity += 20 * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		lightIntensity -= 20 * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
		freeFloating = !freeFloating;
		printf("free floating toggle\n");
	}
	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
		triggered = true;
	}

	if (!freeFloating) {
		position = glm::vec3(
			cos(orbitingAngle) * orbitingDistance,
			orbitingHeight,
			sin(orbitingAngle) * orbitingDistance
		);
		direction = glm::vec3(4, 3., 0) - position;
		direction = glm::normalize(direction);
		up = glm::vec3(0, 1, 0);
	}

	projectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);

	viewMatrix = glm::lookAt(position, position + direction, up);

	lastTime = currentTime;
}

int main() {
	if (glfwInit() != GL_TRUE) {
		cout << "GLFW initialization failed" << endl;
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	try {
		window = glfwCreateWindow(WIDTH, HEIGHT, "GKOM - projekt", nullptr, nullptr);
		if (window == nullptr)
			throw exception("GLFW window not created");

		glfwMakeContextCurrent(window);

		glewExperimental = GL_TRUE;

		if (glewInit() != GLEW_OK)
			throw exception("GLEW Initialization failed");

		glViewport(0, 0, WIDTH, HEIGHT);

		glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glDepthFunc(GL_LESS);

		glm::vec3 hammerHeadColour = glm::vec3(.0f, .0f, .0f);
		glm::vec3 hammerHandleColour = glm::vec3(.6f, .3f, .1f);
		glm::vec3 moldColour = glm::vec3(.1f, .1f, .1f);
		glm::vec3 tableColour = glm::vec3(.5f, .2f, .05);

		ShaderProgram solidColourShader("solidColour.vert", "solidColour.frag");
		ShaderProgram texturedShader("textured.vert", "textured.frag");

		TexturedModel board(&board_vertices[0], _countof(board_vertices), &board_indices[0], _countof(board_indices), texturedShader, "wood.png", GL_TEXTURE0);

		TexturedModel hammerHolderL(hammer_holder_vertices, _countof(hammer_holder_vertices), hammer_holder_indices, _countof(hammer_holder_indices), texturedShader, "wood.png", GL_TEXTURE1);
		hammerHolderL.setModelMatrix(glm::translate(glm::vec3(-.80f, 0, 0)));

		Transformation hammerHolderR(glm::translate(glm::vec3(0, 0, 1.9f)), &hammerHolderL);

		Model table(&table_vertices[0], _countof(table_vertices), &table_indices[0], _countof(table_indices), solidColourShader, tableColour);

		Model mold(&mold_vertices[0], _countof(mold_vertices), &mold_indices[0], _countof(mold_indices), solidColourShader, moldColour);

		TexturedModel moldBase(&mold_base_vertices[0], _countof(mold_base_vertices), &mold_base_indices[0], _countof(mold_base_indices), texturedShader, "coin.png", GL_TEXTURE2);
		moldBase.setModelMatrix(glm::translate(glm::vec3(0, 0, -1.6)));

		DrawableGroup moldGroup;
		moldGroup.addModel(&mold);
		moldGroup.addModel(&moldBase);

		Transformation moldGroupTransformed(glm::translate(glm::vec3(0, 0, -0.25)), &moldGroup);

		Model hammerHandle(&hammer_handle_vertices[0], _countof(hammer_handle_vertices), &hammer_handle_indices[0], _countof(hammer_handle_indices), solidColourShader, hammerHandleColour);
		Model hammerHead(&hammer_head_vertices[0], _countof(hammer_head_vertices), &hammer_head_indices[0], _countof(hammer_head_indices), solidColourShader, hammerHeadColour);
		Model hammerTip(&hammer_tip_vertices[0], _countof(hammer_tip_vertices), &hammer_tip_indices[0], _countof(hammer_tip_indices), solidColourShader, hammerHeadColour);

		hammerHead.setModelMatrix(glm::translate(glm::vec3(0, -0.1, 0)));
		hammerTip.setModelMatrix(glm::translate(glm::vec3(-0.1, -0.1, 0)));

		DrawableGroup hammer = DrawableGroup();
		hammer.addModel(&hammerHandle);
		hammer.addModel(&hammerHead);
		hammer.addModel(&hammerTip);

		Transformation hammerTilted = Transformation(glm::rotate(glm::pi<float>() / 4, glm::vec3(0, 0, -1)), &hammer);

		RotationController hammerRotation(glm::pi<float>() / 4, glm::vec3(0, 0, -1));
		hammerRotation.addModel(&hammerTilted);
		RotationController hammerRotationBack(-glm::pi<float>() / 4, glm::vec3(0, 0, -1));
		hammerRotationBack.addModel(&hammerRotation);

		Transformation transformedHammer = Transformation(glm::translate(glm::vec3(-0.75, 2.2, 0)), &hammerRotationBack);

		glm::mat4 tForward = glm::translate(glm::vec3(0, 0, 2.));
		LinearAnimationController moldMovingForward(tForward);
		moldMovingForward.addModel(&moldGroupTransformed);

		glm::mat4 tBack = glm::translate(glm::vec3(0, 0, -2.));
		LinearAnimationController moldMovingBackward(tBack);
		moldMovingBackward.addModel(&moldMovingForward);

		Drawable *moldFinal = &moldMovingBackward;
		Drawable *hammerFinal = &transformedHammer;

		DrawableGroup boardFinal{};
		boardFinal.addModel(&board);
		boardFinal.addModel(&hammerHolderL);
		boardFinal.addModel(&hammerHolderR);

		// animation times

		TimelineController moldTimeline{};
		moldTimeline.addAnimation(&moldMovingForward, 0.0f, 0.23);
		moldTimeline.addAnimation(&moldMovingBackward, 0.5f, 0.73);
		moldTimeline.addDrawable(moldFinal);

		TimelineController hammerTimeline{};
		hammerTimeline.addAnimation(&hammerRotation, 0.45f, 0.5f);
		hammerTimeline.addAnimation(&hammerRotationBack, 0.6f, 1.0f);
		hammerTimeline.addDrawable(hammerFinal);

		double startTime = glfwGetTime();
		double acSpeed = 0.3;

		int animationStage = 0;

		while (!glfwWindowShouldClose(window)) {
			glfwPollEvents();

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glClearColor(0.1f, 0.2f, 0.2f, 1.0f);

			computeMatricesFromInputs();

			boardFinal.draw(projectionMatrix, viewMatrix);
			table.draw(projectionMatrix, viewMatrix);

			float frame = (glfwGetTime() - startTime) * acSpeed;
			float effectiveFrame = running ? frame : 0;

			moldTimeline.setFrame(effectiveFrame / 2 + animationStage * .5);
			moldTimeline.draw(projectionMatrix, viewMatrix);
			std::cout << effectiveFrame << " " << animationStage << std::endl;

			hammerTimeline.setFrame(effectiveFrame);
			hammerTimeline.draw(projectionMatrix, viewMatrix);

			if (triggered && !running) {
				startTime = glfwGetTime();
				triggered = false;
				running = true;
			} else if (running) {
				if (frame > 1.) {
					animationStage = (animationStage + 1) % 2;
					running = false;
					triggered = false;
				}
			}


			glfwSwapBuffers(window);
		}
	} catch (exception ex) {
		cout << ex.what() << endl;
	}

	glfwTerminate();

	return 0;
}
