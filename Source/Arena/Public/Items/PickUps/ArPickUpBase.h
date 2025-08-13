// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArPickUpBase.generated.h"

class USphereComponent;

UCLASS()
class ARENA_API AArPickUpBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AArPickUpBase();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pick UP")
	USphereComponent* PickUpCollisionSphere ;

	UFUNCTION()
	virtual void OnCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
