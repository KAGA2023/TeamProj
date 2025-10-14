// Fill out your copyright notice in the Description page of Project Settings.


#include "Gold.h"
#include "Kismet/GameplayStatics.h"
#include "../MyGamePlayerState.h"
AGold::AGold()
{
	Mesh->SetRelativeScale3D(FVector(8.f, 8.f, 8.f));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshRes(TEXT("/Game/Building/Models/SM_Coin_01.SM_Coin_01"));
	if (MeshRes.Succeeded())
	{
		Mesh->SetStaticMesh(MeshRes.Object);
	}
	
}

void AGold::OnGet()
{
	APawn* Pawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (Pawn)
	{
		if (AMyGamePlayerState* MyPS = Cast<AMyGamePlayerState>(Pawn->GetPlayerState()))
		{
			UE_LOG(LogTemp, Warning, TEXT("Begin Overlap: %d"), MyPS->Gold);
			MyPS->Gold++;
			UE_LOG(LogTemp, Warning, TEXT("Begin Overlap: %d"), MyPS->Gold);

			Destroy();
		}
	}
}
