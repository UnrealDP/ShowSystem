// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RunTime/ShowBase.h"
#include "InstancedStruct.h"
#include "ShowSequencer.generated.h"


UENUM(BlueprintType)
enum class EShowSequencerState : uint8
{
    ShowSequencer_Wait UMETA(DisplayName = "ShowSequencer Wait"),
    ShowSequencer_Playing UMETA(DisplayName = "ShowSequencer Playing"),
    ShowSequencer_Pause UMETA(DisplayName = "ShowSequencer Pause"),
    ShowSequencer_End UMETA(DisplayName = "ShowSequencer End"),
};


/**
 * 
 */
UCLASS(BlueprintType)
class SHOWSYSTEM_API UShowSequencer : public UObject
{
	GENERATED_BODY()
	
public:
    UShowSequencer();

    UFUNCTION(BlueprintCallable, Category = "Show")
    void Play();

    UFUNCTION(BlueprintCallable, Category = "Show")
    void Stop();

    UFUNCTION(BlueprintCallable, Category = "Show")
    void Pause();

    UFUNCTION(BlueprintCallable, Category = "Show")
    void UnPause();

    UFUNCTION(BlueprintCallable, Category = "Show")
    void ChangeSpeed(float Speed);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Show")
    TArray<FInstancedStruct> ShowKeys;

    // setter getter
    void SetID(int InID) { ID = InID; } 
    void ClearID() { ID = -1; }
    int GetID() { return ID; }

    void SetOwner(AActor* InOwner) { Owner = InOwner; }
    void ClearOwner() { Owner = nullptr; }
    AActor* GetOwner() { return Owner.Get(); }
    // end of setter getter

    void Tick(float DeltaTime);

    EShowSequencerState GetShowSequencerState() const { return ShowSequencerState; }
    bool IsWait() const { return ShowSequencerState == EShowSequencerState::ShowSequencer_Wait; }
    bool IsPlaying() const { return ShowSequencerState == EShowSequencerState::ShowSequencer_Playing; }
    bool IsPause() const { return ShowSequencerState == EShowSequencerState::ShowSequencer_Pause; }
    bool IsEnd() const { return ShowSequencerState == EShowSequencerState::ShowSequencer_End; }

private:
    UShowBase* CreateShowObject(const FShowKey& InShowKey);

private:
    EShowSequencerState ShowSequencerState = EShowSequencerState::ShowSequencer_Wait;
    float PassedTime = 0.0f;
    int ID = -1;
    TObjectPtr<AActor> Owner;
    TArray<TObjectPtr<UShowBase>> RuntimeShowKeys;
};
