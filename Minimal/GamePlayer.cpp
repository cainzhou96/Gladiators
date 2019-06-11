#include "GamePlayer.h"

GamePlayer::GamePlayer()
{
	//handL = new Hand("model/glove_l.obj");
	//handR = new Hand("model/glove_r.obj");
	isMain = false; 
	toWorld = glm::mat4(1); 
}


GamePlayer::~GamePlayer()
{
}

/*void GamePlayer::updatehandPos(glm::mat4 leftPose, glm::mat4 rightPose) {
	handR->toWorld = rightPose;
	handR->toWorld = glm::scale(handR->toWorld, glm::vec3(0.001f));
	handL->toWorld = leftPose;
	handL->toWorld = glm::scale(handL->toWorld, glm::vec3(0.001f));
}
void GamePlayer::draw(GLuint shader, const glm::mat4 & projection, const glm::mat4 & view) {
	handL->Draw(shader, projection, view);
	handR->Draw(shader, projection, view);

}*/