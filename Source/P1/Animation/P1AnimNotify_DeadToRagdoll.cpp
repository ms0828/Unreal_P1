// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/P1AnimNotify_DeadToRagdoll.h"
#include "Character/P1Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
void UP1AnimNotify_DeadToRagdoll::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{

    //AP1Character* Character = Cast< AP1Character>(MeshComp->GetOwner());
    //if (Character)
    //{
    //    UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
    //    UAnimMontage* CurrentMontage = AnimInstance->GetCurrentActiveMontage();

    //    if (CurrentMontage && AnimInstance->Montage_IsPlaying(CurrentMontage))
    //    {
    //        MeshComp->SetAnimationMode(EAnimationMode::AnimationSingleNode);
    //        
    //        UAnimSequence* AnimSequence = Cast<UAnimSequence>(Animation);
    //        if (AnimSequence)
    //        {
    //            MeshComp->PlayAnimation(AnimSequence, false);
    //            float LastFrameTime = AnimSequence->GetPlayLength();
    //            MeshComp->SetPosition(LastFrameTime);
    //        }
    //        
    //    }

    //    /*Character->GetCharacterMovement()->StopMovementImmediately();
    //    MeshComp->SetCollisionProfileName(TEXT("Ragdoll"));
    //    MeshComp->SetEnableGravity(true);

    //    MeshComp->SetPhysicsLinearVelocity(FVector::ZeroVector, false);
    //    MeshComp->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector, false);

    //    MeshComp->SetSimulatePhysics(true);
    //    Character->SetLifeSpan(10.0f);*/
    //}
    
}
