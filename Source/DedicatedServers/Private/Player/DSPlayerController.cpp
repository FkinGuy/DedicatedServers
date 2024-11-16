// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DSPlayerController.h"

ADSPlayerController::ADSPlayerController()
{
	SingleTripTime = 0.f;
}

void ADSPlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();
	if (GetNetMode() == NM_Standalone) return;
	if (IsLocalController())
	{
		Server_Ping(GetWorld()->GetTimeSeconds());
	}
}

void ADSPlayerController::Server_Ping_Implementation(float TimeOfRequest)
{
	Client_Pong(TimeOfRequest);
}

void ADSPlayerController::Client_Pong_Implementation(float TimeOfRequest)
{
	const float RoundTripTime = GetWorld()->GetTimeSeconds() - TimeOfRequest;
	SingleTripTime = RoundTripTime * 0.5f;
}


