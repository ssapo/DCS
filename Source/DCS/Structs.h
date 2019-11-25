#pragma once

#include "CoreMinimal.h"
#include "Vector.h"
#include "Array.h"
#include "Enumerations.h"
#include "Text.h"
#include "NoExportTypes.h"
#include "NameTypes.h"
#include "WeakObjectPtrTemplates.h"
#include "SharedPointer.h"
#include "Structs.generated.h"

class UAnimMontage;
class UTexture2D;
class AActor;
class UPrimitiveComponent;
class ADisplayedItem;
class UMaterialInterface;
class UMaterialInstanceDynamic;
struct FStoredItem;
class FName;
class UItemBase;

USTRUCT()
struct FCollCompHitActors
{
	GENERATED_BODY()

public:
	FCollCompHitActors()
		: Component(nullptr)
		, HitActors()
	{}

	UPrimitiveComponent* Component;
	TArray<TWeakObjectPtr<AActor>> HitActors;
};

USTRUCT()
struct FCollisionComponent
{
	GENERATED_BODY()

public:
	FCollisionComponent()
		: Component(nullptr)
		, Sockets()
	{}

	UPrimitiveComponent* Component;
	TArray<FName> Sockets;
};

USTRUCT()
struct FDisplayedItems
{
	GENERATED_BODY()

	FDisplayedItems()
		: DisplayedItems()
	{}

	TArray<TWeakObjectPtr<ADisplayedItem>> DisplayedItems;
};

USTRUCT()
struct FDissolvedItems
{
	GENERATED_BODY()

public:
	FDissolvedItems()
		:Component(nullptr)
		, Value(0)
		, Materials()
		, DissolveMaterials()
		, Reverse(false)
		, IsRunning(false)
	{}

	UPrimitiveComponent* Component;
	float Value;
	TArray<TWeakObjectPtr<UMaterialInterface>> Materials;
	TArray<TWeakObjectPtr<UMaterialInstanceDynamic>> DissolveMaterials;
	bool Reverse;
	bool IsRunning;
};

USTRUCT()
struct FEffect
{
	GENERATED_BODY()

public:
	FEffect()
		: Type(EEffect::Stun)
		, Duration(0)
		, Applier(nullptr)
	{}

	EEffect Type;
	float Duration;
	TWeakObjectPtr<AActor> Applier;
};

USTRUCT()
struct FEquiopmentSlot
{
	GENERATED_BODY()

public:
	FEquiopmentSlot()
		: Items()
		, ActiveItemIndex(0)
		, IsHidden(false)
	{}

	TArray<FStoredItem> Items;
	int32 ActiveItemIndex;
	bool IsHidden;
};

USTRUCT()
struct FEquiopmentSlots
{
	GENERATED_BODY()
	FEquiopmentSlots()
		: Type(EItem::None)
		, Slots()
	{}

	EItem Type;
	TArray<FEquiopmentSlot> Slots;
};

USTRUCT()
struct FHitData
{
	GENERATED_BODY()

public:
	FHitData()
		: Damage(0)
		, DamageCauser(nullptr)
		, HitFromDirection(FVector::ZeroVector)
		, CanBeParried(false)
		, CanBeBlocked(true)
		, CanReceiveImpact(false)
	{}

	float Damage;
	TWeakObjectPtr<AActor> DamageCauser;
	FVector HitFromDirection;
	bool CanBeParried;
	bool CanBeBlocked;
	bool CanReceiveImpact;
};

USTRUCT()
struct FItem
{
	GENERATED_BODY()

public:
	FItem()
		: Name("None")
		, Description(FText::FromString(TEXT("Item Description")))
		, Type(EItem::None)
		, IsStackable(false)
		, IsDroppable(true)
		, IsConsumable(false)
		, Image(nullptr)
	{}

	FName Name;
	FText Description;
	EItem Type;
	bool IsStackable;
	bool IsDroppable;
	bool IsConsumable;
	TWeakObjectPtr<UTexture2D> Image;
};

USTRUCT()
struct FModifier
{
	GENERATED_BODY()

public:
	FModifier()
		: Type(EStat::None)
		, Value(0)
	{}

	EStat Type;
	float Value;
};

USTRUCT()
struct FMontageAction
{
	GENERATED_BODY()

public:
	FMontageAction()
		: Action(EMontage::None)
		, Montages()
	{}

	EMontage Action;
	TArray<TWeakObjectPtr<UAnimMontage>> Montages;
};

USTRUCT()
struct FStat
{
	GENERATED_BODY()

public:
	FStat()
		: Type(EStat::None)
		, BaseValue(0)
		, ModifiersValue(0)
	{}

	UPROPERTY(EditAnywhere)
		EStat Type;

	UPROPERTY(EditAnywhere)
		float BaseValue;

	UPROPERTY(EditAnywhere)
		float ModifiersValue;
};

USTRUCT()
struct FStoredItem
{
	GENERATED_BODY()

public:
	FStoredItem()
		: Id()
		, ItemClass(nullptr)
		, Amount(0)
	{}

	UPROPERTY(EditAnywhere)
		FGuid Id;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UItemBase> ItemClass;

	UPROPERTY(EditAnywhere)
		int32 Amount;
};

USTRUCT()
struct FDemoRoomInfo
{
	GENERATED_BODY()

public:
	FDemoRoomInfo()
		: RoomName()
		, RoomType(EDemoRoom::Standard)
		, OverrideSize(0)
	{}

	UPROPERTY(EditAnywhere)
		FText RoomName;

	UPROPERTY(EditAnywhere)
		EDemoRoom RoomType;

	UPROPERTY(EditAnywhere)
		int32 OverrideSize;
};