// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Status/StatusStruct.h"
#include "../Skill/SkillStruct.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "Components/SceneComponent.h"
#include "GameCharacterBase.generated.h"

class UStatusComponent;
class USkillComponent;
class UDamageComponent;
class UCapsuleComponent;
class USpringArmComponent;
class UCameraComponent;

UENUM()
enum class ECameraMove : uint8
{
	Up,
	Down,
	Left,
	Right
};

UCLASS()
class TEAMPROJ_API AGameCharacterBase : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* WeaponLeft;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* WeaponRight;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<USkeletalMeshComponent*> Parts;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* HealthScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UWidgetComponent* HealthWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStatusComponent* StatusComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USkillComponent* SkillComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UDamageComponent* DamageComp;

public:
	// Sets default values for this character's properties
	AGameCharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void AttachWeapon(UStaticMesh* pWeaponLeft, UStaticMesh* pWeaponRight) const;

	void AttachParts(USkeletalMesh* pChest, TArray<USkeletalMesh*> Parts) const;

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "e-InitCharcter"))
	virtual void InitCharInfo(EClassType ClassType, FClassConfig StatID, TArray<FSkillConfig> SkillIDs, int32 SkillLevel, 
		USkeletalMesh* pChest, TArray<USkeletalMesh*> pParts, UStaticMesh* pWeaponLeft, UStaticMesh* pWeaponRight, TSubclassOf<UAnimInstance> pAnimInstance, FVector CharScale);
};



UCLASS()
class TEAMPROJ_API APlayerCharacter : public AGameCharacterBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* Dragon;
	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent* SpringArm{};
	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* Camera{};

	//줌인 + 자유시점ㅇㅇ
	float DesiredArmLength;
	UPROPERTY(EditAnywhere, Category = "Zoom")
	float MinArmLength;
	UPROPERTY(EditAnywhere, Category = "Zoom")
	float MaxArmLength;


	//엣지스크롤 화면 가생이에 마우스가면 이도오오오오옹



public:
	APlayerCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void InitCharInfo(EClassType ClassType, FClassConfig StatID, TArray<FSkillConfig> SkillIDs, int32 SkillLevel,
		USkeletalMesh* pChest, TArray<USkeletalMesh*> pParts, UStaticMesh* pWeaponLeft, UStaticMesh* pWeaponRight, TSubclassOf<UAnimInstance> pAnimInstance, FVector CharScale) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//카메라 이동 + 줌인
	bool IsDetach;
	void DetachCamera();
	void AttachCamera();
	void ZoomInCamera(float v);

	//스크린 엣지 스크롤
	float CameraSpeed;
	void DirectionCameraMove(float DeltaTime);
	void CameraMove(ECameraMove CameraWay,float DeltaTime);

};



UCLASS()
class TEAMPROJ_API AEnemyCharacter : public AGameCharacterBase
{
	GENERATED_BODY()

	protected:

public:
	AEnemyCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};


UCLASS()
class TEAMPROJ_API AAllyCharacter : public AGameCharacterBase
{
	GENERATED_BODY()

	protected:

public:
	AAllyCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};



UCLASS()
class TEAMPROJ_API ABossCharacter : public AGameCharacterBase
{
	GENERATED_BODY()

	protected:

public:
	ABossCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};