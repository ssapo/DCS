// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class DCSTarget : TargetRules
{
	public DCSTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		bUseUnityBuild = true;
		bUsePCHFiles = true;

		ExtraModuleNames.AddRange(new string[] { "DCS" });
	}
}
