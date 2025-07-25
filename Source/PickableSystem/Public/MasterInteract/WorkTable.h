// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MasterInteract/MasterInteract.h"
#include "WorkTable.generated.h"

/**
 * 
 */
class UBoxComponent; 
UCLASS()
class PICKABLESYSTEM_API AWorkTable : public AMasterInteract
{
	GENERATED_BODY()


public:
	AWorkTable();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	float GhostMeshScale = 0.25;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* PlacementCollision;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* TableMesh;
	
	
	virtual void Interact_Implementation() override;


	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Config | Ghost Mesh")
	UMaterialInterface* ValidPlacementMaterial;


	UPROPERTY(EditAnywhere, Category = "Config | Ghost Mesh")
	UMaterialInterface* InvalidPlacementMaterial;

private:



	void UpdateGhostMesh(bool bShouldBeVisible, bool bIsLocationValid);


	// void TraceForPlacement();

	bool TraceForPlacement();
	
	void UpdateGhostMesh(bool bIsLocationValid);

	FVector ImpactPoint;

	
	UPROPERTY()
	UStaticMeshComponent* GhostMeshComponent;
	
};
