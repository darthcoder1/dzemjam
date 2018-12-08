// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "threatMeter.generated.h"

UCLASS(BlueprintType, Blueprintable)
class HEISTJAM_API AthreatMeter : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AthreatMeter();
	const float getThreat() { return threat; }
	void setThreat(float threat) { this->threat = threat; }
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Threat")
		float threatLimit;
	UPROPERTY(BlueprintReadWrite, Category = "Threat")
		float threat;
	UFUNCTION(BlueprintCallable, Category = "Threat")
		void IncreaseThreat(float raiseAmount);
	UFUNCTION(BlueprintCallable, Category = "Threat")
		void ThreatLimitReached();


	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnThreatChangedSignature, float , newThreat);
	UPROPERTY(BlueprintAssignable)
		FOnThreatChangedSignature OnThreatChanged;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
