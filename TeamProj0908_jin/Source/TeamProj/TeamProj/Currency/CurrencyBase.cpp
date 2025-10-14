// Fill out your copyright notice in the Description page of Project Settings.


#include "CurrencyBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACurrencyBase::ACurrencyBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(Root);
	SphCom = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphCom->SetupAttachment(Root);
	SphCom->AddRelativeLocation(FVector(0.f, 0.f, 35.f));
	SphCom->SetSphereRadius(35.f);
	// 콜리전 프리셋 설정 해야함. 공중에 떠있음.
	SphCom->SetCollisionProfileName(TEXT("Trigger")); //오버랩 채널 만들어야할듯.
	
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	Mesh->SetupAttachment(SphCom);
}

// Called when the game starts or when spawned
void ACurrencyBase::BeginPlay()
{
	Super::BeginPlay();
	SphCom->SetGenerateOverlapEvents(true);//마지막 줄이 안전함.
	player = Cast<APawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	SphCom->OnComponentBeginOverlap.AddDynamic(this, &ACurrencyBase::OnOverlapBegin);
	UE_LOG(LogTemp, Warning, TEXT("Binding try"));
}

void ACurrencyBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor == player)
	{
		UE_LOG(LogTemp, Warning, TEXT("Begin Overlap with Player: %s"), *OtherActor->GetName());
		OnGet();
	}
}

// Called every frame
void ACurrencyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

