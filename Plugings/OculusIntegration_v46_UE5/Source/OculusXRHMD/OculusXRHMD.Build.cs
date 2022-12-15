// Copyright Epic Games, Inc. All Rights Reserved.

using System;
using System.IO;

namespace UnrealBuildTool.Rules
{
	public class OculusXRHMD : ModuleRules
	{
		public OculusXRHMD(ReadOnlyTargetRules Target) : base(Target)
		{
			var EngineDir = Path.GetFullPath(Target.RelativeEnginePath);

			PrivateIncludePaths.AddRange(
				new string[] {
					Path.Combine(EngineDir, "Source/Runtime/Renderer/Private"),
					Path.Combine(EngineDir, "Source/Runtime/Renderer/Private"),
					Path.Combine(EngineDir, "Source/Runtime/OpenGLDrv/Private"),
					Path.Combine(EngineDir, "Source/Runtime/VulkanRHI/Private"),
					Path.Combine(EngineDir, "Source/Runtime/Engine/Classes/Components"),
					Path.Combine(EngineDir, "Source/Runtime/Engine/Classes/Kismet"),
				});

			PublicIncludePathModuleNames.AddRange(
				new string[] {
					"Launch",
					"ProceduralMeshComponent",
					"AndroidPermission"
				});			

			if (Target.Platform == UnrealTargetPlatform.Win64)
			{
				PublicIncludePaths.Add("Runtime/VulkanRHI/Private/Windows");
			}
			else
			{
				PublicIncludePaths.Add("Runtime/VulkanRHI/Private/" + Target.Platform);
			}

			PrivateDependencyModuleNames.AddRange(
				new string[]
				{
					"Core",
					"CoreUObject",
					"Engine",
					"InputCore",
					"RHI",
					"RHICore",
					"RenderCore",
					"Renderer",
					"Slate",
					"SlateCore",
					"ImageWrapper",
					"MediaAssets",
					"Analytics",
					"OpenGLDrv",
					"VulkanRHI",
					"OVRPluginXR",
					"OculusOpenXRLoader",
					"ProceduralMeshComponent",
					"Projects",
				});

			PublicDependencyModuleNames.AddRange(
				new string[]
				{
					"HeadMountedDisplay",
				});

			if (Target.bBuildEditor == true)
			{
				PrivateDependencyModuleNames.Add("UnrealEd");
			}

			AddEngineThirdPartyPrivateStaticDependencies(Target, "OpenGL");

			if (Target.Platform == UnrealTargetPlatform.Win64)
			{
				// D3D
				{
					PrivateDependencyModuleNames.AddRange(
						new string[]
						{
							"D3D11RHI",
							"D3D12RHI",
						});

					PrivateIncludePaths.AddRange(
						new string[]
						{
							"OculusXRMR/Public",
						});


					PublicIncludePaths.AddRange(
						new string[]
						{
							"Runtime/Windows/D3D11RHI/Private",
							"Runtime/Windows/D3D11RHI/Private/Windows",
							"Runtime/D3D12RHI/Private",
							"Runtime/D3D12RHI/Private/Windows",
						});


					AddEngineThirdPartyPrivateStaticDependencies(Target, "DX11");
					AddEngineThirdPartyPrivateStaticDependencies(Target, "DX12");
					AddEngineThirdPartyPrivateStaticDependencies(Target, "NVAPI");
					AddEngineThirdPartyPrivateStaticDependencies(Target, "DX11Audio");
					AddEngineThirdPartyPrivateStaticDependencies(Target, "DirectSound");
					AddEngineThirdPartyPrivateStaticDependencies(Target, "NVAftermath");
					AddEngineThirdPartyPrivateStaticDependencies(Target, "IntelMetricsDiscovery");
					AddEngineThirdPartyPrivateStaticDependencies(Target, "IntelExtensionsFramework");
				}

				// Vulkan
				{
					AddEngineThirdPartyPrivateStaticDependencies(Target, "Vulkan");
				}

				// OVRPlugin
				{
					RuntimeDependencies.Add("$(PluginDir)/Source/ThirdParty/OVRPlugin/OVRPlugin/Lib/" + Target.Platform.ToString() + "/OVRPlugin.dll");
				}

				if (Target.Platform == UnrealTargetPlatform.Win64)
				{
					RuntimeDependencies.Add("$(PluginDir)/Source/ThirdParty/OVRPlugin/OVRPlugin/Lib/" + Target.Platform.ToString() + "/OpenXR/OVRPlugin.dll");
				}
			}
			else if (Target.Platform == UnrealTargetPlatform.Android)
			{
				// We are not currently supporting Mixed Reality on Android, but we need to include IOculusXRMRModule.h for OCULUS_MR_SUPPORTED_PLATFORMS definition
				PrivateIncludePaths.AddRange(
						new string[]
						{
							"OculusXRMR/Public"
						});

				// Vulkan
				{
					AddEngineThirdPartyPrivateStaticDependencies(Target, "Vulkan");
				}

				// AndroidPlugin
				{
					string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
					AdditionalPropertiesForReceipt.Add("AndroidPlugin", Path.Combine(PluginPath, "OculusMobile_APL.xml"));
				}
			}
		}
	}
}
