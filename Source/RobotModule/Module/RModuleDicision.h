// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/DataSchema.h"
#include "UObject/NoExportTypes.h"
#include "RModuleDicision.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class ROBOTMODULE_API URModuleDicision : public UObject
{
	GENERATED_BODY()

	URModuleDicision() : Super() { };

	bool LoadToModuleInfo();

	template<typename T>
	T* CreateModuleNode();

private:
	TArray<FModuleDefine> mdInfo;

	class ARModuleCharacter* m_ModuleRobot;

public:
	
	void RunToModule();
	void SetToModuleRobot(class ARModuleCharacter* moduleRobot) { m_ModuleRobot = moduleRobot; };
	class UMeshComponent*  CreateRobotModule(const FModuleDefine& moduleDefine, const FString& strCombination);

	class UMeshComponent* GetMeshComponent(const FString& meshMouduleRegion);
	TArray<FModuleDefine>& GetModuleInfo() { return mdInfo; };

	const FString GetModuleRobotName();
};


/**
 *
 */
UCLASS()
class ROBOTMODULE_API URModuleNode : public UObject
{
	GENERATED_BODY()
public:
	URModuleNode() : Super() {};

	virtual bool Run() { return true; }
	void SetToDisicionBB(URModuleDicision* disition) { m_DisicionBB = disition; };

	FModuleDefine* GetRandModuleInfo(INT32 bitflagCommand);

	const FString GetModuleRegionName(eModuleRegion region) { return UDataSchema::GetEnumString<eModuleRegion>("eModuleRegion", region); }
protected:
	URModuleDicision* m_DisicionBB;		
};



/*
 *
 */
UCLASS()
class ROBOTMODULE_API URModuleComposition : public URModuleNode
{
	GENERATED_BODY()

	TArray<URModuleNode*> nodes;

public:

	const TArray<URModuleNode*>& GetNodes() const{ return nodes; }
	void AddNode(URModuleNode* node) { nodes.Emplace(node); }
	void AddNode(TArray<URModuleNode*>&& nodeList) { for (auto n : nodeList) nodes.Emplace(n); }
};

/*
 * OR
 */
UCLASS()
class ROBOTMODULE_API URModuleSelector : public URModuleComposition
{
	GENERATED_BODY()

public:
	virtual bool Run() override
	{
		for (auto n : GetNodes()) 
		{
			if (n->Run()) 
				return true;
		}
		return false;
	}
};

/*
 * AND
 */
UCLASS()
class ROBOTMODULE_API URModuleSequence : public URModuleComposition
{
	GENERATED_BODY()

public:
	virtual bool Run() override
	{
		for (auto n : GetNodes())
		{
			if (!n->Run()) return false;
		}
		return true;
	}
};

/*
 *
 */
UCLASS()
class ROBOTMODULE_API URModuleLeg : public URModuleNode
{
	GENERATED_BODY()

public:
	virtual bool Run() override;
};

/*
 *
 */
UCLASS()
class ROBOTMODULE_API URModuleCockpic : public URModuleNode
{
	GENERATED_BODY()

public:
	virtual bool Run() override;
};

/*
 *
 */
UCLASS()
class ROBOTMODULE_API URModuleShoulder : public URModuleNode
{
	GENERATED_BODY()

public:
	const eModuleShoulder GetShouderType() { return m_eShoulder; };

protected:

	eModuleShoulder m_eShoulder;
	void RandShoulderType();

	bool bCockpicAndShoulder();
};

/*
 *
 */
UCLASS()
class ROBOTMODULE_API URModuleShoulderR : public URModuleShoulder
{
	GENERATED_BODY()

public:
	virtual bool Run() override;
};

/*
 *
 */
UCLASS()
class ROBOTMODULE_API URModuleShoulderL : public URModuleShoulder
{
	GENERATED_BODY()

public:
	virtual bool Run() override;
};