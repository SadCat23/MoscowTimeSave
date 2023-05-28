// Fill out your copyright notice in the Description page of Project Settings.


#include "AIHelperLib.h"
#include "Perception/AISenseConfig_Sight.h"

void UAIHelperLib::SetAISightPerception(UAIPerceptionComponent* Comp, float MaxRadius, float AngleView)
{
    if(!Comp)return;
    const FAISenseID SightSenseID = UAISense::GetSenseID(UAISense_Sight::StaticClass());
    UAISenseConfig* Config = Comp->GetSenseConfig(SightSenseID);
    if(!Config) return;
    UAISenseConfig_Sight* SightConfig = Cast<UAISenseConfig_Sight>(Config);
    if(!SightConfig) return;
    SightConfig->SightRadius=MaxRadius;
    SightConfig->LoseSightRadius=MaxRadius;
    SightConfig->PeripheralVisionAngleDegrees = AngleView;
    Comp->ConfigureSense(*SightConfig);
    
}
