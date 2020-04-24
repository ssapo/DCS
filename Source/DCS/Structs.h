#pragma once

#include "CoreMinimal.h"
#include "Vector.h"
#include "Array.h"
#include "Enumerations.h"
#include "Text.h"
#include "NoExportTypes.h"
#include "NameTypes.h"
#include "SubclassOf.h"
#include "Structs.generated.h"

class UAnimMontage;
class UTexture2D;
class AActor;
class UPrimitiveComponent;
class ADisplayedItem;
class UMaterialInterface;
class UMaterialInstanceDynamic;
class UItemBase;
struct FStoredItem;

USTRUCT()
struct FCollCompHitActors
{
	GENERATED_BODY()

public:
	FCollCompHitActors();

	UPROPERTY(Transient)
		UPrimitiveComponent* Component;

	UPROPERTY(Transient)
		TArray<AActor*> HitActors;
};

USTRUCT()
struct FCollisionComponent
{
	GENERATED_BODY()

public:
	FCollisionComponent();

	UPROPERTY(Transient)
		UPrimitiveComponent* Component;

	UPROPERTY(Transient)
		TArray<FName> Sockets;
};

USTRUCT()
struct FDisplayedItems
{
	GENERATED_BODY()

public:
	FDisplayedItems();

	UPROPERTY(Transient)
		TArray<ADisplayedItem*> DisplayedItemActors;
};

USTRUCT()
struct FDissolvedItems
{
	GENERATED_BODY()

public:
	FDissolvedItems();

	UPROPERTY(Transient)
		UPrimitiveComponent* Component;

	UPROPERTY(Transient)
		float Value;

	UPROPERTY(Transient)
		TArray<UMaterialInterface*> Materials;

	UPROPERTY(Transient)
		TArray<UMaterialInstanceDynamic*> DissolveMaterials;

	UPROPERTY(Transient)
		bool Reverse;

	UPROPERTY(Transient)
		bool IsRunning;
};

USTRUCT()
struct FEffect
{
	GENERATED_BODY()

public:
	FEffect();

	UPROPERTY(Transient)
		EEffect Type;

	UPROPERTY(Transient)
		float Duration;

	UPROPERTY(Transient)
		AActor* Applier;
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
struct FEquipmentSlot
{
	GENERATED_BODY()

public:
	FEquipmentSlot();

	UPROPERTY(EditAnywhere)
		TArray<FStoredItem> Items;

	UPROPERTY(EditAnywhere)
		int32 ActiveItemIndex;

	UPROPERTY(EditAnywhere)
		bool IsHidden;
};

USTRUCT()
struct FEquipmentSlots
{
	GENERATED_BODY()

public:
	FEquipmentSlots();

	UPROPERTY(EditAnywhere)
		EItem Type;

	UPROPERTY(EditAnywhere)
		TArray<FEquipmentSlot> Slots;
};

USTRUCT()
struct FHitData
{
	GENERATED_BODY()

public:
	FHitData();

	UPROPERTY(Transient)
		float Damage;

	UPROPERTY(Transient)
		AActor* DamageCauser;

	UPROPERTY(Transient)
		FVector HitFromDirection;

	UPROPERTY(Transient)
		bool CanBeParried;

	UPROPERTY(Transient)
		bool CanBeBlocked;

	UPROPERTY(Transient)
		bool CanReceiveImpact;
};

USTRUCT()
struct FItem
{
	GENERATED_BODY()

public:
	FItem();

	UPROPERTY(EditAnywhere)
		FName Name;

	UPROPERTY(EditAnywhere)
		FText Description;

	UPROPERTY(EditAnywhere)
		EItem Type;

	UPROPERTY(EditAnywhere)
		bool IsStackable;

	UPROPERTY(EditAnywhere)
		bool IsDroppable;

	UPROPERTY(EditAnywhere)
		bool IsConsumable;

	UPROPERTY(EditAnywhere)
		UTexture2D* Image;
};

USTRUCT()
struct FModifier
{
	GENERATED_BODY()

public:
	FModifier();

	UPROPERTY(Transient)
		EStat Type;

	UPROPERTY(Transient)
		float Value;
};

USTRUCT()
struct FStat
{
	GENERATED_BODY()

public:
	FStat();

	FStat(EStat InStat, float InBase, float InModifiersValue);

	UPROPERTY(EditAnywhere)
		EStat Type;

	UPROPERTY(EditAnywhere)
		float BaseValue;

	UPROPERTY(EditAnywhere)
		float ModifiersValue;
};

USTRUCT()
struct FDemoRoomInfo
{
	GENERATED_BODY()

public:
	FDemoRoomInfo();

	UPROPERTY(EditAnywhere)
		FText RoomName;

	UPROPERTY(EditAnywhere)
		EDemoRoom RoomType;

	UPROPERTY(EditAnywhere)
		int32 OverrideSize;
};