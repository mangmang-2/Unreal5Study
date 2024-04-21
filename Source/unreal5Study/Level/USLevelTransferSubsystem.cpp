// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/USLevelTransferSubsystem.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "MovieSceneSequencePlaybackSettings.h"
#include "MovieSceneSequencePlayer.h"
#include "TimerManager.h"

void UUSLevelTransferSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	ULevelSequence* LevelSequence = LoadObject<ULevelSequence>(nullptr, TEXT("/Script/LevelSequence.LevelSequence'/Game/Study/Level/LevelSequence/MapTransferSequence.MapTransferSequence'"));
	if (LevelSequence)
	{
		FMovieSceneSequencePlaybackSettings PlaybackSettings;
		ALevelSequenceActor* LevelSequenceActor = nullptr;

		LevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
			GetWorld(),
			LevelSequence,
			PlaybackSettings,
			LevelSequenceActor
		);

		if (LevelSequencePlayer)
		{
			LevelSequencePlayer->SetPlaybackPosition(FMovieSceneSequencePlaybackParams(FFrameTime(0), EUpdatePositionMethod::Play));
		}
	}

	ULevelSequence* FadeInOutSequence = LoadObject<ULevelSequence>(nullptr, TEXT("/Script/LevelSequence.LevelSequence'/Game/Study/Level/LevelSequence/FadeInOut.FadeInOut'"));
	if (FadeInOutSequence)
	{
		FMovieSceneSequencePlaybackSettings PlaybackSettings;
		ALevelSequenceActor* LevelSequenceActor = nullptr;

		FadeInOutPlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
			GetWorld(),
			FadeInOutSequence,
			PlaybackSettings,
			LevelSequenceActor
		);
	}
}

void UUSLevelTransferSubsystem::SetTransferSequenceFrameTime(int32 TransferSequenceFrameTime, float DelayTime)
{
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([=]() {
		if (LevelSequencePlayer)
		{
			LevelSequencePlayer->SetPlaybackPosition(FMovieSceneSequencePlaybackParams(FFrameTime(TransferSequenceFrameTime), EUpdatePositionMethod::Play));
		}
		})
		, DelayTime, false);

	
	if (FadeInOutPlayer)
	{
		FadeInOutPlayer->Play();
	}
}
