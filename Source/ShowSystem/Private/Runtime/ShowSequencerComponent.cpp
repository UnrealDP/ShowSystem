// Fill out your copyright notice in the Description page of Project Settings.


#include "RunTime/ShowSequencerComponent.h"
#include "RunTime/ShowSequencer.h"

// Sets default values for this component's properties
UShowSequencerComponent::UShowSequencerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UShowSequencerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UShowSequencerComponent::BeginDestroy()
{
	for (TObjectPtr<UShowSequencer>& ShowSequencer : ShowSequencers)
	{
		ShowSequencer->Dispose();
		ShowSequencer->MarkAsGarbage();
		ShowSequencer = nullptr;
	}
	ShowSequencers.Empty();

	Super::BeginDestroy();
}

// Called every frame
void UShowSequencerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for (TObjectPtr<UShowSequencer>& showSequencer : ShowSequencers)
	{
		showSequencer->Tick(DeltaTime);
	}
}

TObjectPtr<UShowSequencer> UShowSequencerComponent::NewShowSequencer(const FSoftObjectPath& ShowPath)
{
	AActor* Owner = GetOwner();
	checkf(Owner, TEXT("UShowSequencerComponent::NewShowSequencer: The Owner is invalid."));

	if (UShowSequenceAsset* LoadedShowSequenceAsset = Cast<UShowSequenceAsset>(ShowPath.TryLoad())) 
	{
		TObjectPtr<UShowSequencer> NewShowSequencer = NewObject<UShowSequencer>(this, UShowSequencer::StaticClass(), FName(TEXT("MyNewShowSequencer")), EObjectFlags::RF_Public);
		NewShowSequencer->Initialize(Owner, LoadedShowSequenceAsset);
		ShowSequencers.Add(NewShowSequencer);
		return NewShowSequencer;
	}
	return nullptr;
}

void UShowSequencerComponent::PlayShow(UShowSequencer* InShowSequencer)
{
	checkf(InShowSequencer, TEXT("UShowSequencerComponent::PlayShow: The OShowSequencer provided is invalid or null."));

	InShowSequencer->Play();
}

void UShowSequencerComponent::StopShow(UShowSequencer* InShowSequencer) const
{
	checkf(InShowSequencer, TEXT("UShowSequencerComponent::StopShow: InShowSequencer is invalid or null"));

	InShowSequencer->Stop();
	// InShowSequencer->Dispose(); 를 안하는 이유는 Stop은 다시 재생할 수도 있음
	// 완전히 삭제하고 싶으면 DisposeShow를 사용해야함
	// 특히나 자주 사용되는 Show 같은 경우에는 DontDestroy를 사용하여 DisposeShow를 사용하지 않는 것이 좋음
	InShowSequencer->ClearOwner();
}

void UShowSequencerComponent::DisposeShow(UShowSequencer* InShowSequencer)
{
	checkf(InShowSequencer, TEXT("UShowSequencerComponent::DisposeShow: InShowSequencer is invalid or null"));

	InShowSequencer->Dispose();
	InShowSequencer->MarkAsGarbage();
	InShowSequencer = nullptr;
	ShowSequencers.Remove(InShowSequencer);
}

void UShowSequencerComponent::PauseShow(UShowSequencer* InShowSequencer) const
{
	checkf(InShowSequencer, TEXT("UShowSequencerComponent::PauseShow: InShowSequencer is invalid or null."));

	InShowSequencer->Pause();
}

void UShowSequencerComponent::UnPauseShow(UShowSequencer* InShowSequencer) const
{
	checkf(InShowSequencer, TEXT("UShowSequencerComponent::UnPauseShow: InShowSequencer is invalid or null"));

	InShowSequencer->UnPause();
}

void UShowSequencerComponent::ChangeShowTimeScalse(UShowSequencer* InShowSequencer, float InTimeScalse) const
{
	checkf(InShowSequencer, TEXT("UShowSequencerComponent::ChangeShowTimeScalse: InShowSequencer is invalid or null"));

	InShowSequencer->ChangeTimeScale(InTimeScalse);
}