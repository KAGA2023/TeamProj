// Fill out your copyright notice in the Description page of Project Settings.


#include "Exp.h"
#include "Kismet/GameplayStatics.h"
#include "../Status/StatusComponent.h"
#include "../Character/GameCharacterBase.h"
#include "../MyGamePlayerState.h"

AExp::AExp()
{
	Mesh->SetRelativeScale3D(FVector(0.3f,0.3f,0.3f));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshRes(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	if (MeshRes.Succeeded())
	{
		Mesh->SetStaticMesh(MeshRes.Object);
	}
	ConstructorHelpers::FObjectFinder<UMaterialInterface> MTres(TEXT("/Game/Building/Models/MT_EXP.MT_EXP"));
	if (MTres.Succeeded())
		Mesh->SetMaterial(0, MTres.Object);

	RotatingMovement->RotationRate = FRotator(0.f, 180.f, 0.f);
	InterpToMovement->AddControlPointPosition(FVector(0.f, 0.f, 0.f));
	InterpToMovement->AddControlPointPosition(FVector(0.f, 0.f, 100.f));

	InterpToMovement->Duration = 1.0f;
	InterpToMovement->BehaviourType = EInterpToBehaviourType::PingPong;
	InterpToMovement->bPauseOnImpact = false;
}

void AExp::OnGet()
{
	APawn* Pawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (Pawn)
	{
		if (APlayerCharacter* PC = Cast<APlayerCharacter>(Pawn))
		{
			if (UStatusComponent* StatComp = PC->FindComponentByClass<UStatusComponent>())
			{
				StatComp->AddExp(Exp);
			}
		}
		if (AMyGamePlayerState* MyPS = Cast<AMyGamePlayerState>(Pawn->GetPlayerState()))
		{
			UE_LOG(LogTemp, Warning, TEXT("Begin Overlap: %d"), MyPS->Exp);
			MyPS->Exp += Exp;
			UE_LOG(LogTemp, Warning, TEXT("Begin Overlap: %d"), MyPS->Exp);

			Destroy();
		}
	}
}

void AExp::GetExp(int32 getExp)
{
	Exp = getExp;
}
