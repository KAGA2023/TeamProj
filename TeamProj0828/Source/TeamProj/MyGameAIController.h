// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GenericTeamAgentInterface.h"
#include "AICon/GameUnitBase.h"
#include "MyGameAIController.generated.h"

/**
 * 
 */

//일단 타입분류만들어놓으
UENUM()
enum class EAIType : uint8
{
	None,
	Melee,
	Ranged
};

UCLASS()
class TEAMPROJ_API AMyGameAIController : public AAIController
{
	GENERATED_BODY()
	
protected:
	TObjectPtr<class UBehaviorTree> BTData;
	TObjectPtr<class UBlackboardData> BBData;

	EAIType AIType = EAIType::None;
	virtual void OnPossess(APawn* InPawn) override;
	
public:
	AMyGameAIController();

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Team")
	ETeam Team = ETeam::None;

	virtual FGenericTeamId GetGenericTeamId() const override
	{
		return FGenericTeamId(static_cast<uint8>(Team));
	}

	virtual void SetGenericTeamId(const FGenericTeamId& InTeamID) override
	{
		Team = static_cast<ETeam>(InTeamID.GetId());
	}

	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

public:
	static const FName StartPosKey;
	static const FName RoamingPosKey;
	static const FName TargetKey;

	
};
