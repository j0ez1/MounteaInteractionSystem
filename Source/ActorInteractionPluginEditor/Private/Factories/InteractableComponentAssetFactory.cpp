﻿// All rights reserved Dominik Pavlicek 2022.


#include "InteractableComponentAssetFactory.h"

#include "Utilities/ActorInteractionEditorUtilities.h"

#include "Components/ActorInteractableComponentBase.h"
#include "Helpers/ActorInteractionFunctionLibrary.h"
#include "Kismet2/KismetEditorUtilities.h"

#define LOCTEXT_NAMESPACE "ActorInteraction"

UInteractableComponentAssetFactory::UInteractableComponentAssetFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bCreateNew = true;

	// true if the associated editor should be opened after creating a new object.
	bEditAfterNew = false;
	SupportedClass = UActorInteractableComponentBase::StaticClass();

	// Default class
	ParentClass = SupportedClass;
}

bool UInteractableComponentAssetFactory::ConfigureProperties()
{
	static const FText TitleText = FText::FromString(TEXT("Pick Parent Item Class for new Interactable Component Object"));
	ParentClass = nullptr;

	UClass* ChosenClass = nullptr;
	const bool bPressedOk = FActorInteractionEditorUtilities::PickChildrenOfClass(TitleText, ChosenClass, SupportedClass);
	if (bPressedOk)
	{
		ParentClass = ChosenClass;
	}

	return bPressedOk;
}

UObject* UInteractableComponentAssetFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	// Something is not right!
	if (ParentClass == nullptr || !FKismetEditorUtilities::CanCreateBlueprintOfClass(ParentClass))
	{
		FFormatNamedArguments Args;
		Args.Add(TEXT("ClassName"), ParentClass ? FText::FromString(ParentClass->GetName()) : NSLOCTEXT("UnrealEd", "Null", "(null)"));
		FMessageDialog::Open(EAppMsgType::Ok, FText::Format(NSLOCTEXT("UnrealEd", "CannotCreateBlueprintFromClass", "Cannot create a blueprint based on the class '{0}'."), Args));
		return nullptr;
	}

	// Create new Blueprint
	UObject* NewObject = 
	FKismetEditorUtilities::CreateBlueprint(
		ParentClass,
		InParent,
		Name,
		BPTYPE_Normal,
		UBlueprint::StaticClass(),
		UBlueprintGeneratedClass::StaticClass(),
		NAME_None
	);

	bool bModified = false;
	
	if (const auto DefaultWidgetClass = UActorInteractionFunctionLibrary::GetInteractableDefaultWidgetClass())
	{
		UE_LOG(LogTemp, Warning, TEXT("[FactoryCreateNew] Trying to set Default Widget"))
		
		FClassProperty* WidgetClassProperty = FindFProperty<FClassProperty>(Class, "WidgetClass");

		if (WidgetClassProperty->GetClass() == nullptr)
		{
			WidgetClassProperty->SetPropertyClass(DefaultWidgetClass);
			//NewObject->Modify(true);
			bModified = true;
		}
	}

	if (const auto DefaultTable = UActorInteractionFunctionLibrary::GetInteractableDefaultDataTable())
	{
		UE_LOG(LogTemp, Warning, TEXT("[FactoryCreateNew] Trying to set Default Data Table"))
		
		FStructProperty* DataTableProperty = FindFProperty<FStructProperty>(Class, "InteractableData");
		if(DataTableProperty->Struct->IsChildOf(FDataTableRowHandle::StaticStruct()))
		{
			if (FDataTableRowHandle* Value = DataTableProperty->ContainerPtrToValuePtr<FDataTableRowHandle>(DataTableProperty))
			{
				if (Value->IsNull())
				{
					Value->DataTable = DefaultTable;
					bModified = true;
				}
			}
		}	
	}
	
	NewObject->Modify(true);
	return NewObject;
}

#undef LOCTEXT_NAMESPACE