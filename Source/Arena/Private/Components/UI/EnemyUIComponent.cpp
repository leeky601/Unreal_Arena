// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/UI/EnemyUIComponent.h"
#include "Widgets/ArWidgetBase.h"

void UEnemyUIComponent::RegisterEnemyDrawnWidgets(UArWidgetBase* InEnemyWidget)
{
	EnemyDrawnWidgets.Add(InEnemyWidget);
}

void UEnemyUIComponent::RemoveEnemyDrawnWidgetsIfAny()
{
	if (EnemyDrawnWidgets.IsEmpty())
	{
		return;
	}

	for (UArWidgetBase* DrawnWidget : EnemyDrawnWidgets)
	{
		if (DrawnWidget)
		{
			DrawnWidget->RemoveFromParent();
		}
	}
	EnemyDrawnWidgets.Empty();
}
