// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"
#include "GameFramework/Actor.h"

FItem UItemBase::GetItemRef() const
{
	return Item;
}
