// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameController.h"
#include "Character/GameCharacterBase.h"


void AMyGameController::BeginPlay()
{
	Super::BeginPlay();

	if (!IsLocalController()) return;
	
	if (ULocalPlayer* LP = GetLocalPlayer())
	{
		if (auto* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(IMC_Default, 0);
		}
	}
	bShowMouseCursor = true;

	if (MainWidgetClass)
	{
		MainWidget = CreateWidget<UUserWidget>(this, MainWidgetClass);
		if (MainWidget)
		{
			MainWidget->AddToViewport();

		}
	}

	HUD = GetHUD();
	SetIgnoreLookInput(true);
	FInputModeGameAndUI Mode;
	Mode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	Mode.SetHideCursorDuringCapture(false);                         
	SetInputMode(Mode);
}

void AMyGameController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (auto* EIC = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EIC->BindAction(IA_LMB, ETriggerEvent::Started, this, &AMyGameController::OnLMB_Pressed);
		EIC->BindAction(IA_LMB, ETriggerEvent::Completed, this, &AMyGameController::OnLMB_Released);
		EIC->BindAction(IA_RMB, ETriggerEvent::Started, this, &AMyGameController::OnRMB_Pressed);
		EIC->BindAction(IA_RMB, ETriggerEvent::Completed, this, &AMyGameController::OnRMB_Released);
		EIC->BindAction(IA_Wheel, ETriggerEvent::Triggered, this, &AMyGameController::OnWheelAxis_Triggered);
		EIC->BindAction(IA_WheelClick, ETriggerEvent::Started, this, &AMyGameController::OnWheelClick_Pressed);
		EIC->BindAction(IA_WheelClick, ETriggerEvent::Completed, this, &AMyGameController::OnWheelClick_Released);
		EIC->BindAction(IA_SPACEBAR, ETriggerEvent::Started, this, &AMyGameController::OnSPACEBAR_Pressed);
	}


}



void AMyGameController::OnLMB_Pressed()
{
	IsLMBPress = true;
	if (IWidgetInterface* IF = Cast<IWidgetInterface>(GetHUD()))
		IF->DragPressed_Implementation();
}



void AMyGameController::OnLMB_Released()
{
	IsLMBPress = false;
	if (IWidgetInterface* IF = Cast<IWidgetInterface>(GetHUD()))
		IF->DragReleased_Implementation();
}


void AMyGameController::OnRMB_Pressed()
{
	if (IWidgetInterface* IF = Cast<IWidgetInterface>(GetHUD()))
	{
		FHitResult Point;
		GetHitResultUnderCursorByChannel(TraceTypeQuery1, true, Point);
		IF->Move_Implementation(FVector2D(Point.ImpactPoint.X,Point.ImpactPoint.Y));
	}
}


void AMyGameController::OnRMB_Released()
{
}

void AMyGameController::OnWheelAxis_Triggered(const FInputActionInstance& Instance)
{
	float Value = Instance.GetValue().Get<float>();
	if (APlayerCharacter* CH = Cast<APlayerCharacter>(GetPawn()))
	{
		CH->ZoomInCamera(Value);
	}
}


void AMyGameController::OnWheelClick_Pressed()
{
}

void AMyGameController::OnWheelClick_Released()
{
}

void AMyGameController::OnSPACEBAR_Pressed()
{
	IsDetachCamera = !IsDetachCamera;
	
	if (APlayerCharacter* CH = Cast<APlayerCharacter>(GetPawn()))
	{
		if (IsDetachCamera)
			CH->DetachCamera();
		else
			CH->AttachCamera();
	}

}


AMyGameController::AMyGameController()
{
	IA_LMB = nullptr;
	IA_RMB = nullptr;
	IA_SPACEBAR = nullptr;
	IA_Wheel = nullptr;
	IA_WheelClick = nullptr;
	IMC_Default = nullptr;
	MainWidget = nullptr;
	MainWidgetClass = nullptr;
	IsLMBPress = false;
	HUD = nullptr;
	IsDetachCamera = false;

	PrimaryActorTick.bCanEverTick = true;
}

void AMyGameController::Tick(float DeltaTime)
{
	if (IsLMBPress)
	{
		if (IWidgetInterface* IF = Cast<IWidgetInterface>(GetHUD()))
		{
			IF->DragHold_Implementation();
		}
	}
}


void AMyGameController::OnPossess(APawn* pPawn)
{
	Super::OnPossess(pPawn);
}
