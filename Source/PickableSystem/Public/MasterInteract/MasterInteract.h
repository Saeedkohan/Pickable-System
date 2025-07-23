// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractInterface.h"
#include "MasterInteract.generated.h"
class UBoxComponent;
class UWidgetComponent;
class UMasterDataAsset;
class IInteractInterface;
UCLASS()
class PICKABLESYSTEM_API AMasterInteract : public AActor , public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMasterInteract();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UWidgetComponent* InteractWidgetComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UBoxComponent* OverlapBox;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Config | Item Data")
	UMasterDataAsset* ItemData;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
protected:
	void ClearFocus();

	UPROPERTY()
	ACharacter* OverlappingCharacter;

	bool bIsFocused=false;

public:
	virtual void Interact_Implementation() override;


};
