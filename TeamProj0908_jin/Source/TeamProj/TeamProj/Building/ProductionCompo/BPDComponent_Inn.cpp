// Fill out your copyright notice in the Description page of Project Settings.


#include "BPDComponent_Inn.h"
#include "Kismet/GameplayStatics.h"
#include "../../Currency/Gold.h"
#include "../../Stage/RoundComponent.h"
#include "../../MyGamePlayerState.h"

UBPDComponent_Inn::UBPDComponent_Inn()
{
	OwnEffect = { EBuildingEffectType::Statue_Exp };// 데이터 테이블 바꾸면 얘도 바꿔야함.

}

void UBPDComponent_Inn::OnBinding()
{
	UE_LOG(LogTemp, Warning, TEXT("Binding Try"));
	APawn* Pawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (Pawn)
	{
		if (URoundComponent* RoundComp = Cast<AMyGamePlayerState>(Pawn->GetPlayerState())->RoundCom)
		{
			RoundComp->OnWarEnd.AddDynamic(this, &UBPDComponent_Inn::HandleOnActivate);
			UE_LOG(LogTemp, Warning, TEXT("Binding Succes"));
		}
	}
}

void UBPDComponent_Inn::HandleOnActivate()
{
	for (FBuildingEffect ef : Effects)
	{
		if (CheckEffect(ef))
		{
			UE_LOG(LogTemp, Warning, TEXT("In binding Activate"));
			OnActivate(ef); // 실제 효과 적용
		}
	}
}

void UBPDComponent_Inn::ActivateEffect(const FBuildingEffect& Effect)
{
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	FVector SpawnLocation = GetOwner()->GetActorLocation() - GetOwner()->GetActorForwardVector() * 400;
	FRotator SpawnRotation = FRotator::ZeroRotator;
	switch (Effect.Type)
	{
	case EBuildingEffectType::Statue_Exp:
		makeExp = Effect.intValue;
		for (int i = 0; i < makeExp; i++)
		{
			UE_LOG(LogTemp, Warning, TEXT("Spawn Exp"));
			AGold* SpawnedGold = GetWorld()->SpawnActor<AGold>(
				AGold::StaticClass(),
				SpawnLocation + GetOwner()->GetActorRightVector() * (-160 + 80 * i),
				SpawnRotation,
				SpawnParams
			);
		}
		break;
	default:
		break;
	}

}
