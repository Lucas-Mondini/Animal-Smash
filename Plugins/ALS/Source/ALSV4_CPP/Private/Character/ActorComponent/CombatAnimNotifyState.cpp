// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ActorComponent/CombatAnimNotifyState.h"
#include "Engine.h"
#include "Character/ActorComponent/CombatAnimMontage.h"

void UCombatAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                         float TotalDuration, const FAnimNotifyEventReference& EventReference) {
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if(CombatAnimMontageRef) {
		CombatAnimMontageRef->CreateAnimCollisions();
	}
}

void UCombatAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) {
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	
}

void UCombatAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference) {
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if(CombatAnimMontageRef) {
		CombatAnimMontageRef->DestroyAnimCollisions();
	}
	
}

void UCombatAnimNotifyState::SetCombatAnimMontageRef(UCombatAnimMontage* NewCombatAnimMontageRef) {
	this->CombatAnimMontageRef = NewCombatAnimMontageRef;
}
