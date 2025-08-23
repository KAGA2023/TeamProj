// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SkillTrigger.h"
#include "TargetingUtil.h"
#include "TargetingUtil.h"
#include "../Damage/DamageComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Targeting.generated.h"

/**
 * 
 */
UCLASS(EditInlineNew)
class TEAMPROJ_API UTargeting_ConeForward : public UTargetingTask
{
	GENERATED_BODY()
	
public:
	UTargeting_ConeForward()
	{
		// 기본적으로 Pawn은 잡히게
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
		// 건물도 잡고 싶으면 필요에 맞게 추가
		// ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));
		// ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
	}



	UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	
	virtual void ResolveTargets(const FSkillContext& Ctx, TArray<AActor*>& OutTargets) override
	{
		const float Range = Ctx.Data->Range + (Ctx.Data->SkillLevels.IsValidIndex(Ctx.SkillLevel) ? Ctx.Data->SkillLevels[Ctx.SkillLevel].AddRange : 0.f);
		const float Radius = Ctx.Data->Radius + (Ctx.Data->SkillLevels.IsValidIndex(Ctx.SkillLevel) ? Ctx.Data->SkillLevels[Ctx.SkillLevel].AddRadius : 0.f);
		const float HalfAngle = Ctx.Data->ConeHalfAngle + (Ctx.Data->SkillLevels.IsValidIndex(Ctx.SkillLevel) ? Ctx.Data->SkillLevels[Ctx.SkillLevel].AddConeHalfAngle : 0.f);


		UE_LOG(LogTemp, Warning, TEXT("[ConeForward] Caster=%s Range=%.1f Radius=%.1f HalfAngle=%.1f Affiliation=%d"),
			*GetNameSafe(Ctx.Caster), Range, Radius, HalfAngle, (int32)Affiliation);

		if (ObjectTypes.Num() == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("[ConeForward] ObjectTypes is EMPTY! (이러면 SphereOverlap에서 'Invalid object types')"));
		}
		else
		{
			for (int32 i = 0; i < ObjectTypes.Num(); ++i)
			{
				const ECollisionChannel CC = UEngineTypes::ConvertToCollisionChannel(ObjectTypes[i]);
				UE_LOG(LogTemp, Warning, TEXT("[ConeForward] ObjectTypes[%d]=%d  (CollisionChannel=%d)"),
					i, (int32)ObjectTypes[i], (int32)CC);
			}
		}


		TArray<AActor*> Temp;
		TargetingUtil::CollectActors_ConeForward(Ctx.Caster, Range, Radius, HalfAngle, Temp, ObjectTypes);

		UE_LOG(LogTemp, Warning, TEXT("[ConeForward] Candidates before team filter: %d"), Temp.Num());

		OutTargets.Reset();
		for (AActor* A : Temp)
		{
			if (!IsValid(A)) continue;
			if (TeamFilterUtil::PassAffiliation(Ctx.Caster, A, Affiliation))
				OutTargets.AddUnique(A);
		}
		UE_LOG(LogTemp, Warning, TEXT("[ConeForward] Final targets: %d"), OutTargets.Num());
	}
};

UCLASS(EditInlineNew)
class UTargeting_Self : public UTargetingTask
{
	GENERATED_BODY()
public:
	UTargeting_Self() { Affiliation = ETargetAffiliation::SelfOnly; }

	virtual void ResolveTargets(const FSkillContext& Ctx, TArray<AActor*>& OutTargets) override
	{
		OutTargets.Reset();
		if (IsValid(Ctx.Caster))
			OutTargets.Add(Ctx.Caster);
	}
};


UCLASS(EditInlineNew)
class TEAMPROJ_API UTargeting_SelfRadius : public UTargetingTask
{
	GENERATED_BODY()
	
public:
	UTargeting_SelfRadius()
	{
		// 기본적으로 Pawn은 잡히게
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
		// 건물도 잡고 싶으면 필요에 맞게 추가
		// ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));
		// ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	
	virtual void ResolveTargets(const FSkillContext& Ctx, TArray<AActor*>& OutTargets) override
	{
		if (!Ctx.Caster || !Ctx.Data) return;
		UE_LOG(LogTemp, Warning, TEXT("SelfRadius Targeting Start"));
		const float Radius = Ctx.Data->Radius + (Ctx.Data->SkillLevels.IsValidIndex(Ctx.SkillLevel) ? Ctx.Data->SkillLevels[Ctx.SkillLevel].AddRadius : 0.f);

		TargetingUtil::CollectActors_Sphere(Ctx.Caster, Radius, OutTargets, ObjectTypes);
	}
};



UCLASS(EditInlineNew)
class TEAMPROJ_API UTargeting_PointRadius : public UTargetingTask
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;

	virtual void ResolveTargets(const FSkillContext& Ctx, TArray<AActor*>& OutTargets) override
	{
		if (!Ctx.Caster || !Ctx.Data) return;
		UE_LOG(LogTemp, Warning, TEXT("PointRadius Targeting Start"));
		const float Radius = Ctx.Data->Radius + (Ctx.Data->SkillLevels.IsValidIndex(Ctx.SkillLevel) ? Ctx.Data->SkillLevels[Ctx.SkillLevel].AddRadius : 0.f);

		TArray<AActor*> Ignored; Ignored.Add(Ctx.Caster);

		TArray<FHitResult> Hits;
		UKismetSystemLibrary::SphereTraceMultiForObjects( Ctx.Caster, Ctx.OptionalLocation, Ctx.OptionalLocation, Radius, ObjectTypes,
			false, Ignored, EDrawDebugTrace::None, Hits, true);

		for (const auto& H : Hits)
			if (AActor* A = H.GetActor()) OutTargets.AddUnique(A);
	}
};