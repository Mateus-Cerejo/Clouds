#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <functional>

using namespace std;


class InputManager
{
public:
	static InputManager* GetInstance();

	void PopulateKeys();

	void RegisterKeyFunction(int key, std::function<void()> function);

	void KeyPressed(int key);
	void KeyReleased(int key);
	void MouseMoved(float xPos, float yPos);

	bool IsKeyPressed(int key);

	float GetLastMouseXMovement();

	float GetLastMouseYMovement();

private:
	static InputManager* InstancePtr;
	InputManager() {};

	bool keys[1024] = { false };

	map<int, vector<std::function<void()>>> ActionsByKey;

	bool mouseFirstMove;

	// Current Mouse position
	float mouseXPosition = -1;
	float mouseYPosition = -1;

	// Last (Current Mouse Position - Last Mouse Position) calculated in MouseMoved function
	float mouseLastXMovement = 0;
	float mouseLastYMovement = 0;
};