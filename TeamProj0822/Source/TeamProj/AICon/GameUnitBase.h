// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GenericTeamAgentInterface.h"
#include "../MyGameAIController.h"
#include "GameUnitBase.generated.h"

UCLASS()
class AGameUnitBase : public AActor, public IGenericTeamAgentInterface
{
    GENERATED_BODY()
    
public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Team")
    ETeam Team = ETeam::None;

    virtual FGenericTeamId GetGenericTeamId() const override { return FGenericTeamId(static_cast<uint8>(Team)); }
    virtual void SetGenericTeamId(const FGenericTeamId& InTeamID) override { Team = static_cast<ETeam>(InTeamID.GetId()); }
};