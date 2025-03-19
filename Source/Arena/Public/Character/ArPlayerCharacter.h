// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ArCharacterBase.h"
#include "ArPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

/**
 * 
 */
UCLASS()
class ARENA_API AArPlayerCharacter : public AArCharacterBase
{
	GENERATED_BODY()
	
public:
    AArPlayerCharacter();

protected:
    virtual void BeginPlay() override;

private:

#pragma region Components

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    USpringArmComponent* CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    UCameraComponent* FollowCamera;

#pragma endregion
};
