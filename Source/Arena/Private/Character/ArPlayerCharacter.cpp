// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ArPlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DataAssets/Input/DataAsset_InputConfig.h"
#include "Components/Input/ArenaInputComponent.h"
#include "ArenaGameplayTags.h"
#include "AbilitySystem/ArAbilitySystemComponent.h"
#include "AbilitySystem/ArAttributeSet.h"
#include "DataAssets/StartUpData/DataAsset_PlayerStartUpData.h"
#include "Components/Combat/PlayerCombatComponent.h"
#include "Components/UI/PlayerUIComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameModes/ArGameMode.h"

#include "DebugHelper.h"


AArPlayerCharacter::AArPlayerCharacter()
{
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(GetRootComponent());
    CameraBoom->TargetArmLength = 200.f;
    CameraBoom->SocketOffset = FVector(0.f, 55.f, 65.f);
    CameraBoom->bUsePawnControlRotation = true;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
    GetCharacterMovement()->MaxWalkSpeed = 400.f;
    GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

    PlayerCombatComponent = CreateDefaultSubobject<UPlayerCombatComponent>(TEXT("PlayerCombatComponent"));

    PlayerUIComponent = CreateDefaultSubobject<UPlayerUIComponent>(TEXT("PlayerUIComponent"));
}

UPawnCombatComponent* AArPlayerCharacter::GetPawnCombatComponent() const
{
    return PlayerCombatComponent;
}

UPawnUIComponent* AArPlayerCharacter::GetPawnUIComponent() const
{
    return PlayerUIComponent;
}

UPlayerUIComponent* AArPlayerCharacter::GetPlayerUIComponent() const
{
    return PlayerUIComponent;
}

void AArPlayerCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    if (!CharacterStartUpData.IsNull())
    {
        if (UDataAsset_StartUpDataBase* LoadedData = CharacterStartUpData.LoadSynchronous())
        {
            uint32 ApplyLevel = 1;

            if(AArGameMode* ArGameMode = GetWorld()->GetAuthGameMode<AArGameMode>())
            {
                switch (ArGameMode->GetGameDifficulty())
                {
                case EArGameDifficulty::Easy:
                    ApplyLevel = 4;
                    break;

                case EArGameDifficulty::Normal                    :
                    ApplyLevel = 3;
                    break;

                case EArGameDifficulty::Hard:
                    ApplyLevel = 2;
                    break;

                case EArGameDifficulty::Impossible:
                    ApplyLevel = 1;
                    break;

                default:
                    break;
                }
            }

            LoadedData->GiveToAbilitySystemComponent(ArAbilitySystemComponent, ApplyLevel);
        }
    }
}

void AArPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    checkf(InputConfigDataAsset, TEXT("Forgot to assign a valid data asset as input config"));

    ULocalPlayer* LocalPlayer = GetController<APlayerController>()->GetLocalPlayer();

    UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

    check(Subsystem);

    Subsystem->AddMappingContext(InputConfigDataAsset->DefaultMappingContext, 0);

    UArenaInputComponent* ArenaInputComponent = CastChecked<UArenaInputComponent>(PlayerInputComponent);

    ArenaInputComponent->BindNativeInputAction(InputConfigDataAsset, ArenaGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
    ArenaInputComponent->BindNativeInputAction(InputConfigDataAsset, ArenaGameplayTags::InputTag_Look, ETriggerEvent::Triggered, this, &ThisClass::Input_Look);

    ArenaInputComponent->BindNativeInputAction(InputConfigDataAsset, ArenaGameplayTags::InputTag_SwitchTarget, ETriggerEvent::Triggered, this, &ThisClass::Input_SwitchTargetTriggered);
    ArenaInputComponent->BindNativeInputAction(InputConfigDataAsset, ArenaGameplayTags::InputTag_SwitchTarget, ETriggerEvent::Completed, this, &ThisClass::Input_SwitchTargetCompleted);
    
    ArenaInputComponent->BindNativeInputAction(InputConfigDataAsset, ArenaGameplayTags::InputTag_PickUp_Stones, ETriggerEvent::Started, this, &ThisClass::Input_PickUpStonesStarted);

    ArenaInputComponent->BindAbilityInputAction(InputConfigDataAsset, this, &ThisClass::Input_AbilityInputPressed, &ThisClass::Input_AbilityInputReleased);
}

void AArPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

}

void AArPlayerCharacter::Input_Move(const FInputActionValue& InputActionValue)
{
    const FVector2D MovementVector = InputActionValue.Get<FVector2D>();

    const FRotator MovementRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);

    if (MovementVector.Y != 0.f)
    {
        const FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);

        AddMovementInput(ForwardDirection, MovementVector.Y);
    }

    if (MovementVector.X != 0.f)
    {
        const FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);

        AddMovementInput(RightDirection, MovementVector.X);
    }
}

void AArPlayerCharacter::Input_Look(const FInputActionValue& InputActionValue)
{
    const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

    if (LookAxisVector.X != 0.f)
    {
        AddControllerYawInput(LookAxisVector.X);
    }

    if (LookAxisVector.Y != 0.f)
    {
        AddControllerPitchInput(LookAxisVector.Y);
    }
}

void AArPlayerCharacter::Input_SwitchTargetTriggered(const FInputActionValue& InputActionValue)
{
    SwitchDirection = InputActionValue.Get<FVector2D>();
}

void AArPlayerCharacter::Input_SwitchTargetCompleted(const FInputActionValue& InputActionValue)
{
    FGameplayEventData Data;

    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
        this,
        SwitchDirection.X > 0.f ? ArenaGameplayTags::Player_Event_SwitchTarget_Right : ArenaGameplayTags::Player_Event_SwitchTarget_Left,
        Data
    );
}

void AArPlayerCharacter::Input_PickUpStonesStarted(const FInputActionValue& InputActionValue)
{
    FGameplayEventData Data;

    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
        this,
        ArenaGameplayTags::Player_Event_ConsumeStones,
        Data
    );
}

void AArPlayerCharacter::Input_AbilityInputPressed(FGameplayTag InInputTag)
{
    ArAbilitySystemComponent->OnAbilityInputPressed(InInputTag);
}

void AArPlayerCharacter::Input_AbilityInputReleased(FGameplayTag InInputTag)
{
    ArAbilitySystemComponent->OnAbilityInputReleased(InInputTag);
}


