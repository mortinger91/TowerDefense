// Unreal Engine 4 Tower Defense

#pragma once

namespace Game
{
	const float maxHealth = 10.f;
	const int32 startingGold = 20;
	const int32 maxGold = 999;
}

namespace Spawn
{
	const float spawnCooldown = 1.f;
}

namespace Ninja
{
	const float walkSpeed = 600.f;
	const float baseHealth = 100.f;
	const float gold = 1.f;
}

namespace Cannon
{
	const float shotCooldown = 0.5f;
	
	const float damage1 = 40.f;
	const float damage2 = 50.f;
	const float damage3 = 100.f;
	
	const int32 goldToBuild = 10;
	const int32 goldToUpgrade1 = 10;
	const int32 goldToUpgrade2 = 25;
	const int32 goldToSell1 = 5;
	const int32 goldToSell2 = 10;
	const int32 goldToSell3 = 20;

	const float AISightRadius = 1200.f;
}
