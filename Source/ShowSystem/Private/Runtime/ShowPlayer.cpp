// Fill out your copyright notice in the Description page of Project Settings.


#include "RunTime/ShowPlayer.h"
#include "RunTime/ShowSequencer.h"
#include "RunTime/ShowSequencerComponent.h"

/** Implement this for initialization of instances of the system */
void UShowPlayer::Initialize(FSubsystemCollectionBase& Collection)
{
    Initialize();
    Super::Initialize(Collection);
    UE_LOG(LogTemp, Log, TEXT("UShowPlayer Initialized successfully."));
}

void UShowPlayer::Initialize()
{
    if (Initialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("UShowPlayer::Initialize called again, skipping as it is already initialized."));
        return;
    }
    ON_SCOPE_EXIT
    {
        Initialized = true;
    };
}

/** Implement this for deinitialization of instances of the system */
void UShowPlayer::Deinitialize()
{
    Super::Deinitialize();
    UE_LOG(LogTemp, Log, TEXT("UShowPlayer: Object pool cleaned up."));
}

void UShowPlayer::Tick(float DeltaTime)
{
    if (!Initialized)
    {
        return;
    }
}

UShowSequencer* UShowPlayer::NewShowSequencer(AActor* Owner, const FSoftObjectPath& ShowPath)
{
    checkf(Owner, TEXT("UShowPlayer::NewShowSequencer: The Owner provided is invalid or null."));

    UShowSequencerComponent* ShowSequencerComponent = Owner->FindComponentByClass<UShowSequencerComponent>();
    if (!ShowSequencerComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("UShowPlayer::NewShowSequencer add UShowSequencerComponent"));

        ShowSequencerComponent = NewObject<UShowSequencerComponent>(Owner);
        if (ShowSequencerComponent)
        {
            Owner->AddInstanceComponent(ShowSequencerComponent);
            ShowSequencerComponent->RegisterComponent();
        }
    }

    if (ShowSequencerComponent)
    {
        return ShowSequencerComponent->NewShowSequencer(ShowPath);
    }

    return nullptr;
}

void UShowPlayer::PlaySoloShow(AActor* Owner, UShowSequencer* ShowSequencerPtr)
{
    checkf(Owner, TEXT("UShowPlayer::PlaySoloShow: The Owner provided is invalid or null."));
    checkf(ShowSequencerPtr, TEXT("UShowPlayer::PlaySoloShow: The ShowSequencer provided is invalid or null."));

    UShowSequencerComponent* ShowSequencerComponent = Owner->FindComponentByClass<UShowSequencerComponent>();
    if (!ShowSequencerComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("UShowPlayer::PlaySoloShow add UShowSequencerComponent"));

        ShowSequencerComponent = NewObject<UShowSequencerComponent>(Owner);
        if (ShowSequencerComponent)
        {
            Owner->AddInstanceComponent(ShowSequencerComponent);
            ShowSequencerComponent->RegisterComponent();
        }
    }

    ShowSequencerComponent->PlayShow(ShowSequencerPtr);
}

void UShowPlayer::PauseSoloShow(AActor* Owner, UShowSequencer* ShowSequencerPtr)
{
    checkf(Owner, TEXT("UShowPlayer::PauseSoloShow: The Owner provided is invalid or null."));

    UShowSequencerComponent* ShowSequencerComponent = Owner->FindComponentByClass<UShowSequencerComponent>();
    checkf(ShowSequencerComponent, TEXT("UShowPlayer::PauseSoloShow: The ShowSequencerComponent provided is invalid or null."));

    ShowSequencerComponent->PauseShow(ShowSequencerPtr);
}

void UShowPlayer::UnPauseSoloShow(AActor* Owner, UShowSequencer* ShowSequencerPtr)
{
    checkf(Owner, TEXT("UShowPlayer::UnPauseSoloShow: The Owner provided is invalid or null."));

    UShowSequencerComponent* ShowSequencerComponent = Owner->FindComponentByClass<UShowSequencerComponent>();
    checkf(ShowSequencerComponent, TEXT("UShowPlayer::UnPauseSoloShow: The ShowSequencerComponent provided is invalid or null."));

    ShowSequencerComponent->UnPauseShow(ShowSequencerPtr);
}

void UShowPlayer::ChangeTimeScale(AActor* Owner, UShowSequencer* ShowSequencerPtr, float scale)
{
    checkf(Owner, TEXT("UShowPlayer::UnPauseSoloShow: The Owner provided is invalid or null."));

    UShowSequencerComponent* ShowSequencerComponent = Owner->FindComponentByClass<UShowSequencerComponent>();
    checkf(ShowSequencerComponent, TEXT("UShowPlayer::UnPauseSoloShow: The ShowSequencerComponent provided is invalid or null."));

    ShowSequencerComponent->ChangeShowTimeScalse(ShowSequencerPtr, scale);
}

void UShowPlayer::ResetSoloShow(AActor* Owner, UShowSequencer* ShowSequencerPtr)
{
    checkf(Owner, TEXT("UShowPlayer::ResetSoloShow: The Owner provided is invalid or null."));

    UShowSequencerComponent* ShowSequencerComponent = Owner->FindComponentByClass<UShowSequencerComponent>();
    checkf(ShowSequencerComponent, TEXT("UShowPlayer::ResetSoloShow: The ShowSequencerComponent provided is invalid or null."));

    ShowSequencerComponent->ResetShow(ShowSequencerPtr);
}

void UShowPlayer::DisposeSoloShow(AActor* Owner, UShowSequencer* ShowSequencerPtr)
{
    checkf(Owner, TEXT("UShowPlayer::StopSoloShow: The Owner provided is invalid or null."));

    UShowSequencerComponent* ShowSequencerComponent = Owner->FindComponentByClass<UShowSequencerComponent>();
    checkf(ShowSequencerComponent, TEXT("UShowPlayer::StopSoloShow: The ShowSequencerComponent provided is invalid or null."));

    ShowSequencerComponent->DisposeShow(ShowSequencerPtr);
}

bool UShowPlayer::HasShowSequencer(const AActor* Owner, const UShowSequencer* SequencerPtr) const
{
    checkf(Owner, TEXT("UShowPlayer::StopSoloShow: The Owner provided is invalid or null."));

    UShowSequencerComponent* ShowSequencerComponent = Owner->FindComponentByClass<UShowSequencerComponent>();
    checkf(ShowSequencerComponent, TEXT("UShowPlayer::StopSoloShow: The ShowSequencerComponent provided is invalid or null."));

	return ShowSequencerComponent->HasShowSequencer(SequencerPtr);
}
