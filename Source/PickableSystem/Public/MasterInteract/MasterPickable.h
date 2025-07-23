// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MasterInteract/MasterInteract.h"
#include "MasterPickable.generated.h"

/**
 * 
 */
UCLASS()
class PICKABLESYSTEM_API AMasterPickable : public AMasterInteract
{
	GENERATED_BODY()
public:
	virtual void Interact_Implementation() override;
	
};
