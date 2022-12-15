/*
Copyright (c) Meta Platforms, Inc. and affiliates.
All rights reserved.

This source code is licensed under the license found in the
LICENSE file in the root directory of this source tree.
*/

#include "OculusXRAnchorBPFunctionLibrary.h"
#include "OculusXRHMD.h"
#include "OculusXRSpatialAnchorComponent.h"
#include "OculusXRAnchorsPrivate.h"
#include "Kismet/BlueprintFunctionLibrary.h"

AActor* UOculusXRAnchorBPFuctionLibrary::SpawnActorWithAnchorHandle(UObject* WorldContextObject, FOculusXRUInt64 Handle, FOculusXRUUID UUID, EOculusXRSpaceStorageLocation Location, UClass* ActorClass,
	AActor* Owner, APawn* Instigator, ESpawnActorCollisionHandlingMethod CollisionHandlingMethod)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Owner = Owner;
	SpawnInfo.Instigator = Instigator;
	SpawnInfo.ObjectFlags |= RF_Transient;
	SpawnInfo.SpawnCollisionHandlingOverride = CollisionHandlingMethod;

	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (World == nullptr)
	{
		UE_LOG(LogOculusXRAnchors, Warning, TEXT("Invalid WorldContext Object for SpawnActorWithAnchorHandle."));
		return nullptr;
	}

	AActor* NewSpatialAnchorActor = World->SpawnActor(ActorClass, nullptr, nullptr, SpawnInfo);
	if (NewSpatialAnchorActor == nullptr)
	{
		UE_LOG(LogOculusXRAnchors, Warning, TEXT("Failed to spawn Actor in SpawnActorWithAnchorHandle"));
		return nullptr;
	}

	UOculusXRSpatialAnchorComponent* SpatialAnchorComponent = NewSpatialAnchorActor->FindComponentByClass<UOculusXRSpatialAnchorComponent>();
	if (SpatialAnchorComponent == nullptr)
	{
		SpatialAnchorComponent = Cast<UOculusXRSpatialAnchorComponent>(NewSpatialAnchorActor->AddComponentByClass(UOculusXRSpatialAnchorComponent::StaticClass(), false, FTransform::Identity, false));
	}

	if (!IsValid(SpatialAnchorComponent))
	{
		UE_LOG(LogOculusXRAnchors, Warning, TEXT("Failed to find or spawn Spatial Anchor component in SpawnActorWithAnchorHandle"));
		return nullptr;
	}

	SpatialAnchorComponent->SetHandle(Handle);
	SpatialAnchorComponent->SetUUID(UUID);
	SpatialAnchorComponent->SetStoredLocation(Location, true);
	return NewSpatialAnchorActor;
}

AActor* UOculusXRAnchorBPFuctionLibrary::SpawnActorWithAnchorQueryResults(UObject* WorldContextObject, const FOculusXRSpaceQueryResult& QueryResult, UClass* ActorClass, AActor* Owner, APawn* Instigator, ESpawnActorCollisionHandlingMethod CollisionHandlingMethod)
{
	return SpawnActorWithAnchorHandle(WorldContextObject, QueryResult.Space, QueryResult.UUID, QueryResult.Location, ActorClass, Owner, Instigator, CollisionHandlingMethod);
}

bool UOculusXRAnchorBPFuctionLibrary::GetAnchorComponentStatus(AActor* TargetActor, EOculusXRSpaceComponentType ComponentType, bool& bIsEnabled)
{
	UOculusXRAnchorComponent* AnchorComponent = Cast<UOculusXRAnchorComponent>(TargetActor->GetComponentByClass(UOculusXRAnchorComponent::StaticClass()));

	if (!IsValid(AnchorComponent))
	{
		UE_LOG(LogOculusXRAnchors, Warning, TEXT("Invalid Anchor Component provided to GetAnchorComponentStatus"));
		bIsEnabled = false;
		return false;
	}

	bool bOutIsEnabled = false;
	bool bIsChangePending = false;

	bool bDidCallStart = OculusXRAnchors::FOculusXRAnchors::GetAnchorComponentStatus(AnchorComponent, ComponentType, bOutIsEnabled, bIsChangePending);
	if (!bDidCallStart)
	{
		UE_LOG(LogOculusXRAnchors, Warning, TEXT("Failed to start call to internal GetAnchorComponentStatus"));
		bIsEnabled = false;
		return false;
	}

	bIsEnabled = bOutIsEnabled;
	return bIsEnabled;
}

bool UOculusXRAnchorBPFuctionLibrary::GetAnchorTransformByHandle(const FOculusXRUInt64& Handle, FTransform& OutTransform)
{
	OculusXRHMD::FOculusXRHMD* OutHMD = GEngine->XRSystem.IsValid() ? (OculusXRHMD::FOculusXRHMD*)(GEngine->XRSystem->GetHMDDevice()) : nullptr;
	if (!OutHMD)
	{
		UE_LOG(LogOculusXRAnchors, Warning, TEXT("Unable to retrieve OculusXRHMD, cannot calculate anchor transform."));
		return false;
	}

	ovrpTrackingOrigin ovrpOrigin = ovrpTrackingOrigin_EyeLevel;
	const bool bTrackingOriginSuccess = FOculusXRHMDModule::GetPluginWrapper().GetInitialized() && OVRP_SUCCESS(FOculusXRHMDModule::GetPluginWrapper().GetTrackingOriginType2(&ovrpOrigin));
	if (!bTrackingOriginSuccess)
	{
		UE_LOG(LogOculusXRAnchors, Warning, TEXT("Unable to get tracking origin, cannot calculate anchor transform."));
		return false;
	}

	const ovrpUInt64 ovrpSpace = Handle.GetValue();
	ovrpPosef ovrpPose;

	const bool bSuccess = FOculusXRHMDModule::GetPluginWrapper().GetInitialized() && OVRP_SUCCESS(FOculusXRHMDModule::GetPluginWrapper().LocateSpace(&ovrpPose, &ovrpSpace, ovrpOrigin));
	if (bSuccess)
	{
		OculusXRHMD::FPose Pose;
		OutHMD->ConvertPose(ovrpPose, Pose);
		const FTransform trackingToWorld = OutHMD->GetLastTrackingToWorld();

		OutTransform.SetLocation(trackingToWorld.TransformPosition(Pose.Position));
		OutTransform.SetRotation(FRotator(trackingToWorld.TransformRotation(FQuat(Pose.Orientation))).Quaternion());
	}

	return bSuccess;
}

FString UOculusXRAnchorBPFuctionLibrary::AnchorHandleToString(const FOculusXRUInt64 Value)
{
	return FString::Printf(TEXT("%llu"), Value.Value);
}

FString UOculusXRAnchorBPFuctionLibrary::AnchorUUIDToString(const FOculusXRUUID& Value)
{
	return Value.ToString();
}

FOculusXRUUID UOculusXRAnchorBPFuctionLibrary::StringToAnchorUUID(const FString& Value)
{ 
	// Static size for the max length of the string, two chars per hex digit, 16 digits.
	checkf(Value.Len() == 32, TEXT("'%s' is not a valid UUID"), *Value);

	ovrpUuid newID;
	HexToBytes(Value, newID.data);

	return FOculusXRUUID(newID.data);
}