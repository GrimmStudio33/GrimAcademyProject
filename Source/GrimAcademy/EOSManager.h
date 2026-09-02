// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Online/Auth.h"
#include "EOSManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
    FOnEOSLoginComplete,
    bool, bSuccess,
    FString, ErrorMessage
);

UCLASS()
class GRIMACADEMY_API UEOSManager : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:

    UPROPERTY(BlueprintAssignable, Category = "EOS")
    FOnEOSLoginComplete OnLoginComplete;

    UFUNCTION(BlueprintCallable, Category = "EOS")
    void Login();

    UFUNCTION(BlueprintPure, Category = "EOS")
    bool IsLoggedIn() const;

private:

    bool bIsLoggedIn = false;

    UE::Online::FAccountId AccountId;
};