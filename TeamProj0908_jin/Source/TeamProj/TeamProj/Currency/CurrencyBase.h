// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "CurrencyBase.generated.h"

UCLASS()
class TEAMPROJ_API ACurrencyBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACurrencyBase();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 디버그용으로 폰 가져오기
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
	APawn* player;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* Root;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* SphCom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* Mesh;


	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnGet() {};
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
