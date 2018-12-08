// Fill out your copyright notice in the Description page of Project Settings.

#include "threatMeter.h"

// Sets default values
AthreatMeter::AthreatMeter()
{
	threatLimit = 1;
	threat = 0;
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

}

// Called when the game starts or when spawned
void AthreatMeter::BeginPlay()
{
	Super::BeginPlay();

}

void AthreatMeter::ThreatLimitReached() {
}
void AthreatMeter::IncreaseThreat(float raiseAmount) {
	threat += raiseAmount;
}

void AthreatMeter::OnRep_threat() {
	OnThreatChanged.Broadcast(threat);
	if (threat >= threatLimit)
	{
		OnAlarmTriggered.Broadcast();
	}

}
void AthreatMeter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AthreatMeter, threat);
}
// Called every frame
void AthreatMeter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

