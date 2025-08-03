// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ArCharacterBase.h"
#include "GameplayTagContainer.h"
#include "ArPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UDataAsset_InputConfig;
struct FInputActionValue;
class UPlayerCombatComponent;
class UPlayerUIComponent;

/**
 * 
 */
UCLASS()
class ARENA_API AArPlayerCharacter : public AArCharacterBase
{
	GENERATED_BODY()
	
public:
    AArPlayerCharacter();

    //~ Begin IPawnCombatInterface Interface.
    virtual UPawnCombatComponent* GetPawnCombatComponent() const override;
    //~ End IPawnCombatInterface Interface

    //~ Begin IPawnUIInterface Interface.
    virtual UPawnUIComponent* GetPawnUIComponent() const override;
    virtual UPlayerUIComponent* GetPlayerUIComponent() const override;
    //~ End IPawnUIInterface Interface



protected:
    //~ Begin APawn Interface.
    virtual void PossessedBy(AController* NewController) override;
    //~ End APawn Interface

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    virtual void BeginPlay() override;

private:

#pragma region Components

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    USpringArmComponent* CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    UCameraComponent* FollowCamera;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
    UPlayerCombatComponent* PlayerCombatComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
    UPlayerUIComponent* PlayerUIComponent;

#pragma endregion

#pragma region Inputs
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData", meta = (AllowPrivateAccess = "true"))
    UDataAsset_InputConfig* InputConfigDataAsset;

    void Input_Move(const FInputActionValue& InputActionValue);
    void Input_Look(const FInputActionValue& InputActionValue);

    void Input_SwitchTargetTriggered(const FInputActionValue& InputActionValue);
    void Input_SwitchTargetCompleted(const FInputActionValue& InputActionValue);
    FVector2D SwitchDirection = FVector2D::ZeroVector;

    void Input_AbilityInputPressed(FGameplayTag InInputTag);
    void Input_AbilityInputReleased(FGameplayTag InInputTag);
    
#pragma endregion

public:
    FORCEINLINE UPlayerCombatComponent* GetPlayerCombatComponent() const { return PlayerCombatComponent; }
};
