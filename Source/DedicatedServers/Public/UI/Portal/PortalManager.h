// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/HTTP/HTTPRequestManager.h"
#include "Interfaces/IHttpRequest.h"
#include "PortalManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBroadcastJoinGameSessionMessage, const FString&, StatusMessage);

/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UPortalManager : public UHTTPRequestManager
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintAssignable)
	FBroadcastJoinGameSessionMessage BroadcastJoinGameSessionMessage;
	
	void JoinGameSession();

private:

	void FindOrCreateGameSession_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
};









