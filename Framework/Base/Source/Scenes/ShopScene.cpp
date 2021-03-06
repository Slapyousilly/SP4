#include <iostream>
#include "ShopScene.h"
#include "GL\glew.h"
#include "LoadTGA.h"

#include "../Application.h"
#include "GraphicsManager.h"
#include "MeshBuilder.h"
#include "RenderHelper.h"

#include "KeyboardController.h"
#include "SceneManager.h"

#include "../Lua/LuaInterface.h"
#include "SoundEngine\SoundEngine.h"

#include "../Entity/EntityFactory.h"
#include "../Entity/Entity2D.h"

#include "Collider\Collider_2DAABB.h"

#include "../Animation/AnimationsContainer.h"

using namespace std;

ShopScene::ShopScene()
{

}
ShopScene::~ShopScene()
{

}

void ShopScene::Init()
{
	// Create and attach the camera to the scene
	camera.Init(Vector3(0, 0, 0), Vector3(0, 0, 1), Vector3(0, 1, 0));
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	// Load all the meshes
	MeshBuilder::GetInstance()->GenerateQuad("INTROSTATE_BKGROUND", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("INTROSTATE_BKGROUND")->textureID = LoadTGA("Image//splash.tga");

	MeshBuilder::GetInstance()->GenerateQuad("button_background", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("button_background")->textureID = LoadTGA("Image//Buttons//button_background.tga");

	MeshBuilder::GetInstance()->GenerateQuad("button_background_alt", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("button_background_alt")->textureID = LoadTGA("Image//Buttons//button_background_alt.tga");

	MeshBuilder::GetInstance()->GenerateQuad("Gold", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("Gold")->textureID = LoadTGA("Image//Gold.tga");

	MeshBuilder::GetInstance()->GenerateText("text", 16, 16);
	MeshBuilder::GetInstance()->GetMesh("text")->textureID = LoadTGA("FontData//pixelFont.tga");
	MeshBuilder::GetInstance()->GetMesh("text")->material.kAmbient.Set(1, 0, 0);
	
	MeshBuilder::GetInstance()->GenerateQuad("Collider", Color(1.f, 1.f, 1.f))->textureID = LoadTGA("Image//collider.tga");

	EntityFactory::GetInstance()->AttachEntityManager(&EManager);

	float windowWidth = Application::GetInstance().GetWindowWidth();
	float windowHeight = Application::GetInstance().GetWindowHeight();

	MeshBuilder::GetInstance()->GenerateSpriteAnimation("Character", 4, 9);
	MeshBuilder::GetInstance()->GetMesh("Character")->textureID = LoadTGA("Image//character.tga");

	cout << "ShopScene loaded\n" << endl;

	MeshBuilder::GetInstance()->GenerateSpriteAnimation("character", 4, 9)->textureID = LoadTGA("Image//character.tga");

	//init Menus
	itemtabs = new Shop_Menu();
	itemtabs->SetItemsPerPage(2);
	utilitybuttons = new Menu();
	
	cart_menu = new Cart_Menu();

	acceptpurchase = false;  
	buyingTab = true;

	//Initialise inventory
	shop_inventory = new Inventory();
	cart_inventory = new Inventory();
	player_inventory = new Inventory();

	//Some random potions
	shop_inventory->AddItem(new HealthPotion());
	shop_inventory->AddItem(new HealthPotion());	shop_inventory->AddItem(new MaxHealthPotion());

	shop_inventory->AddItem(new HealthPotion());
	shop_inventory->AddItem(new HealthPotion());	shop_inventory->AddItem(new MaxHealthPotion());

	shop_inventory->AddItem(new HealthPotion());

	shop_inventory->AddItem(new MaxHealthPotion());
	shop_inventory->AddItem(new MaxHealthPotion());	shop_inventory->AddItem(new MaxHealthPotion());

	shop_inventory->AddItem(new MaxHealthPotion());

	//A special item
	HealthPotion* specialPotion = new HealthPotion();
	specialPotion->SetName("LingLongDingDong");
	specialPotion->SetMesh(MeshBuilder::GetInstance()->GetMesh("character"));
	shop_inventory->AddItem(specialPotion);

	Equipment *swordy = new Equipment(TYPE_WEAPON, 10, 0, 0, 0);
	swordy->SetName("Black Sword");
	swordy->SetDescription("What a black sword.");
	swordy->SetMesh(MeshBuilder::GetInstance()->GetMesh("INTROSTATE_BKGROUND"));
	shop_inventory->AddItem(swordy);

	Toggle_Button* buyingsellingtabbtn = new Toggle_Button();
	buyingsellingtabbtn->SetSwitch(buyingTab);
	buyingsellingtabbtn->SetImage(MeshBuilder::GetInstance()->GetMesh("button_background"));
	buyingsellingtabbtn->SetHighlightedImage(MeshBuilder::GetInstance()->GetMesh("button_background_alt"));
	buyingsellingtabbtn->SetText("Buying");
	buyingsellingtabbtn->SetScale(400, 200);
	buyingsellingtabbtn->SetPosition(200, 1000);
	utilitybuttons->AddButton(buyingsellingtabbtn);

	Toggle_Button* buyingsellingtabbtn1 = new Toggle_Button();
	buyingsellingtabbtn1->SetSwitch(buyingTab);
	buyingsellingtabbtn1->SetImage(MeshBuilder::GetInstance()->GetMesh("button_background"));
	buyingsellingtabbtn1->SetHighlightedImage(MeshBuilder::GetInstance()->GetMesh("button_background_alt"));
	buyingsellingtabbtn1->SetText("Selling");
	buyingsellingtabbtn1->SetScale(400, 200);
	buyingsellingtabbtn1->SetPosition(200, 800);
	utilitybuttons->AddButton(buyingsellingtabbtn1);

	SetBool_Button* acceptpurchasebtn = new SetBool_Button();
	acceptpurchasebtn->SetSwitch(acceptpurchase);
	acceptpurchasebtn->SetBoolean(true);
	acceptpurchasebtn->SetImage(MeshBuilder::GetInstance()->GetMesh("button_background"));
	acceptpurchasebtn->SetHighlightedImage(MeshBuilder::GetInstance()->GetMesh("button_background_alt"));
	acceptpurchasebtn->SetText("Accept Purchase");
	acceptpurchasebtn->SetScale(400, 200);
	acceptpurchasebtn->SetPosition(1750, 1000);
	utilitybuttons->AddButton(acceptpurchasebtn);

	Increment_Button* nextpage = new Increment_Button();
	nextpage->SetNumber(currentPage);
	nextpage->SetIncrementAmount(1);
	nextpage->SetImage(MeshBuilder::GetInstance()->GetMesh("button_background"));
	nextpage->SetHighlightedImage(MeshBuilder::GetInstance()->GetMesh("button_background_alt"));
	nextpage->SetText("Next page");
	nextpage->SetScale(400, 200);
	nextpage->SetPosition(1750, 750);
	utilitybuttons->AddButton(nextpage);

	itemtabs->SetCurrentPage(currentPage);

	Increment_Button* prevpage = new Increment_Button();
	prevpage->SetNumber(currentPage);
	prevpage->SetIncrementAmount(-1);
	prevpage->SetImage(MeshBuilder::GetInstance()->GetMesh("button_background"));
	prevpage->SetHighlightedImage(MeshBuilder::GetInstance()->GetMesh("button_background_alt"));
	prevpage->SetText("Previous page");
	prevpage->SetScale(400, 200);
	prevpage->SetPosition(1750, 500);
	utilitybuttons->AddButton(prevpage);

	int offset = 0;
	for (unsigned i = 0; i < shop_inventory->m_inventoryList.size(); ++i)
	{
		if (offset % itemtabs->GetNumberOfItemsPerPage() == 0)
			offset = 0;

		ShopItem_Button* btn1 = new ShopItem_Button();
		btn1->SetPosition(windowWidth * 0.5f, (windowHeight - offset * 170) - 80);
		btn1->SetActive(true);
		btn1->SetImage(MeshBuilder::GetInstance()->GetMesh("button_background"));
		btn1->SetHighlightedImage(MeshBuilder::GetInstance()->GetMesh("button_background_alt"));
		btn1->SetTargetInventory(*cart_inventory);
		btn1->SetItem(*shop_inventory->m_inventoryList[i]);
		itemtabs->AddButton(btn1);

		offset++;
	}
	AnimationsContainer::GetInstance()->AddAnimation("walk", new Animation("character", 0, 8, 1.f, LIFETIME_INFINITE));
	AnimationsContainer::GetInstance()->AddAnimation("asd", new Animation("character", 0, 8, 1.f, LIFETIME_INFINITE));

	//Test Particles
	particle = Create::MovingParticle("walk", Vector3(0, 600, 0), Vector3(1000, 600, 0), 500.f);
	EManager.AddEntity(particle);
	particle = Create::ExplodingParticle("walk", "asd", Vector3(0, 0, 0), Vector3(500, 500, 0), 600.f);
	EManager.AddEntity(particle);
	particle = Create::StaticParticle("walk", Vector3(500, 500, 0));
	EManager.AddEntity(particle);

	cart_menu->SetPosition(500.0f, 500.0f);
	cart_menu->SetTargetInventory(*cart_inventory);
	cart_menu->InitialiseButtons();
}
void ShopScene::Update()
{
	float dt = StopWatch::GetInstance()->GetDeltaTime();
	camera.Update(dt);

	utilitybuttons->Update();
	itemtabs->Update();
	cart_menu->Update();

	EManager.Update();

	if (KeyboardController::GetInstance()->IsKeyPressed(VK_ESCAPE))
		SceneManager::GetInstance()->quit = true;

	if (KeyboardController::GetInstance()->IsKeyPressed(VK_SPACE))
		std::cout << player_inventory->m_inventoryList.size() << std::endl;


	if (acceptpurchase)
	{
		for (unsigned i = 0; i < cart_inventory->m_inventoryList.size(); ++i)
			player_inventory->AddItem(cart_inventory->m_inventoryList[i]);

		cart_inventory->m_inventoryList.clear();

		acceptpurchase = false;
	}

}
void ShopScene::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GraphicsManager::GetInstance()->AttachCamera(&camera);
	// Setup 2D pipeline then render 2D
	GraphicsManager::GetInstance()->SetOrthographicProjection(0, 
															  Application::GetInstance().GetWindowWidth(), 
															  0, 
															  Application::GetInstance().GetWindowHeight(), 
															  -10, 10);
	GraphicsManager::GetInstance()->DetachCamera();

	EManager.Render();

	// Render the required entities
	itemtabs->Render();
	utilitybuttons->Render();
	cart_menu->Render();
	
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(Application::GetInstance().GetWindowWidth() * 0.5f - 100, Application::GetInstance().GetWindowHeight() * 0.5 - 20.f, 0);
	modelStack.Scale(70.f, 70.f, 10.f);
	RenderHelper::RenderText(MeshBuilder::GetInstance()->GetMesh("text"), std::to_string(currentPage + 1), Color(0, 1, 0));
	//RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("character"));
	modelStack.PopMatrix();
	EManager.RenderUI();

}
void ShopScene::Exit()
{
	// Remove the meshes which are specific to ShopScene
	MeshBuilder::GetInstance()->RemoveMesh("INTROSTATE_BKGROUND");

	EManager.ClearEntityList();

	// Detach camera from other entities
	GraphicsManager::GetInstance()->DetachCamera();
}