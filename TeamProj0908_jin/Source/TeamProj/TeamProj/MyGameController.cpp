// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Skill/SkillComponent.h" 
#include "Skill/SkillStruct.h"
#include "Character/GameCharacterBase.h"
#include "Item/ItemComponent.h"


AMyGameController::AMyGameController()
{
	IA_LMB = nullptr;
	IA_RMB = nullptr;
	IA_SPACEBAR = nullptr;
	IA_Wheel = nullptr;
	IA_WheelClick = nullptr;
	IMC_Default = nullptr;
	MainWidget = nullptr;
	MainWidgetClass = nullptr;
	IsLMBPress = false;
	HUD = nullptr;
	IsDetachCamera = false;

	PrimaryActorTick.bCanEverTick = true;
}

void AMyGameController::BeginPlay()
{
	Super::BeginPlay();

	if (!IsLocalController()) return;
	
	if (ULocalPlayer* LP = GetLocalPlayer())
	{
		if (auto* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(IMC_Default, 0);
		}
	}
	bShowMouseCursor = true;

	if (MainWidgetClass)
	{
		MainWidget = CreateWidget<UUserWidget>(this, MainWidgetClass);
		if (MainWidget)
		{
			MainWidget->AddToViewport();

		}
	}

	HUD = GetHUD();
	SetIgnoreLookInput(true);
	FInputModeGameAndUI Mode;
	Mode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	Mode.SetHideCursorDuringCapture(false);                         
	SetInputMode(Mode);
}

void AMyGameController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (auto* EIC = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EIC->BindAction(IA_LMB, ETriggerEvent::Started, this, &AMyGameController::OnLMB_Pressed);
		EIC->BindAction(IA_LMB, ETriggerEvent::Completed, this, &AMyGameController::OnLMB_Released);
		EIC->BindAction(IA_RMB, ETriggerEvent::Started, this, &AMyGameController::OnRMB_Pressed);
		EIC->BindAction(IA_RMB, ETriggerEvent::Completed, this, &AMyGameController::OnRMB_Released);
		EIC->BindAction(IA_Wheel, ETriggerEvent::Triggered, this, &AMyGameController::OnWheelAxis_Triggered);
		EIC->BindAction(IA_WheelClick, ETriggerEvent::Started, this, &AMyGameController::OnWheelClick_Pressed);
		EIC->BindAction(IA_WheelClick, ETriggerEvent::Completed, this, &AMyGameController::OnWheelClick_Released);
		EIC->BindAction(IA_SPACEBAR, ETriggerEvent::Started, this, &AMyGameController::OnSPACEBAR_Pressed);
		EIC->BindAction(IA_Q, ETriggerEvent::Started, this, &AMyGameController::OnQ_Pressed);
		EIC->BindAction(IA_E, ETriggerEvent::Started, this, &AMyGameController::OnE_Pressed);
		EIC->BindAction(IA_R, ETriggerEvent::Started, this, &AMyGameController::OnR_Pressed);
		EIC->BindAction(IA_F, ETriggerEvent::Started, this, &AMyGameController::OnF_Pressed);
		EIC->BindAction(IA_Tab, ETriggerEvent::Started, this, &AMyGameController::OnTab_Pressed);
	}


}



void AMyGameController::OnLMB_Pressed()
{
	IsLMBPress = true;
	if (IWidgetInterface* IF = Cast<IWidgetInterface>(GetHUD()))
		IF->DragPressed_Implementation();

	if (bTargeting)
	{
		if (FinalizeAimForSkill())
			EndTargeting(true);
		else
			EndTargeting(false);
	}
	else if (bItemTargeting)
	{
		if (FinalizeAimForItem())
			EndItemTargeting(true);
		else
			EndItemTargeting(false);
	}
}



void AMyGameController::OnLMB_Released()
{
	IsLMBPress = false;
	if (IWidgetInterface* IF = Cast<IWidgetInterface>(GetHUD()))
		IF->DragReleased_Implementation();
}


void AMyGameController::OnRMB_Pressed()
{
	if (IWidgetInterface* IF = Cast<IWidgetInterface>(GetHUD()))
	{
		FHitResult Point;
		GetHitResultUnderCursorByChannel(TraceTypeQuery1, true, Point);
		IF->Move_Implementation(FVector2D(Point.ImpactPoint.X,Point.ImpactPoint.Y));
	}

	if (bTargeting)
		EndTargeting(false);

	if (bItemTargeting)
		EndItemTargeting(false);
}


void AMyGameController::OnRMB_Released()
{
}

void AMyGameController::OnQ_Pressed()
{
	BeginTargeting(0);
}

void AMyGameController::OnE_Pressed()
{
	BeginTargeting(1);
}

void AMyGameController::OnR_Pressed()
{
	BeginTargeting(2);
}

void AMyGameController::OnF_Pressed()
{
	UE_LOG(LogTemp, Warning, TEXT("Item Use Key"));
	BeginItemTargeting();
}

void AMyGameController::OnTab_Pressed()
{
	UE_LOG(LogTemp, Warning, TEXT("Object Interaction Key"));
}

void AMyGameController::OnWheelAxis_Triggered(const FInputActionInstance& Instance)
{
	float Value = Instance.GetValue().Get<float>();
	if (APlayerCharacter* CH = Cast<APlayerCharacter>(GetPawn()))
	{
		CH->ZoomInCamera(Value);
	}
}


void AMyGameController::OnWheelClick_Pressed()
{
}

void AMyGameController::OnWheelClick_Released()
{
}

void AMyGameController::OnSPACEBAR_Pressed()
{
	IsDetachCamera = !IsDetachCamera;
	
	if (APlayerCharacter* CH = Cast<APlayerCharacter>(GetPawn()))
	{
		if (IsDetachCamera)
			CH->DetachCamera();
		else
			CH->AttachCamera();
	}

}



void AMyGameController::Tick(float DeltaTime)
{
	if (IsLMBPress)
	{
		if (IWidgetInterface* IF = Cast<IWidgetInterface>(GetHUD()))
		{
			IF->DragHold_Implementation();
		}
	}

	if (bTargeting)
	{
		UpdateTargeting(DeltaTime);
	}

	if (bItemTargeting)
	{
		UpdateItemTargeting(DeltaTime);
	}
}


void AMyGameController::OnPossess(APawn* pPawn)
{
	Super::OnPossess(pPawn);
}


bool AMyGameController::GetMouseAimLocation(FVector& OutLocation, FHitResult* OutHit) const
{
	FHitResult Hit;
	const bool bHit = GetHitResultUnderCursor(TargetTraceChannel, true, Hit);

	if (bHit)
	{
		OutLocation = Hit.ImpactPoint;
		if (OutHit) *OutHit = Hit;
		return true;
	}
	return false;
}

void AMyGameController::BeginTargeting(int32 SkillSlotIndex)
{
	PendingSkillIndex = SkillSlotIndex;
	bTargeting = true;

	if (!TargetDecal && TargetDecalMaterial)
	{
		const FVector SpawnAt = GetPawn() ? GetPawn()->GetActorLocation() : FVector::ZeroVector;
		TargetDecal = UGameplayStatics::SpawnDecalAtLocation(
			GetWorld(),
			TargetDecalMaterial,
			FVector(DefaultDecalThickness, 128.f, 128.f),
			SpawnAt,
			FRotator(-90.f, 0.f, 0.f),
			0.f
		);
		if (TargetDecal)
			TargetDecal->SetFadeScreenSize(0.f);
	}
	else if (TargetDecal)
	{
		TargetDecal->SetHiddenInGame(false);
	}
}

void AMyGameController::EndTargeting(bool bApply)
{
	if (bApply && GetPawn())
	{
		if (USkillComponent* Skills = GetPawn()->FindComponentByClass<USkillComponent>())
		{
			Skills->TryUseSKill(PendingSkillIndex, nullptr, CurrentAim);
		}
	}
	bTargeting = false;
	PendingSkillIndex = INDEX_NONE;
	CurrentAim = FVector::ZeroVector;

	if (TargetDecal)
		TargetDecal->SetHiddenInGame(true);
}

bool AMyGameController::FinalizeAimForSkill()
{
	if (!bTargeting || !GetPawn()) return false;

	FVector Aim; 
	FHitResult Hit;
	if (!GetMouseAimLocation(Aim, &Hit)) return false;

	const FVector O = GetPawn()->GetActorLocation();
	const float Range = GetSkillRangeFromSlot(PendingSkillIndex);
	{
		FHitResult G;
		FVector S = Aim + FVector(0, 0, 3000), E = Aim - FVector(0, 0, 3000);
		if (GetWorld()->LineTraceSingleByChannel(G, S, E, ECC_Visibility))
			Aim = G.ImpactPoint;
	}

	const bool bClamp = bClampToSkillRange;
	const FVector V = Aim - O;
	const float   D = V.Size2D();

	if (Range > 0.f && D > Range)
	{
		if (bClamp)
			Aim = O + V.GetSafeNormal2D() * Range;        
		else
			return false;                                 
	}

	CurrentAim = Aim; 
	return true;

}

void AMyGameController::UpdateTargeting(float DeltaSeconds)
{
	FHitResult Hit;
	if (!GetMouseAimLocation(CurrentAim, &Hit))
		return;

	if (bClampToSkillRange && GetPawn())
	{
		const float Range = GetSkillRangeFromSlot(PendingSkillIndex);
		if (Range > 0.f)
		{
			const FVector O = GetPawn()->GetActorLocation();
			const FVector V = CurrentAim - O;
			const float D = V.Size();
			if (D > Range)
				CurrentAim = O + V / D * Range;
		}
	}

	if (TargetDecal)
	{
		const float Range = GetSkillRangeFromSlot(PendingSkillIndex);
		const float Radius = GetSkillRadiusFromSlot(PendingSkillIndex);
		TargetDecal->SetWorldLocation(CurrentAim + FVector(0, 0, 2.f));

		FRotator DecalRot = FRotator(-90.f, 0.f, -90.f);
		if (bSnapToSurfaceNormal && Hit.bBlockingHit)
		{
			DecalRot = UKismetMathLibrary::MakeRotFromZ(-Hit.ImpactNormal);
		}
		TargetDecal->SetWorldRotation(DecalRot);

		TargetDecal->DecalSize = FVector(DefaultDecalThickness, 50.f, 50.f);
	}
}

float AMyGameController::GetSkillRangeFromSlot(int32 SlotIndex) const
{
	if (const APawn* P = GetPawn())
		if (const USkillComponent* SkillComp = P->FindComponentByClass<USkillComponent>())
		{
			USkill* S = SkillComp->GetSkillSlot(SlotIndex);
			const float Range = S->GetSkillData().Range + S->GetSkillLevelData().AddRange;
			return Range;
		}
	return 800.f;
}

float AMyGameController::GetSkillRadiusFromSlot(int32 SlotIndex) const
{
	if (const APawn* P = GetPawn())
		if (const USkillComponent* SkillComp = P->FindComponentByClass<USkillComponent>())
		{
			USkill* S = SkillComp->GetSkillSlot(SlotIndex);
			const float Radius = S->GetSkillData().Radius + S->GetSkillLevelData().AddRadius;
			return Radius;
		}
	return 200.f;
}

///////////////////////////////////////////////////////////// 아이템

// 아이템 타겟팅 함수들
void AMyGameController::BeginItemTargeting()
{
	// 아이템 수량 확인
	if (const APawn* P = GetPawn())
	{
		if (const UItemComponent* ItemComp = P->FindComponentByClass<UItemComponent>())
		{
			if (UItem* EquippedItem = ItemComp->GetEquippedItem())
			{
				// 수량이 0이면 타겟팅 시작하지 않음
				if (!EquippedItem->HasQuantity())
				{
					return;
				}
			}
			else
			{
				// 장착된 아이템이 없으면 타겟팅 시작하지 않음
				return;
			}
		}
		else
		{
			// ItemComponent가 없으면 타겟팅 시작하지 않음
			return;
		}
	}

	bItemTargeting = true;

	// 데칼 생성 또는 표시
	if (!TargetDecal)
	{
		UE_LOG(LogTemp, Warning, TEXT("1"));

		if (TargetDecalMaterial)
		{
			UE_LOG(LogTemp, Warning, TEXT("2"));
			const FVector SpawnAt = GetPawn() ? GetPawn()->GetActorLocation() : FVector::ZeroVector;
			TargetDecal = UGameplayStatics::SpawnDecalAtLocation(
				GetWorld(),
				TargetDecalMaterial,
				FVector(DefaultDecalThickness, 128.f, 128.f),
				SpawnAt,
				FRotator(-90.f, 0.f, 0.f),
				0.f
			);
			if (TargetDecal)
			{
				UE_LOG(LogTemp, Warning, TEXT("3"));
				TargetDecal->SetFadeScreenSize(0.f);
			}
		}
	}
	else
	{
		TargetDecal->SetHiddenInGame(false);
	}
}

void AMyGameController::EndItemTargeting(bool bApply)
{
	if (bApply && GetPawn())
	{
		if (UItemComponent* ItemComp = GetPawn()->FindComponentByClass<UItemComponent>())
		{
			ItemComp->TryUseItem(nullptr, CurrentItemAim);
		}
	}
	bItemTargeting = false;
	CurrentItemAim = FVector::ZeroVector;

	if (TargetDecal)
		TargetDecal->SetHiddenInGame(true);
}

bool AMyGameController::FinalizeAimForItem()
{
	if (!bItemTargeting || !GetPawn()) return false;

	FVector Aim;
	FHitResult Hit;
	if (!GetMouseAimLocation(Aim, &Hit)) return false;

	const FVector O = GetPawn()->GetActorLocation();
	const float Range = GetItemRange();
	{
		FHitResult G;
		FVector S = Aim + FVector(0, 0, 3000), E = Aim - FVector(0, 0, 3000);
		if (GetWorld()->LineTraceSingleByChannel(G, S, E, ECC_Visibility))
			Aim = G.ImpactPoint;
	}

	const bool bClamp = bClampToSkillRange;
	const FVector V = Aim - O;
	const float   D = V.Size2D();

	if (Range > 0.f && D > Range)
	{
		if (bClamp)
			Aim = O + V.GetSafeNormal2D() * Range;
		else
			return false;
	}

	CurrentItemAim = Aim;
	return true;
}

void AMyGameController::UpdateItemTargeting(float DeltaSeconds)
{
	FHitResult Hit;
	if (!GetMouseAimLocation(CurrentItemAim, &Hit))
		return;
	UE_LOG(LogTemp, Warning, TEXT("dawdwa"));
	if (bClampToSkillRange && GetPawn())
	{
		const float Range = GetItemRange();
		if (Range > 0.f)
		{
			const FVector O = GetPawn()->GetActorLocation();
			const FVector V = CurrentItemAim - O;
			const float D = V.Size();
			if (D > Range)
				CurrentItemAim = O + V / D * Range;
		}
	}

	if (TargetDecal)
	{
		const float Range = GetItemRange();
		const float Radius = GetItemRadius();
		TargetDecal->SetWorldLocation(CurrentItemAim + FVector(0, 0, 2.f));

		FRotator DecalRot = FRotator(-90.f, 0.f, -90.f);
		if (bSnapToSurfaceNormal && Hit.bBlockingHit)
		{
			DecalRot = UKismetMathLibrary::MakeRotFromZ(-Hit.ImpactNormal);
		}
		TargetDecal->SetWorldRotation(DecalRot);

		TargetDecal->DecalSize = FVector(DefaultDecalThickness, 50.f, 50.f);
	}
}

float AMyGameController::GetItemRange() const
{
	if (const APawn* P = GetPawn())
		if (const UItemComponent* ItemComp = P->FindComponentByClass<UItemComponent>())
		{
			if (UItem* EquippedItem = ItemComp->GetEquippedItem())
			{
				const float Range = EquippedItem->GetItemData().Range + EquippedItem->GetItemLevelData().AddRange;
				return Range;
			}
		}
	return 800.f;
}

float AMyGameController::GetItemRadius() const
{
	if (const APawn* P = GetPawn())
		if (const UItemComponent* ItemComp = P->FindComponentByClass<UItemComponent>())
		{
			if (UItem* EquippedItem = ItemComp->GetEquippedItem())
			{
				const float Radius = EquippedItem->GetItemData().Radius + EquippedItem->GetItemLevelData().AddRadius;
				return Radius;
			}
		}
	return 200.f;
}
