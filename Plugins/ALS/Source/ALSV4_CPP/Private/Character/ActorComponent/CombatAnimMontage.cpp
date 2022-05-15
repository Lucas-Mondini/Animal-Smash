// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ActorComponent/CombatAnimMontage.h"
#include "Components/CapsuleComponent.h"
#include "Character/ALSBaseCharacter.h"
#include "Character/ActorComponent/ActorCombatComponent.h"
#include "Character/ActorComponent/CombatAnimNotifyState.h"

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
	const TMap<FString, TArray<FTransform>> SocketNameToCollision, AActor* playerRef, const TCHAR* ObjectToFind,
	float playRate) {
	UCombatAnimMontage* CombatAnimMontage = NewObject<UCombatAnimMontage>(playerRef);
	ConstructorHelpers::FObjectFinder<UAnimMontage> anim(ObjectToFind);
	if (!anim.Succeeded()) {
		throw std::strcat("CombatAnimMontage-> Animation not found: ", TCHAR_TO_ANSI(ObjectToFind));
	}
	CombatAnimMontage->SocketNameToCollision = SocketNameToCollision;
	CombatAnimMontage->ActorToPlayMontage = Cast<AALSBaseCharacter>(playerRef);
	CombatAnimMontage->AnimMontage = anim.Object;
	CombatAnimMontage->PlayRate = playRate;
	
	for (FAnimNotifyEvent &i : CombatAnimMontage->AnimMontage->Notifies) {
		if(UCombatAnimNotifyState* animMontage = Cast<UCombatAnimNotifyState>(i.NotifyStateClass)) {
			animMontage->SetCombatAnimMontageRef(CombatAnimMontage);
		}
	}
	
	return CombatAnimMontage;
}

void UCombatAnimMontage::CreateAnimCollisions() {
	for (auto mapItem : SocketNameToCollision) {
		for (auto ArrayItem : mapItem.Value) {
			UCapsuleComponent* capsule = NewObject<UCapsuleComponent>(ActorToPlayMontage);
			capsule->SetRelativeTransform(ArrayItem);
			capsule->SetupAttachment(ActorToPlayMontage->GetMesh(), FName(mapItem.Key));
			capsule->RegisterComponent();
			capsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
			capsule->OnComponentBeginOverlap.AddDynamic(this, &UCombatAnimMontage::CallCombatComponentProcessOverlapProcess);
			capsule->SetIsReplicated(true);
			CollisionCapsuleArray.Add(capsule);
		}
	}
}


void UCombatAnimMontage::DestroyAnimCollisions() {
	try {
		for (UCapsuleComponent *c : CollisionCapsuleArray) {
			CollisionCapsuleArray.Remove(c);
			c->DestroyComponent();
		}
	} catch (std::exception e) {
		
	}
}

void UCombatAnimMontage::CallCombatComponentProcessOverlapProcess_Implementation(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult) {
	ActorToPlayMontage->
	GetCombatComponent()->
	CombatComponentProcessOverlapProcess(	OverlappedComp, OtherActor, OtherComp,
											OtherBodyIndex, bFromSweep, SweepResult);
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
