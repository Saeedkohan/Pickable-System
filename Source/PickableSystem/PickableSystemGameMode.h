// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PickableSystemGameMode.generated.h"

/**
 *  Simple GameMode for a first person game
 */
UCLASS(abstract)
class APickableSystemGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	APickableSystemGameMode();
};



