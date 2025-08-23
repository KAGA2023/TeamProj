// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GenericTeamAgentInterface.h"
#include "MyGameAIController.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class ETeam : uint8
{
	None		= 0,
	Player		= 1,
	Ally		= 2,
	Building	= 3,
	Enemy		= 4
};


UCLASS()
class TEAMPROJ_API AMyGameAIController : public AAIController
{
	GENERATED_BODY()
	
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

protected:
	virtual void OnPossess(APawn* InPawn) override;

	
};
