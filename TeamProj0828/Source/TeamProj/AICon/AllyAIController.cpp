// Fill out your copyright notice in the Description page of Project Settings.


#include "AllyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

AAllyAIController::AAllyAIController()
{
	Team = ETeam::Ally;

	static ConstructorHelpers::FObjectFinder<UBehaviorTree>
		Tree(TEXT("/Game/Data/AI/Ally/BT_Ally.BT_Ally"));
	if (Tree.Succeeded())
		BTData = Tree.Object;

	static ConstructorHelpers::FObjectFinder<UBlackboardData>
		Data(TEXT("/Game/Data/AI/Ally/BB_Ally.BB_Ally"));
	if (Data.Succeeded())
		BBData = Data.Object;
}

void AAllyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}
