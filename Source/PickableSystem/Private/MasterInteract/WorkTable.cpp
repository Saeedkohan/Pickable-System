// Fill out your copyright notice in the Description page of Project Settings.


#include "MasterInteract/WorkTable.h"
#include "PickableSystemCharacter.h"
#include "MasterInteract/MasterPickable.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "Components/SceneComponent.h"
#include "DrawDebugHelpers.h" 
#include "Engine/EngineTypes.h" 
AWorkTable::AWorkTable()
{
	PrimaryActorTick.bCanEverTick = true;

	TableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TableMesh"));
	RootComponent = TableMesh;
	
	TableMesh->SetCollisionProfileName(TEXT("BlockAll"));

	PlacementCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("PlacementCollision"));
	PlacementCollision->SetupAttachment(RootComponent);
	PlacementCollision->SetBoxExtent(FVector(100.f, 50.f, 5.f)); 
	PlacementCollision->SetRelativeLocation(FVector(0.f, 0.f, 100.f));

	PlacementCollision->SetCollisionProfileName(TEXT("BlockAll"));


	OverlapBox->SetupAttachment(RootComponent);
}





void AWorkTable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APickableSystemCharacter* PlayerCharacter = Cast<APickableSystemCharacter>(OverlappingCharacter);


	if (bIsFocused && PlayerCharacter && PlayerCharacter->IsInPlacementMode())
	{
		
		bool bIsSpotValid = TraceForPlacement();

	
		UpdateGhostMesh(true, bIsSpotValid);
	}
	else
	{
		
		UpdateGhostMesh(false, false);
	}
}

bool AWorkTable::TraceForPlacement()
{
	APickableSystemCharacter* PlayerCharacter = Cast<APickableSystemCharacter>(UGameplayStatics::GetPlayerController(this, 0)->GetPawn());
	if (!PlayerCharacter) return false;
	
	FVector StartLocation;
	FRotator ViewRotation;
	PlayerCharacter->GetController()->GetPlayerViewPoint(StartLocation, ViewRotation);
	FVector EndLocation = StartLocation + (ViewRotation.Vector() * 1000.f);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(PlayerCharacter);
	if (PlayerCharacter->GetHeldItem())
	{
		QueryParams.AddIgnoredActor(PlayerCharacter->GetHeldItem());
	}
	
	FHitResult HitResult;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(13.0f); 

	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult, StartLocation, EndLocation, FQuat::Identity,
		ECC_Visibility, Sphere, QueryParams
	);
	
	DrawDebugSphere(GetWorld(), bHit ? HitResult.ImpactPoint : EndLocation, 13.0f, 12, FColor::Green, false, 0.0f);


	if (bHit && HitResult.GetComponent() == this->PlacementCollision && FVector::DotProduct(HitResult.ImpactNormal, FVector::UpVector) > 0.9f)
	{
		ImpactPoint = HitResult.ImpactPoint;
		return true; 
	}


	if (bHit)
	{
		ImpactPoint = HitResult.ImpactPoint;
	}

	return false;
}
void AWorkTable::UpdateGhostMesh(bool bShouldBeVisible, bool bIsLocationValid)
{
	APickableSystemCharacter* PlayerCharacter = Cast<APickableSystemCharacter>(UGameplayStatics::GetPlayerController(this, 0)->GetPawn());
	if (!PlayerCharacter || !PlayerCharacter->GetHeldItem())
	{
		if (GhostMeshComponent) GhostMeshComponent->SetVisibility(false);
		return;
	}


	if (!GhostMeshComponent)
	{
		GhostMeshComponent = NewObject<UStaticMeshComponent>(this);
		GhostMeshComponent->RegisterComponent();
		GhostMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (bShouldBeVisible)
	{

		GhostMeshComponent->SetVisibility(true);

		UStaticMeshComponent* OriginalMeshComp = PlayerCharacter->GetHeldItem()->FindComponentByClass<UStaticMeshComponent>();
		if (!OriginalMeshComp || !OriginalMeshComp->GetStaticMesh()) return;
		
		UStaticMesh* MeshToUse = OriginalMeshComp->GetStaticMesh();
		GhostMeshComponent->SetStaticMesh(MeshToUse);
		
	
		const float ItemHeight = MeshToUse->GetBoundingBox().GetSize().Z;
		const float HeightOffset = ItemHeight / 6.0f;
		const FVector PlacementLocation = ImpactPoint + FVector(0, 0, HeightOffset);
		
		GhostMeshComponent->SetWorldLocation(PlacementLocation);
		GhostMeshComponent->SetWorldScale3D(FVector(GhostMeshScale));
		
	
		UMaterialInterface* MaterialToUse = bIsLocationValid ? ValidPlacementMaterial : InvalidPlacementMaterial;
		if (MaterialToUse)
		{
			GhostMeshComponent->SetMaterial(0, MaterialToUse);
		}
	}
	else
	{
		
		GhostMeshComponent->SetVisibility(false);
	}
}

void AWorkTable::Interact_Implementation()
{
	Super::Interact_Implementation();
	
	APickableSystemCharacter* PlayerCharacter = Cast<APickableSystemCharacter>(UGameplayStatics::GetPlayerController(this, 0)->GetPawn());
	if (!PlayerCharacter || !PlayerCharacter->GetHeldItem()) return;

	
	if (TraceForPlacement())
	{
	
		AMasterPickable* ItemToPlace = PlayerCharacter->GetHeldItem();

		UStaticMeshComponent* ItemMeshComp = ItemToPlace->FindComponentByClass<UStaticMeshComponent>();
		if (!ItemMeshComp || !ItemMeshComp->GetStaticMesh()) return;

		const float ItemHeight = ItemMeshComp->GetStaticMesh()->GetBoundingBox().GetSize().Z;
		const float HeightOffset = ItemHeight / 6.0f;
		const FVector PlacementLocation = ImpactPoint + FVector(0, 0, HeightOffset);

		ItemToPlace->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		ItemToPlace->SetActorLocation(PlacementLocation);
		ItemToPlace->SetActorEnableCollision(true);
		PlayerCharacter->SetHeldItem(nullptr);
		UpdateGhostMesh(false, false); 
	}
	else
	{
	
		UE_LOG(LogTemp, Warning, TEXT("Placement failed: Invalid location!"));
	}
}