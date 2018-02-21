// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "DataSchema.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTMODULE_API UDataSchema : public UObject
{
	GENERATED_BODY()

public:
	// ENUM 을 스트링으로.
	template<typename T>
	static FORCEINLINE FString GetEnumString(const FString& Name, T value)
	{
		const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, *Name);
		if (!enumPtr)
		{
			return FString("None");
		}
		return *(enumPtr ? enumPtr->GetNameStringByIndex(static_cast<uint8>(value)) : "NULL");
	}
};

/**
 *
 */

UENUM(BlueprintType, meta = (Bitflags))
enum class eModuleRegion : uint8
{
	None = 0,
	Leg,
	LegAndFullArmHand,
	CockpicAndShoulder,
	CockpicRiser,
	CockpicBase,
	CockpicSeat,
	Cockpic,
	Shoulder,
	ForeArm,
	LeftArm,
	LeftArmNoHand,
	RightArm,
	RightArmNoHand,
	Weapon,
	Max
};
ENUM_CLASS_FLAGS(eModuleRegion);

//UENUM(BlueprintType)
//enum class eModuleRegionUnique : uint8
//{
//	None = 0,
//	Leg, 
//	CockpicAndShoulder,
//	CockpicRiser,
//	CockpicBase,
//	Cockpic,
//	Shoulder_Left,
//	Shoulder_Right,
//
//	Max
//};

/**
 *
 */
UENUM(BlueprintType)
enum class eModuleShoulder : uint8
{
	NoShoulder = 0,
	ForeArm,
	ArmNoHand,
	ArmHand,
	Max,
};

/**
 *
 */
UENUM(BlueprintType)
enum class eModuleMesh : uint8
{
	None = 0,
	SkeletalMesh,
	StaticMesh,
	Max
};

/**
 *
 */
UENUM(BlueprintType)
enum class eModuleBody : uint8
{
	None = 0,
	Center,
	Max
};

/*
 *
 */
UENUM(BlueprintType)
enum class eModuleWeaponRange : uint8
{
	None = 0,
	Melee,
	Long,
	Shield,
	Max
};

USTRUCT(BlueprintType)
struct FModuleDefine : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	FModuleDefine() {}

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int32 id;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FString strName;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	eModuleRegion eRegion;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	eModuleMesh eMesh;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TArray<eModuleRegion> eParentRegion;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TArray<FString> strSockets;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool bRoot;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	eModuleBody eBody;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FString strPath;
};

USTRUCT(BlueprintType)
struct FWeaponDefine : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	FWeaponDefine() {}

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int32 id;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FString strName;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	eModuleRegion eRegion;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	eModuleMesh eMesh;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	eModuleWeaponRange eWeaponRange;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FString strPath;
};




