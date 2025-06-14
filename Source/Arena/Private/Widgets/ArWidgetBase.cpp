// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/ArWidgetBase.h"
#include "Interfaces/PawnUIInterface.h"

void UArWidgetBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (IPawnUIInterface* PawnUIInterface = Cast<IPawnUIInterface>(GetOwningPlayerPawn()))
	{
		if (UPlayerUIComponent* PlayerUIComponent = PawnUIInterface->GetPlayerUIComponent())
		{
			BP_OnOwningPlayerUIComponent(PlayerUIComponent);
		}
	}
}

void UArWidgetBase::InitEnemyWidget(AActor* EnemyActor)
{
	if (IPawnUIInterface* PawnUIInterface = Cast<IPawnUIInterface>(EnemyActor))
	{
		UEnemyUIComponent* EnemyUIComponent = PawnUIInterface->GetEnemyUIComponent();

		checkf(EnemyUIComponent, TEXT("couldn't extract EnemyUIComponent from %s"), *EnemyActor->GetActorNameOrLabel());

		BP_OnOwningEnemyUIComponent(EnemyUIComponent);
	}
}
