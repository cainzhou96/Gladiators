#include "pch.h"
#include "Player.h"
#include "rpc/server.h"
#include <string>
#include <iostream>

using std::string;
#define PORT 8080


int main()
{
	// Set up rpc server and listen to PORT
	rpc::server srv(PORT);
	std::cout << "Listening to port: " << PORT << std::endl;

	Mat4 player1;
	Mat4 player2;
	Mat4 p1HandL;
	Mat4 p1HandR;
	Mat4 p2HandL;
	Mat4 p2HandR;
	int p1Score = 0;
	int p2Score = 0;

	//// Define a rpc function: auto echo(string const& s, Player& p){} (return type is deduced)
	//srv.bind("echo"/*function name*/, [&player1]
	//(string const& s, Player& p) /*function parameters*/
	//{
	//	std::cout << "Get message: " << s << std::endl;
	//	std::cout << "Before: " << p.to_string() << std::endl;
	//	p.hp -= 1;
	//	p.pos.y += 1;
	//	p.rotation = glm::rotate(p.rotation, glm::radians(90.0f), glm::vec3(0, 1, 0));
	//	std::cout << "After: " << p.to_string() << std::endl;
	//	// return value : that will be returned back to client side
	//	return std::make_tuple(string("> ") + s, p);
	//});


	srv.bind("updatePlayer", [&player1, &player2](Mat4& p, int id)
	{
		if (id == 1) {
			player1.toWorld = p.toWorld;
			//std::cout << "Player 1 position: " << player1.to_string() << std::endl;
		}
		else {
			player2.toWorld = p.toWorld;
			//std::cout << "Player 2 position: " << glm::to_string(player2.pos) << std::endl;
		}
		
		return;
	});

	srv.bind("updateHands", [&p1HandL, &p1HandR, &p2HandL, &p2HandR](Mat4& left, Mat4& right, int id)
	{
		if (id == 1) {
			p1HandL.toWorld = left.toWorld;
			p1HandR.toWorld = right.toWorld;
			//std::cout << "Player 1 position: " << player1.to_string() << std::endl;
		}
		else {
			p2HandL.toWorld = left.toWorld;
			p2HandR.toWorld = right.toWorld;
			//std::cout << "Player 2 hand rot: " << glm::to_string(p2HandL.rotation) << std::endl;
		}

		return;
	});

	srv.bind("getPlayer", [&player1, &player2](int id)
	{
		if (id == 1)
			return player1;
		else
			return player2;
	});

	srv.bind("getHandL", [&p1HandL, &p2HandL](int id)
	{
		if (id == 1)
			return p1HandL;
		else
			return p2HandL;
	});

	srv.bind("getHandR", [&p1HandR, &p2HandR](int id)
	{
		if (id == 1)
			return p1HandR;
		else
			return p2HandR;
	});

	srv.bind("getScore", [&p1Score, &p2Score](int id)
	{
		if (id == 1)
			return p1Score;
		else
			return p2Score;
	});

	srv.bind("updateScore", [&p1Score, &p2Score](int score, int id)
	{
		if (id == 1)
			p1Score = score;
		else
			p2Score = score;
		//std::cout << "P1: " << p1Score << std::endl;
		//std::cout << "P2: " << p2Score << std::endl;
	});


	// Blocking call to start the server: non-blocking call is srv.async_run(threadsCount);
	srv.run();
	return 0;
}