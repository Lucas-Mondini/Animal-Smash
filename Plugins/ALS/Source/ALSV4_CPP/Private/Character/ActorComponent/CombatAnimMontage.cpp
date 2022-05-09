// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ActorComponent/CombatAnimMontage.h"

#include "Character/ALSBaseCharacter.h"

/***
 *@param CollisionStartAndEndTime must have two keys startTime and endTime
 */
UCombatAnimMontage::UCombatAnimMontage()
{
	
}

UCombatAnimMontage::~UCombatAnimMontage()
{
}

UCombatAnimMontage* UCombatAnimMontage::CreateCombatAnimMontage(
	const TMap<FString, TArray<UCapsuleComponent*>> SocketNameToCollision, AActor* playerRef, const TCHAR* ObjectToFind,
	float playRate) {
	UCombatAnimMontage* CombatAnimMontage = NewObject<UCombatAnimMontage>(playerRef);
	CombatAnimMontage->ActorToPlayMontage = Cast<AALSBaseCharacter>(playerRef);
	ConstructorHelpers::FObjectFinder<UAnimMontage> anim(ObjectToFind);
	if (!anim.Succeeded()) {
		throw std::strcat("CombatAnimMontage-> Animation not found: ", TCHAR_TO_ANSI(ObjectToFind));
	}
	CombatAnimMontage->AnimMontage = anim.Object;
	CombatAnimMontage->PlayRate = playRate;
	
	return CombatAnimMontage;
}

float UCombatAnimMontage::GetPlayLength() {
	return this->AnimMontage->GetPlayLength();
}

void UCombatAnimMontage::PlayAnimation(float playrate) {
	ActorToPlayMontage->Replicated_PlayMontage(AnimMontage, playrate);
}

void UCombatAnimMontage::PlayAnimation() {
	ActorToPlayMontage->Replicated_PlayMontage(AnimMontage, PlayRate);
}