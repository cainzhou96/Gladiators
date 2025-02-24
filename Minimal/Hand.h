#pragma once

#ifndef HAND_H
#define HAND_H

#include "Core.h"
#include "Model.h"

class Hand
{
public:

	Model * model;
	glm::mat4 toWorld;
	glm::vec3 color; 

	Hand(string const &path) {
		model = new Model(path);
		color = glm::vec3(0.698, 0.133, 0.133); 
	}

	void Draw(GLuint shader, const glm::mat4 & projection, const glm::mat4 & view) {
		model->setColor(color); 
		glm::vec3 lightPos = glm::vec3(0, -10, 10);
		glUniform3fv(glGetUniformLocation(shader, "lightPos"), 1, &lightPos[0]);
		model->Draw(shader, projection, view, toWorld);
	}

	~Hand() {

	}
};

#endif
