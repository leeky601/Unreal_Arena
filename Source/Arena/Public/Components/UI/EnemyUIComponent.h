// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/UI/PawnUIComponent.h"
#include "EnemyUIComponent.generated.h"

class UArWidgetBase;
/**
 * 
 */
UCLASS()
class ARENA_API UEnemyUIComponent : public UPawnUIComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void RegisterEnemyDrawnWidgets(UArWidgetBase* InEnemyWidget) ;

	UFUNCTION(BlueprintCallable)
	void RemoveEnemyDrawnWidgetsIfAny() ;
	
private:
	TArray<UArWidgetBase*> EnemyDrawnWidgets;
};
