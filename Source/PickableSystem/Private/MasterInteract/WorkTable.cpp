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
	// دستور می‌دهیم که مش اصلی حتماً برخورد داشته باشد و همه چیز را بلاک کند
	TableMesh->SetCollisionProfileName(TEXT("BlockAll"));

	PlacementCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("PlacementCollision"));
	PlacementCollision->SetupAttachment(RootComponent);
	PlacementCollision->SetBoxExtent(FVector(100.f, 50.f, 5.f)); // یک اندازه پیش‌فرض
	PlacementCollision->SetRelativeLocation(FVector(0.f, 0.f, 100.f)); // یک مکان پیش‌فرض
	// دستور می‌دهیم که کامپوننت اختصاصی ما هم حتماً برخورد داشته باشد
	PlacementCollision->SetCollisionProfileName(TEXT("BlockAll"));


	OverlapBox->SetupAttachment(RootComponent);
}

void AWorkTable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsFocused)
	{
		TraceForPlacement();
		UpdateGhostMesh(true); 
	}
	else
	{
		UpdateGhostMesh(false); 
	}
}

// void AWorkTable::Interact_Implementation()
// {
// 	Super::Interact_Implementation();
// 	
// 	APickableSystemCharacter* PlayerCharacter = Cast<APickableSystemCharacter>(UGameplayStatics::GetPlayerController(this, 0)->GetPawn());
// 	if (!PlayerCharacter)
// 	{
// 		UE_LOG(LogTemp, Error, TEXT("Interact Error: PlayerCharacter is NULL!"));
// 		return;
// 	}
//
// 	AMasterPickable* ItemToPlace = PlayerCharacter->GetHeldItem();
// 	if (ItemToPlace)
// 	{
// 		// لاگ ۶: آیا تابع Interact اجرا شده و آیتم برای گذاشتن پیدا شده؟
// 		UE_LOG(LogTemp, Warning, TEXT("Interact Called on Table. Item to Place: %s"), *ItemToPlace->GetName());
//
// 		// لاگ ۷: مقدار ImpactPoint قبل از گذاشتن آیتم چیست؟
// 		UE_LOG(LogTemp, Warning, TEXT("Placing item at ImpactPoint: %s"), *ImpactPoint.ToString());
//
// 		ItemToPlace->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
// 		ItemToPlace->SetActorLocation(ImpactPoint);
// 		ItemToPlace->SetActorEnableCollision(true);
// 		PlayerCharacter->SetHeldItem(nullptr);
// 		UpdateGhostMesh(false);
// 	}
// 	else
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("Interact Called on Table, but Player has no HeldItem."));
// 	}
// }
//
// void AWorkTable::TraceForPlacement()
// {
// 	APickableSystemCharacter* PlayerCharacter = Cast<APickableSystemCharacter>(UGameplayStatics::GetPlayerController(this, 0)->GetPawn());
// 	if (!PlayerCharacter) return;
// 	
// 	FVector StartLocation;
// 	FRotator ViewRotation;
// 	PlayerCharacter->GetController()->GetPlayerViewPoint(StartLocation, ViewRotation);
// 	FVector EndLocation = StartLocation + (ViewRotation.Vector() * 1000.f);
//
// 	// لاگ ۲: آیا نقطه شروع و پایان Trace منطقی است؟
// 	// UE_LOG(LogTemp, Log, TEXT("TraceForPlacement: Start=[%s], End=[%s]"), *StartLocation.ToString(), *EndLocation.ToString());
//
// 	TArray<AActor*> ActorsToIgnore;
// 	ActorsToIgnore.Add(PlayerCharacter);
// 	if (PlayerCharacter->GetHeldItem())
// 	{
// 		ActorsToIgnore.Add(PlayerCharacter->GetHeldItem());
// 	}
// 	
// 	FHitResult HitResult;
//
// 	// استفاده از همان روش قبلی که می‌دانیم کامپایل می‌شود
// 	bool bHit = UKismetSystemLibrary::SphereTraceSingle(
// 		this,
// 		StartLocation,
// 		EndLocation,
// 		10.f,
// 		UEngineTypes::ConvertToTraceType(ECC_Visibility),
// 		true, // از برخورد پیچیده برای تست استفاده می‌کنیم
// 		ActorsToIgnore,
// 		EDrawDebugTrace::ForOneFrame,
// 		HitResult,
// 		true
// 	);
//
// 	if (bHit)
// 	{
// 		// لاگ ۳: Trace به چه چیزی برخورد کرده است؟
// 		UE_LOG(LogTemp, Warning, TEXT("Trace HIT! Actor: %s, Component: %s"), 
// 			*HitResult.GetActor()->GetName(), 
// 			*HitResult.GetComponent()->GetName()
// 		);
//
// 		// لاگ ۴: نقطه برخورد کجاست؟
// 		ImpactPoint = HitResult.ImpactPoint;
// 		UE_LOG(LogTemp, Warning, TEXT("ImpactPoint updated to: %s"), *ImpactPoint.ToString());
// 	}
// 	else
// 	{
// 		// لاگ ۵: آیا Trace به هیچ چیز برخورد نکرده؟
// 		UE_LOG(LogTemp, Error, TEXT("Trace MISSED! Did not hit any object."));
// 		ImpactPoint = EndLocation;
// 	}
// }

// ATable.cpp

void AWorkTable::TraceForPlacement()
{
	APickableSystemCharacter* PlayerCharacter = Cast<APickableSystemCharacter>(UGameplayStatics::GetPlayerController(this, 0)->GetPawn());
	if (!PlayerCharacter) return;
	
	FVector StartLocation;
	FRotator ViewRotation;
	PlayerCharacter->GetController()->GetPlayerViewPoint(StartLocation, ViewRotation);

	FVector EndLocation = StartLocation + (ViewRotation.Vector() * 1000.f);

	FHitResult HitResult;

	// --- بازسازی دقیق تابع Trace از پروژه Evil Chef ---

	// ۱. پارامترهای پیچیده Trace را در این ساختار تعریف می‌کنیم
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(PlayerCharacter); // بازیکن را نادیده بگیر
	if (PlayerCharacter->GetHeldItem())
	{
		QueryParams.AddIgnoredActor(PlayerCharacter->GetHeldItem()); // آیتم در دست را نادیده بگیر
	}
	
	// ۲. شکل Trace را تعریف می‌کنیم (یک کره با شعاع ۱۰)
	FCollisionShape Sphere = FCollisionShape::MakeSphere(10.0f);

	// ۳. تابع Trace اصلی موتور را صدا می‌زنیم (این همان تابع استفاده شده در evil chef است)
	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		FQuat::Identity, // چرخش کره مهم نیست
		ECC_Visibility,
		Sphere,
		QueryParams
	);
	
	// --- پایان بخش بازسازی شده ---
	
	// نمایش بصری Trace برای دیباگ
	DrawDebugSphere(GetWorld(), bHit ? HitResult.ImpactPoint : EndLocation, 10.0f, 12, FColor::Red, false, 0.0f);

	if (bHit)
	{
		ImpactPoint = HitResult.ImpactPoint;
	}
	else
	{
		ImpactPoint = EndLocation;
	}
}
// void AWorkTable::UpdateGhostMesh(bool IsVisible)
// {
// 	APickableSystemCharacter* PlayerCharacter = Cast<APickableSystemCharacter>(UGameplayStatics::GetPlayerController(this, 0)->GetPawn());
// 	if (!PlayerCharacter || !PlayerCharacter->GetHeldItem())
// 	{
// 		
// 		if (GhostMeshComponent)
// 		{
// 			GhostMeshComponent->DestroyComponent();
// 			GhostMeshComponent = nullptr;
// 		}
// 		return;
// 	}
//
// 	if (IsVisible)
// 	{
// 		UStaticMesh* MeshToUse = PlayerCharacter->GetHeldItem()->FindComponentByClass<UStaticMeshComponent>()->GetStaticMesh();
// 		if (!MeshToUse) return;
//
// 		if (!GhostMeshComponent)
// 		{
// 			
// 			GhostMeshComponent = NewObject<UStaticMeshComponent>(this);
// 			GhostMeshComponent->RegisterComponent();
// 			GhostMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
// 		
// 		}
// 		
// 		GhostMeshComponent->SetStaticMesh(MeshToUse);
// 		GhostMeshComponent->SetWorldLocation(ImpactPoint);
// 		GhostMeshComponent->SetVisibility(true);
// 	}
// 	else
// 	{
// 		if (GhostMeshComponent)
// 		{
// 			GhostMeshComponent->SetVisibility(false);
// 		}
// 	}
// }


///////////////////////////
///// ATable.cpp// ATable.cpp// ATable.cpp

void AWorkTable::Interact_Implementation()
{
	Super::Interact_Implementation();
	
	APickableSystemCharacter* PlayerCharacter = Cast<APickableSystemCharacter>(UGameplayStatics::GetPlayerController(this, 0)->GetPawn());
	if (!PlayerCharacter) return;

	AMasterPickable* ItemToPlace = PlayerCharacter->GetHeldItem();
	if (ItemToPlace)
	{
		UStaticMeshComponent* ItemMeshComp = ItemToPlace->FindComponentByClass<UStaticMeshComponent>();
		if (!ItemMeshComp || !ItemMeshComp->GetStaticMesh()) return;

		// ۱. ارتفاع کامل آیتم را محاسبه می‌کنیم
		const float ItemHeight = ItemMeshComp->GetStaticMesh()->GetBoundingBox().GetSize().Z;
		
		// ۲. یک‌سوم ارتفاع را به عنوان آفست در نظر می‌گیریم
		const float HeightOffset = ItemHeight / 5.0f;
		
		// ۳. نقطه نهایی را با اضافه کردن آفست به نقطه برخورد، به دست می‌آوریم
		const FVector PlacementLocation = ImpactPoint + FVector(0, 0, HeightOffset);

		ItemToPlace->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		ItemToPlace->SetActorLocation(PlacementLocation); // از مکان محاسبه‌شده جدید استفاده می‌کنیم
		ItemToPlace->SetActorEnableCollision(true);
		PlayerCharacter->SetHeldItem(nullptr);
		UpdateGhostMesh(false);
	}
}

void AWorkTable::UpdateGhostMesh(bool IsVisible)
{
	APickableSystemCharacter* PlayerCharacter = Cast<APickableSystemCharacter>(UGameplayStatics::GetPlayerController(this, 0)->GetPawn());
	if (!PlayerCharacter || !PlayerCharacter->GetHeldItem())
	{
		if (GhostMeshComponent)
		{
			GhostMeshComponent->DestroyComponent();
			GhostMeshComponent = nullptr;
		}
		return;
	}

	if (IsVisible)
	{
		UStaticMeshComponent* OriginalMeshComp = PlayerCharacter->GetHeldItem()->FindComponentByClass<UStaticMeshComponent>();
		if(!OriginalMeshComp || !OriginalMeshComp->GetStaticMesh()) return;
		
		UStaticMesh* MeshToUse = OriginalMeshComp->GetStaticMesh();

		if (!GhostMeshComponent)
		{
			GhostMeshComponent = NewObject<UStaticMeshComponent>(this);
			GhostMeshComponent->RegisterComponent();
			GhostMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		
		GhostMeshComponent->SetStaticMesh(MeshToUse);
		
		// همین منطق را برای گوست مش هم تکرار می‌کنیم
		const float ItemHeight = MeshToUse->GetBoundingBox().GetSize().Z;
		const float HeightOffset = ItemHeight / 5.0f;

		const FVector PlacementLocation = ImpactPoint + FVector(0, 0, HeightOffset);

		GhostMeshComponent->SetWorldLocation(PlacementLocation);
		GhostMeshComponent->SetWorldScale3D(FVector(GhostMeshScale)); 
		GhostMeshComponent->SetVisibility(true);
	}
	else
	{
		if (GhostMeshComponent)
		{
			GhostMeshComponent->SetVisibility(false);
		}
	}
}