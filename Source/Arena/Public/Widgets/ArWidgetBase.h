// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ArWidgetBase.generated.h"

class UPlayerUIComponent;
class UEnemyUIComponent;
/**
 * 
 */
UCLASS()
class ARENA_API UArWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Owning Player UI Component"))
	void BP_OnOwningPlayerUIComponent(UPlayerUIComponent* OwningPlayerUIComponent);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Owning Enemy UI Component"))
	void BP_OnOwningEnemyUIComponent(UEnemyUIComponent* OwningEnemyUIComponent);

public:
	UFUNCTION(BlueprintCallable)
	void InitEnemyWidget(AActor* EnemyActor);
};
