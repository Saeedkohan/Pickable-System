// Fill out your copyright notice in the Description page of Project Settings.


#include "MasterInteract/InteractWidget.h"

#include "Components/TextBlock.h"

void UInteractWidget::SetItemName(const FText& NewName)
{
	
	if (ItemNameText)
	{
		ItemNameText->SetText(NewName);
	}
}
