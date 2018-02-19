// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/DataSchema.h"
#include "Module/RModuleDicision.h"
#include "GameFramework/Character.h"
#include "RModuleCharacter.generated.h"

UCLASS()
class ROBOTMODULE_API ARModuleCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ARModuleCharacter();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	class UMeshComponent* GetMeshComponent(const FString& meshMouduleRegion);

	class UMeshComponent* CreateModule(const FModuleDefine& moduleInfo, const FString& strCommand);
	
private:
	template<typename T>
	T* CreateMeshComp(const FModuleDefine& moduleInfo, const FString& strCommand);

	URModuleDicision* m_MDisicion = nullptr;

	// class USkeletalMeshComponent* m_RootSkeletalMesh;
	// TArray<FModuleDefine> m_RModuleInfos;
	//
	// TArray<FModuleDefine*> m_RModuleInfos;
	TArray<class UMeshComponent*> m_RModuleMesh;

protected :
	
};
