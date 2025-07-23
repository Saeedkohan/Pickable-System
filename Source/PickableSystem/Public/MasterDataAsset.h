// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MasterDataAsset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class PICKABLESYSTEM_API UMasterDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	FName ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	bool IsPickable;
	
};
