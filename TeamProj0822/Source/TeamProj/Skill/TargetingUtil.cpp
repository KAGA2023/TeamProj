// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetingUtil.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"
#define DEBUG_DRAW

void TargetingUtil::CollectActors_Sphere(AActor* Origin, float Radius, TArray<AActor*>& Out, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, const TArray<AActor*>& ActorsToIgnore)
{
    if (!Origin) return;
    FVector C = Origin->GetActorLocation();
    TArray<AActor*> Ignored = ActorsToIgnore; Ignored.Add(Origin);

    TArray<FHitResult> Hits;
    UKismetSystemLibrary::SphereTraceMultiForObjects(
        Origin, C, C, Radius, ObjectTypes, false, Ignored, EDrawDebugTrace::None, Hits, true);

    DrawDebugSphere(Origin->GetWorld(), C, Radius, 20, FColor::Blue, false, 10.f, 0, 2.f);

    for (const auto& H : Hits)
        if (AActor* A = H.GetActor()) Out.AddUnique(A);
}

void TargetingUtil::CollectActors_ConeForward(AActor* Origin, float Range, float Radius, float HalfAngleDeg, TArray<AActor*>& Out, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, const TArray<AActor*>& ActorsToIgnore)
{
    // 간단: 우선 구면 수집 후 발사체 기준 각도 필터링
    if (!Origin) return;
    UWorld* World = Origin->GetWorld();
    if (!World) return;
    TArray<AActor*> Candidates;
    CollectActors_Sphere(Origin, FMath::Max(Range, Radius), Candidates, ObjectTypes, ActorsToIgnore);

    const FVector O = Origin->GetActorLocation();
    const FVector Fwd = Origin->GetActorForwardVector();
    const float HalfRad = FMath::Cos(FMath::DegreesToRadians(FMath::Clamp(HalfAngleDeg,0.f,89.9f)));
    const float CosHalf = FMath::Cos(HalfRad);
    const float   MaxDistSq = FMath::Square(Range + Radius);

    DrawConeDebug(Origin, Range, HalfAngleDeg, FColor::Blue, 10.f, 2.f);


    for (AActor* A : Candidates)
    {
        if (!IsValid(A)) continue;

        const FVector ToTarget = A->GetActorLocation() - O;
        const float   DistSq = ToTarget.SizeSquared();
        if (DistSq > MaxDistSq) {/* DrawPointDebug(World, A->GetActorLocation(), false, 10.f);*/ continue; }

        const FVector Dir = ToTarget.GetSafeNormal();
        const float   Dot = FVector::DotProduct(Fwd, Dir); // cos(theta)

        const bool bInside = (Dot >= CosHalf);
        //DrawPointDebug(World, A->GetActorLocation(), bInside, 10.f);

        if (bInside)
        {
            Out.AddUnique(A);
        }
    }
}









void TargetingUtil::DrawConeDebug(AActor* Origin, float Range, float HalfAngleDeg, FColor Color, float LifeTime, float Thickness)
{
    if (!Origin) return;
    UWorld* World = Origin->GetWorld();
    if (!World) return;

    const FVector O = Origin->GetActorLocation();
    const FVector Fwd = Origin->GetActorForwardVector();

    const float HalfRad = FMath::DegreesToRadians(FMath::Clamp(HalfAngleDeg, 0.f, 89.9f));

    // 콘 표면
    DrawDebugCone(World, O, Fwd, Range, HalfRad, HalfRad, 32, Color, false, LifeTime, 0, Thickness);
    // 중심선
    DrawDebugLine(World, O, O + Fwd * Range, Color, false, LifeTime, 0, Thickness);
}

void TargetingUtil::DrawPointDebug(UWorld* World, const FVector& P, bool bHit, float LifeTime)
{
    DrawDebugSphere(World, P, 16.f, 12, bHit ? FColor::Green : FColor::Red, false, LifeTime, 0, 1.5f);
}

