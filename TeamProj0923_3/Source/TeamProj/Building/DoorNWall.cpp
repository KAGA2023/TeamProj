// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorNWall.h"
#include "ProductionCompo/BPDComponent_NoEffectBuilding.h"
#include "Components/SplineComponent.h"
#include "CastleDoor.h"
#include "CastleWall.h"

ADoorNWall::ADoorNWall()
{
	ProductionComp = CreateDefaultSubobject<UBPDComponent_NoEffectBuilding>(TEXT("ProductionComp"));
	bisDoorWallManager = true;

	SplineComp = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComp"));
	SplineComp->SetupAttachment(Root);

	spawnPos.Empty();
	spawnRot.Empty();
	SpawnedActors.Empty();


}

void ADoorNWall::BeginPlay()
{
	Super::BeginPlay();
}

void ADoorNWall::PreInit()
{
	SetDataTable(Path);
	//if (SavedPoints.Num() <= 1) 
	//{
	//}
	//else
	//{
		SplinePointNum = SplineComp->GetNumberOfSplinePoints();
		for (int32 i = 0; i < SplinePointNum; i++)
		{
			FSplinePointData Data;
			Data.Location = SplineComp->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);
			Data.Rotation = SplineComp->GetRotationAtSplinePoint(i, ESplineCoordinateSpace::Local);
			Data.Scale = SplineComp->GetScaleAtSplinePoint(i);
			Data.ArriveTangent = SplineComp->GetArriveTangentAtSplinePoint(i, ESplineCoordinateSpace::Local);
			Data.LeaveTangent = SplineComp->GetLeaveTangentAtSplinePoint(i, ESplineCoordinateSpace::Local);
			Data.PointType = SplineComp->GetSplinePointType(i);
			SavedPoints.Add(Data);
		}
	//}
	UE_LOG(LogTemp, Warning, TEXT("[WallNDoor] point Num[%d]"), SplinePointNum);
}

void ADoorNWall::CalculateSpawnTrans()
{
	if (SplinePointNum >= 2)
	{
		FVector start{};
		FVector next{};
		spawnPos.Empty();
		spawnRot.Empty();
		for (int i = 0; i < SplinePointNum-1; i++)
		{
			if (i == 0)
				start = GetActorRotation().RotateVector(SplineComp->GetSplinePointAt(i, ESplineCoordinateSpace::World).Position);
			else
				start = next;
			spawnPos.Add(GetActorLocation() + start);
			next = GetActorRotation().RotateVector(SplineComp->GetSplinePointAt(i + 1, ESplineCoordinateSpace::World).Position);

			FVector Extent;
			FVector Dummy;
			if (i != doorPointID)
				next = start + ((next - start).GetSafeNormal() * wallX);
			else
				next = start + ((next - start).GetSafeNormal() * doorX);

			DrawDebugSphere(GetWorld(), spawnPos[i], 50.f, 4, FColor::Blue, false, 5.f);
			UE_LOG(LogTemp, Warning, TEXT("[WallNDoor] Spawnpos : [%d][%f, %f, %f]"), spawnPos.Num(), spawnPos[i].X, spawnPos[i].Y, spawnPos[i].Z);
			spawnRot.Add((next - start).GetSafeNormal().Rotation());
		}
	}
}

void ADoorNWall::getMeshExtent(FString path1, FString path2)
{
	FVector Origin, Extent;
	UObject* dt = StaticLoadObject(UDataTable::StaticClass(), nullptr, *path1);
	if (dt)
	{
		DataTableAsset = Cast<UDataTable>(dt);
		const FBuildingLevelData* Row = Cast<UDataTable>(dt)->FindRow<FBuildingLevelData>(*FString::FromInt(CurrentLevel), TEXT("GetRow"));
		if (!Row)
		{
			UE_LOG(LogTemp, Warning, TEXT("[%s] No Row for Level %d"), *GetName(), CurrentLevel);
		}
		else
		{
			UStaticMesh* MeshToUse = LoadObject<UStaticMesh>(nullptr, *Row->meshRef);
			if (MeshToUse)
			{
				UStaticMeshComponent* TempComp = NewObject<UStaticMeshComponent>();
				TempComp->SetStaticMesh(MeshToUse);
				TempComp->GetLocalBounds(Origin, Extent);
				doorX = Extent.X;

				TempComp->DestroyComponent();
			}
		}
	}
	dt = StaticLoadObject(UDataTable::StaticClass(), nullptr, *path2);
	if (dt)
	{
		DataTableAsset = Cast<UDataTable>(dt);
		const FBuildingLevelData* Row = Cast<UDataTable>(dt)->FindRow<FBuildingLevelData>(*FString::FromInt(CurrentLevel), TEXT("GetRow"));
		if (!Row)
		{
			UE_LOG(LogTemp, Warning, TEXT("[%s] No Row for Level %d"), *GetName(), CurrentLevel);
		}
		else
		{
			UStaticMesh* MeshToUse = LoadObject<UStaticMesh>(nullptr, *Row->meshRef);
			if (MeshToUse)
			{
				UStaticMeshComponent* TempComp = NewObject<UStaticMeshComponent>();
				TempComp->SetStaticMesh(MeshToUse);
				TempComp->GetLocalBounds(Origin, Extent);
				wallX = Extent.X;

				TempComp->DestroyComponent();
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("[WallNDoor] get Extent Wall:[%f] Door:[%f]"),wallX,doorX);
}

void ADoorNWall::SpawnDoorWall()
{
	for (int i = 0; i < spawnPos.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("[WallNDoor] Spawning"));
		FActorSpawnParameters SpawnPara;
		SpawnPara.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		if (i != doorPointID)
		{
			ACastleWall* Spawnedwall = GetWorld()->SpawnActor<ACastleWall>(
				ACastleWall::StaticClass(), // 클래스
				spawnPos[i],                // 위치
				spawnRot[i],                // 회전
				SpawnPara                   // FActorSpawnParameters
			);

			SpawnedActors.Add(Spawnedwall);
			Spawnedwall->ImportStateData(CurrentLevel);
			UE_LOG(LogTemp, Warning, TEXT("[WallNDoor] Spawning : wall"));
		}
		else
		{
			ACastleDoor* Spawneddoor = GetWorld()->SpawnActor<ACastleDoor>(
				ACastleDoor::StaticClass(), // 클래스
				spawnPos[i],                // 위치
				spawnRot[i],                // 회전
				SpawnPara                   // FActorSpawnParameters
			);

			SpawnedActors.Add(Spawneddoor);
			Spawneddoor->ImportStateData(CurrentLevel);
			UE_LOG(LogTemp, Warning, TEXT("[WallNDoor] Spawning : door"));
		}
	}
}

void ADoorNWall::PostApplyRow()
{
	getMeshExtent(DoorPath, WallPath);
	CalculateSpawnTrans();

	for (AActor* act : SpawnedActors)
	{
		Cast<ABuildingBase>(act)->Destroy();
	}
	SpawnedActors.Empty();
	SpawnDoorWall();
	
}

void ADoorNWall::PostSave(FBuildingStateInfo* Info)
{
	Info->isManager = true;
	Info->WallDoorManager.DoorID = doorPointID;
	Info->WallDoorManager.points = SavedPoints;
	UE_LOG(LogTemp, Warning, TEXT("[WallNDoor] Save PointNum : %d"),SavedPoints.Num());
}

void ADoorNWall::RecieveData(FWallDoorManager data)
{

	UE_LOG(LogTemp, Warning, TEXT("[WallNDoor] Recieve Data!!"));
	doorPointID = data.DoorID;
	SavedPoints = data.points;

	SplineComp->ClearSplinePoints();
	for (const FSplinePointData& P : SavedPoints)
	{
		SplineComp->AddSplinePoint(P.Location, ESplineCoordinateSpace::Local, false);
		int32 idx = SplineComp->GetNumberOfSplinePoints() - 1;

		SplineComp->SetRotationAtSplinePoint(idx, P.Rotation, ESplineCoordinateSpace::Local, false);
		SplineComp->SetScaleAtSplinePoint(idx, P.Scale, false);
		SplineComp->SetTangentsAtSplinePoint(idx, P.ArriveTangent, P.LeaveTangent, ESplineCoordinateSpace::Local, false);
		SplineComp->SetSplinePointType(idx, P.PointType, false);
	}

	UE_LOG(LogTemp, Warning, TEXT("[WallNDoor] REceive PointNum : %d"), SplineComp->GetNumberOfSplinePoints());
	SplineComp->UpdateSpline();
	SavedPoints.Empty();
}

// 로테이터 제로로 박자
// 박스 익스텐트 x가 피벗 중심인지 origin중심인지 확인 필요.
// start와 넥스트만 바꿀게 아니라 스플라인 포인트 지점 자체도 변경해야함.