// Fill out your copyright notice in the Description page of Project Settings.


#include "Main.h"
#include "ProductionCompo/BPDComponent_Main.h"
#include "Kismet/GameplayStatics.h"
#include "../MyGamePlayerState.h"
#include "BuildingStateComponent.h"
#include "Perception/AISense_Sight.h"

AMain::AMain()
{
	ProductionComp = CreateDefaultSubobject<UBPDComponent_Main>(TEXT("ProductionComp"));

	RespawnPos = CreateDefaultSubobject<USceneComponent>(TEXT("RespawnPos"));
	RespawnPos->SetupAttachment(DefaultMesh);
	RespawnPos->SetRelativeLocation(FVector(-30.f, 0.f, 0.f));

	
}

void AMain::PreInit()
{
	SetDataTable(Path);	
	bIsBuilt = true;

}

void AMain::PostInit()
{
	AMyGamePlayerState* PS = Cast<AMyGamePlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
	if (!PS) return;
	UBuildingStateComponent* BSComp = PS->FindComponentByClass<UBuildingStateComponent>();
	if (!BSComp) return;
	BSComp->MainCastle = this;
	Stimuli->RegisterForSense(UAISense_Sight::StaticClass());
	Stimuli->RegisterWithPerceptionSystem();
}

void AMain::PostApplyRow()
{
	Mesh->SetVisibility(true);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	FBoxSphereBounds bound = Mesh->GetStaticMesh()->GetBounds();
	Boxcollision->SetWorldLocation(bound.Origin + GetActorLocation());
	Boxcollision->SetBoxExtent(bound.BoxExtent + FVector(overlapoffset, overlapoffset, 0.f));

}



