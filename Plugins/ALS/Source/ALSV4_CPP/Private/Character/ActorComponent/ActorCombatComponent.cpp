// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ActorComponent/ActorCombatComponent.h"

#include "Character/ALSBaseCharacter.h"

// Sets default values for this component's properties
UActorCombatComponent::UActorCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

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
		State = ACCStateMachine::Attacking; 
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

