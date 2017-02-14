#include <iostream>
#include "SceneManager.h"
#include "Scene.h"

SceneManager::SceneManager() :nextScene(nullptr), quit(false), overlay(false), back(false)
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::Update()
{
	if (nextScene)
	{
		if (!overlay && activeScene.size() >= 1)
		{
			activeScene.back()->Exit();
			activeScene.pop_back();
		}

		activeScene.push_back(nextScene);
		activeScene.back()->Init();
		nextScene = nullptr;
	}

	if (back)
	{
		if (activeScene.size() > 1)
		{
			activeScene.back()->Exit();
			activeScene.pop_back();
			back = false;
		}
		else
			std::cout << "There is not previous scene to go back to." << std::endl;
	}

	activeScene.back()->Update();
}

void SceneManager::Render()
{
	for (SceneList::iterator it = activeScene.begin(); it != activeScene.end(); ++it)
	{
		(*it)->Render();
	}
}

void SceneManager::Exit()
{
	// Delete all scenes stored and empty the entire map
	std::map<std::string, Scene*>::iterator it, end;
	end = sceneMap.end();
	for (it = sceneMap.begin(); it != end; ++it)
	{
		delete it->second;
	}
	sceneMap.clear();
	activeScene.clear();
}

void SceneManager::AddScene(const std::string& _name, Scene* _scene)
{
	if (CheckSceneExist(_name))
	{
		// Scene Exist, unable to proceed
		throw std::exception("Duplicate scene name provided");
	}

	if (_scene == nullptr)
	{
		throw std::invalid_argument("Invalid scene pointer");
	}

	// Nothing wrong, add the scene to our map
	sceneMap[_name] = _scene;
}

void SceneManager::RemoveScene(const std::string& _name)
{
	// Does nothing if it does not exist
	if (!CheckSceneExist(_name))
		return;

	Scene* target = sceneMap[_name];
	if (target == activeScene.front() || target == nextScene)
	{
		throw std::exception("Unable to remove active/next scene");
	}

	// Delete and remove from our map
	delete target;
	sceneMap.erase(_name);
}

void SceneManager::SetActiveScene(const std::string& _name, bool overlay)
{
	if (!CheckSceneExist(_name))
	{
		// Scene does not exist
		throw std::exception("Scene does not exist");
	}

	// Scene exist, set the next scene pointer to that scene
	nextScene = sceneMap[_name];
	this->overlay = overlay;
}

bool SceneManager::CheckSceneExist(const std::string& _name)
{
	return sceneMap.count(_name) != 0;
}

void SceneManager::PreviousScene()
{
	back = true;
}