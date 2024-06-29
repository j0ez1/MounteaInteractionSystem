// Copyright Dominik Pavlicek 2022. All Rights Reserved.

#include "Helpers/MounteaInteractionSystemEditorLog.h"

// Log category definition
DEFINE_LOG_CATEGORY(LogActorInteractionEditor);

void PrintLog(const FString& Message, FLinearColor Color, float Duration)
{
	if (GEditor)
	{
		GEditor->AddOnScreenDebugMessage(0, Duration, Color.ToFColor(true), Message);
	}
}