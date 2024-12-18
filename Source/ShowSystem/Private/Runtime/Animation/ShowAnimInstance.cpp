// Fill out your copyright notice in the Description page of Project Settings.


#include "RunTime/Animation/ShowAnimInstance.h"
#include "Animation/AnimSingleNodeInstanceProxy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimNodes/AnimNode_LayeredBoneBlend.h"
#include "Runtime/ShowKeys/ShowAnimStatic.h"

void UShowAnimInstance::BeginDestroy()
{
	OwnerComponent = nullptr;
	Skeleton = nullptr;
	OwnerActor = nullptr;
	MovementComponent = nullptr;

	Super::BeginDestroy();
}

void UShowAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    OwnerComponent = GetSkelMeshComponent();
    if(!OwnerComponent)
	{
		return;
	}
    else
    {
        USkeletalMesh* SkeletalMesh = OwnerComponent->GetSkeletalMeshAsset();
        if (SkeletalMesh)
        {
            Skeleton = SkeletalMesh->GetSkeleton();
        }
    }

    OwnerActor = OwnerComponent->GetOwner();
    if (!OwnerActor)
    {
        return;
    }

    MovementComponent = OwnerActor->FindComponentByClass<UCharacterMovementComponent>();
}

UAnimMontage* UShowAnimInstance::PlayAnimation(UAnimSequenceBase* NewAsset, int32 LoopCount, float BlendOutTriggerTime, float InTimeToStartMontageAt, float PlayRate)
{
    checkf(NewAsset, TEXT("UShowAnimInstance::PlayAnimation NewAsset is invalid"));
    checkf(AnimContainer, TEXT("UShowAnimInstance::PlayAnimation AnimContainer is invalid"));

    FSoftObjectPath NewAssetPath = FSoftObjectPath(NewAsset);

    if (const FAnimData* FoundData = AnimContainer->FindAnimData(NewAssetPath))
    {
        checkf(Skeleton->ContainsSlotName(FoundData->Slot), TEXT("UShowAnimInstance::PlayAnimation -> Slot [ %s ] not exist"), *FoundData->Slot.ToString());

        const FMontageBlendSettings* BlendInSettingsPtr = nullptr;
        const FMontageBlendSettings* BlendOutSettingsPtr = nullptr;
        if (const FAnimBlendData* FoundBlendData = AnimContainer->FindAnimBlendData(NewAssetPath))
        {
            BlendInSettingsPtr = &FoundBlendData->BlendInSettings;
            BlendOutSettingsPtr = &FoundBlendData->BlendOutSettings;
        }
        else
        {
            BlendInSettingsPtr = &AnimContainer->DefaultAnimBlendData.BlendInSettings;
            BlendOutSettingsPtr = &AnimContainer->DefaultAnimBlendData.BlendOutSettings;
        }

        UAnimMontage* DynamicMontage = PlaySlotAnimationAsDynamicMontage_WithBlendSettings(
            NewAsset,
            FoundData->Slot,
            *BlendInSettingsPtr,
            *BlendOutSettingsPtr,
            PlayRate,
            LoopCount,
            BlendOutTriggerTime,
            InTimeToStartMontageAt);
        
        if (!DynamicMontage)
        {
            UE_LOG(LogTemp, Error, TEXT("UShowAnimInstance::PlayAnimation DynamicMontage is null [ %s ]"), *NewAsset->GetFName().ToString());
        }

        return DynamicMontage;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("UShowAnimInstance::PlayAnimation AnimData is not found"));
        return nullptr;
    }
}
