// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Skill/SkillStruct.h"
#include "DamageComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAMPROJ_API UDamageComponent : public UActorComponent
{
	GENERATED_BODY()
	
private:
	bool b_Stunned = false;			//스턴
	bool b_Invincible = false;		//무적
	bool b_Provoke = false;			//도발
	bool b_Provoked = false;		//도발당함
	bool b_SuperArmor = false;		//강직(면역)

	FTimerHandle Timer_Stun;
	FTimerHandle Timer_Invincible;
	FTimerHandle Timer_Provoke;
	FTimerHandle Timer_Provoked;
	FTimerHandle Timer_SuperArmor;


	void ApplyStun(float Duration);
	void ClearStun();
	void ApplyInvincible(float Duration);
	void ClearInvincible();
	void ApplyProvoke(float Duration);
	void ClearProvoke();
	void ApplyProvoked(float Duration);
	void ClearProvoked();
	void ApplySuperArmor(float Duration);
	void ClearSuperArmor();
	
	void ApplyKnockBack(float Distance, const FVector& Dir);
	void ApplyKnockFront(float Distance, AActor* Source);

protected:
	TArray<FActiveBuff> ActiveBuffs;					//디버프
	TArray<FActiveStatusEffect> ActiveStatusEffects;	//상태이상


public:	
	UDamageComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;



protected:
	virtual void BeginPlay() override;

public:	
	// CC & StatusEffect
	UFUNCTION(BlueprintCallable)
	void CalculateDamage(AActor* pClass, float Amount);

	UFUNCTION(BlueprintCallable)
	void CalculateHeal(AActor* pClass, float Amount);

	UFUNCTION(BlueprintCallable)
	void AddBuff(const FSkillLevelData& pBuff);
	UFUNCTION(BlueprintCallable)
	void RemoveBuff(const FSkillLevelData& pBuff);
	UFUNCTION(BlueprintCallable)
	void AddStatusEffect(const FSkillEffectInfo& pStatusEffect);
	UFUNCTION(BlueprintCallable)
	void HandleExpiredStatusEffect(const FActiveStatusEffect& ExpiredEffect);
	UFUNCTION(BlueprintCallable)
	void ApplyDotDamage(const FActiveStatusEffect& Effect);

		
};
