// Fill out your copyright  notice in the Description page of Project Settings.

#include "RModuleDicision.h"
#include "Actor/RModuleCharacter.h"

#include "Components/MeshComponent.h"
#include "Components/SkinnedMeshComponent.h"
#include "Runtime/Engine/Classes/Engine/BlueprintGeneratedClass.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"


/*
 */
void URModuleDicision::RunToModule()
{
	//
	LoadToModuleInfo();

	URModuleSequence* root = CreateModuleNode<URModuleSequence>();
	
	//
	URModuleLeg*		legNode			= CreateModuleNode<URModuleLeg>();
	URModuleCockpic*	cockNode		= CreateModuleNode<URModuleCockpic>();
	URModuleShoulderL*  shoulderLNode   = CreateModuleNode<URModuleShoulderL>();
	URModuleShoulderR*  shoulderRNode   = CreateModuleNode<URModuleShoulderR>();
	
	root->AddNode(legNode);
	root->AddNode(cockNode);
	root->AddNode(shoulderLNode);
	root->AddNode(shoulderRNode);

	root->Run();	
}

/*
 */
bool URModuleDicision::LoadToModuleInfo()
{
	FString path = FString::Printf(TEXT("DataTable'/Game/Data/ModuleDefine.ModuleDefine'"));

	// UE_LOG(LogClass, Log, TEXT("LoadToModuleInfo path ---     %s"), *path);

	UDataTable* loadObj = LoadObject<UDataTable>(nullptr, *path);
	if (loadObj == nullptr) return false;

	UDataTable*  data = Cast<UDataTable>(loadObj);
	if (data == nullptr) return false;

	for (int loop = 0, max = data->RowMap.Num(); loop < max; loop++)
	{
		FString str = FString::Printf(TEXT("%d"), loop + 1);
		FName entry = FName(*str);

		auto pRow = data->FindRow<FModuleDefine>(entry, FString("ModuleDefine"));
		if (pRow != nullptr)
		{
			FModuleDefine node(*pRow);
			mdInfo.Add(node);
		}
	}
	return true;
}

/*
 */
template<typename T>
T* URModuleDicision::CreateModuleNode()
{
	T* nObject = NewObject<T>();
	if (nObject == nullptr) return nullptr;
	
	URModuleNode* nModule =  Cast<URModuleNode>(nObject);
	if (nModule == nullptr) return nullptr;

	nModule->AddToRoot();
	nModule->SetToDisicionBB(this);

	return Cast<T>(nModule);
}

UMeshComponent* URModuleDicision::CreateRobotModule(const FModuleDefine& moduleDefine, const FString& strCombination)
{ 
	return m_ModuleRobot->CreateModule(moduleDefine, strCombination);
}

UMeshComponent* URModuleDicision::GetMeshComponent(const FString& meshMouduleRegion)
{ 
	return m_ModuleRobot->GetMeshComponent(meshMouduleRegion); 
}

const FString URModuleDicision::GetModuleRobotName()
{
	return m_ModuleRobot->GetName();
}


/*
 */
FModuleDefine* URModuleNode::GetRandModuleInfo(INT32 bitflagCommand)
{
	TArray<FModuleDefine*> module;
	for (int loop = 0, max = m_DisicionBB->GetModuleInfo().Num(); loop < max; loop++) 
	{
		INT32 bitReg = static_cast<int32>(1 << (INT32)m_DisicionBB->GetModuleInfo()[loop].eRegion);
		if ((bitflagCommand & bitReg) != 0)
		{
			module.Emplace(&m_DisicionBB->GetModuleInfo()[loop]);
		}
	}

	if (module.Num() == 0)
	{
		return nullptr;
	}

	int rID = FMath::RandRange(0, module.Num() - 1);

	if (!module.IsValidIndex(rID))
	{
		return nullptr;
	}

	return module[rID];
}



/*
 */
bool URModuleLeg::Run()
{
	// eModuleRegion::LegAndFullArmHand 사용 안함.
	auto rModule = GetRandModuleInfo(static_cast<int32>(1 << (INT32)eModuleRegion::Leg)
									 /*static_cast<int32>(1 << (INT32)eModuleRegion::LegAndFullArmHand)*/);

	if (rModule == nullptr) return false;
	
	m_DisicionBB->CreateRobotModule(*rModule, *FString::Printf(TEXT("%s;None"), *GetModuleRegionName(rModule->eRegion)));
	return true;
}

/*
 */
bool URModuleCockpic::Run()
{

	auto rCockpicBase = GetRandModuleInfo(static_cast<int32>(1 << (INT32)eModuleRegion::CockpicAndShoulder) |
										  static_cast<int32>(1 << (INT32)eModuleRegion::CockpicRiser) |
										  static_cast<int32>(1 << (INT32)eModuleRegion::CockpicBase));
	
	if (rCockpicBase == nullptr) return false;

	m_DisicionBB->CreateRobotModule(*rCockpicBase, *FString::Printf(TEXT("%s;Mount_Top"), *GetModuleRegionName(rCockpicBase->eRegion)));


	if (rCockpicBase->eRegion != eModuleRegion::CockpicRiser)
	{
		FModuleDefine* rCockpic = nullptr;
		if (rCockpicBase->eRegion == eModuleRegion::CockpicAndShoulder)
		{
			rCockpic = GetRandModuleInfo(static_cast<int32>(1 << (INT32)eModuleRegion::Cockpic) |
										 static_cast<int32>(1 << (INT32)eModuleRegion::CockpicSeat));
		}
		else
		{
			rCockpic = GetRandModuleInfo(static_cast<int32>(1 << (INT32)eModuleRegion::Cockpic));
		}
		
		m_DisicionBB->CreateRobotModule(*rCockpic, *FString::Printf(TEXT("%s;Mount_Cockpit"), *GetModuleRegionName(rCockpic->eRegion)));
	}
	return true;
}


/*
 */
bool URModuleShoulder::bCockpicAndShoulder() 
{ 
	return 
		m_DisicionBB->GetMeshComponent(*GetModuleRegionName(eModuleRegion::CockpicAndShoulder)) 
						!= nullptr ? true : false; 
}

/*
 */
void URModuleShoulder::RandShoulderType()
{
	// UE_LOG(LogClass, Log, TEXT("robotname %s"), *m_DisicionBB->GetModuleRobotName());
	
	bool bCockpicShoulder = bCockpicAndShoulder();
	
	int randEnum = 0;
	while (true)
	{
		randEnum = FMath::RandRange(0, (int)eModuleShoulder::Max - 1);
		if ((eModuleShoulder)randEnum == eModuleShoulder::NoShoulder && bCockpicShoulder) 
		{
			continue;
		}
		
		if ((eModuleShoulder)randEnum == eModuleShoulder::NoShoulder)
		{
			continue;
		}


		m_eShoulder = (eModuleShoulder)randEnum;

		UE_LOG(LogClass, Log, TEXT("m_eShoulder enum ---  %s"), *UDataSchema::GetEnumString<eModuleShoulder>("eModuleShoulder", m_eShoulder));
		return;
	}
}


bool URModuleShoulderR::Run()
{
#pragma region NOT CODE
	/*if (m_DisicionBB->GetMeshComponent(*GetModuleRegionName(eModuleRegion::LegAndFullArmHand)) != nullptr)
	{
		if (m_DisicionBB->GetMeshComponent(*GetModuleRegionName(eModuleRegion::CockpicAndShoulder)) == nullptr)
		{
			auto shoulder = GetRandModuleInfo(static_cast<int32>(1 << (INT32)eModuleRegion::Shoulder));

			if (shoulder == nullptr) return false;

			m_DisicionBB->CreateRobotModule(*shoulder, *FString::Printf(TEXT("%s_R;Mount_Weapon_R"), *GetModuleRegionName(shoulder->eRegion)));
			return true;
		}
	}*/
#pragma endregion

	RandShoulderType();


	if (m_eShoulder != eModuleShoulder::NoShoulder && bCockpicAndShoulder() == false) 
	{
		// 어깨 생성
		auto shoulder = GetRandModuleInfo(static_cast<int32>(1 << (INT32)eModuleRegion::Shoulder));

		if (shoulder == nullptr) return false;

		m_DisicionBB->CreateRobotModule(*shoulder, *FString::Printf(TEXT("%s_R;Mount_Weapon_R"), *GetModuleRegionName(shoulder->eRegion)));
	}

	switch (m_eShoulder)
	{
		case eModuleShoulder::NoShoulder:
		{
			//
			break;
		}
		case eModuleShoulder::ForeArm:
		{
			auto foreArm = GetRandModuleInfo(static_cast<int32>(1 << (INT32)eModuleRegion::ForeArm));

			if (foreArm == nullptr) return false;

			if (bCockpicAndShoulder()) 
			{
				m_DisicionBB->CreateRobotModule(*foreArm, *FString::Printf(TEXT("%s_R;Mount_Weapon_R"), *GetModuleRegionName(foreArm->eRegion)));
			}
			else
			{
				m_DisicionBB->CreateRobotModule(*foreArm, *FString::Printf(TEXT("%s_R;Mount_Weapon;%s_R"), *GetModuleRegionName(foreArm->eRegion), *GetModuleRegionName(eModuleRegion::Shoulder)));
			}
			break;
		}
		case eModuleShoulder::ArmHand:
		case eModuleShoulder::ArmNoHand:
		{
			UMeshComponent* RootLeg = m_DisicionBB->GetMeshComponent(*GetModuleRegionName(eModuleRegion::Leg));
			if (RootLeg == nullptr) return false;

			FModuleDefine* ArmRegionInfo = GetRandModuleInfo(m_eShoulder == eModuleShoulder::ArmHand ? static_cast<int32>(1 << (INT32)eModuleRegion::RightArm) : static_cast<int32>(1 << (INT32)eModuleRegion::RightArmNoHand));
		
			if (ArmRegionInfo == nullptr) return false;

			UMeshComponent* region = m_DisicionBB->CreateRobotModule(*ArmRegionInfo, *FString::Printf(TEXT("%s;None"), *GetModuleRegionName(ArmRegionInfo->eRegion)));

			if (region == nullptr) return false;

			USkinnedMeshComponent* skinnedRegion = Cast<USkinnedMeshComponent>(region);

			skinnedRegion->SetMasterPoseComponent(Cast<USkinnedMeshComponent>(RootLeg));
			
			break;
		}
	}
	return true;
}

bool URModuleShoulderL::Run()
{
#pragma region NOT CODE
	/*if (m_DisicionBB->GetMeshComponent(*GetModuleRegionName(eModuleRegion::LegAndFullArmHand)) != nullptr)
	{
		if (m_DisicionBB->GetMeshComponent(*GetModuleRegionName(eModuleRegion::CockpicAndShoulder)) == nullptr)
		{
			auto shoulder = GetRandModuleInfo(static_cast<int32>(1 << (INT32)eModuleRegion::Shoulder));

			if (shoulder == nullptr) return false;

			m_DisicionBB->CreateRobotModule(*shoulder, *FString::Printf(TEXT("%s_R;Mount_Weapon_R"), *GetModuleRegionName(shoulder->eRegion)));
			return true;

		}
	}*/
#pragma endregion

	RandShoulderType();


	if (m_eShoulder != eModuleShoulder::NoShoulder && bCockpicAndShoulder() == false)
	{
		// 어깨 생성
		auto shoulder = GetRandModuleInfo(static_cast<int32>(1 << (INT32)eModuleRegion::Shoulder));

		if (shoulder == nullptr) return false;

		m_DisicionBB->CreateRobotModule(*shoulder, *FString::Printf(TEXT("%s_L;Mount_Weapon_L"), *UDataSchema::GetEnumString<eModuleRegion>("eModuleRegion", shoulder->eRegion)));
	}

	switch (m_eShoulder)
	{
		case eModuleShoulder::NoShoulder:
		{
			break;
		}
		case eModuleShoulder::ForeArm:
		{
			auto foreArm = GetRandModuleInfo(static_cast<int32>(1 << (INT32)eModuleRegion::ForeArm));

			if (foreArm == nullptr) return false;

			if (bCockpicAndShoulder())
			{
				m_DisicionBB->CreateRobotModule(*foreArm, *FString::Printf(TEXT("%s_L;Mount_Weapon_L"), *GetModuleRegionName(foreArm->eRegion)));
			}
			else
			{
				m_DisicionBB->CreateRobotModule(*foreArm, *FString::Printf(TEXT("%s_L;Mount_Weapon;%s_L"), *GetModuleRegionName(foreArm->eRegion), *GetModuleRegionName(eModuleRegion::Shoulder)));
			}

			break;
		}

		case eModuleShoulder::ArmHand:
		case eModuleShoulder::ArmNoHand:
		{
			UMeshComponent* RootLeg = m_DisicionBB->GetMeshComponent(*GetModuleRegionName(eModuleRegion::Leg));
			if (RootLeg == nullptr) return false;

			FModuleDefine* ArmRegionInfo = GetRandModuleInfo(m_eShoulder == eModuleShoulder::ArmHand ? static_cast<int32>(1 << (INT32)eModuleRegion::LeftArm) : static_cast<int32>(1 << (INT32)eModuleRegion::LeftArmNoHand));

			if (ArmRegionInfo == nullptr) return false;

			UMeshComponent* region = m_DisicionBB->CreateRobotModule(*ArmRegionInfo, *FString::Printf(TEXT("%s;None"), *GetModuleRegionName(ArmRegionInfo->eRegion)));

			if (region == nullptr) return false;

			USkinnedMeshComponent* skinnedRegion = Cast<USkinnedMeshComponent>(region);

			skinnedRegion->SetMasterPoseComponent(Cast<USkinnedMeshComponent>(RootLeg));

			break;
		}
	}
	return true;
}
