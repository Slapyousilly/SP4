#include "DialogueScene.h"

// Graphics
#include "GL\glew.h"
#include "LoadTGA.h"
#include "GraphicsManager.h"
#include "Mesh.h"
#include "MeshBuilder.h"

// Utilities
#include "../Application.h"
#include "KeyboardController.h"
#include "SceneManager.h"

DialogueScene::DialogueScene() :
textPos(0)
{
}


DialogueScene::~DialogueScene()
{
}

void DialogueScene::Init()
{
	SetInit(true);
	MeshBuilder::GetInstance()->GenerateText("text", 16, 16)->textureID = LoadTGA("FontData//pixelFont.tga");
	MeshBuilder::GetInstance()->GenerateQuad("text_background", Color(1, 1, 1))->textureID = LoadTGA("Image//Buttons//button_background.tga");
	
	text = new TextEntity(MeshBuilder::GetInstance()->GetMesh("text"), dialogue[textPos], Color(1.f, 1.f, 1.f));
	textBG = new SpriteEntity(MeshBuilder::GetInstance()->GetMesh("text_background"));
}

void DialogueScene::Update()
{
	if (KeyboardController::GetInstance()->IsKeyPressed(VK_SPACE))
	{
		textPos++;
	}

	if (textPos > dialogue.size())
	{
		if (targetScene != "")
			SceneManager::GetInstance()->SetActiveScene(targetScene);
		else
			SceneManager::GetInstance()->PreviousScene();
	}
}

void DialogueScene::Render()
{
	GraphicsManager::GetInstance()->SetOrthographicProjection(0, Application::GetInstance().GetWindowWidth(), 0, Application::GetInstance().GetWindowHeight(), -10, 10);
	text->Render();
}

void DialogueScene::Exit()
{
	delete text;
}

void DialogueScene::Pause()
{
	textPos = 0;
	dialogue.clear();
	targetScene = "";
}