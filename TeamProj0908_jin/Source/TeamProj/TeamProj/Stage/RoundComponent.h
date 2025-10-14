// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StageStruct.h"
#include "RoundComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRoundStart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRoundEnd);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWarStart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWarEnd);

UENUM(BlueprintType)
enum class ERoundState : uint8
{
	None
	, RoundStart
	, WarStart
	, WarEnd
	, RoundEnd
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAMPROJ_API URoundComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnRoundStart OnRoundStart;

	UPROPERTY(BlueprintAssignable)
	FOnRoundEnd OnRoundEnd;

	UPROPERTY(BlueprintAssignable)
	FOnWarStart OnWarStart;

	UPROPERTY(BlueprintAssignable)
	FOnWarEnd OnWarEnd;

public:	
	URoundComponent();

	UPROPERTY(BlueprintReadOnly)
	FRound RoundData;

	UPROPERTY(BlueprintReadOnly)
	TArray<FVector> PortalPos{};

	UPROPERTY(BlueprintReadOnly)
	int32 TotalMobCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PotalBP")
	TSubclassOf<class APortal> PortalBP;

	UPROPERTY(BlueprintReadWrite)
	class UStageComponent* StageCom;

	UPROPERTY(BlueprintReadWrite)
	ERoundState RoundState = ERoundState::None;

	//UPROPERTY(EditAnywhere, Category = Potal)
//TObjectPtr<class UPortal> potalActor;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void InitRound(FStage* stageInfo, FRound& roundInfo);

	UFUNCTION(BlueprintCallable)
	void SetState(ERoundState state);

	void InitRoundStart();
	void InitRoundEnd();
	void InitWarStart();
	void InitWarEnd();

	void SetPortalPos(TArray<FVector> ptps);


	void CheckMonsterCount();
		
};
