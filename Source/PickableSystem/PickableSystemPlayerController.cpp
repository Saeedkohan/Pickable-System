// Copyright Epic Games, Inc. All Rights Reserved.


#include "PickableSystemPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "PickableSystemCameraManager.h"

APickableSystemPlayerController::APickableSystemPlayerController()
{
	// set the player camera manager class
	PlayerCameraManagerClass = APickableSystemCameraManager::StaticClass();
}

void APickableSystemPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
		{
			Subsystem->AddMappingContext(CurrentContext, 0);
		}
	}
}
