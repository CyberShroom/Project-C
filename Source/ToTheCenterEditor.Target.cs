// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class ToTheCenterEditorTarget : TargetRules
{
	public ToTheCenterEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V4;

		ExtraModuleNames.AddRange( new string[] { "ToTheCenter","SDIO_UI","SDIO_Inventory","TTC_UI","TTC_Inventory" } );
    }
}

/*
+ClassRedirects=(OldName="/Script/ToTheCenter.UI_Master",NewName="/Script/SDIO_UI.UI_Master")
+ClassRedirects=(OldName="/Script/ToTheCenter.UI_Manager",NewName="/Script/SDIO_UI.Manager")
+ClassRedirects=(OldName="/Script/ToTheCenter.UI_Controller",NewName="/Script/SDIO_UI.UI_Window")
+EnumRedirects=(OldName="/Script/ToTheCenter.ESDIO_UI_Enums",NewName="/Script/SDIO_UI.ESDIO_UI_Enums")
+ClassRedirects=(OldName="/Script/ToTheCenter.Item_Slot",NewName="/Script/SDIO_Inventory.Item_Slot")
+ClassRedirects=(OldName="/Script/ToTheCenter.TTC_Item",NewName="/Script/SDIO_Inventory.SDIO_Item")
+EnumRedirects=(OldName="/Script/ToTheCenter.ETTC_Enums",NewName="/Script/SDIO_Inventory.ESDIO_Inventory_Enums")
+ClassRedirects=(OldName="/Script/ToTheCenter.Inventory_Slot",NewName="/Script/SDIO_Inventory.Button_Item_Slot")
+ClassRedirects=(OldName="/Script/ToTheCenter.SDIO_UIManager_Game",NewName="/Script/TTC_UI.SDIO_UIManager_Game")
+ClassRedirects=(OldName="/Script/ToTheCenter.SDIO_UIManager_Lobby",NewName="/Script/TTC_UI.SDIO_UIManager_Lobby")
+ClassRedirects=(OldName="/Script/ToTheCenter.SDIO_UIManager_Title",NewName="/Script/TTC_UI.SDIO_UIManager_Title")
+ClassRedirects=(OldName="/Script/ToTheCenter.SDIO_UIWindow_Inventory",NewName="/Script/TTC_UI.SDIO_UIWindow_Inventory")
+ClassRedirects=(OldName="/Script/ToTheCenter.SDIO_UIWindow_Lobby",NewName="/Script/TTC_UI.SDIO_UIWindow_Lobby")
+ClassRedirects=(OldName="/Script/ToTheCenter.SDIO_UIWindow_Multiplayer",NewName="/Script/TTC_UI.SDIO_UIWindow_Multiplayer")
+ClassRedirects=(OldName="/Script/ToTheCenter.SDIO_UIWindow_Options",NewName="/Script/TTC_UI.SDIO_UIWindow_Options")
+ClassRedirects=(OldName="/Script/ToTheCenter.SDIO_UIWindow_Title",NewName="/Script/TTC_UI.SDIO_UIWindow_Title")
+ClassRedirects=(OldName="/Script/ToTheCenter.TTC_Item",NewName="/Script/TTC_Inventory.TTC_Item")
+ClassRedirects=(OldName="/Script/ToTheCenter.TextButton",NewName="/Script/SDIO_UI.SDIO_UI_TextButton")
+ClassRedirects=(OldName="/Script/ToTheCenter.UI_TextButton",NewName="/Script/SDIO_UI.SDIO_UI_TextButton")
+ClassRedirects=(OldName="/Script/ToTheCenter.SDIO_UI_TextButton",NewName="/Script/SDIO_UI.SDIO_UI_TextButton")
*/