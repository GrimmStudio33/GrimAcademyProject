// Fill out your copyright notice in the Description page of Project Settings.


#include "EOSManager.h"

#include "Online/OnlineServices.h"
#include "Online/Auth.h"
#include "Online/OnlineAsyncOpHandle.h"
#include "Online/OnlineResult.h"
#include "Online/OnlineError.h"

#include "Engine/GameInstance.h"
#include "Engine/LocalPlayer.h" 

void UEOSManager::Login()
{
    UGameInstance* GameInstance = GetGameInstance();

    if (!GameInstance)
    {
        OnLoginComplete.Broadcast(false, TEXT("GameInstance is invalid."));
        return;
    }

    ULocalPlayer* LocalPlayer = GameInstance->GetFirstGamePlayer();

    if (!LocalPlayer)
    {
        OnLoginComplete.Broadcast(false, TEXT("No local player found."));
        return;
    }

    UE::Online::IOnlineServicesPtr OnlineServices = UE::Online::GetServices();

    if (!OnlineServices.IsValid())
    {
        OnLoginComplete.Broadcast(false, TEXT("Online Services is not available."));
        return;
    }

    UE::Online::IAuthPtr AuthInterface = OnlineServices->GetAuthInterface();

    if (!AuthInterface.IsValid())
    {
        OnLoginComplete.Broadcast(false, TEXT("EOS Auth interface is not available."));
        return;
    }

    UE::Online::FAuthLogin::Params Params;

    Params.PlatformUserId = LocalPlayer->GetPlatformUserId();
    Params.CredentialsType = UE::Online::LoginCredentialsType::AccountPortal;

    AuthInterface->Login(MoveTemp(Params)).OnComplete(
        [this](const auto& Result)
        {
            if (Result.IsOk())
            {
                const TSharedRef<UE::Online::FAccountInfo> AccountInfo =
                    Result.GetOkValue().AccountInfo;

                AccountId = AccountInfo->AccountId;
                bIsLoggedIn = true;

                OnLoginComplete.Broadcast(true, TEXT("EOS login successful."));
            }
            else
            {
                bIsLoggedIn = false;

                const UE::Online::FOnlineError Error =
                    Result.GetErrorValue();

                OnLoginComplete.Broadcast(false, Error.GetLogString());
            }
        }
    );
}

bool UEOSManager::IsLoggedIn() const
{
    return bIsLoggedIn;
}