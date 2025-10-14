// Fill out your copyright notice in the Description page of Project Settings.


#include "Barrack.h"

ABarrack::ABarrack()
{
}

void ABarrack::SetSpawnposList()
{
	FBoxSphereBounds bound = Mesh->GetStaticMesh()->GetBounds();
	Boxcollision->SetWorldLocation(bound.Origin);
	Boxcollision->SetBoxExtent(bound.BoxExtent);

	for (FVector pos : SpawnPosList)
	{
		pos = GetActorTransform().TransformPosition(pos);

		//디버그용
		DrawDebugSphere(GetWorld(), pos, 50.f, 4, FColor::Blue, false, 5.f);
	}
	// 스폰할때 
}

void ABarrack::PostApplyRow()
{
	SetSpawnposList();
}

void ABarrack::SelectIdentity(FName select)
{
	//ui에서 서브직업 선택시 호출
	if (CurSpawnIdentity == select)
		return;

	// 프로덕션에게 다지우고 새로 만들라는 신호 줘야함.
}

