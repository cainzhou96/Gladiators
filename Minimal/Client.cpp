// Client.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//#include "pch.h"
#include <iostream>

#include "rpc/client.h"
#include <string>
#include <glm/gtx/string_cast.hpp>

// Shared struct
#include "Player.h"
using std::string;

//int main()
//{
//	// Setup an rpc client that connects to "localhost:8080"
//	rpc::client c("localhost", 8080);
//	std::cout << "Connected" << std::endl;
//
//	// Dummy player struct
//	Player p;
//	p.hp = 100;
//
//	// Client side logic
//	string input;
//	while (std::getline(std::cin, input))
//	{
//		if (!input.empty())
//		{
//			/* Call echo rpc function that is defined on the server
//			   Similar as calling
//			      auto [result, player] = echo(result, p);
//			*/
//			auto [result, player]/* structured binding: similar to 
//									result = std::get<0>(return value); 
//									player = std::get<1>(return value);
//								*/ = 
//				c.call("echo", input, p).as<std::tuple<string, Player>/*cast back the respond message to string and Player*/>();
//			std::cout << result << std::endl;
//			p = player;
//			std::cout << p.to_string() << std::endl;
//		}
//	}
//}
