// Fill out your copyright notice in the Description page of Project Settings.


#include "BPDComponent_Statue.h"
#include "Kismet/GameplayStatics.h"
#include "../../Currency/Exp.h"
#include "../../Stage/RoundComponent.h"
#include "../../MyGamePlayerState.h"


UBPDComponent_Statue::UBPDComponent_Statue()
{
    OwnEffect = { EBuildingEffectType::Statue_Exp };
}

void UBPDComponent_Statue::OnBinding()
{
    UE_LOG(LogTemp, Warning, TEXT("Binding Try"));
    APawn* Pawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (Pawn)
    {
        if (URoundComponent* RoundComp = Cast<AMyGamePlayerState>(Pawn->GetPlayerState())->RoundCom)
        {
            RoundComp->OnWarEnd.AddDynamic(this, &UBPDComponent_Statue::HandleOnActivate);
            UE_LOG(LogTemp, Warning, TEXT("Binding Succes"));
        }
    }
}

void UBPDComponent_Statue::HandleOnActivate()
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

void UBPDComponent_Statue::ActivateEffect(const FBuildingEffect& Effect)
{
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
    UE_LOG(LogTemp, Warning, TEXT("Spawn Exp"));
    FVector SpawnLocation = GetOwner()->GetActorLocation() - GetOwner()->GetActorForwardVector() * 400;
    FRotator SpawnRotation = FRotator::ZeroRotator;
    switch (Effect.Type)
    {
    case EBuildingEffectType::Statue_Exp:
        makeExp = Effect.intValue;
        for (int i = 0; i < makeExp; i++)
        {
            UE_LOG(LogTemp, Warning, TEXT("Spawn Exp"));
            AExp* SpawnedExp = GetWorld()->SpawnActor<AExp>(
                AExp::StaticClass(), 
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

// destroy될때 알아서 바인드 풀기
void UBPDComponent_Statue::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    APawn* Pawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (Pawn)
    {
        if (URoundComponent* RoundComp = Cast<AMyGamePlayerState>(Pawn->GetPlayerState())->RoundCom)
        {
            RoundComp->OnWarEnd.RemoveDynamic(this, &UBPDComponent_Statue::HandleOnActivate);
            UE_LOG(LogTemp, Warning, TEXT("Binding Remove"));
        }
    }

    Super::EndPlay(EndPlayReason);
}



