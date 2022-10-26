// All rights reserved Dominik Pavlicek 2022.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ActorInteractorInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UActorInteractorInterface : public UInterface
{
	GENERATED_BODY()
};

enum class EInteractorStateV2 : uint8;
class IActorInteractableInterface;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractableSelected, const TScriptInterface<IActorInteractableInterface>&, SelectedInteractable);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractableFound, const TScriptInterface<IActorInteractableInterface>&, FoundInteractable);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractableLost, const TScriptInterface<IActorInteractableInterface>&, LostInteractable);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInteractionKeyPressed, const float, TimeKeyPressed, const FKey&, PressedKey);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInteractionKeyReleased, const float, TimeKeyReleased, const FKey&, ReleasedKey);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStateChanged, const EInteractorStateV2&, NewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCollisionChanged, const TEnumAsByte<ECollisionChannel>&, NewCollisionChannel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAutoActivateChanged, const bool, NewAutoActivate);

/**
 * 
 */
class ACTORINTERACTIONPLUGIN_API IActorInteractorInterface
{
	GENERATED_BODY()

public:
		
	virtual void StartInteraction() = 0;
	virtual void StopInteraction() = 0;
	
	virtual bool ActivateInteractor(FString& ErrorMessage) = 0;
	virtual bool WakeUpInteractor(FString& ErrorMessage) = 0;
	virtual bool SuppressInteractor(FString& ErrorMessage) = 0;
	virtual void DeactivateInteractor() = 0;

	virtual void AddInteractionDependency(const TScriptInterface<IActorInteractorInterface> InteractionDependency) = 0;
	virtual void RemoveInteractionDependency(const TScriptInterface<IActorInteractorInterface> InteractionDependency) = 0;
	virtual TArray<TScriptInterface<IActorInteractorInterface>> GetInteractionDependencies() const = 0;

	virtual void CompareInteractable(const TScriptInterface<IActorInteractableInterface>& FoundInteractable) = 0;

	virtual bool CanInteract() const = 0;

	virtual void TickInteraction(const float DeltaTime) = 0;

	virtual ECollisionChannel GetResponseChannel() const = 0;
	virtual void SetResponseChannel(const ECollisionChannel NewResponseChannel) = 0;

	virtual EInteractorStateV2 GetState() const = 0;
	virtual void SetState(const EInteractorStateV2 NewState) = 0;

	virtual bool DoesAutoActivate() const = 0;
	virtual void ToggleAutoActivate(const bool bNewAutoActivate) = 0;

	virtual FKey GetInteractionKey(const FString& RequestedPlatform) const = 0;
	virtual void SetInteractionKey(const FString& Platform, const FKey NewInteractorKey) = 0;
	virtual TMap<FString, FKey> GetInteractionKeys() const = 0;
	virtual bool FindKey(const FKey& RequestedKey) const = 0;

	virtual TScriptInterface<IActorInteractableInterface> GetActiveInteractable() const = 0;
	virtual void SetActiveInteractable(const TScriptInterface<IActorInteractableInterface> NewInteractable) = 0;

	virtual void ToggleDebug() = 0;

	virtual FInteractableSelected& GetOnInteractableSelectedHandle() = 0;
	virtual FInteractableFound& GetOnInteractableFoundHandle() = 0;
	virtual FInteractableLost& GetOnInteractableLostHandle() = 0;
	virtual FInteractionKeyPressed& OnInteractionKeyPressedHandle() = 0;
	virtual FInteractionKeyReleased& OnInteractionKeyReleasedHandle() = 0;
};
