// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ActorComponent/ActorCombatComponent.h"

#include "Character/ALSBaseCharacter.h"
#include "Character/ActorComponent/CombatAnimMontage.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"

// Sets default values for this component's properties
UActorCombatComponent::UActorCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	State = EACCStateMachine::Default;

	//Punch animations
	if(GetOwner()) {
		UCombatAnimMontage* Jab_L = UCombatAnimMontage::CreateCombatAnimMontage(TMap<FString, TArray<UCapsuleComponent*>>(), GetOwner(),TEXT("AnimMontage'/Game/AnimalSmash/Assets/Anim/KB_m_Jab_L_Montage.KB_m_Jab_L_Montage'"),PunchAnimVelocity);
		UCombatAnimMontage* Jab_R = UCombatAnimMontage::CreateCombatAnimMontage(TMap<FString, TArray<UCapsuleComponent*>>(), GetOwner(), TEXT("AnimMontage'/Game/AnimalSmash/Assets/Anim/KB_m_Jab_R_Montage.KB_m_Jab_R_Montage'"), PunchAnimVelocity);
		UCombatAnimMontage* Uppercut_L = UCombatAnimMontage::CreateCombatAnimMontage(TMap<FString, TArray<UCapsuleComponent*>>(), GetOwner(), TEXT("AnimMontage'/Game/AnimalSmash/Assets/Anim/KB_m_Uppercut_L_Montage.KB_m_Uppercut_L_Montage'"), PunchAnimVelocity);
		UCombatAnimMontage* Superpunch = UCombatAnimMontage::CreateCombatAnimMontage(TMap<FString, TArray<UCapsuleComponent*>>(), GetOwner(), TEXT("AnimMontage'/Game/AnimalSmash/Assets/Anim/KB_Superpunch_Montage.KB_Superpunch_Montage'"), PunchAnimVelocity);
	
		//Kick animations
		UCombatAnimMontage* NinjaCutDownKick = UCombatAnimMontage::CreateCombatAnimMontage(TMap<FString, TArray<UCapsuleComponent*>>(), GetOwner(), TEXT("AnimMontage'/Game/AnimalSmash/Assets/Anim/NinjaCutDownKick_Retargeted_Montage.NinjaCutDownKick_Retargeted_Montage'"), KickAnimVelocity);
		UCombatAnimMontage* NinjaLowRightThaiKick = UCombatAnimMontage::CreateCombatAnimMontage(TMap<FString, TArray<UCapsuleComponent*>>(), GetOwner(), TEXT("AnimMontage'/Game/AnimalSmash/Assets/Anim/NinjaLowRightThaiKick_Retargeted_Montage.NinjaLowRightThaiKick_Retargeted_Montage'"), KickAnimVelocity);
		UCombatAnimMontage* NinjaStraightKick = UCombatAnimMontage::CreateCombatAnimMontage(TMap<FString, TArray<UCapsuleComponent*>>(), GetOwner(), TEXT("AnimMontage'/Game/AnimalSmash/Assets/Anim/NinjaStraightKick_Retargeted_Montage.NinjaStraightKick_Retargeted_Montage'"), KickAnimVelocity);
		UCombatAnimMontage* NinjaWheelKick = UCombatAnimMontage::CreateCombatAnimMontage(TMap<FString, TArray<UCapsuleComponent*>>(), GetOwner(), TEXT("AnimMontage'/Game/AnimalSmash/Assets/Anim/NinjaWheelKick_Retargeted_Montage.NinjaWheelKick_Retargeted_Montage'"), KickAnimVelocity);
	
		PunchComboAnimations.Add(Jab_L);
		PunchComboAnimations.Add(Jab_R);
		PunchComboAnimations.Add(Uppercut_L);
		PunchComboAnimations.Add(Superpunch);

		KickComboAnimations.Add(NinjaCutDownKick);
		KickComboAnimations.Add(NinjaLowRightThaiKick);
		KickComboAnimations.Add(NinjaStraightKick);
		KickComboAnimations.Add(NinjaWheelKick);
	}
	
	
	Attack2ComboLength = PunchComboAnimations.Num();
	Attack2ComboLength = KickComboAnimations.Num();
	
}

void UActorCombatComponent::Attack1() {
	if(OwnerCharacter->GetMovementState() != EALSMovementState::InAir && CanAttack) {
		CanAttack = false;
		Attack1ComboLength = PunchComboAnimations.Num();
		UCombatAnimMontage *anim = PunchComboAnimations[Attack1ComboCount++ % Attack1ComboLength];
		anim->PlayAnimation();
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
		UCombatAnimMontage *anim = KickComboAnimations[Attack2ComboCount++ % Attack2ComboLength];
		anim->PlayAnimation();
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

