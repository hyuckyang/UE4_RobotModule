// Fill out your copyright notice in the Description page of Project Settings.

#include "RModuleCharacter.h"

#include "Runtime/Engine/Classes/Engine/BlueprintGeneratedClass.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"


ARModuleCharacter::ARModuleCharacter()
{
 	PrimaryActorTick.bCanEverTick = true;

	

}
void ARModuleCharacter::BeginPlay()
{
	Super::BeginPlay();

	m_MDisicion = NewObject<URModuleDicision>();
	m_MDisicion->SetToModuleRobot(this);
	m_MDisicion->RunToModule();
}

void ARModuleCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARModuleCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

/*
 *
 */
UMeshComponent* ARModuleCharacter::GetMeshComponent(const FString& meshMouduleRegion)
{
	UMeshComponent** pMesh = nullptr;
	
	pMesh = m_RModuleMesh.FindByPredicate([&](const UMeshComponent* data)
	{
		TArray<FString> pStrName;
		data->GetName().ParseIntoArray(pStrName, TEXT(";"), true);

		// 
		return pStrName[0].Contains(*meshMouduleRegion);
	});
	
	return pMesh == nullptr ? nullptr : *pMesh;
}


/*
 *
 */
UMeshComponent* ARModuleCharacter::CreateModule(const FModuleDefine& moduleInfo, const FString& strCommand)
{
	FString meshStr		= UDataSchema::GetEnumString<eModuleMesh>("eModuleMesh", moduleInfo.eMesh);
	FString path		= FString::Printf(TEXT("%s'%s%s.%s'"), *meshStr, *moduleInfo.strPath, *moduleInfo.strName, *moduleInfo.strName);

	UObject* loadObj	= LoadObject<UObject>(nullptr, *path);
	if (loadObj == nullptr) { return nullptr; }
	
	if (moduleInfo.eMesh == eModuleMesh::SkeletalMesh) 
	{
		USkeletalMeshComponent* comp = CreateMeshComp<USkeletalMeshComponent>(moduleInfo, strCommand);

		if (comp == nullptr)
		{
			//UE_LOG(LogClass, Log, TEXT("USkeletalMeshComponent NULLPTR"));
			return nullptr;
		}

		comp->SetSkeletalMesh(Cast<USkeletalMesh>(loadObj));

		m_RModuleMesh.Emplace(comp);

		return comp;
	}
	else
	{
		UStaticMeshComponent* comp = CreateMeshComp<UStaticMeshComponent>(moduleInfo, strCommand);;

		if (comp == nullptr) 
		{ 
			// UE_LOG(LogClass, Log, TEXT("UStaticMeshComponent NULLPTR"));
			return nullptr; 
		}
		comp->SetStaticMesh(Cast<UStaticMesh>(loadObj));

		m_RModuleMesh.Emplace(comp);

		return comp;
	}

}

/*
 *
 */
template<typename T>
T* ARModuleCharacter::CreateMeshComp(const FModuleDefine& moduleInfo, const FString& strCommand)
{
	T*	comp = nullptr;
	
	if (moduleInfo.bRoot) 
	{
		comp = FindComponentByClass<T>();
		// return comp;
	}
	else
	{
		// ¿Ã∞‘ ø÷ µ ??
		comp = NewObject<T>(this, T::StaticClass());
	}

	TArray<FString> splitStr;
	strCommand.ParseIntoArray(splitStr, TEXT(";"), true);

	FString strRegion	= splitStr[0];
	FString strSocket	= splitStr[1];
	FString strName		= moduleInfo.strName;
	FString strParent	= splitStr.IsValidIndex(2) ? splitStr[2] : FString::Printf(TEXT("None"));

	// UE_LOG(LogClass, Log, TEXT("region -%s- , sockek -%s- ,  name -%s-"), *strRegion, *strSocket, *strName);

	UMeshComponent* mesh = Cast<UMeshComponent>(comp);
	mesh->Rename(*(strRegion + TEXT(";") + strName));
	mesh->RegisterComponent();

	/* Root */
	if (moduleInfo.eParentRegion.Num() == 1 && moduleInfo.eParentRegion[0] == eModuleRegion::None) return comp;

	//
	for (auto pRegion : moduleInfo.eParentRegion)
	{
		auto pInfo = m_MDisicion->GetModuleInfo().FilterByPredicate([&](const FModuleDefine& data)
		{
			return data.eRegion == pRegion;
		});

		// 
		for (auto p : pInfo) 
		{
			auto pMesh = m_RModuleMesh.FindByPredicate([&](const UMeshComponent* data)
			{
				TArray<FString> pStrName;
				data->GetName().ParseIntoArray(pStrName, TEXT(";"), true);

				if (splitStr.IsValidIndex(2))
				{
					return pStrName[0].Contains(*strParent) && p.strName.Contains(pStrName[1]);
				}
				// 
				return p.strName.Contains(pStrName[1]);
			});

			if (pMesh != nullptr) 
			{
				// 
				mesh->AttachToComponent(*pMesh, FAttachmentTransformRules::KeepRelativeTransform, FName(*strSocket));
				return comp;
			}
		}
	}
	return nullptr;
}
