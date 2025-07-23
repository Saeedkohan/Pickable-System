// Copyright Epic Games, Inc. All Rights Reserved.

#include "PickableSystemCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include  "MasterInteract/MasterInteract.h"
DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// APickableSystemCharacter

APickableSystemCharacter::APickableSystemCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	
	// Create the first person mesh that will be viewed only by this character's owner
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("First Person Mesh"));

	FirstPersonMesh->SetupAttachment(GetMesh());
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
	FirstPersonMesh->SetCollisionProfileName(FName("NoCollision"));

	// Create the Camera Component	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	FirstPersonCameraComponent->SetupAttachment(FirstPersonMesh, FName("head"));
	FirstPersonCameraComponent->SetRelativeLocationAndRotation(FVector(-2.8f, 5.89f, 0.0f), FRotator(0.0f, 90.0f, -90.0f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	FirstPersonCameraComponent->bEnableFirstPersonFieldOfView = true;
	FirstPersonCameraComponent->bEnableFirstPersonScale = true;
	FirstPersonCameraComponent->FirstPersonFieldOfView = 70.0f;
	FirstPersonCameraComponent->FirstPersonScale = 0.6f;

	// configure the character comps
	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;

	GetCapsuleComponent()->SetCapsuleSize(34.0f, 96.0f);

	// Configure character movement
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->AirControl = 0.5f;

	AttachPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("AttachPoint"));
	AttachPoint->SetupAttachment(FirstPersonMesh); 
	AttachPoint->SetRelativeLocation(FVector(50.0f, 0.0f, 0.0f));
}

// void APickableSystemCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
// {	
// 	// Set up action bindings
// 	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
// 	{
// 		// Jumping
// 		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APickableSystemCharacter::DoJumpStart);
// 		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &APickableSystemCharacter::DoJumpEnd);
//
// 		// Moving
// 		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APickableSystemCharacter::MoveInput);
//
// 		// Looking/Aiming
// 		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APickableSystemCharacter::LookInput);
// 		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &APickableSystemCharacter::LookInput);
// 	}
// 	else
// 	{
// 		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
// 	}
// }


void APickableSystemCharacter::MoveInput(const FInputActionValue& Value)
{
	// get the Vector2D move axis
	FVector2D MovementVector = Value.Get<FVector2D>();

	// pass the axis values to the move input
	DoMove(MovementVector.X, MovementVector.Y);

}

void APickableSystemCharacter::LookInput(const FInputActionValue& Value)
{
	// get the Vector2D look axis
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// pass the axis values to the aim input
	DoAim(LookAxisVector.X, LookAxisVector.Y);

}

void APickableSystemCharacter::DoAim(float Yaw, float Pitch)
{
	if (GetController())
	{
		// pass the rotation inputs
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void APickableSystemCharacter::DoMove(float Right, float Forward)
{
	if (GetController())
	{
		// pass the move inputs
		AddMovementInput(GetActorRightVector(), Right);
		AddMovementInput(GetActorForwardVector(), Forward);
	}
}

void APickableSystemCharacter::DoJumpStart()
{
	// pass Jump to the character
	Jump();
}

void APickableSystemCharacter::DoJumpEnd()
{
	// pass StopJumping to the character
	StopJumping();
}

//////////////////////////////////////////////////////////////////
void APickableSystemCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{   
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APickableSystemCharacter::DoJumpStart);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &APickableSystemCharacter::DoJumpEnd);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APickableSystemCharacter::MoveInput);

		// Looking/Aiming
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APickableSystemCharacter::LookInput);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &APickableSystemCharacter::LookInput);

		
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
    

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APickableSystemCharacter::OnInteract);
}


void APickableSystemCharacter::OnInteract()
{
	
	UE_LOG(LogTemp, Warning, TEXT("Interact key pressed!"));

	
	if (InteractRefrence)
	{
	
		IInteractInterface::Execute_Interact(InteractRefrence);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Interact Reference is Null, not looking at anything."));
	}
}

void APickableSystemCharacter::SendInteractReference_Implementation(AActor* InteractActor)
{

	this->InteractRefrence = Cast<AMasterInteract>(InteractActor);
}

void APickableSystemCharacter::SetHeldItem(AMasterPickable* NewItem)
{
	HeldItem = NewItem;
}
