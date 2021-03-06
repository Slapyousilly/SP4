#include "MonsterFactory.h"

MonsterFactory::MonsterFactory() :
currID(0)
{

}

MonsterFactory::~MonsterFactory()
{

}

Enemy* MonsterFactory::CreateEnemy1(int id)
{
    Enemy* newEnemy = new Enemy();

    newEnemy->GetInfo()->stats.AddVit(50);
    newEnemy->GetInfo()->stats.AddStr(40);
    newEnemy->GetInfo()->stats.AddInt(5);
    newEnemy->GetInfo()->stats.AddMind(20);
    newEnemy->GetInfo()->stats.AddDex(10);
    newEnemy->GetInfo()->stats.AddAgi(7);
    newEnemy->GetInfo()->stats.AddLevel(2);
    newEnemy->GetInfo()->name = "Triple H";
    newEnemy->GetInfo()->id = id;
    newEnemy->enemyType = BattleEntity::ENEMY;
    newEnemy->GetInfo()->stats.UpdateStats();
    newEnemy->SetPosition(Vector3(windowWidth * 0.25f, windowHeight * (0.15f * id), 1.f));
    newEnemy->GetInfo()->HP = newEnemy->GetInfo()->stats.GetMaxHP();

    newEnemy->AddStats(newEnemy->GetInfo()->stats);

    return newEnemy;
}

Enemy* MonsterFactory::CreateEnemy2(int id)
{
    Enemy* newEnemy = new Enemy();

    newEnemy->GetInfo()->stats.AddVit(60);
    newEnemy->GetInfo()->stats.AddStr(70);
    newEnemy->GetInfo()->stats.AddInt(10);
    newEnemy->GetInfo()->stats.AddMind(20);
    newEnemy->GetInfo()->stats.AddDex(15);
    newEnemy->GetInfo()->stats.AddAgi(18);
    newEnemy->GetInfo()->stats.AddLevel(4);
    newEnemy->GetInfo()->name = "Kayne West";
    newEnemy->GetInfo()->id = id;
    newEnemy->enemyType = BattleEntity::ENEMY;
    newEnemy->GetInfo()->stats.UpdateStats();
    newEnemy->SetPosition(Vector3(windowWidth * 0.25f, windowHeight * (0.15f * id), 1.f));
    newEnemy->GetInfo()->HP = newEnemy->GetInfo()->stats.GetMaxHP();

    return newEnemy;
}

Enemy* MonsterFactory::CreateEnemy3(int id)
{
    Enemy* newEnemy = new Enemy();

    newEnemy->GetInfo()->stats.AddVit(20);
    newEnemy->GetInfo()->stats.AddStr(100);
    newEnemy->GetInfo()->stats.AddInt(30);
    newEnemy->GetInfo()->stats.AddMind(5);
    newEnemy->GetInfo()->stats.AddDex(12);
    newEnemy->GetInfo()->stats.AddAgi(10);
    newEnemy->GetInfo()->stats.AddLevel(3);
    newEnemy->GetInfo()->name = "Dwayne Johnson";
    newEnemy->GetInfo()->id = id;
    newEnemy->enemyType = BattleEntity::ENEMY;
    newEnemy->GetInfo()->stats.UpdateStats();
    newEnemy->SetPosition(Vector3(windowWidth * 0.25f, windowHeight * (0.15f * id), 1.f));
    newEnemy->GetInfo()->HP = newEnemy->GetInfo()->stats.GetMaxHP() * 0.2;

    return newEnemy;
}

Enemy* MonsterFactory::CreateRandomEnemy(int id)
{
    int randChoice = Math::RandIntMinMax(1, 3);
    if (randChoice == 1)
        return CreateEnemy1(id);
    if (randChoice == 2)
        return CreateEnemy2(id);
    if (randChoice == 3)
        return CreateEnemy3(id);
}