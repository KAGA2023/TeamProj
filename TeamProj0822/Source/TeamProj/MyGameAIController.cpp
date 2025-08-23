// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameAIController.h"

AMyGameAIController::AMyGameAIController()
{
	//ÆÛ¼Á¼Ç ÄÄÆ÷³ÍÆ®
}



void AMyGameAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (const IGenericTeamAgentInterface* T = Cast<IGenericTeamAgentInterface>(InPawn))
	{
		SetGenericTeamId(T->GetGenericTeamId());
	}
	else if (IGenericTeamAgentInterface* W = Cast<IGenericTeamAgentInterface>(InPawn))
		W->SetGenericTeamId(GetGenericTeamId());
}

static bool IsPAB(uint8 id) //ÇÃ·¹ÀÌ¾î ¾Æ±º ºôµù ¹­À½
{
	return	(id == static_cast<uint8>(ETeam::Player)) ||
			(id == static_cast<uint8>(ETeam::Ally)) ||
			(id == static_cast<uint8>(ETeam::Building));
}


ETeamAttitude::Type AMyGameAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	const IGenericTeamAgentInterface* SA = Cast<IGenericTeamAgentInterface>(this);
	const IGenericTeamAgentInterface* SB = Cast<IGenericTeamAgentInterface>(&Other);

	const FGenericTeamId A = SA ? SA->GetGenericTeamId() : FGenericTeamId::NoTeam;
	const FGenericTeamId B = SB ? SB->GetGenericTeamId() : FGenericTeamId::NoTeam;

	if (A == B) return ETeamAttitude::Friendly;

	if (IsPAB(A.GetId()) && IsPAB(B.GetId())) return ETeamAttitude::Friendly; 
	return ETeamAttitude::Hostile;
}

