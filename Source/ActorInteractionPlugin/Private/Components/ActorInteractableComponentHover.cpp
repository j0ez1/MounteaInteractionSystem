﻿// All rights reserved Dominik Pavlicek 2022.


#include "Components/ActorInteractableComponentHover.h"

#define LOCTEXT_NAMESPACE "ActorInteractableComponentHover"

UActorInteractableComponentHover::UActorInteractableComponentHover()
{
	bInteractionHighlight = true;
	DefaultInteractableState = EInteractableStateV2::EIS_Awake;
	InteractionPeriod = 3.f;
	InteractableName = LOCTEXT("ActorInteractableComponentHover", "Hover");
}

void UActorInteractableComponentHover::BeginPlay()
{
	Super::BeginPlay();

	OnInteractorOverlapped.										RemoveDynamic(this, &UActorInteractableComponentHover::OnInteractableBeginOverlapEvent);
	OnInteractorStopOverlap.										RemoveDynamic(this, &UActorInteractableComponentHover::OnInteractableStopOverlapEvent);
	OnInteractorTraced.												RemoveDynamic(this, &UActorInteractableComponentHover::OnInteractableTracedEvent);
}

void UActorInteractableComponentHover::BindCollisionShape_Implementation(UPrimitiveComponent* PrimitiveComponent) const
{
	Super::BindCollisionShape(PrimitiveComponent);

	if (PrimitiveComponent)
	{
		PrimitiveComponent->OnBeginCursorOver.		AddUniqueDynamic(this, &UActorInteractableComponentHover::OnHoverBeginsEvent);
		PrimitiveComponent->OnEndCursorOver.			AddUniqueDynamic(this, &UActorInteractableComponentHover::OnHoverStopsEvent);
	}
}

void UActorInteractableComponentHover::UnbindCollisionShape_Implementation(UPrimitiveComponent* PrimitiveComponent) const
{
	Super::UnbindCollisionShape(PrimitiveComponent);

	if (PrimitiveComponent)
	{
		PrimitiveComponent->OnBeginCursorOver.		RemoveDynamic(this, &UActorInteractableComponentHover::OnHoverBeginsEvent);
		PrimitiveComponent->OnEndCursorOver.			RemoveDynamic(this, &UActorInteractableComponentHover::OnHoverStopsEvent);
	}
}

bool UActorInteractableComponentHover::CanInteract_Implementation() const
{
	return Super::CanInteract() && OverlappingComponent != nullptr;
}

void UActorInteractableComponentHover::OnHoverBeginsEvent(UPrimitiveComponent* PrimitiveComponent)
{
	OverlappingComponent = PrimitiveComponent;
	OnCursorBeginsOverlap.Broadcast(PrimitiveComponent);
}

void UActorInteractableComponentHover::OnHoverStopsEvent(UPrimitiveComponent* PrimitiveComponent)
{
	OverlappingComponent = nullptr;
	OnCursorStopsOverlap.Broadcast(PrimitiveComponent);
}

#undef LOCTEXT_NAMESPACE
