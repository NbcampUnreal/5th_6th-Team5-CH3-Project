#include "AugmentSystem.h"
#include "FPSCharacter.h"

void UAugmentSystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    UE_LOG(LogTemp, Log, TEXT("AugmentSystem Initialized"));
}

void UAugmentSystem::Deinitialize()
{
    Super::Deinitialize();
    UE_LOG(LogTemp, Log, TEXT("AugmentSystem Deinitialized"));
}

TArray<FAugmentData> UAugmentSystem::GetRandomAugments(int32 Count)
{
    TArray<FAugmentData*> AllAugments;
    if (!AugmentDataTable)
    {
        UE_LOG(LogTemp, Warning, TEXT("AugmentDataTable is null"));
        return TArray<FAugmentData>();
    }
    AugmentDataTable->GetAllRows(TEXT(""), AllAugments);

    TArray<FAugmentData> Selected;
    float TotalWeight = 0.0f;
    for (FAugmentData* Aug : AllAugments)
    {
        TotalWeight += RarityWeights.FindRef(Aug->Rarity);
    }

    TArray<FAugmentData*> Remaining = AllAugments;
    for (int32 i = 0; i < Count && Remaining.Num() > 0; ++i)
    {
        float Rand = FMath::FRand() * TotalWeight;
        float Cumul = 0.0f;
        for (int32 j = 0; j < Remaining.Num(); ++j)
        {
            Cumul += RarityWeights.FindRef(Remaining[j]->Rarity);
            if (Rand <= Cumul)
            {
                Selected.Add(*Remaining[j]);
                TotalWeight -= RarityWeights.FindRef(Remaining[j]->Rarity);
                Remaining.RemoveAt(j);
                break;
            }
        }
    }
    return Selected;
}

void UAugmentSystem::ApplyAugment(AFPSCharacter* Character, FName AugmentID)
{
    if (!Character || !AugmentDataTable)
    {
        UE_LOG(LogTemp, Warning, TEXT("ApplyAugment: Invalid Character or AugmentDataTable"));
        return;
    }
    FAugmentData* Data = AugmentDataTable->FindRow<FAugmentData>(AugmentID, TEXT(""));
    if (Data)
    {
        Character->ApplyAugment(AugmentID);
        UE_LOG(LogTemp, Log, TEXT("Applied Augment: %s"), *AugmentID.ToString());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("AugmentID %s not found in DataTable"), *AugmentID.ToString());
    }
}