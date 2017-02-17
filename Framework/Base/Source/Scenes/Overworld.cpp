#include "Overworld.h"

// Graphic includes
#include "GL\glew.h"
#include "LoadTGA.h"
#include "GraphicsManager.h"
#include "MeshBuilder.h"
#include "../Application.h"

// Entity
#include "../Entity/EntityFactory.h"

// Collider
#include "Collider\Collider_2DAABB.h"

// Utility
#include "SceneManager.h"
#include "KeyboardController.h"
#include "MatrixStack.h"

Overworld::Overworld() :
battle(false), lastRotX(0.f)
{
}


Overworld::~Overworld()
{
}

void Overworld::Init()
{
	MeshBuilder::GetInstance()->GenerateSpriteAnimation("character", 4, 9)->textureID = LoadTGA("Image//character.tga");
	MeshBuilder::GetInstance()->GenerateQuad("background", Color(1.f, 1.f, 1.f))->textureID = LoadTGA("Image//overworldBG.tga");
	MeshBuilder::GetInstance()->GenerateQuad("twee", Color(1.f, 1.f, 1.f))->textureID = LoadTGA("Image//twee.tga");
	MeshBuilder::GetInstance()->GenerateQuad("mountain", Color(1.f, 1.f, 1.f))->textureID = LoadTGA("Image//mountain.tga");
	MeshBuilder::GetInstance()->GenerateQuad("Collider", Color(1.f, 1.f, 1.f))->textureID = LoadTGA("Image//collider.tga");

	EntityFactory::GetInstance()->AttachEntityManager(&EManager);
	EManager.ShowCollider(true);
	// Window Scale
	float windowWidth = Application::GetInstance().GetWindowWidth();
	float windowHeight = Application::GetInstance().GetWindowHeight();

	// Camera Init
	camera.Init(1.f, 0.5f);
	camera.SetFollowSpeed(0.3f);
	camera.SetRotSpeed(200.f);
	camera.SetDistSpeed(100.f);
	camera.SetDist(80.f);

	// Player Init
	player = new OverworldEntity();
	player->GetAnimator()->AddAnimation("walk", new Animation("character", 0, 8, 1.f, -1));
	player->GetAnimator()->PlayAnimation("walk");
	player->SetScale(Vector3(5.f, 5.f, 1.f));
	player->SetPosition(Vector3(0.f, -50.f, 1.f));
	player->AttachCamera(&camera);
	player->SetCollider(new CCollider_2DAABB());
	EManager.AddEntity(player);


	// Background Init
	background = new SpriteEntity(MeshBuilder::GetInstance()->GetMesh("background"));
	background->SetTextRenderMode(SpriteEntity::MODE_3D);
	background->SetPosition(Vector3(0.f, 0.f, 0.f));
	background->SetScale(Vector3(300, 300, 1.f));
	EManager.AddEntity(background);

	TriggerArea* trigger = new TriggerArea();
	trigger->SetPosition(Vector3(30.f, 10.f, 0.1f));
	trigger->SetScale(Vector3(10.f, 10.f, 1.f));
	trigger->SetCollider(new CCollider_2DAABB());
	EManager.AddEntity(trigger);

	OverworldAsset* asset;
	Math::InitRNG();
	// Assets Init
	for (int i = 0; i < 50; ++i)
	{
		float posX = Math::RandFloatMinMax(-150.f, 150.f);
		float posY = Math::RandFloatMinMax(-150.f, 150.f);
		asset = new OverworldAsset("twee");
		asset->SetPosition(Vector3(posX, posY, 0.1f));
		asset->SetScale(Vector3(10.f, 10.f, 1.f));
		asset->SetCamera(&camera);
		asset->SetCollider(new CCollider_2DAABB());
		EManager.AddEntity(asset);
	}
	

	asset = new OverworldAsset("mountain");
	asset->SetPosition(Vector3(30.f, 30.f, 0.1f));
	asset->SetScale(Vector3(70.f, 50.f, 1.f));
	asset->SetCamera(&camera);
	asset->SetCollider(new CCollider_2DAABB());
	EManager.AddEntity(asset);
}

void Overworld::Update()
{
	EManager.Update();
	camera.Update();

	if (KeyboardController::GetInstance()->IsKeyPressed(VK_ESCAPE))
		SceneManager::GetInstance()->quit = true;

	if (KeyboardController::GetInstance()->IsKeyPressed(VK_SPACE))
	{
		battle = !battle;
	}

	if (battle)
	{
		camera.SetRotX(70.f);
		camera.SetDist(50.f);
	}
}

void Overworld::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GraphicsManager::GetInstance()->UpdateLightUniforms();

	// Setup 3D pipeline then render 3D
	GraphicsManager::GetInstance()->SetPerspectiveProjection(4.0f / 3.0f, 0.1f, 10000.0f);
	GraphicsManager::GetInstance()->AttachCamera(&camera);
	EManager.Render();

	// Setup 2D pipeline then render 2D
	GraphicsManager::GetInstance()->SetOrthographicProjection(0.f, Application::GetInstance().GetWindowWidth(), 0.f, Application::GetInstance().GetWindowHeight(), -10, 10);
	GraphicsManager::GetInstance()->DetachCamera();
	EManager.RenderUI();
}

void Overworld::Exit()
{
	EManager.ClearEntityList();
}
