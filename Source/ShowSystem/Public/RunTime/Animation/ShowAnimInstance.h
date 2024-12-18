// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RunTime/Animation/AnimContainer.h"
#include "ShowAnimInstance.generated.h"

class UShowAnimStatic;
class UCharacterMovementComponent;

/**
 * class FShowAnimInstanceProxy : public FAnimSingleNodeInstanceProxy 프록시 같은건 안만들었다!!
 * 지금은 ShowSystem 테스트 용으로 나중에 확장 구현해야한다
 */
UCLASS(Blueprintable)
class SHOWSYSTEM_API UShowAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
    virtual void BeginDestroy() override;
    virtual void NativeInitializeAnimation() override;

    UAnimMontage* PlayAnimation(UAnimSequenceBase* NewAsset, int32 LoopCount, float BlendOutTriggerTime = -1.0f, float InTimeToStartMontageAt = 0.0f, float PlayRate = 1.0f);
    
public:
    // 애니메이션 정보 (애니메이션 정보만 담고 있음)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UAnimContainer> AnimContainer = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
    TObjectPtr<UCharacterMovementComponent> MovementComponent = nullptr;

private:
    UPROPERTY()
    TObjectPtr<USkeletalMeshComponent> OwnerComponent = nullptr;

    UPROPERTY()
    TObjectPtr<USkeleton> Skeleton = nullptr;

    AActor* OwnerActor = nullptr;
};
