// Unreal Engine 4 Tower Defense

using UnrealBuildTool;
using System.Collections.Generic;

public class TowerDefenseTarget : TargetRules
{
	public TowerDefenseTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "TowerDefense" } );
	}
}
