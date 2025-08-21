// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageComponent.h"
#include "../Character/GameCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"

UDamageComponent::UDamageComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	
}


// Called when the game starts
void UDamageComponent::BeginPlay()
{
	Super::BeginPlay();

	
	
}

void UDamageComponent::AddBuff(const FSkillLevelData& pBuff)
{

}

void UDamageComponent::RemoveBuff(const FSkillLevelData& pBuff)
{
}

void UDamageComponent::AddStatusEffect(const FSkillEffectInfo& pStatusEffect)
{
}

void UDamageComponent::HandleExpiredStatusEffect(const FActiveStatusEffect& ExpiredEffect)
{
}

void UDamageComponent::ApplyDotDamage(const FActiveStatusEffect& Effect)
{
}


// Called every frame
void UDamageComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Buff
	for (int32 i = ActiveBuffs.Num() - 1; i >= 0; i++)
	{
		ActiveBuffs[i].RemainingDuration -= DeltaTime;

		if (ActiveBuffs[i].RemainingDuration <= 0.f)
			RemoveBuff(ActiveBuffs[i].BuffData);
	}

	for (int32 i = ActiveStatusEffects.Num() - 1; i >= 0; --i)
	{
		FActiveStatusEffect& Effect = ActiveStatusEffects[i];

		Effect.RemainingDuration -= DeltaTime;
		Effect.TickDmgTime += DeltaTime;

		if (Effect.TickDmgTime >= 1.0f)
		{
			ApplyDotDamage(Effect);
			Effect.TickDmgTime = 0.f;
		}

		if (Effect.RemainingDuration <= 0.f)
		{
			HandleExpiredStatusEffect(Effect);
			ActiveStatusEffects.RemoveAt(i);
		}
	}
}

void UDamageComponent::ApplyStun(float Duration)
{
	if (b_Stunned)
		GetWorld()->GetTimerManager().ClearTimer(Timer_Stun);
	b_Stunned = true;

	if (AGameCharacterBase* Char = Cast<AGameCharacterBase>(GetOwner()))
	{
		if (UCharacterMovementComponent* MoveComp = Char->GetCharacterMovement())
		{
			MoveComp->DisableMovement();
			//TODO : 스킬시전도 안되게 넣어야할듯
		}
	}
	GetWorld()->GetTimerManager().SetTimer(Timer_Stun, this, &UDamageComponent::ClearStun, false);
}

void UDamageComponent::ClearStun()
{
	b_Stunned = false;
	if (AGameCharacterBase* Char = Cast<AGameCharacterBase>(GetOwner()))
	{
		{
			if (UCharacterMovementComponent* MoveComp = Char->GetCharacterMovement())
			{
				MoveComp->SetMovementMode(MOVE_Walking);
				//TODO : 스킬시전도 가능하게 넣어야할듯
			}
		}
	}
}

void UDamageComponent::ApplyInvincible(float Duration)
{
	if(b_Invincible)
		GetWorld()->GetTimerManager().ClearTimer(Timer_Invincible);
	b_Invincible = true;
	//TODO : 무적 작업 -> statComponent에서 따로 구현해야할듯
	GetWorld()->GetTimerManager().SetTimer(Timer_Invincible, this, &UDamageComponent::ClearInvincible, false);
}

void UDamageComponent::ClearInvincible()
{
	b_Invincible = false;
	//추후작업
}

void UDamageComponent::ApplyProvoke(float Duration)
{
	if (b_Provoke)
		GetWorld()->GetTimerManager().ClearTimer(Timer_Provoke);
	b_Provoke = true;
	//TODO : 도발 작업 -> 추후
	GetWorld()->GetTimerManager().SetTimer(Timer_Provoke, this, &UDamageComponent::ClearProvoke, false);
}

void UDamageComponent::ClearProvoke()
{
	b_Provoke = false;
	//추후작업
}

void UDamageComponent::ApplyProvoked(float Duration)
{
	if (b_Provoked)
		GetWorld()->GetTimerManager().ClearTimer(Timer_Provoked);
	b_Provoke = true;
	//TODO : 도발당한상태 작업 -> 추후
	GetWorld()->GetTimerManager().SetTimer(Timer_Provoked, this, &UDamageComponent::ClearProvoked, false);
}

void UDamageComponent::ClearProvoked()
{
	b_Provoked = false;
	//추후작업
}

void UDamageComponent::ApplySuperArmor(float Duration)
{
	if (b_SuperArmor)
		GetWorld()->GetTimerManager().ClearTimer(Timer_SuperArmor);
	b_SuperArmor = true;
	//TODO : 강직 작업 -> 추후
	GetWorld()->GetTimerManager().SetTimer(Timer_SuperArmor, this, &UDamageComponent::ClearSuperArmor, false);
}

void UDamageComponent::ClearSuperArmor()
{
	b_SuperArmor = false;
	//추후작업
}

void UDamageComponent::ApplyKnockBack(float Distance, const FVector& Dir)
{
	if (AGameCharacterBase* Char = Cast<AGameCharacterBase>(GetOwner()))
	{
		const float KnockTime = 0.2f;
		const FVector Vel = Dir.GetSafeNormal() * (Distance / KnockTime);
		Char->LaunchCharacter(Vel, true, false);
	}
}

void UDamageComponent::ApplyKnockFront(float Distance, AActor* Source)
{
	if (!Source) return;
	const FVector ToSource = Source->GetActorLocation() - GetOwner()->GetActorLocation();
	const FVector Dir = ToSource.GetSafeNormal();
	ApplyKnockBack(Distance, Dir);
}
