#include <rpc/client.h>
#include "glm/glm.hpp"
#include <glm/gtx/string_cast.hpp>

#ifndef PLAYER_H
#define PLAYER_H

struct Transform
{

	glm::vec3 pos;
	glm::quat rotation;

	std::string to_string()
	{
		return "Position: " + glm::to_string(pos) + "\nRotation: " + glm::to_string(rotation);
	}

	// rpc Macro to generate serialize code for the struct (Note: for glm object, manually specify x,y,z,w)
	MSGPACK_DEFINE_MAP(pos.x, pos.y, pos.z, rotation.x, rotation.y, rotation.z, rotation.w)
};

struct Mat4
{
	int score;
	glm::mat4 toWorld;

	// rpc Macro to generate serialize code for the struct (Note: for glm object, manually specify x,y,z,w)
	MSGPACK_DEFINE_MAP(score,
		toWorld[0][0], toWorld[0][1], toWorld[0][2], toWorld[0][3], 
		toWorld[1][0], toWorld[1][1], toWorld[1][2], toWorld[1][3], 
		toWorld[2][0], toWorld[2][1], toWorld[2][2], toWorld[2][3], 
		toWorld[3][0], toWorld[3][1], toWorld[3][2], toWorld[3][3])
};

#endif