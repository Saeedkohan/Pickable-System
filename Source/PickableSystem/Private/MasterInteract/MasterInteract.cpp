// Fill out your copyright notice in the Description page of Project Settings.


#include "MasterInteract/MasterInteract.h"

#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "MasterDataAsset.h" 
#include "Kismet/GameplayStatics.h"
#include "MasterInteract/InteractInterface.h"
#include "MasterInteract/InteractWidget.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Character.h"
// Sets default values
AMasterInteract::AMasterInteract()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = Root;

	OverlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapBox"));
	OverlapBox->SetupAttachment(RootComponent);
	OverlapBox->SetCollisionProfileName("Trigger");

	InteractWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractWidget"));
	InteractWidgetComponent->SetupAttachment(RootComponent);
	InteractWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	InteractWidgetComponent->SetVisibility(false);
}

// Called when the game starts or when spawned
void AMasterInteract::BeginPlay()
{
	Super::BeginPlay();

	OverlapBox->OnComponentBeginOverlap.AddDynamic(this, &AMasterInteract::OnOverlapBegin);
	OverlapBox->OnComponentEndOverlap.AddDynamic(this, &AMasterInteract::OnOverlapEnd);


	if (ItemData)
	{
		
		UInteractWidget* MyWidget = Cast<UInteractWidget>(InteractWidgetComponent->GetUserWidgetObject());
		if (MyWidget)
		{

			MyWidget->SetItemName(FText::FromName(ItemData->ItemName));
		}
	}
}






void AMasterInteract::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (OverlappingCharacter)
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
		if (!PlayerController) return;

		FVector PlayerLocation;
		FRotator PlayerRotation;
		PlayerController->GetPlayerViewPoint(PlayerLocation, PlayerRotation);
       
		FVector PlayerToActorDir = (GetActorLocation() - PlayerLocation).GetSafeNormal();
		FVector PlayerForwardDir = PlayerRotation.Vector();

		float DotProduct = FVector::DotProduct(PlayerToActorDir, PlayerForwardDir);

		if (DotProduct > 0.95f) 
		{
			if (!bIsFocused) 
			{
				bIsFocused = true;
				InteractWidgetComponent->SetVisibility(true);
				IInteractInterface::Execute_SendInteractReference(OverlappingCharacter, this);
			}
		}
		else 
		{
			if (bIsFocused) 
			{
				ClearFocus();
			}
		}
	}
}


void AMasterInteract::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                     const FHitResult& SweepResult)
{
	
	if (OtherActor && OtherActor != this && OtherActor == UGameplayStatics::GetPlayerCharacter(this, 0))
	{
		OverlappingCharacter = Cast<ACharacter>(OtherActor);
		SetActorTickEnabled(true); 
	}
}

void AMasterInteract::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	if (OtherActor && OtherActor == OverlappingCharacter)
	{
		ClearFocus();
		OverlappingCharacter = nullptr;
		SetActorTickEnabled(false); 
	}
}

void AMasterInteract::ClearFocus()
{

	bIsFocused = false;
	InteractWidgetComponent->SetVisibility(false);

	
	if(OverlappingCharacter && OverlappingCharacter->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
	{
		IInteractInterface::Execute_SendInteractReference(OverlappingCharacter, nullptr);
	}
}


void AMasterInteract::Interact_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Base MasterInteract Interact() called. This should be overridden in a child class."));
}