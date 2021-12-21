// Fill out your copyright notice in the Description page of Project Settings.


#include "Usable.h"

AUsable::AUsable()
	: Super()
{

}

void AUsable::BeginPlay()
{
	Super::BeginPlay();
}

void AUsable::Interact()
{
	Destroy();
}