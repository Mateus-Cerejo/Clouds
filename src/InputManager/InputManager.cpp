#include "InputManager.h"

InputManager* InputManager::InstancePtr = nullptr;

InputManager* InputManager::GetInstance() {
	if (InstancePtr == nullptr)
	{
		InstancePtr = new InputManager();
	}
	return InstancePtr;
}

void InputManager::PopulateKeys() {
	for (size_t i = 0; i < 1024; i++) {
		keys[i] = 0;
	}
}

void InputManager::RegisterKeyFunction(int key, std::function<void()> function)
{
	vector<std::function<void()>> keyActions( {function} );

	if (ActionsByKey.contains(key))
	{
		keyActions.append_range(ActionsByKey.at(key));
	}

	ActionsByKey.insert_or_assign(key, keyActions);
}

void InputManager::KeyPressed(int key)
{
	if (key >= 0 && key < 1024) {
		// Call any function registered to this key
		if (ActionsByKey.contains(key))
		{
			// TODO don't use auto because it creates a copy in this case??
			for (auto action : ActionsByKey.at(key))
			{
				action();
			}
		}

		// Mark key as pressed
		keys[key] = true;
	}
}

void InputManager::KeyReleased(int key)
{
	if (key >= 0 && key < 1024) {
		// Mark key as not pressed
		keys[key] = false;
	}
}

bool InputManager::IsKeyPressed(int key) {
	return keys[key];
}

void InputManager::MouseMoved(float xPos, float yPos)
{
	if (mouseFirstMove)
	{
		mouseXPosition = xPos;
		mouseYPosition = yPos;
		mouseFirstMove = false;
	}

	mouseLastXMovement = xPos - mouseXPosition;
	mouseLastYMovement = mouseYPosition - yPos;

	mouseXPosition = xPos;
	mouseYPosition = yPos;
}

//TODO this is dumb and retarded can only get it once but it works for now
float InputManager::GetLastMouseXMovement() {
	float theMovement = mouseLastXMovement;
	mouseLastXMovement = 0;
	return theMovement;
}

//TODO this is dumb and retarded can only get it once but it works for now
float InputManager::GetLastMouseYMovement() {
	float theMovement = mouseLastYMovement;
	mouseLastYMovement = 0;
	return theMovement;
}
