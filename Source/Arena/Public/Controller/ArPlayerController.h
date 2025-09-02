// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GenericTeamAgentInterface.h"
#include "ArPlayerController.generated.h"

class UUserWidget;

/**
 * 
 */
UCLASS()
class ARENA_API AArPlayerController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
	
public:
	AArPlayerController();

	//~ Begin IGenericTeamAgentInterface Interface.
	virtual FGenericTeamId GetGenericTeamId() const override;
	//~ End IGenericTeamAgentInterface Interface.

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player|OverlayWidget")
	TObjectPtr<UUserWidget> OverlayWidget ;

private:
	FGenericTeamId PlayerTeamID;
};
