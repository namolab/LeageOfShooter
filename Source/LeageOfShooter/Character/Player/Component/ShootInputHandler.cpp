// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootInputHandler.h"

// Sets default values for this component's properties
UShootInputHandler::UShootInputHandler()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UShootInputHandler::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UShootInputHandler::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

