// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

namespace UnrealBuildTool.Rules
{
	public class Bspii : ModuleRules
	{
		public Bspii(TargetInfo Target)
		{
			PublicIncludePaths.AddRange(
				new string[] {
					// ... add public include paths required here ...
				}
				);

			PrivateIncludePaths.AddRange(
				new string[] {
					"Developer/Bspii/Private",
					"Developer/Bspii/Private/Formats",
					// ... add other private include paths required here ...
				}
				);

			/*PublicDependencyModuleNames.AddRange(
				new string[]
				{
					"Core",
                    "Engine",
                    "UnrealEd",
                    "Slate",
                    "EditorStyle",
					// ... add other public dependencies that you statically link with here ...
				}
				);

			PrivateDependencyModuleNames.AddRange(
				new string[]
				{
                    "Core",
                    "Engine",
                    "UnrealEd",
                    "EditorStyle",
                    "Projects",
                    "LevelEditor",
                    "AssetTools",
                    "BspMode",
					// ... add private dependencies that you statically link with here ...
				}
				);*/

            /*PublicDependencyModuleNames.AddRange(
            new string[] {
            "Core",
            //"Engine",
            "UnrealEd",
            "CoreUObject",
            "InputCore",
            "SlateCore",
            "Slate"
            }
            );*/
            PrivateDependencyModuleNames.AddRange(
            new string[] {
            "EditorStyle",
            "Projects",
            "LevelEditor",
            "AssetTools",
            "Core",
            "Engine",
            "UnrealEd",
            "CoreUObject",
            "InputCore",
            "SlateCore",
            "Slate"
            }
        );

			DynamicallyLoadedModuleNames.AddRange(
				new string[]
				{
					// ... add any modules that your module loads dynamically here ...
				}
				);
		}
	}
}