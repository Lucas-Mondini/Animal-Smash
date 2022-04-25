// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ActorComponent/ActorCombatComponent.h"

#include "Character/ALSBaseCharacter.h"

// Sets default values for this component's properties
UActorCombatComponent::UActorCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	//Punchs
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
	
	//Kicks
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
	float animfactor = 0.65;
	if(OwnerCharacter->GetMovementState() != EALSMovementState::InAir && CanAttack) {
		CanAttack = false;
		Attack1ComboLength = PunchComboAnimations.Num();
		UAnimMontage *anim = PunchComboAnimations[Attack1ComboCount++ % Attack1ComboLength];
		OwnerCharacter->Replicated_PlayMontage(anim, 1);
		Attack1ComboAccumulator = 0;
		AnimAccumulator = 0;
		AnimTime = anim->GetPlayLength() * animfactor;
		State = ACCStateMachine::Punching; 
	}
}

void UActorCombatComponent::Attack2() {
	float animfactor = 0.8;
	if(OwnerCharacter->GetMovementState() != EALSMovementState::InAir && CanAttack) {
		CanAttack = false;
		Attack2ComboLength = KickComboAnimations.Num();
		UAnimMontage *anim = KickComboAnimations[Attack2ComboCount++ % Attack2ComboLength];
		OwnerCharacter->Replicated_PlayMontage(anim, animfactor);
		Attack2ComboAccumulator = 0;
		AnimAccumulator = 0;
		AnimTime = anim->GetPlayLength() * animfactor;
		State = ACCStateMachine::Kicking; 
	}
}


// Called when the game starts
void UActorCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<AALSBaseCharacter>(GetOwner());
	
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
		CanAttack = true;
	}
}

