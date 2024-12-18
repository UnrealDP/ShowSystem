// Fill out your copyright notice in the Description page of Project Settings.


#include "RunTime/ShowKeys/ShowAnimStatic.h"
#include "RunTime/Animation/ShowAnimInstance.h"

FString UShowAnimStatic::GetTitle()
{
    if (!AnimSequenceBase)
	{
		return "ShowAnimStatic";
	}
    return AnimSequenceBase->GetName();
}

void UShowAnimStatic::Initialize() 
{
    checkf(ShowKey, TEXT("UShowAnimStatic::Initialize ShowKey is invalid"));

    AnimStaticKeyPtr = static_cast<const FShowAnimStaticKey*>(ShowKey);
    checkf(AnimStaticKeyPtr, TEXT("UShowAnimStatic::Initialize AnimStaticKeyPtr is invalid [ %d ]"), static_cast<int>(ShowKey->KeyType));

    if (!AnimSequenceBase)
    {
        if (AnimStaticKeyPtr->AnimSequenceAsset.IsNull())
        {
            ShowKeyState = EShowKeyState::ShowKey_End;
            return;
        }

        if (!AnimStaticKeyPtr->AnimSequenceAsset.IsValid())
        {
            AnimStaticKeyPtr->AnimSequenceAsset.LoadSynchronous();
        }
        
        AnimSequenceBase = AnimStaticKeyPtr->AnimSequenceAsset.Get();
        if (!AnimSequenceBase)
        {
            checkf(false, TEXT("UShowAnimStatic::Initialize AnimSequence is invalid"));
            ShowKeyState = EShowKeyState::ShowKey_End;
            return;
        }
    }
}

float UShowAnimStatic::GetLength()
{
    if (!AnimSequenceBase)
	{
		return 0.0f;
	}

    return AnimSequenceBase->GetPlayLength() / ShowKey->PlayRate;
}

void UShowAnimStatic::Dispose()
{
    if (AnimInstance && AnimInstance->OnMontageEnded.IsAlreadyBound(this, &UShowAnimStatic::OnMontageEnded))
    {
        AnimInstance->OnMontageEnded.RemoveDynamic(this, &UShowAnimStatic::OnMontageEnded);
    }

    AnimStaticKeyPtr = nullptr;
    AnimSequenceBase = nullptr;
    AnimMontage = nullptr;
    AnimInstance = nullptr;
    OriginalLength = 0.0f;
}

void UShowAnimStatic::Reset()
{
    Dispose();
    Initialize();
}

void UShowAnimStatic::Play() 
{
    AActor* ShowOwner = GetShowOwner();
    checkf(ShowOwner, TEXT("UShowAnimStatic::Play ShowOwner is invalid"));
    checkf(AnimSequenceBase, TEXT("UShowAnimStatic::Play AnimSequence is invalid"));
    checkf(!AnimInstance, TEXT("UShowAnimStatic::Play AnimInstance need reset"));

    USkeletalMeshComponent* SkeletalMeshComp = ShowOwner->FindComponentByClass<USkeletalMeshComponent>();
    checkf(SkeletalMeshComp, TEXT("UShowAnimStatic::Play SkeletalMeshComponent is invalid"));

    if (!SkeletalMeshComp->AnimClass)
    {
        UE_LOG(LogTemp, Error, TEXT("UShowAnimStatic::Play SkeletalMeshComp->AnimClass is invalid"));
        ShowKeyState = EShowKeyState::ShowKey_End;
        return;
    }

    if (UShowAnimInstance* ShowAnimInstance = Cast<UShowAnimInstance>(SkeletalMeshComp->GetAnimInstance()))
    {
        AnimInstance = ShowAnimInstance;
        checkf(!AnimInstance->OnMontageEnded.IsAlreadyBound(this, &UShowAnimStatic::OnMontageEnded), TEXT("UShowAnimStatic::Play MontageEndedHandler need reset"));

        AnimMontage = ShowAnimInstance->PlayAnimation(
            AnimSequenceBase, 
            AnimStaticKeyPtr->LoopCount,
            AnimStaticKeyPtr->BlendOutTriggerTime,
            AnimStaticKeyPtr->InTimeToStartMontageAt,
            ShowKey->PlayRate * CachedTimeScale);

        if (AnimMontage)
        {
            ShowAnimInstance->OnMontageEnded.AddDynamic(this, &UShowAnimStatic::OnMontageEnded);
            return;
		}
        else
        {
            UE_LOG(LogTemp, Error, TEXT("UShowAnimStatic::Play ShowAnimInstance->PlayAnimation is invalid"));
        }
    }

    AnimInstance = SkeletalMeshComp->GetAnimInstance();
    if (!AnimMontage && AnimInstance)
    {
        checkf(!AnimInstance->OnMontageEnded.IsAlreadyBound(this, &UShowAnimStatic::OnMontageEnded), TEXT("UShowAnimStatic::Play MontageEndedHandler need reset"));

        AnimMontage = AnimInstance->PlaySlotAnimationAsDynamicMontage(
            AnimSequenceBase, 
            TEXT("DefaultSlot"), 
            0.25f,
            0.25f,
            ShowKey->PlayRate * CachedTimeScale,
            AnimStaticKeyPtr->LoopCount,
            AnimStaticKeyPtr->BlendOutTriggerTime,
            AnimStaticKeyPtr->InTimeToStartMontageAt);

        if (AnimMontage)
        {
            AnimInstance->OnMontageEnded.AddDynamic(this, &UShowAnimStatic::OnMontageEnded);
            return;
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("UShowAnimStatic::Play AnimInstance->PlaySlotAnimationAsDynamicMontage is invalid"));
        }
    }
    
    if (!AnimMontage)
    { 
        UE_LOG(LogTemp, Error, TEXT("UShowAnimStatic::Play AnimInstance is invalid"));
        ShowKeyState = EShowKeyState::ShowKey_End;
    }
}

void UShowAnimStatic::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    if (!AnimMontage)
    {
        ShowKeyState = EShowKeyState::ShowKey_End;
        return;
    }

    if (AnimMontage == Montage)
    {
        if (ShowKeyState != EShowKeyState::ShowKey_Playing &&
            ShowKeyState != EShowKeyState::ShowKey_Pause)
        {
            checkf(false, TEXT("UShowAnimStatic::OnMontageEnded ShowKeyState is not Playing"));
        }

        if (AnimInstance && AnimInstance->OnMontageEnded.IsAlreadyBound(this, &UShowAnimStatic::OnMontageEnded))
        {
            AnimInstance->OnMontageEnded.RemoveDynamic(this, &UShowAnimStatic::OnMontageEnded);
        }

        AnimMontage = nullptr;
        ShowKeyState = EShowKeyState::ShowKey_End;
    }
}

void UShowAnimStatic::ApplyTimeScale(float FinalTimeScale) 
{
    if (AnimInstance && AnimMontage)
    {
        AnimInstance->Montage_SetPlayRate(AnimMontage, ShowKey->PlayRate * FinalTimeScale);
    }
}
