using UnrealBuildTool;

public class RoguelikeFPS : ModuleRules
{
    public RoguelikeFPS(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(new string[] { "RoguelikeFPS" });

        // 이 부분에 "EnhancedInput"과 다른 모듈들이 공용으로 들어갔을 것으로 추정합니다.
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "AIModule", "NavigationSystem", "GameplayTasks", "UMG" });

        PrivateDependencyModuleNames.AddRange(new string[] { "EnhancedInput" });

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");
    }
}