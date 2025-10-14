// Fill out your copyright notice in the Description page of Project Settings.


#include "Gold.h"
#include "Kismet/GameplayStatics.h"
#include "../MyGamePlayerState.h"
AGold::AGold()
{
	

	Mesh->SetRelativeScale3D(FVector(8.f, 8.f, 8.f));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MoneyMesh1(TEXT("/Game/Building/Models/SM_Coin_01.SM_Coin_01"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MoneyMesh2(TEXT("/Game/Building/Models/SM_Coin_06.SM_Coin_06"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MoneyMesh3(TEXT("/Game/Building/Models/SM_Coin_07.SM_Coin_07"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MoneyMesh4(TEXT("/Game/Building/Models/SM_Money_Bag_02.SM_Money_Bag_02"));
	if (MoneyMesh1.Succeeded())
	{
		money1 = MoneyMesh1.Object;
	}
	if (MoneyMesh2.Succeeded())
	{
		money2 = MoneyMesh2.Object;
	}
	if (MoneyMesh3.Succeeded())
	{
		money3 = MoneyMesh3.Object;
	}
	if (MoneyMesh4.Succeeded())
	{
		money4 = MoneyMesh4.Object;
	}

	RotatingMovement->RotationRate = FRotator(0.f, 180.f, 0.f);
	InterpToMovement->AddControlPointPosition(FVector(0.f, 0.f, 0.f));
	InterpToMovement->AddControlPointPosition(FVector(0.f, 0.f, 100.f));

	InterpToMovement->Duration = 1.0f;
	InterpToMovement->BehaviourType = EInterpToBehaviourType::PingPong;
	InterpToMovement->bPauseOnImpact = false;
	
}

void AGold::OnGet()
{
	APawn* Pawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (Pawn)
	{
		if (AMyGamePlayerState* MyPS = Cast<AMyGamePlayerState>(Pawn->GetPlayerState()))
		{
			UE_LOG(LogTemp, Warning, TEXT("Begin Overlap: %d"), MyPS->Gold);
			MyPS->Gold+= Money;
			UE_LOG(LogTemp, Warning, TEXT("Begin Overlap: %d"), MyPS->Gold);

			Destroy();
		}
	}
}

 void AGold::GetMoney(int32 getMoney)
{
	 Money = getMoney;
	 if (getMoney < 5)
	 {
		 Mesh->SetStaticMesh(money1);
		 Mesh->SetWorldRotation(FRotator(90.f, 0.f, 0.f));
	 }
	 else if (getMoney >= 5 && getMoney < 10)
		 Mesh->SetStaticMesh(money2);
	 else if (getMoney >= 10 && getMoney < 15)
		 Mesh->SetStaticMesh(money3);
	 else if (getMoney >= 15)
		 Mesh->SetStaticMesh(money4);
}
