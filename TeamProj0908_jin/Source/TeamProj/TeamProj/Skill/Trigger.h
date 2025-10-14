// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TimerManager.h"
#include "SkillTrigger.h"
#include "SkillStruct.h"
#include "Trigger.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJ_API UTrigger_Immediate : public USkillTriggerStrategy
{
	GENERATED_BODY()
	
public:
	virtual void ExecuteTrigger(const FSkillContext& Ctx,  FOnFireWithCtx* OnFire) override
	{
		if (Ctx.Data && Ctx.Data->Animation) PlayMontageIfAny(Ctx, Ctx.Data->Animation,1.f);
		
		if (OnFire && OnFire->IsBound()) OnFire->Execute(Ctx);

		// 즉발은 곧바로 종료 = 쿨다운 시작
		if (USkill* OwnerSkill = Cast<USkill>(GetOuter()))
		{
			OwnerSkill->CoolStart();
		}
	}
};


UCLASS()
class TEAMPROJ_API UTrigger_CastTime : public USkillTriggerStrategy
{
	GENERATED_BODY()

protected:
	//현재 진행중인 캐스팅 정보 저장 = 캐스팅 도중 CC기 맞을때 끊기
	FSkillContext CurrentCtx;
	FOnFireWithCtx CurrentOnFire;

	FTimerHandle CastTimeHandle;
	bool bCasting = false;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bUseSkillDataCasting = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float ExtraCastTime = 0.f;
	
	UFUNCTION(BlueprintCallable) void CancelCast() //CC기 함수 끝에 이거 부르면 댐님덤
	{
		if (CurrentCtx.Caster && CurrentCtx.Caster->GetWorld())
		{
			CurrentCtx.Caster->GetWorld()->GetTimerManager().ClearTimer(CastTimeHandle);
		}

		if (CurrentCtx.Data) StopMontageIfAny(CurrentCtx, CurrentCtx.Data->Animation);

		bCasting = false;
	}


	virtual void ExecuteTrigger(const FSkillContext& Ctx,  FOnFireWithCtx* OnFire) override
	{
		if (!Ctx.Caster) { if (OnFire->IsBound()) OnFire->Execute(Ctx); return; }

		if (USkill* OwnerSkill = Cast<USkill>(GetOuter()))
		{
			OwnerSkill->CoolStart();
		}
		float BaseCast = 0.f;
		if (bUseSkillDataCasting && Ctx.Data)
		{
			const int32 LV = Ctx.SkillLevel;
			const float LevelRec = (Ctx.Data->SkillLevels.IsValidIndex(LV) ? Ctx.Data->SkillLevels[LV].CastingTimeRec : 0.f);
			BaseCast = Ctx.Data->CastingTime - LevelRec;
		}


		const float CastTime = FMath::Max(0.f, BaseCast + ExtraCastTime);

		//부동소수점 연산 오차보정하기위해 쓰는 매크로 ㄷㄷ 뭔가멋있ㄸ
		if (CastTime <= KINDA_SMALL_NUMBER) { if (OnFire->IsBound()) OnFire->Execute(Ctx); return; }

		
		CurrentCtx = Ctx;
		CurrentOnFire = *OnFire;
		bCasting = true;

		if (Ctx.Data && Ctx.Data->Animation)
			PlayMontageIfAny(Ctx, Ctx.Data->Animation);

			if (UWorld* World = Ctx.Caster->GetWorld())
			{																//시전 도중 객체파괴 자동콜백가ㅇ능
				World->GetTimerManager().SetTimer(CastTimeHandle, FTimerDelegate::CreateWeakLambda(this, [this]()
					{
						if (bCasting && CurrentOnFire.IsBound())
						{
							CurrentOnFire.Execute(CurrentCtx);
						}
						bCasting = false;
						if (CurrentCtx.Data) StopMontageIfAny(CurrentCtx, CurrentCtx.Data->Animation);
						
					}),
				CastTime, false);
			}
	};



};

