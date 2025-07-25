// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractWidget.generated.h"

/**
 * 
 */

class UTextBlock;
UCLASS()
class PICKABLESYSTEM_API UInteractWidget : public UUserWidget
{
	GENERATED_BODY()

	
public:

	void SetItemName(const FText& NewName);

protected:

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemNameText;
	
};
