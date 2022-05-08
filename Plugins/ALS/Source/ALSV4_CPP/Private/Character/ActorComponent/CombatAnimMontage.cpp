// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ActorComponent/CombatAnimMontage.h"

#include "Character/ALSBaseCharacter.h"

/***
 *@param CollisionStartAndEndTime must have two keys startTime and endTime
 */
CombatAnimMontage::CombatAnimMontage(const TMap<FString, TArray<UCapsuleComponent*>> SocketNameToCollision, AActor* playerRef, const TCHAR* ObjectToFind, float playRate)
{
	ActorToPlayMontage = Cast<AALSBaseCharacter>(playerRef);
	ConstructorHelpers::FObjectFinder<UAnimMontage> anim(ObjectToFind);
	if (!anim.Succeeded()) {
		throw std::strcat("CombatAnimMontage-> Animation not found: ", TCHAR_TO_ANSI(ObjectToFind));
	}
	AnimMontage = anim.Object;
	this->PlayRate = playRate;
}

CombatAnimMontage::~CombatAnimMontage()
{
}

void CombatAnimMontage::PlayAnimation() {
	ActorToPlayMontage->Replicated_PlayMontage(AnimMontage, PlayRate);
}
