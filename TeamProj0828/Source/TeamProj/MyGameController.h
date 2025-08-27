// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "Blueprint/UserWidget.h"
#include "MyGameHUD.h"
#include "Components/DecalComponent.h"
#include "Interface/WidgetInterface.h"
#include "MyGameController.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJ_API AMyGameController : public APlayerController, public IWidgetInterface
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;


	//InputMapping
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")	UInputAction* IA_LMB;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")	UInputAction* IA_RMB;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")	UInputAction* IA_Wheel;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")	UInputAction* IA_WheelClick;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")	UInputAction* IA_SPACEBAR;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")	UInputAction* IA_Q;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")	UInputAction* IA_E;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")	UInputAction* IA_R;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")	UInputAction* IA_F;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")	UInputAction* IA_Tab;

	void OnLMB_Pressed();	void OnLMB_Released();
	void OnRMB_Pressed();	void OnRMB_Released();
	void OnQ_Pressed();	void OnE_Pressed(); void OnR_Pressed(); void OnF_Pressed(); void OnTab_Pressed();
	void OnWheelAxis_Triggered(const FInputActionInstance& Instance);
	void OnWheelClick_Pressed();void OnWheelClick_Released();
	bool IsLMBPress;
	void OnSPACEBAR_Pressed();
	bool IsDetachCamera;

	//Widget
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI") TSubclassOf<UUserWidget> MainWidgetClass;
	UPROPERTY()													UUserWidget* MainWidget;

	//HUD
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD") AHUD* HUD;
	
	//SkillTarget
	UFUNCTION(BlueprintCallable)
	void BeginTargeting(int32 SkillSlotIndex);
	UFUNCTION(BlueprintCallable)
	void EndTargeting(bool bApply);
	bool FinalizeAimForSkill();
	bool GetMouseAimLocation(FVector& OutLocation, FHitResult* OutHit = nullptr) const;

protected:
	void UpdateTargeting(float DeltaSeconds);
	float GetSkillRangeFromSlot(int32 SlotIndex) const;
	float GetSkillRadiusFromSlot(int32 SlotIndex) const;

private:
	bool  bTargeting = false;
	int32 PendingSkillIndex = INDEX_NONE;
	FVector CurrentAim = FVector::ZeroVector;

	// 미리보기 데칼
	UPROPERTY(EditDefaultsOnly, Category = "Targeting")
	UMaterialInterface* TargetDecalMaterial = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Targeting")
	float DefaultDecalThickness = 64.f; // 데칼 박스의 X(투영 두께)

	UPROPERTY(EditDefaultsOnly, Category = "Targeting")
	TEnumAsByte<ECollisionChannel> TargetTraceChannel = ECC_Visibility;

	UPROPERTY()
	UDecalComponent* TargetDecal = nullptr;

	// 범위/반경 클램프 옵션
	UPROPERTY(EditDefaultsOnly, Category = "Targeting")
	bool bClampToSkillRange = true;

	// 스냅 옵션(땅/표면 노말로 데칼 회전)
	UPROPERTY(EditDefaultsOnly, Category = "Targeting")
	bool bSnapToSurfaceNormal = true;

	
public:
	AMyGameController();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* IMC_Default;
	
	virtual void OnPossess(APawn* pPawn) override;
};
