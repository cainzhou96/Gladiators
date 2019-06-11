#pragma once
#ifndef SCENE_H
#define SCENE_H
//////////////////////////////////////////////////////////////////////
//
// The remainder of this code is specific to the scene we want to 
// render.  I use glfw to render an array of cubes, but your 
// application would perform whatever rendering you want
//

#include <vector>
#include <chrono>
#include "Core.h"
#include "shader.h"
#include "Cube.h"
#include "GamePlayer.h"
#include "Particle.h"
#include "SpringDamper.h"
#include "rpc/client.h"
#include "Player.h"
#include "irrKlang.h"

using namespace irrklang;

#define INITIAL_CUBE_SCALE 0.1f
#define BUFFER_SIZE 30
#define WIRE_LENGTH 7.0f
#define KS 20
#define KD 2
#define PART_NUM 7
#define FLY_TIME 1
#define HEAD_SCALE 0.02f
#define MOVE_SPEED 3.0f
#define GLOVE_RADIUS 0.2f
#define PLAYER_RADIUS 1
#define G 9.8
#define STRENGTH 0.1

// a class for building and rendering cubes
class Scene
{
	// Program
	std::vector<glm::mat4> instance_positions;
	GLuint instanceCount;
	GLuint skyboxShader;
	GLuint shaderID2;

	std::unique_ptr<TexturedCube> cube;
	std::unique_ptr<Skybox> skyboxL;
	std::unique_ptr<Skybox> skyboxR;
	GamePlayer* mainPlayer; 
	glm::mat4 headPose; 
	glm::mat4 handPoses[2]; 
	vector<Particle*> particlesL; 
	vector<SpringDamper*> springsL; 
	vector<Particle*> particlesR;
	vector<SpringDamper*> springsR;

	Model* head1;
	Model* head2;
	Model* gloveL;
	Model* gloveR;
	Model * platform;
	Model * body;
	Model * spikes;
	Model * star;

	glm::mat4 star1Transform;
	glm::mat4 star2Transform;
	glm::mat4 star3Transform;

	//glm::mat4 p1ToWorld;
	//glm::mat4 p2ToWorld;
	glm::mat4 playerToWorld;
	glm::mat4 opponentToWorld;
	glm::mat4 playerGloveL;
	glm::mat4 playerGloveR;
	glm::mat4 opponentGloveL;
	glm::mat4 opponentGloveR;
	glm::mat4 opponentPrevGloveL; 
	glm::mat4 opponentPrevGloveR;
	int id = 2;
	Mat4 p1;
	Mat4 p2;
	Mat4 p1HandL;
	Mat4 p1HandR;
	Mat4 p2HandL;
	Mat4 p2HandR;
	rpc::client* c;

	unsigned int skyboxTexture;
	vector<std::string> faces
	{
		"skybox/skyrender0001.tga",
		"skybox/skyrender0004.tga",
		"skybox/skyrender0003.tga",
		"skybox/skyrender0006.tga",
		"skybox/skyrender0005.tga",
		"skybox/skyrender0002.tga"
	};

	ISoundEngine * soundEngine;
	ISound * bgm;
	ISound * hit;
	ISound * fire;

	int p1Score = 0;
	int p2Score = 0;
	bool isDead = false;

	const unsigned int GRID_SIZE{ 5 };

	bool X_Pressed = false; 
	// bool A_Pressed = false;
	bool LIndTrigger_Pressed = false; 
	bool RIndTrigger_Pressed = false;
	bool LMidTrigger_Pressed = false;
	bool RMidTrigger_Pressed = false;

	bool firedL = false; 
	bool firedR = false; 
	bool retrievingL = false; 
	bool retrievingR = false; 
	bool firstFrame = true; 
	bool status = false; 
	bool failTimeSet = false; 
	std::chrono::high_resolution_clock::time_point time; 
	std::chrono::high_resolution_clock::time_point failed_time;
	std::chrono::high_resolution_clock::time_point retrieveTimeL;
	std::chrono::high_resolution_clock::time_point retrieveTimeR;
	std::chrono::high_resolution_clock::time_point punchTime;

	float curCubeSize = INITIAL_CUBE_SCALE;
public:
	Scene()
	{
		time = std::chrono::high_resolution_clock::now(); 
		punchTime = std::chrono::high_resolution_clock::now();

		// Create two cube
		instance_positions.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -0.3)));
		instance_positions.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -0.9)));

		instanceCount = instance_positions.size();

		// Shader Programs 
		skyboxShader = LoadShaders("skybox.vert", "skybox.frag");
		shaderID2 = LoadShaders("shader.vert", "shader.frag");

		cube = std::make_unique<TexturedCube>("cube");

		// 10m wide sky box: size doesn't matter though
		skyboxL = std::make_unique<Skybox>("skybox");
		skyboxL->toWorld = glm::scale(glm::mat4(1.0f), glm::vec3(5.0f));

		skyboxR = std::make_unique<Skybox>("skybox_righteye");
		skyboxR->toWorld = glm::scale(glm::mat4(1.0f), glm::vec3(5.0f));

		mainPlayer = new GamePlayer(id); 
		headPose = glm::mat4(1); 
		handPoses[0] = glm::mat4(1);
		handPoses[1] = glm::mat4(1);

		playerToWorld = glm::mat4(1);
		opponentToWorld = glm::mat4(1);
		playerGloveL = glm::mat4(1);
		playerGloveR = glm::mat4(1);
		opponentGloveL = glm::mat4(1);
		opponentGloveR = glm::mat4(1);
		opponentPrevGloveL = glm::mat4(1);
		opponentPrevGloveR = glm::mat4(1);
		head1 = new Model("model/rino.obj");
		head2 = new Model("model/unicorn.obj");
		gloveL = new Model("model/glove_l.obj");
		gloveR = new Model("model/glove_r.obj");
		if (id == 1) {
			mainPlayer->handL->color = glm::vec3(0.098, 0.098, 0.439); 
			mainPlayer->handR->color = glm::vec3(0.098, 0.098, 0.439);
			gloveL->setColor(glm::vec3(0.698, 0.133, 0.133));
			gloveR->setColor(glm::vec3(0.698, 0.133, 0.133));
		}
		else {
			mainPlayer->handL->color = glm::vec3(0.698, 0.133, 0.133);
			mainPlayer->handR->color = glm::vec3(0.698, 0.133, 0.133);
			gloveL->setColor(glm::vec3(0.098, 0.098, 0.439));
			gloveR->setColor(glm::vec3(0.098, 0.098, 0.439));
		}
		platform = new Model("model/platform.obj");
		platform->setColor(glm::vec3(1, 1, 0.878));
		body = new Model("model/body.obj");
		spikes = new Model("model/spikes.obj");
		spikes->setColor(glm::vec3(0.8)); 
		star = new Model("model/star.obj");
		status = false; 

		skyboxTexture = loadCubemap(faces);
		soundEngine = createIrrKlangDevice();
		bgm = soundEngine->play2D("sound/BGM.mp3", true, false, true);
		bgm->setVolume(0.25f);

		glm::mat4 S3 = glm::scale(glm::mat4(1), glm::vec3(2));
		glm::mat4 T3 = glm::translate(glm::mat4(1), glm::vec3(19, 15, -10));
		glm::mat4 R3 = glm::rotate(glm::mat4(1), glm::radians(105.0f), glm::vec3(0, 0, 1));
		star1Transform = S3 * T3 * R3;

		S3 = glm::scale(glm::mat4(1), glm::vec3(2));
		T3 = glm::translate(glm::mat4(1), glm::vec3(21, 0, -11));
		R3 = glm::rotate(glm::mat4(1), glm::radians(95.0f), glm::vec3(0, 0, 1));
		star2Transform = S3 * T3 * R3;

		S3 = glm::scale(glm::mat4(1), glm::vec3(2));
		T3 = glm::translate(glm::mat4(1), glm::vec3(20, -15, -10));
		R3 = glm::rotate(glm::mat4(1), glm::radians(85.0f), glm::vec3(0, 0, 1));
		star3Transform = S3 * T3 * R3;

		if(id == 1)
			c = new rpc::client("localhost", 8080);
		else
			c = new rpc::client("128.54.70.52", 8080);
	}

	void render(const glm::mat4& projection, glm::mat4 headPose, ovrEyeType eye, ovrPosef handPoses[])
	{
		if (status) {
			float elapsedTime = chrono::duration_cast<chrono::microseconds>(std::chrono::high_resolution_clock::now() - failed_time).count() * 0.000001;
			if (!isDead) {
				isDead = true;
				if (id == 1) {
					p2Score++;
				}
				else {
					p1Score++;
				}
			}
			if (elapsedTime >= 3.0f) {
				if (id == 1) {
					mainPlayer->toWorld = glm::mat4(1); 
					mainPlayer->toWorld[3] = glm::vec4(0, 2.0f, 5, 1);
				}
				else {
					mainPlayer->toWorld = glm::mat4(1);
					mainPlayer->toWorld[3] = glm::vec4(0, 2.0f, -5, 1);
				}
				status = false; 
				failTimeSet = false; 
				isDead = false;
			}
			
			
		}
		if (eye == 0) {
			this->headPose = headPose;
			mainPlayer->headPos = mainPlayer->toWorld * headPose[3]; 
		}
		this->handPoses[0] = ovr::toGlm(handPoses[ovrHand_Left]); 
		this->handPoses[1] = ovr::toGlm(handPoses[ovrHand_Right]);
		
		playerToWorld = mainPlayer->toWorld * this->headPose;
		playerGloveL = mainPlayer->handL->toWorld;
		playerGloveR = mainPlayer->handR->toWorld;
		updatePlayersFromServer();
		cout << "P1 Score" << p1Score << endl;
		cout << "P2 Score" << p2Score << endl;

		// update particles & springs
		float elapsedTime = chrono::duration_cast<chrono::microseconds>(std::chrono::high_resolution_clock::now() - time).count() * 0.000001;
		if (!firstFrame) {
			checkCollisions(elapsedTime);
		} else {
			firstFrame = false;
		}
		time = std::chrono::high_resolution_clock::now(); 
		int timeStep = 100; 
		elapsedTime /= (float)timeStep; 
		for (int t = 0; t < timeStep; t++) {

			// update player
			mainPlayer->applyVelocity(glm::vec3(0, -G, 0) * elapsedTime);
			status = mainPlayer->updatePos(elapsedTime);
			if (status && !failTimeSet) {
				failTimeSet = true; 
				failed_time = std::chrono::high_resolution_clock::now(); 
			}

			// update left
			for (int i = 0; i < particlesL.size(); i++) {
				if (i == 0) {
					particlesL[i]->setPosition((mainPlayer->toWorld * this->handPoses[0])[3]);
				}
				particlesL[i]->applyForce(glm::vec3(0, -G, 0) * particlesL[i]->getMass()); 
			}
			for (int i = 0; i < springsL.size(); i++) {
				springsL[i]->computeForce();
			}
			for (int i = 0; i < particlesL.size(); i++) {
				particlesL[i]->update(elapsedTime);
			}

			// update right
			for (int i = 0; i < particlesR.size(); i++) {
				if (i == 0) {
					particlesR[i]->setPosition((mainPlayer->toWorld * this->handPoses[1])[3]);
				}
				particlesR[i]->applyForce(glm::vec3(0, -G, 0) * particlesR[i]->getMass()); 
			}
			for (int i = 0; i < springsR.size(); i++) {
				springsR[i]->computeForce();
			}
			for (int i = 0; i < particlesR.size(); i++) {
				particlesR[i]->update(elapsedTime);
			}
		}

		glm::mat4 view = glm::inverse(mainPlayer->toWorld * headPose);

		// check left glove
		if (!firedL) {
			mainPlayer->updateRelhandPosL(ovr::toGlm(handPoses[ovrHand_Left]));
		}
		else {
			particlesL[0]->setPosition((mainPlayer->toWorld * this->handPoses[0])[3]);
			if (retrievingL) {
				if (glm::length(glm::vec3((mainPlayer->handL->toWorld)[3] - (mainPlayer->toWorld * this->handPoses[0])[3])) < 0.1f) {
					firedL = false; 
					retrievingL = false; 
					particlesL.clear();
					springsL.clear();
					mainPlayer->updateRelhandPosL(ovr::toGlm(handPoses[ovrHand_Left]));
				}
				else {
					if (chrono::duration_cast<chrono::seconds>(std::chrono::high_resolution_clock::now() - retrieveTimeL).count() >= FLY_TIME) { // drag the glove back whatsoever
						glm::mat4 tempPose = ovr::toGlm(handPoses[ovrHand_Left]);
						tempPose[3] = glm::vec4(glm::vec3(mainPlayer->handL->toWorld[3] - 0.1 * (mainPlayer->handL->toWorld[3] - (mainPlayer->toWorld * this->handPoses[0])[3])), 1);
						mainPlayer->updatehandPosL(tempPose);
					}
					else {
						glm::mat4 tempPose = ovr::toGlm(handPoses[ovrHand_Left]);
						tempPose[3] = glm::vec4(particlesL[springsL.size() - 1]->getPosition(), 1);
						mainPlayer->updatehandPosL(tempPose);
					}
				}
			}
			else {
				glm::mat4 tempPose = ovr::toGlm(handPoses[ovrHand_Left]);
				tempPose[3] = glm::vec4(particlesL[springsL.size() - 1]->getPosition(), 1);
				mainPlayer->updatehandPosL(tempPose);
			}
		}

		// check right glove
		if (!firedR) {
			mainPlayer->updateRelhandPosR(ovr::toGlm(handPoses[ovrHand_Right]));
		}
		else {
			particlesR[0]->setPosition((mainPlayer->toWorld * this->handPoses[1])[3]);
			if (retrievingR) {
				if (glm::length(glm::vec3((mainPlayer->handR->toWorld)[3] - (mainPlayer->toWorld * this->handPoses[1])[3])) < 0.1f) {
					firedR = false;
					retrievingR = false;
					particlesR.clear();
					springsR.clear();
					mainPlayer->updateRelhandPosR(ovr::toGlm(handPoses[ovrHand_Right]));
				}
				else {
					if (chrono::duration_cast<chrono::seconds>(std::chrono::high_resolution_clock::now() - retrieveTimeR).count() >= FLY_TIME) { // drag the glove back whatsoever
						glm::mat4 tempPose = ovr::toGlm(handPoses[ovrHand_Right]);
						tempPose[3] = glm::vec4(glm::vec3(mainPlayer->handR->toWorld[3] - 0.1 * (mainPlayer->handR->toWorld[3] - (mainPlayer->toWorld * this->handPoses[1])[3])), 1);
						mainPlayer->updatehandPosR(tempPose);
					}
					else {
						glm::mat4 tempPose = ovr::toGlm(handPoses[ovrHand_Right]);
						tempPose[3] = glm::vec4(particlesR[springsR.size() - 1]->getPosition(), 1);
						mainPlayer->updatehandPosR(tempPose);
					}
				}
			}
			else {
				glm::mat4 tempPose = ovr::toGlm(handPoses[ovrHand_Right]);
				tempPose[3] = glm::vec4(particlesR[springsR.size() - 1]->getPosition(), 1);
				mainPlayer->updatehandPosR(tempPose);
			}
		}
		
		glm::mat4 T = glm::translate(glm::mat4(1), glm::vec3(0, -1, 0));
		glm::mat4 S = glm::scale(glm::mat4(1), glm::vec3(PLATFORM_RADIUS));
		platform->Draw(shaderID2, projection, view, S*T);
		spikes->Draw(shaderID2, projection, view, S*T);
		mainPlayer->draw(shaderID2, projection, view);
		glm::mat4 R = glm::rotate(glm::mat4(1), glm::radians(180.0f), glm::vec3(0, 1, 0));
		if (id == 1) {
			head2->Draw(shaderID2, projection, view, glm::scale(opponentToWorld, glm::vec3(HEAD_SCALE)) * R);	
		}
		else {
			head1->Draw(shaderID2, projection, view, glm::scale(opponentToWorld, glm::vec3(HEAD_SCALE)) * R);
		}
		glm::mat4 T2 = glm::translate(glm::mat4(1), glm::vec3(0, -0.8f, 0));
		glm::mat4 S2 = glm::scale(glm::mat4(1), glm::vec3(0.8f));
		body->Draw(shaderID2, projection, view, opponentToWorld * T2 * S2);
		
		if (id == 1) {
			if (p1Score >= 1) 
				star->Draw(shaderID2, projection, view, playerGloveR * star1Transform);
			if(p1Score >= 2)
				star->Draw(shaderID2, projection, view, playerGloveR * star2Transform);
			if(p1Score >= 3)
				star->Draw(shaderID2, projection, view, playerGloveR * star3Transform);
		}
		if (id == 2) {
			if (p2Score >= 1)
				star->Draw(shaderID2, projection, view, playerGloveR * star1Transform);
			if (p2Score >= 2)
				star->Draw(shaderID2, projection, view, playerGloveR * star2Transform);
			if (p2Score >= 3)
				star->Draw(shaderID2, projection, view, playerGloveR * star3Transform);
		}

		/*glm::mat4 S3 = glm::scale(glm::mat4(1), glm::vec3(2));
		glm::mat4 T3 = glm::translate(glm::mat4(1), glm::vec3(19, 15, -10));
		glm::mat4 R3 = glm::rotate(glm::mat4(1), glm::radians(105.0f), glm::vec3(0, 0, 1));
		star->Draw(shaderID2, projection, view, playerGloveR * S3 * T3 * R3);

		S3 = glm::scale(glm::mat4(1), glm::vec3(2));
		T3 = glm::translate(glm::mat4(1), glm::vec3(21, 0, -11));
		R3 = glm::rotate(glm::mat4(1), glm::radians(95.0f), glm::vec3(0, 0, 1));
		star->Draw(shaderID2, projection, view, playerGloveR * S3 * T3 * R3);

		S3 = glm::scale(glm::mat4(1), glm::vec3(2));
		T3 = glm::translate(glm::mat4(1), glm::vec3(20, -15, -10));
		R3 = glm::rotate(glm::mat4(1), glm::radians(85.0f), glm::vec3(0, 0, 1));
		star->Draw(shaderID2, projection, view, playerGloveR * S3 * T3 * R3);*/

		gloveL->Draw(shaderID2, projection, view, opponentGloveL);
		gloveR->Draw(shaderID2, projection, view, opponentGloveR);


		//draw skybox
		glUseProgram(skyboxShader);
		glm::mat4 S4 = glm::scale(glm::mat4(1), glm::vec3(100));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader, "projection"), 1, GL_FALSE, &projection[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader, "model"), 1, GL_FALSE, &S4[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader, "view"), 1, GL_FALSE, &view[0][0]);
		
		glBindVertexArray(skyboxL->VAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
		glUniform1i(glGetUniformLocation(skyboxShader, "skybox"), 0);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		
	}


	void updatePlayersFromServer() {

		if (id == 1) {
			p1.toWorld = playerToWorld;
			c->call("updatePlayer", p1, id);
			p2 = c->call("getPlayer", 2).as<Mat4>();
			opponentToWorld = p2.toWorld;

			p1HandL.toWorld = playerGloveL;
			p1HandR.toWorld = playerGloveR;
			c->call("updateHands", p1HandL, p1HandR, 1);
			p2HandL = c->call("getHandL", 2).as<Mat4>();
			p2HandR = c->call("getHandR", 2).as<Mat4>();
			opponentGloveL = p2HandL.toWorld;
			opponentGloveR = p2HandR.toWorld;

		}
		else {// P2
			p2.toWorld = playerToWorld;
			c->call("updatePlayer", p2, id);
			p1 = c->call("getPlayer", 1).as<Mat4>();
			opponentToWorld = p1.toWorld;

			p2HandL.toWorld = playerGloveL;
			p2HandR.toWorld = playerGloveR;
			c->call("updateHands", p2HandL, p2HandR, 2);
			p1HandL = c->call("getHandL", 1).as<Mat4>();
			p1HandR = c->call("getHandR", 1).as<Mat4>();
			opponentGloveL = p1HandL.toWorld;
			opponentGloveR = p1HandR.toWorld;

		}
		c->call("updateScore", p1Score, 1);
		c->call("updateScore", p2Score, 2);
		p1Score = c->call("getScore", 1).as<int>();
		p2Score = c->call("getScore", 2).as<int>();
	}


	void checkCollisions(float elapsedTime) {
		// check if opponent's gloves hit main player
		glm::vec3 playerPos = glm::vec3(playerToWorld[3]);
		playerPos += glm::vec3(0, -0.8f, 0);
		glm::vec3 oppPos = glm::vec3(opponentToWorld[3]);
		glm::vec3 oppGloveLPos = glm::vec3(opponentGloveL[3]);
		glm::vec3 oppGloveRPos = glm::vec3(opponentGloveR[3]);
		glm::vec3 gloveLPos = glm::vec3(playerGloveL[3]);
		glm::vec3 gloveRPos = glm::vec3(playerGloveR[3]);

		glm::vec3 prevGloveLPos = glm::vec3(opponentPrevGloveL[3]);
		glm::vec3 prevGloveRPos = glm::vec3(opponentPrevGloveR[3]);
		if (glm::length(playerPos - oppGloveLPos) < (GLOVE_RADIUS + PLAYER_RADIUS) && chrono::duration_cast<chrono::microseconds>(std::chrono::high_resolution_clock::now() - punchTime).count() * 0.000001 > 0.1) {
			glm::vec3 moveVec = STRENGTH * (oppGloveLPos - prevGloveLPos) / elapsedTime;
			mainPlayer->applyVelocity(moveVec);
			playHitSound();
		}
		if (glm::length(playerPos - oppGloveRPos) < (GLOVE_RADIUS + PLAYER_RADIUS)) {
			glm::vec3 moveVec = STRENGTH * (oppGloveRPos - prevGloveRPos) / elapsedTime;
			mainPlayer->applyVelocity(moveVec);
			playHitSound();
		}
		if (glm::length(oppPos - gloveLPos) < (GLOVE_RADIUS + PLAYER_RADIUS) ||
			glm::length(oppPos - gloveRPos) < (GLOVE_RADIUS + PLAYER_RADIUS)) {
			playHitSound();
		}
		opponentPrevGloveL = opponentGloveL; 
		opponentPrevGloveR = opponentGloveR;
		punchTime = std::chrono::high_resolution_clock::now();
	}


	void playHitSound() {
		hit = soundEngine->play2D("sound/hit.mp3", false, false, true);
		hit->setVolume(0.25f);
	}


	void handleButtonInput(int buttonState) {
		if (buttonState & ovrButton_X) {
			if (!X_Pressed) {
				X_Pressed = true;
			}
		}
		else {
			X_Pressed = false;
		}

		/*if (buttonState & ovrButton_A) {
			if (!A_Pressed) {
				A_Pressed = true;
			}
		}
		else {
			A_Pressed = false;
		}*/

	}

	void handleLeftThumbstickInput(ovrVector2f tsState) {
		glm::vec3 moveVec = glm::mat3(headPose) * glm::vec3(tsState.x, 0, -tsState.y);
		if (glm::length(moveVec) > 0.000001) {
			mainPlayer->setMoveVelocity(glm::normalize(glm::vec3(moveVec.x, 0, moveVec.z)) * MOVE_SPEED);
		}
		else {
			mainPlayer->setMoveVelocity(glm::vec3(0));
		}
	}

	void handleRightThumbstickInput(ovrVector2f tsState) {

	}

	void handleLeftIndTriggerInput(float value) {
		if (value > 0.7) {
			if (!LIndTrigger_Pressed) {
				LIndTrigger_Pressed = true; 
				if (firedL && !retrievingL) {
					particlesL.clear();
					springsL.clear();
					for (int i = 0; i < PART_NUM; i++) {
						glm::vec3 tempPos = (mainPlayer->handL->toWorld)[3];
						tempPos -= (PART_NUM -1 - i) * (tempPos - glm::vec3((mainPlayer->toWorld * handPoses[0])[3])) / (float)(PART_NUM - 1);
						Particle* tempP;
						if (i == 0) {
							tempP = new Particle(0.001, true, tempPos);
						}
						else {
							tempP = new Particle(0.001, false, tempPos);
						}
						particlesL.push_back(tempP);
					}
					for (int i = 0; i < PART_NUM - 1; i++) {
						SpringDamper* tempS = new SpringDamper(KS, KD, 0, particlesL[i], particlesL[i + 1]);
						springsL.push_back(tempS);
					}
					retrievingL = true;
					retrieveTimeL = std::chrono::high_resolution_clock::now();
				}
			} 
		}
		else {
			LIndTrigger_Pressed = false;
		}
	}

	void handleRightIndTriggerInput(float value) {
		if (value > 0.7) {
			if (!RIndTrigger_Pressed) {
				RIndTrigger_Pressed = true;
				if (firedR && !retrievingR) {
					particlesR.clear();
					springsR.clear();
					for (int i = 0; i < PART_NUM; i++) {
						glm::vec3 tempPos = (mainPlayer->handR->toWorld)[3];
						tempPos -= (PART_NUM - 1 - i) * (tempPos - glm::vec3((mainPlayer->toWorld * handPoses[1])[3])) / (float)(PART_NUM - 1);
						Particle* tempP;
						if (i == 0) {
							tempP = new Particle(0.001, true, tempPos);
						}
						else {
							tempP = new Particle(0.001, false, tempPos);
						}
						particlesR.push_back(tempP);
					}
					for (int i = 0; i < PART_NUM - 1; i++) {
						SpringDamper* tempS = new SpringDamper(KS, KD, 0, particlesR[i], particlesR[i + 1]);
						springsR.push_back(tempS);
					}
					retrievingR = true;
					retrieveTimeR = std::chrono::high_resolution_clock::now();
				}
			}
		}
		else {
			RIndTrigger_Pressed = false;
		}
	}

	void handleLeftMidTriggerInput(float value) {
		if (value > 0.7) {
			if (!LMidTrigger_Pressed) {
				LMidTrigger_Pressed = true;
				fire = soundEngine->play2D("sound/fire.wav", false, false, true);
				fire->setVolume(0.8f);
				if (!firedL && !retrievingL) {
					for (int i = 0; i < PART_NUM; i++) {
						glm::vec3 tempPos = (mainPlayer->toWorld * handPoses[0])[3]; 
						tempPos -= (PART_NUM - 1 - i) * glm::normalize(-glm::vec3((mainPlayer->toWorld * handPoses[0])[2])) * 0.1 / (float)(PART_NUM - 1);
						Particle* tempP; 
						if (i == 0) {
							tempP = new Particle(0.001, true, tempPos);
						}
						else {
							tempP = new Particle(0.001, false, tempPos);
						}
						particlesL.push_back(tempP);
					}
					for (int i = 0; i < PART_NUM - 1; i++) {
						SpringDamper* tempS = new SpringDamper(KS, KD, WIRE_LENGTH / (float)(PART_NUM - 1), particlesL[i], particlesL[i + 1]);
						springsL.push_back(tempS); 
					}
					firedL = true; 
				}
			}
		}
		else {
			LMidTrigger_Pressed = false;
		}
	}

	void handleRightMidTriggerInput(float value) {
		if (value > 0.7) {
			if (!RMidTrigger_Pressed) {
				RMidTrigger_Pressed = true;
				fire = soundEngine->play2D("sound/fire.wav", false, false, true);
				fire->setVolume(0.8f);
				if (!firedR && !retrievingR) {
					for (int i = 0; i < PART_NUM; i++) {
						glm::vec3 tempPos = (mainPlayer->toWorld * handPoses[1])[3];
						tempPos -= (PART_NUM - 1 - i) * glm::normalize(-glm::vec3((mainPlayer->toWorld * handPoses[1])[2])) * 0.1 / (float)(PART_NUM - 1);
						Particle* tempP;
						if (i == 0) {
							tempP = new Particle(0.001, true, tempPos);
						}
						else {
							tempP = new Particle(0.001, false, tempPos);
						}
						particlesR.push_back(tempP);
					}
					for (int i = 0; i < PART_NUM - 1; i++) {
						SpringDamper* tempS = new SpringDamper(KS, KD, WIRE_LENGTH / (float)(PART_NUM - 1), particlesR[i], particlesR[i + 1]);
						springsR.push_back(tempS);
					}
					firedR = true;
				}
			}
		}
		else {
			RMidTrigger_Pressed = false;
		}
	}


	unsigned int loadCubemap(vector<string> faces)
	{
		unsigned int textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

		int width, height, nrChannels;
		for (unsigned int i = 0; i < faces.size(); i++)
		{
			unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
			if (data)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
					0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
				);
				stbi_image_free(data);
			}
			else
			{
				cout << "Cubemap texture failed to load at path: " << endl;
				stbi_image_free(data);
			}
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		return textureID;
	}

};

#endif // SCENE_H