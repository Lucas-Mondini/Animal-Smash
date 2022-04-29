// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ActorComponent/ActorCombatComponent.h"

#include "Character/ALSBaseCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"

// Sets default values for this component's properties
UActorCombatComponent::UActorCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	State = EACCStateMachine::Default;

	//Punch animations
	ConstructorHelpers::FObjectFinder<UAnimMontage> Jab_L(TEXT("AnimMontage'/Game/AnimalSmash/Assets/Anim/KB_m_Jab_L_Montage.KB_m_Jab_L_Montage'"));
	if (Jab_L.Succeeded()) {
		PunchComboAnimations.Add(Jab_L.Object);
	}
	ConstructorHelpers::FObjectFinder<UAnimMontage> Jab_R(TEXT("AnimMontage'/Game/AnimalSmash/Assets/Anim/KB_m_Jab_R_Montage.KB_m_Jab_R_Montage'"));
	if (Jab_R.Succeeded()) {
		PunchComboAnimations.Add(Jab_R.Object);
	}
	ConstructorHelpers::FObjectFinder<UAnimMontage> Uppercut_L(TEXT("AnimMontage'/Game/AnimalSmash/Assets/Anim/KB_m_Uppercut_L_Montage.KB_m_Uppercut_L_Montage'"));
	if (Uppercut_L.Succeeded()) {
		PunchComboAnimations.Add(Uppercut_L.Object);
	}
	ConstructorHelpers::FObjectFinder<UAnimMontage> Superpunch(TEXT("AnimMontage'/Game/AnimalSmash/Assets/Anim/KB_Superpunch_Montage.KB_Superpunch_Montage'"));
	if (Superpunch.Succeeded()) {
		PunchComboAnimations.Add(Superpunch.Object);
	}
	
	//Kick animations
	ConstructorHelpers::FObjectFinder<UAnimMontage> NinjaCutDownKick(TEXT("AnimMontage'/Game/AnimalSmash/Assets/Anim/NinjaCutDownKick_Retargeted_Montage.NinjaCutDownKick_Retargeted_Montage'"));
	if (NinjaCutDownKick.Succeeded()) {
		KickComboAnimations.Add(NinjaCutDownKick.Object);
	}
	ConstructorHelpers::FObjectFinder<UAnimMontage> NinjaLowRightThaiKick(TEXT("AnimMontage'/Game/AnimalSmash/Assets/Anim/NinjaLowRightThaiKick_Retargeted_Montage.NinjaLowRightThaiKick_Retargeted_Montage'"));
	if (NinjaLowRightThaiKick.Succeeded()) {
		KickComboAnimations.Add(NinjaLowRightThaiKick.Object);
	}
	
	ConstructorHelpers::FObjectFinder<UAnimMontage> NinjaStraightKick(TEXT("AnimMontage'/Game/AnimalSmash/Assets/Anim/NinjaStraightKick_Retargeted_Montage.NinjaStraightKick_Retargeted_Montage'"));
	if (NinjaStraightKick.Succeeded()) {
		KickComboAnimations.Add(NinjaStraightKick.Object);
	}
	
	ConstructorHelpers::FObjectFinder<UAnimMontage> NinjaWheelKick(TEXT("AnimMontage'/Game/AnimalSmash/Assets/Anim/NinjaWheelKick_Retargeted_Montage.NinjaWheelKick_Retargeted_Montage'"));
	if (NinjaWheelKick.Succeeded()) {
		KickComboAnimations.Add(NinjaWheelKick.Object);
	}
	
	Attack2ComboLength = KickComboAnimations.Num();
	
}

void UActorCombatComponent::Attack1() {
	if(OwnerCharacter->GetMovementState() != EALSMovementState::InAir && CanAttack) {
		CanAttack = false;
		Attack1ComboLength = PunchComboAnimations.Num();
		UAnimMontage *anim = PunchComboAnimations[Attack1ComboCount++ % Attack1ComboLength];
		OwnerCharacter->Replicated_PlayMontage(anim, PunchAnimVelocity);
		Attack1ComboAccumulator = 0;
		AnimAccumulator = 0;
		AnimTime = anim->GetPlayLength() * PunchAnimOffsetVelocity;
		State = EACCStateMachine::Punching; 
	}
}

void UActorCombatComponent::Attack2() {
	if(OwnerCharacter->GetMovementState() != EALSMovementState::InAir && CanAttack) {
		CanAttack = false;
		Attack2ComboLength = KickComboAnimations.Num();
		UAnimMontage *anim = KickComboAnimations[Attack2ComboCount++ % Attack2ComboLength];
		OwnerCharacter->Replicated_PlayMontage(anim, KickAnimVelocity);
		Attack2ComboAccumulator = 0;
		AnimAccumulator = 0;
		AnimTime = anim->GetPlayLength() * KickAnimOffsetVelocity;
		State = EACCStateMachine::Kicking; 
	}
}


// Called when the game starts
void UActorCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<AALSBaseCharacter>(GetOwner());
	
}


void UActorCombatComponent::PunchOverlapProcess(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if(Cast<AActor>(OtherActor) && State == EACCStateMachine::Punching && !ActorHitted.Contains(OtherActor) && OtherActor != GetOwner())
		HitProcess(OtherActor);
}

void UActorCombatComponent::KickOverlapProcess(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if(Cast<AActor>(OtherActor) && State == EACCStateMachine::Kicking && !ActorHitted.Contains(OtherActor) && OtherActor != GetOwner())
		HitProcess(OtherActor);
}

void UActorCombatComponent::HitProcess(AActor* OtherActor) {
	if(!ActorHitted.Contains(OtherActor)) {
		ActorHitted.Add(OtherActor);
		GLog->Log("Hitting " + OtherActor->GetName());
	}
}


// Called every frame
void UActorCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(!CanAttack) 
		AnimAccumulator += DeltaTime;
	
		Attack2ComboAccumulator += DeltaTime;
		Attack1ComboAccumulator += DeltaTime;

	if(Attack2ComboAccumulator >= ComboTime) {
		Attack2ComboCount = 0;
		Attack2ComboAccumulator = 0;
	}
	if(Attack1ComboAccumulator >= ComboTime) {
		Attack1ComboCount = 0;
		Attack1ComboAccumulator = 0;
	}
	
	if(AnimAccumulator > AnimTime && !CanAttack) {
		GLog->Log("esvaziando");
		ActorHitted.Empty();
		State = EACCStateMachine::Default;
		CanAttack = true;
	}
}

