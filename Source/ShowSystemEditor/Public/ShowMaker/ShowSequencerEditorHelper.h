// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RunTime/ShowSequencer.h"
#include "RunTime/ShowBase.h"

class FShowSequencerEditorToolkit;
class SShowMakerWidget;
class UShowBase;

DECLARE_DELEGATE_TwoParams(FShowKeyChange, UShowBase*, FName);

/**
 * 
 */
class SHOWSYSTEMEDITOR_API FShowSequencerEditorHelper : public FTickableEditorObject
{
public:
	~FShowSequencerEditorHelper();

	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FShowSequencerEditorHelper, STATGROUP_Tickables);
	}
	virtual bool IsTickable() const override
	{
		return true;
	}

	void SetShowMakerWidget(TSharedPtr<SShowMakerWidget> InShowMakerWidget);
	const TArray<UShowBase*>* RuntimeShowKeysPtr()const;
	void SetShowBaseStartTime(UShowBase* InShowBasePtr, float StartTime);

	TArray<UShowBase*>* EditorGetShowKeys() { return &EditShowSequencerPtr->RuntimeShowKeys; }
	UShowBase* AddKey(FInstancedStruct& NewKey);
	bool RemoveKey(UShowBase* RemoveShowBasePtr);
	int32 FindShowKeyIndex(const FShowKey* ShowKey) const;
	int ShowKeyNum() const { return EditShowSequencerPtr->RuntimeShowKeys.Num(); }

	UScriptStruct* GetShowKeyStaticStruct(UShowBase* ShowBasePtr);
	FShowKey* GetMutableShowKey(UShowBase* ShowBasePtr);

	void NotifyShowKeyChange(const FPropertyChangedEvent& PropertyChangedEvent, FEditPropertyChain* PropertyThatChanged);
	bool ShowSequenceAssetMarkPackageDirty()
	{
		bool DirtyResult = EditShowSequencerPtr->ShowSequenceAsset->MarkPackageDirty();
		return DirtyResult;
	}
	TObjectPtr<UShowSequenceAsset> GetShowSequenceAsset() { return EditShowSequencerPtr ? EditShowSequencerPtr->ShowSequenceAsset : nullptr; }

	void Dispose();
	void HelperNewShowSequencer(TObjectPtr<UShowSequenceAsset> InShowSequenceAsset);
	void SetShowSequencerOwner(AActor* InShowOwner) { EditShowSequencerPtr->ShowOwner = InShowOwner; }
	void Play();
	void ShowSequencerReset();
	void ShowSequencerStop();
	void ShowSequencerClearShowObjects();
	float GetWidgetLengthAlignedToInterval(float Interval);
	void ChangeTimeScale(float scale);

	UShowBase* CheckGetSelectedShowBase();
	bool ValidateRuntimeShowKeys();

	UClass* GetLastSelectedActorClass();
	USkeletalMesh* LoadLastSelectedOrDefaultSkeletalMesh();	
	UClass* GetLastSelectedAnimInstanceClass();
	
	void ReplaceActorPreviewWorld(UClass* ActorClass);
	void ReplaceSkeletalMeshPreviewWorld(USkeletalMesh* SelectedSkeletalMesh);
	void ReplaceAnimInstancePreviewWorld(UClass* AnimInterfaceClass);

	UShowSequencer* EditShowSequencerPtr = nullptr;
	TSharedPtr<SShowMakerWidget> ShowMakerWidget = nullptr;
	UShowBase* SelectedShowBasePtr = nullptr;
	TArray<FObjectPoolTypeSettings> EditorPoolSettings;

	bool bIsCreatedFromHlper = false;

	FShowKeyChange OnShowKeyChange = nullptr;
};

