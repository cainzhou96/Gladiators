#pragma once

#ifndef GAMEPLAYER_H
#define GAMEPLAYER_H

#include "Core.h"
#include "Hand.h"
#define GLOVE_SCALE  0.0015
#define PLATFORM_RADIUS 20



class GamePlayer
{
public:
	glm::mat4 toWorld;
	Hand * handL;
	Hand * handR;
	float mass; 
	glm::vec3 velocity; 
	glm::vec3 moveVelocity; 
	glm::vec3 force; 
	float groundLevel; 
	float groundRadius; 
	glm::vec3 headPos; 

	GamePlayer(int id) {
		handL = new Hand("model/glove_l.obj");
		handR = new Hand("model/glove_r.obj");
		toWorld = glm::mat4(1);
		
		if (id == 1) {
			toWorld[3] = glm::vec4(0, 2.0f, 5, 1);
		}
		else {
			toWorld[3] = glm::vec4(0, 2.0f, -5, 1);
		}
		handL->toWorld = toWorld;
		handR->toWorld = toWorld;
		velocity = glm::vec3(0); 
		moveVelocity = glm::vec3(0); 
		groundLevel = 0; 
	}
	~GamePlayer() {

	}
	void updateRelhandPos(glm::mat4 leftPose, glm::mat4 rightPose) {
		handL->toWorld = toWorld * leftPose;
		handL->toWorld = glm::scale(handL->toWorld, glm::vec3(GLOVE_SCALE));
		handR->toWorld = toWorld * rightPose;
		handR->toWorld = glm::scale(handR->toWorld, glm::vec3(GLOVE_SCALE));
	}

	void updateRelhandPosL(glm::mat4 leftPose) {
		handL->toWorld = toWorld * leftPose;
		handL->toWorld = glm::scale(handL->toWorld, glm::vec3(GLOVE_SCALE));
	}

	void updateRelhandPosR(glm::mat4 rightPose) {
		handR->toWorld = toWorld * rightPose;
		handR->toWorld = glm::scale(handR->toWorld, glm::vec3(GLOVE_SCALE));
	}

	void updatehandPosL(glm::mat4 leftPose) {
		handL->toWorld = leftPose;
		handL->toWorld = glm::scale(handL->toWorld, glm::vec3(GLOVE_SCALE));
	}

	void updatehandPosR(glm::mat4 rightPose) {
		handR->toWorld = rightPose;
		handR->toWorld = glm::scale(handR->toWorld, glm::vec3(GLOVE_SCALE));
	}

	void draw(GLuint shader, const glm::mat4 & projection, const glm::mat4 & view) {
		handL->Draw(shader, projection, view);
		handR->Draw(shader, projection, view);
	}

	void applyVelocity(glm::vec3 v) {
		velocity += v; 
	}

	void setMoveVelocity(glm::vec3 v) {
		moveVelocity = v; 
	}

	bool updatePos(float deltaTime) {
		if (toWorld[3][1] <= - PLATFORM_RADIUS + 1.0f) { // fail
			toWorld[3][1] = -PLATFORM_RADIUS + 1.0f;
			return true;
		}
		glm::vec3 friction = glm::vec3(0); 
		if (glm::length(glm::vec3(headPos.x, 0, headPos.z)) <= PLATFORM_RADIUS + 0.1f && toWorld[3][1] >= 0) { // within ground
			// cout << glm::length(glm::vec3(headPos.x, 0, headPos.z)) << endl;
			velocity.y = velocity.y > 0 ? velocity.y : 0;
			if (glm::length(velocity) > 0) {
				friction = glm::normalize(velocity) * 1.0f * deltaTime;
				if (glm::length(friction) >= glm::length(velocity)) {
					velocity = glm::vec3(0);
				}
				else {
					velocity -= glm::normalize(velocity) * 15.0f * deltaTime;
				}
			}
			toWorld[3] = glm::vec4(glm::vec3(toWorld[3]) + deltaTime * (velocity + moveVelocity), 1);
		}
		else { // out
			// velocity.y += -3.0f * deltaTime; 
			toWorld[3] = glm::vec4(glm::vec3(toWorld[3]) + deltaTime * (velocity), 1);
		}
		

	}

};

#endif // GAMEPLAYER_H
