// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RunTime/ShowSequencer.h"

struct FShowKey;

/**
 * 
 */
class SHOWSYSTEMEDITOR_API FShowSequencerEditorHelper
{
public:
	FShowSequencerEditorHelper();
	~FShowSequencerEditorHelper();

	void SetShowSequencerEditor(UShowSequencer* Sequencer);

	TArray<FShowKey*> GetShowKeys();

	void Play();

	TObjectPtr<UShowSequencer> EditShowSequencer;
};
