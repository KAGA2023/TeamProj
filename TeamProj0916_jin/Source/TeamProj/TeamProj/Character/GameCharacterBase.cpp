// Fill out your copyright notice in the Description page of Project Settings.


#include "GameCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFrameWork/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "../AICon/AllyAIController.h"
#include "../Status/StatusComponent.h"
#include "../Skill/SkillComponent.h"
#include "../Damage/DamageComponent.h"
#include "../Item/ItemComponent.h" //추가
#include "../Widget/HealthBarWidget.h"
#include "../MyGameController.h"


AGameCharacterBase::AGameCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	

	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -90.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	//Capsule->SetCapsuleSize(35.f, 90.f);
	//capCom->SetCollisionProfileName(TEXT("PlayerPreset"));
	
	Parts.Add(CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Head")));
	Parts[0]->SetupAttachment(GetMesh());
	Parts.Add(CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hair")));
	Parts[1]->SetupAttachment(GetMesh());
	Parts.Add(CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Face")));
	Parts[2]->SetupAttachment(GetMesh());
	Parts.Add(CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Shoulder")));
	Parts[3]->SetupAttachment(GetMesh());
	Parts.Add(CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hand")));
	Parts[4]->SetupAttachment(GetMesh());
	Parts.Add(CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Leg")));
	Parts[5]->SetupAttachment(GetMesh());
	Parts.Add(CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Foot")));
	Parts[6]->SetupAttachment(GetMesh());

	HealthScene= CreateDefaultSubobject<USceneComponent>(TEXT("HealthScene"));
	HealthScene->SetupAttachment(RootComponent);
	HealthScene->SetRelativeLocation(FVector(0.f, -20.f, 250.f));


	HealthWidget=CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthWidget"));
	HealthWidget->SetupAttachment(HealthScene);
	HealthWidget->SetWidgetSpace(EWidgetSpace::Screen);
	HealthWidget->SetDrawSize(FVector2D(200.f, 60.f));

	WeaponLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponLeft"));
	WeaponLeft->SetupAttachment(GetMesh());
	WeaponRight= CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponRight"));
	WeaponRight->SetupAttachment(GetMesh());

	StatusComp = CreateDefaultSubobject<UStatusComponent>(TEXT("StatusComp"));
	SkillComp = CreateDefaultSubobject<USkillComponent>(TEXT("SkillComp"));
	DamageComp = CreateDefaultSubobject<UDamageComponent>(TEXT("DamageComp"));
	ItemComp = CreateDefaultSubobject<UItemComponent>(TEXT("ItemComp")); //추가
	

}

// Called when the game starts or when spawned
void AGameCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	USkeletalMeshComponent* Leader = GetMesh();

	for (USkeletalMeshComponent* pt : Parts)
	{
		if (pt && Leader)
			pt->SetLeaderPoseComponent(Leader);
	}
}

// Called every frame
void AGameCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGameCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void AGameCharacterBase::InitCharInfo(EClassType ClassType, FClassConfig StatID, TArray<FSkillConfig> SkillIDs,int32 SkillLevel, USkeletalMesh* pChest, TArray<USkeletalMesh*> pParts, UStaticMesh* pWeaponLeft, UStaticMesh* pWeaponRight, TSubclassOf<UAnimInstance> pAnimInstance, FVector pCharScale)
{
	StatusComp->Init(ClassType, StatID);
	SkillComp->InitSkill(SkillIDs, SkillLevel);
	ItemComp->InitAllItems(); //추가
	AttachParts(pChest, pParts);
	AttachWeapon(pWeaponLeft, pWeaponRight);
	GetMesh()->SetRelativeScale3D(pCharScale);

	GetMesh()->SetAnimInstanceClass(pAnimInstance);
	
}



void AGameCharacterBase::AttachParts(USkeletalMesh* pChest, TArray<USkeletalMesh*> pParts) const
{
	GetMesh()->SetSkeletalMesh(pChest);
	int32 i = 0;
	for (auto pt : pParts)
	{
		if (!pt)
		{
			++i;
			continue;
		}


		Parts[i++]->SetSkeletalMesh(pt);
	}
}

void AGameCharacterBase::AttachWeapon(UStaticMesh* pWeaponLeft, UStaticMesh* pWeaponRight) const
{
	USkeletalMeshComponent* Chest = GetMesh();
	if (Chest && pWeaponLeft)
	{
		const FName SocketName(TEXT("Weapon_l"));

		if (Chest->DoesSocketExist(SocketName))
		{
			WeaponLeft->AttachToComponent(Chest, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
			WeaponLeft->SetStaticMesh(pWeaponLeft);
		}
	}
	if (Chest && pWeaponRight)
	{
		const FName SocketName(TEXT("Weapon_r"));

		if (Chest->DoesSocketExist(SocketName))
		{
			WeaponRight->AttachToComponent(Chest, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
			WeaponRight->SetStaticMesh(pWeaponRight);
		}
	}
}




///////////////////////Player//////////////////////////////////////////////////

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	IsDetach = false;
	DesiredArmLength = 2000.f;
	CameraSpeed = 4000.f;
	MinArmLength = 1000.f;
	MaxArmLength = 3000.f;
	Dragon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Dragon"));
	Dragon->SetupAttachment(GetMesh());
	Dragon->SetRelativeScale3D(FVector(2.f, 2.f, 2.f));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = DesiredArmLength;
	SpringArm->SetRelativeRotation(FRotator(-50.f,0.f,0.f));
	SpringArm->bEnableCameraLag= true;
	SpringArm->CameraLagSpeed = 10.f;
	SpringArm->bDoCollisionTest = false;
	SpringArm->bInheritYaw = false;
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	Camera->SetupAttachment(SpringArm);
	
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->bOrientRotationToMovement = true;
	}

	Team = ETeam::Player;
	
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

}



void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (SpringArm)
	{
		const float Cur = SpringArm->TargetArmLength;
		const float NewLen = FMath::FInterpTo(Cur, DesiredArmLength, DeltaTime, 10.f);
		SpringArm->TargetArmLength = NewLen;
	}

	DirectionCameraMove(DeltaTime);

}

void APlayerCharacter::InitCharInfo(EClassType ClassType, FClassConfig StatID, TArray<FSkillConfig> SkillIDs, int32 SkillLevel, USkeletalMesh* pChest, TArray<USkeletalMesh*> pParts, UStaticMesh* pWeaponLeft, UStaticMesh* pWeaponRight, TSubclassOf<UAnimInstance> pAnimInstance, FVector CharScale)
{
	Super::InitCharInfo(ClassType, StatID, SkillIDs, SkillLevel, pChest, pParts, pWeaponLeft, pWeaponRight, pAnimInstance, CharScale);

	USkeletalMeshComponent* DragonRiding = GetMesh();
	if (!GetMesh()) return;

	if (DragonRiding)
	{
		const FName SocketName(TEXT("Dragon"));

		if (DragonRiding->DoesSocketExist(SocketName))
		{
			Dragon->AttachToComponent(DragonRiding, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
		}
	}


}



void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	
}

void APlayerCharacter::DetachCamera()
{
	IsDetach = true;
	SpringArm->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	SpringArm->SetRelativeRotation(FRotator(-50.f, 0.f, 0.f));
}

void APlayerCharacter::AttachCamera()
{
	IsDetach = false;
	SpringArm->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	SpringArm->SetRelativeLocation(FVector(0.f, 0.f, 0.f),false,nullptr);
	SpringArm->SetRelativeRotation(FRotator(-50.f, 0.f, 0.f));
}

void APlayerCharacter::ZoomInCamera(float v)
{
	if (FMath::IsNearlyZero(v)) return;

	DesiredArmLength = FMath::Clamp(DesiredArmLength + (-v) * 100.f,MinArmLength, MaxArmLength);
}

void APlayerCharacter::DirectionCameraMove(float DeltaTime)
{
	float X{};
	float Y{};

	int32 ViewX{};
	int32 ViewY{};

	AMyGameController* Cont = Cast<AMyGameController>(GetController());
	Cont->GetMousePosition(X, Y);
	Cont->GetViewportSize(ViewX, ViewY);

	if (Y <= 150.f) CameraMove(ECameraMove::Up,DeltaTime);
	if (X <= 150.f)CameraMove(ECameraMove::Left, DeltaTime);
	if (Y >= (float)ViewY - 150.f)CameraMove(ECameraMove::Down, DeltaTime);
	if (X >= (float)ViewX - 150.f)CameraMove(ECameraMove::Right, DeltaTime);

}


void APlayerCharacter::CameraMove(ECameraMove CameraWay,float DeltaTime)
{
	if (!IsDetach) return;
	float MoveSpeed = CameraSpeed * DeltaTime;
	switch (CameraWay)
	{
	case ECameraMove::Up:
		SpringArm->AddRelativeLocation(FVector(MoveSpeed, 0.f, 0.f), false, nullptr);
		break;
	case ECameraMove::Down:
		SpringArm->AddRelativeLocation(FVector(MoveSpeed*-1.f, 0.f, 0.f), false, nullptr);
		break;
	case ECameraMove::Left:
		SpringArm->AddRelativeLocation(FVector(0.f, MoveSpeed * -1.f, 0.f), false, nullptr);
		break;
	case ECameraMove::Right:
		SpringArm->AddRelativeLocation(FVector(0.f, MoveSpeed, 0.f), false, nullptr);
		break;
	}
}




///////////////////////Enemy//////////////////////////////////////////////////



AEnemyCharacter::AEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	Team = ETeam::Enemy;
}

void AEnemyCharacter::BeginPlay()
{

	Super::BeginPlay();


}

void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}



///////////////////////Ally//////////////////////////////////////////////////


AAllyCharacter::AAllyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	Team = ETeam::Ally;
}

void AAllyCharacter::BeginPlay()
{
	Super::BeginPlay();


}


void AAllyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAllyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
void AAllyCharacter::SelectUnit_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("Selected!! : %s"), *this->GetName());
}
void AAllyCharacter::DeSelectUnit_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("DeSelected!! : %s"), *this->GetName());
}

void AAllyCharacter::Move_Implementation(FVector2D movePos)
{
	
	if (AAllyAIController* Con = Cast<AAllyAIController>(GetController()))
	{
		UE_LOG(LogTemp, Log, TEXT("Controller : %s"), *this->GetController()->GetName());
		Con->MoveFromMousePick(movePos);
	}
	
	
	
}




///////////////////////Boss//////////////////////////////////////////////////



ABossCharacter::ABossCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	Team = ETeam::Enemy;
}

void ABossCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABossCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABossCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
