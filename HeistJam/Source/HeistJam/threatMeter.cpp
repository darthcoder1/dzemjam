// Fill out your copyright notice in the Description page of Project Settings.

#include "threatMeter.h"

// Sets default values
AthreatMeter::AthreatMeter()
{
	threatLimit = 1;
	threat = 0;
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AthreatMeter::BeginPlay()
{
	Super::BeginPlay();


}

void AthreatMeter::ThreatLimitReached() {
	if (threat >= threatLimit) 
		{
		//alarm
		}
	}
void AthreatMeter::IncreaseThreat(float raiseAmount) {
	threat += raiseAmount;
}


// Called every frame
void AthreatMeter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

