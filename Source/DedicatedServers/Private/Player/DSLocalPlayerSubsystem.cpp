// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DSLocalPlayerSubsystem.h"

#include "UI/Portal/PortalManager.h"

void UDSLocalPlayerSubsystem::InitializeTokens(const FDSAuthenticationResult& AuthResult, UPortalManager* Manager)
{
	AuthenticationResult = AuthResult;
	PortalManager = Manager;
	SetRefreshTokenTimer();
}

void UDSLocalPlayerSubsystem::SetRefreshTokenTimer()
{
	UWorld* World = GetWorld();
	if (IsValid(World))
	{
		FTimerDelegate RefreshDelegate;
		RefreshDelegate.BindLambda([this](){ PortalManager->RefreshTokens(AuthenticationResult.RefreshToken); });
		World->GetTimerManager().SetTimer(RefreshTimer, RefreshDelegate, TokenRefreshInterval, false);
	}
}

void UDSLocalPlayerSubsystem::UpdateTokens(const FString& AccessToken, const FString& IdToken)
{
	AuthenticationResult.AccessToken = AccessToken;
	AuthenticationResult.IdToken = IdToken;
	AuthenticationResult.Dump();
	SetRefreshTokenTimer();
}

FDSAuthenticationResult UDSLocalPlayerSubsystem::GetAuthResult() const
{
	return AuthenticationResult;
}
