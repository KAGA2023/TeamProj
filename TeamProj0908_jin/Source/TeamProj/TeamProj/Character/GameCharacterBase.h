// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Status/StatusStruct.h"
#include "../Skill/SkillStruct.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "Components/SceneComponent.h"
#include "../AICon/GameUnitBase.h"
#include "../Interface/WidgetInterface.h"
#include "GenericTeamAgentInterface.h"
#include "GameCharacterBase.generated.h"

class UStatusComponent;
class USkillComponent;
class UDamageComponent;
class UCapsuleComponent;
class USpringArmComponent;
class UCameraComponent;
class UItemComponent;  //추가

UENUM()
enum class ECameraMove : uint8
{
	Up,
	Down,
	Left,
	Right
};

UCLASS()
class TEAMPROJ_API AGameCharacterBase : public ACharacter,  public IGenericTeamAgentInterface
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)  //추가
	UItemComponent* ItemComp;  //추가가
	
public:
	AGameCharacterBase();


protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	void AttachWeapon(UStaticMesh* pWeaponLeft, UStaticMesh* pWeaponRight) const;

	void AttachParts(USkeletalMesh* pChest, TArray<USkeletalMesh*> Parts) const;

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "e-InitCharcter"))
	virtual void InitCharInfo(EClassType ClassType, FClassConfig StatID, TArray<FSkillConfig> SkillIDs, int32 SkillLevel, 
		USkeletalMesh* pChest, TArray<USkeletalMesh*> pParts, UStaticMesh* pWeaponLeft, UStaticMesh* pWeaponRight, TSubclassOf<UAnimInstance> pAnimInstance, FVector CharScale);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Team")
	ETeam Team = ETeam::Player;

	virtual FGenericTeamId GetGenericTeamId() const override
	{
		return FGenericTeamId(static_cast<uint8>(Team));
	}

	virtual void SetGenericTeamId(const FGenericTeamId& InTeamID) override
	{
		Team = static_cast<ETeam>(InTeamID.GetId());
	}
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

	//���� + ������������
	float DesiredArmLength;
	UPROPERTY(EditAnywhere, Category = "Zoom")
	float MinArmLength;
	UPROPERTY(EditAnywhere, Category = "Zoom")
	float MaxArmLength;


	//������ũ�� ȭ�� �����̿� ���콺���� �̵�����������



public:
	APlayerCharacter();


protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void InitCharInfo(EClassType ClassType, FClassConfig StatID, TArray<FSkillConfig> SkillIDs, int32 SkillLevel,
		USkeletalMesh* pChest, TArray<USkeletalMesh*> pParts, UStaticMesh* pWeaponLeft, UStaticMesh* pWeaponRight, TSubclassOf<UAnimInstance> pAnimInstance, FVector CharScale) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//ī�޶� �̵� + ����
	bool IsDetach;
	void DetachCamera();
	void AttachCamera();
	void ZoomInCamera(float v);

	//��ũ�� ���� ��ũ��
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
class TEAMPROJ_API AAllyCharacter : public AGameCharacterBase, public IWidgetInterface
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

	virtual void SelectUnit_Implementation();
	virtual void DeSelectUnit_Implementation();
	virtual void Move_Implementation(FVector2D movePos);

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