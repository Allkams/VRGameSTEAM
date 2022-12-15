// Copyright Epic Games, Inc. All Rights Reserved.

namespace UnrealBuildTool.Rules
{
	public class OculusXRMR : ModuleRules
	{
		public OculusXRMR(ReadOnlyTargetRules Target) : base(Target)
		{
			PrivateIncludePathModuleNames.AddRange(
				new string[]
				{
					"InputDevice",			// For IInputDevice.h
					"HeadMountedDisplay",	// For IMotionController.h
					"ImageWrapper",
					"Engine"
				});

			PrivateDependencyModuleNames.AddRange(
				new string[]
				{
					"Core",
					"CoreUObject",
					"Engine",
					"InputCore",
					"Slate",
					"SlateCore",
					"RHI",
					"VulkanRHI",
					"RenderCore",
					"MediaAssets",
					"HeadMountedDisplay",
					"OculusXRHMD",
					"OVRPluginXR"
				});

			PrivateIncludePaths.AddRange(
				new string[] {
					"OculusXRHMD/Private",
					"OculusXRInput/Private",
				});

			PublicIncludePaths.AddRange(
				new string[] {
				"Runtime/Renderer/Private",
				"Runtime/Engine/Classes/Components",
				"Runtime/MediaAssets/Private",
			});

			if (Target.Platform == UnrealTargetPlatform.Win64)
			{
				PublicDelayLoadDLLs.Add("OVRPluginXR.dll");
				RuntimeDependencies.Add("$(PluginDir)/Source/ThirdParty/OVRPlugin/OVRPlugin/Lib/" + Target.Platform.ToString() + "/OVRPlugin.dll");
			}

			if (Target.Platform == UnrealTargetPlatform.Android)
			{
				PublicIncludePaths.Add("Runtime/VulkanRHI/Private");
				PublicIncludePaths.Add("Runtime/VulkanRHI/Private/" + Target.Platform);
				AddEngineThirdPartyPrivateStaticDependencies(Target, "Vulkan");
			}

			if (Target.bBuildEditor == true)
			{
				PrivateDependencyModuleNames.Add("UnrealEd");
			}
		}
	}
}
