// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RModuleWeapon.generated.h"

UCLASS()
class ROBOTMODULE_API ARModuleWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ARModuleWeapon();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	
	
};
