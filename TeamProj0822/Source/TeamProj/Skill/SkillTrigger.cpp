// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillTrigger.h"
#include "../Character/GameCharacterBase.h"

UAnimInstance* USkillTriggerStrategy::GetAnim(const FSkillContext& Ctx) const
{
	if (!Ctx.Caster) return nullptr;
	AGameCharacterBase* CB = Cast<AGameCharacterBase>(Ctx.Caster);
	if (!CB) return nullptr;

	UAnimInstance* GetAnim = Cast<UAnimInstance>(CB->GetMesh()->GetAnimInstance());
	if (!GetAnim) return nullptr;

	return GetAnim;
}

float USkillTriggerStrategy::PlayMontageIfAny(const FSkillContext& Ctx, UAnimMontage* Montage, float InPlayRate, FName StartSection) const
{
	if (!Montage) return 0.f;
	if (UAnimInstance* Anim = GetAnim(Ctx))
	{
		const float Len = Anim->Montage_Play(Montage, InPlayRate);
		if (Len > 0.f && StartSection != NAME_None)
			Anim->Montage_JumpToSection(StartSection, Montage);
		return Len;
	}
	return 0.0f;
}

void USkillTriggerStrategy::StopMontageIfAny(const FSkillContext& Ctx, UAnimMontage* Montage, float BlendOut) const
{
	if (!Montage) return;
	if (UAnimInstance* Anim = GetAnim(Ctx))
		Anim->Montage_Stop(BlendOut, Montage);
}







void USkillTriggerStrategy::ExecuteTrigger(const FSkillContext& Ctx,  FOnFireWithCtx* OnFire)
{
	//디폴트는 스킬 즉발
	UE_LOG(LogTemp, Warning, TEXT("Default Trigger Start"));
	if (OnFire->IsBound())
		OnFire->Execute(Ctx);
}



void USkillEffect::Apply(const FSkillContext& Ctx, AActor* Target) 
{
	UE_LOG(LogTemp, Warning, TEXT("Default Effect Start"));
	
}
