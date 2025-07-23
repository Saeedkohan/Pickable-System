// Fill out your copyright notice in the Description page of Project Settings.


#include "MasterInteract/MasterPickable.h"

#include "PickableSystemCharacter.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"

void AMasterPickable::Interact_Implementation()
{
	Super::Interact_Implementation();

	UE_LOG(LogTemp, Warning, TEXT("MasterPickable Interact() called!"));
	
	APickableSystemCharacter* PlayerCharacter = Cast<APickableSystemCharacter>(UGameplayStatics::GetPlayerController(this, 0)->GetPawn());
	
	if (PlayerCharacter)
	{
		SetActorEnableCollision(false);

	
		this->AttachToComponent(
			PlayerCharacter->GetAttachPoint(), 
			FAttachmentTransformRules::SnapToTargetIncludingScale,
			NAME_None 
		);

		PlayerCharacter->SetHeldItem(this);
	}
}