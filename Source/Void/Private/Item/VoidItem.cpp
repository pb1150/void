// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/VoidItem.h"


AVoidItem::AVoidItem()
{
	PrimaryActorTick.bCanEverTick = false;

}


void AVoidItem::BeginPlay()
{
	Super::BeginPlay();
	
}


void AVoidItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

