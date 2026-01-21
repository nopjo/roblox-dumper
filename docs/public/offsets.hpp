//================================================================
// Dumped By Jonah's Roblox Dumper | Discord: jonahw
// Github Link: https://github.com/nopjo/roblox-dumper
// Contributors: @4hd8, @fulore 
// Dumped at: 2026-01-21 13:49:19
// Roblox Version: version-0f5223fb6e7e4955
// Total offsets dumped: 226
//================================================================

#pragma once
#include <cstdint>

namespace offsets {
    namespace VisualEngine {
        inline constexpr uintptr_t FakeToRealDataModel = 0x1C0;
        inline constexpr uintptr_t Pointer = 0x7A7E950;
        inline constexpr uintptr_t RenderView = 0x800;
        inline constexpr uintptr_t ToFakeDataModel = 0x700;
        inline constexpr uintptr_t ViewMatrix = 0x180;
        inline constexpr uintptr_t WindowDimensions = 0x720;
    }

    namespace RenderView {
        inline constexpr uintptr_t InvalidateLighting = 0x148;
    }

    namespace DataModel {
        inline constexpr uintptr_t ClientReplicator = 0x3D8;
        inline constexpr uintptr_t CreatorId = 0x188;
        inline constexpr uintptr_t GameId = 0x190;
        inline constexpr uintptr_t GameLoaded = 0x5F8;
        inline constexpr uintptr_t JobId = 0x138;
        inline constexpr uintptr_t PlaceId = 0x198;
        inline constexpr uintptr_t RunService = 0x390;
        inline constexpr uintptr_t ServerIP = 0x5E0;
        inline constexpr uintptr_t UserInputService = 0x3A0;
        inline constexpr uintptr_t Workspace = 0x178;
    }

    namespace Instance {
        inline constexpr uintptr_t AttributeContainer = 0x48;
        inline constexpr uintptr_t AttributeList = 0x18;
        inline constexpr uintptr_t AttributeToNext = 0x58;
        inline constexpr uintptr_t AttributeToValue = 0x18;
        inline constexpr uintptr_t ChildrenEnd = 0x8;
        inline constexpr uintptr_t ChildrenStart = 0x70;
        inline constexpr uintptr_t ClassDescriptor = 0x18;
        inline constexpr uintptr_t ClassName = 0x8;
        inline constexpr uintptr_t Name = 0xB0;
        inline constexpr uintptr_t Parent = 0x68;
    }

    namespace Workspace {
        inline constexpr uintptr_t Camera = 0x460;
        inline constexpr uintptr_t Gravity = 0x9B8;
    }

    namespace Players {
        inline constexpr uintptr_t LocalPlayer = 0x130;
    }

    namespace Player {
        inline constexpr uintptr_t Character = 0x370;
        inline constexpr uintptr_t DisplayName = 0x130;
        inline constexpr uintptr_t Team = 0x280;
        inline constexpr uintptr_t UserId = 0x2A8;
    }

    namespace Team {
        inline constexpr uintptr_t Color = 0xD0;
    }

    namespace Part {
        inline constexpr uintptr_t AssemblyAngularVelocity = 0xFC;
        inline constexpr uintptr_t AssemblyLinearVelocity = 0xF0;
        inline constexpr uintptr_t CastShadow = 0xF5;
        inline constexpr uintptr_t Color3 = 0x194;
        inline constexpr uintptr_t Locked = 0xF6;
        inline constexpr uintptr_t Massless = 0xF7;
        inline constexpr uintptr_t Material = 0x246;
        inline constexpr uintptr_t Position = 0xE4;
        inline constexpr uintptr_t Primitive = 0x148;
        inline constexpr uintptr_t PrimitiveFlags = 0x1AE;
        inline constexpr uintptr_t Reflectance = 0xEC;
        inline constexpr uintptr_t Rotation = 0xC0;
        inline constexpr uintptr_t Shape = 0x1B1;
        inline constexpr uintptr_t Size = 0x1B0;
        inline constexpr uintptr_t Transparency = 0xF0;
    }

    namespace PrimitiveFlags {
        inline constexpr uintptr_t Anchored = 0x2;
        inline constexpr uintptr_t CanCollide = 0x8;
        inline constexpr uintptr_t CanTouch = 0x10;
    }

    namespace Humanoid {
        inline constexpr uintptr_t AutoJumpEnabled = 0x1D8;
        inline constexpr uintptr_t AutoRotate = 0x1D9;
        inline constexpr uintptr_t BreakJointsOnDeath = 0x1DB;
        inline constexpr uintptr_t FloorMaterial = 0x190;
        inline constexpr uintptr_t Health = 0x194;
        inline constexpr uintptr_t HealthDisplayDistance = 0x198;
        inline constexpr uintptr_t HipHeight = 0x1A0;
        inline constexpr uintptr_t IsWalking = 0x956;
        inline constexpr uintptr_t Jump = 0x1DD;
        inline constexpr uintptr_t JumpHeight = 0x1AC;
        inline constexpr uintptr_t JumpPower = 0x1B0;
        inline constexpr uintptr_t MaxHealth = 0x1B4;
        inline constexpr uintptr_t MaxSlopeOffset = 0x1B8;
        inline constexpr uintptr_t MoveDirection = 0x158;
        inline constexpr uintptr_t NameDisplayDistance = 0x1BC;
        inline constexpr uintptr_t RequiresNeck = 0x1E0;
        inline constexpr uintptr_t RigType = 0x1C8;
        inline constexpr uintptr_t UseJumpPower = 0x1E3;
        inline constexpr uintptr_t WalkSpeed = 0x1D4;
        inline constexpr uintptr_t WalkSpeedCheck = 0x3C0;
        inline constexpr uintptr_t WalkToPoint = 0x17C;
    }

    namespace Mesh {
        inline constexpr uintptr_t MeshId = 0x2E8;
        inline constexpr uintptr_t TextureId = 0x318;
    }

    namespace Value {
        inline constexpr uintptr_t Value = 0xD0;
    }

    namespace SpecialMesh {
        inline constexpr uintptr_t MeshId = 0x108;
        inline constexpr uintptr_t Scale = 0xDC;
    }

    namespace Lighting {
        inline constexpr uintptr_t Ambient = 0xD8;
        inline constexpr uintptr_t Atmosphere = 0x1E8;
        inline constexpr uintptr_t Brightness = 0x120;
        inline constexpr uintptr_t ClockTime = 0x1B8;
        inline constexpr uintptr_t ColorShift_Bottom = 0xE4;
        inline constexpr uintptr_t ColorShift_Top = 0xF0;
        inline constexpr uintptr_t EnvironmentDiffuseScale = 0x10C;
        inline constexpr uintptr_t EnvironmentSpecularScale = 0x128;
        inline constexpr uintptr_t OutdoorAmbient = 0x108;
        inline constexpr uintptr_t Sky = 0x1D8;
    }

    namespace Sky {
        inline constexpr uintptr_t MoonAngularSize = 0x25C;
        inline constexpr uintptr_t MoonTextureId = 0xE0;
        inline constexpr uintptr_t SkyboxBk = 0x110;
        inline constexpr uintptr_t SkyboxDn = 0x140;
        inline constexpr uintptr_t SkyboxFt = 0x170;
        inline constexpr uintptr_t SkyboxLf = 0x1A0;
        inline constexpr uintptr_t SkyboxOrientation = 0x254;
        inline constexpr uintptr_t SkyboxRt = 0x1D0;
        inline constexpr uintptr_t SkyboxUp = 0x200;
        inline constexpr uintptr_t StarCount = 0x260;
        inline constexpr uintptr_t SunAngularSize = 0x264;
        inline constexpr uintptr_t SunTextureId = 0x230;
    }

    namespace Atmosphere {
        inline constexpr uintptr_t Color = 0xD0;
        inline constexpr uintptr_t Decay = 0xDC;
        inline constexpr uintptr_t Density = 0xE8;
        inline constexpr uintptr_t Glare = 0xEC;
        inline constexpr uintptr_t Haze = 0xF0;
        inline constexpr uintptr_t Offset = 0xF4;
    }

    namespace BloomEffect {
        inline constexpr uintptr_t Intensity = 0xD0;
        inline constexpr uintptr_t Size = 0xD4;
        inline constexpr uintptr_t Threshold = 0xD8;
    }

    namespace SunRaysEffect {
        inline constexpr uintptr_t Intensity = 0xD0;
        inline constexpr uintptr_t Spread = 0xD0;
    }

    namespace ColorCorrectionEffect {
        inline constexpr uintptr_t Brightness = 0xDC;
        inline constexpr uintptr_t Contrast = 0xE0;
        inline constexpr uintptr_t Saturation = 0xE4;
        inline constexpr uintptr_t TintColor = 0xD0;
    }

    namespace DepthOfFieldEffect {
        inline constexpr uintptr_t Density = 0xD0;
        inline constexpr uintptr_t FocusDistance = 0xD4;
        inline constexpr uintptr_t InFocusRadius = 0xD8;
        inline constexpr uintptr_t NearIntensity = 0xDC;
    }

    namespace MouseService {
        inline constexpr uintptr_t InputObject = 0x110;
		inline constexpr uintptr_t Position = 0xEC;
    }

    namespace UIGradient {
        inline constexpr uintptr_t Color = 0x134;
        inline constexpr uintptr_t Offset = 0x158;
        inline constexpr uintptr_t Rotation = 0x160;
        inline constexpr uintptr_t Transparency = 0xF4;
    }

    namespace GuiObject {
        inline constexpr uintptr_t AbsolutePositionX = 0x110;
        inline constexpr uintptr_t AbsolutePositionY = 0x114;
        inline constexpr uintptr_t AbsoluteSizeX = 0x118;
        inline constexpr uintptr_t AbsoluteSizeY = 0x11C;
        inline constexpr uintptr_t AnchorPoint = 0x570;
        inline constexpr uintptr_t BackgroundColor3 = 0x558;
        inline constexpr uintptr_t BackgroundTransparency = 0x57C;
        inline constexpr uintptr_t BorderColor3 = 0x564;
        inline constexpr uintptr_t BorderSizePixel = 0x584;
        inline constexpr uintptr_t LayoutOrder = 0x594;
        inline constexpr uintptr_t Position = 0x528;
        inline constexpr uintptr_t Rotation = 0x188;
        inline constexpr uintptr_t Size = 0x548;
        inline constexpr uintptr_t Visible = 0x5C1;
        inline constexpr uintptr_t ZIndex = 0x5B8;
    }

    namespace TextLabel {
        inline constexpr uintptr_t ContentText = 0xAC8;
        inline constexpr uintptr_t LineHeight = 0xB3C;
        inline constexpr uintptr_t MaxVisibleGraphemes = 0xEFC;
        inline constexpr uintptr_t Text = 0xE28;
        inline constexpr uintptr_t TextBounds = 0xD00;
        inline constexpr uintptr_t TextColor3 = 0xED8;
        inline constexpr uintptr_t TextSize = 0xD04;
        inline constexpr uintptr_t TextStrokeColor3 = 0xEE4;
        inline constexpr uintptr_t TextStrokeTransparency = 0xF08;
        inline constexpr uintptr_t TextTransparency = 0xF0C;
    }

    namespace TextButton {
        inline constexpr uintptr_t ContentText = 0xD48;
        inline constexpr uintptr_t LineHeight = 0xDBC;
        inline constexpr uintptr_t Text = 0x10A8;
        inline constexpr uintptr_t TextBounds = 0xF80;
        inline constexpr uintptr_t TextColor3 = 0x1158;
        inline constexpr uintptr_t TextSize = 0xF84;
        inline constexpr uintptr_t TextStrokeColor3 = 0x1164;
        inline constexpr uintptr_t TextTransparency = 0x118C;
    }

    namespace TextBox {
        inline constexpr uintptr_t ContentText = 0xAC0;
        inline constexpr uintptr_t LineHeight = 0xB34;
        inline constexpr uintptr_t Text = 0xE20;
        inline constexpr uintptr_t TextBounds = 0xCF8;
        inline constexpr uintptr_t TextColor3 = 0xEFC;
        inline constexpr uintptr_t TextSize = 0xCFC;
        inline constexpr uintptr_t TextStrokeColor3 = 0xF08;
        inline constexpr uintptr_t TextTransparency = 0xF40;
    }

    namespace Tool {
        inline constexpr uintptr_t CanBeDropped = 0x4A0;
        inline constexpr uintptr_t Enabled = 0x4A1;
        inline constexpr uintptr_t Grip = 0x470;
        inline constexpr uintptr_t GripForward = 0x488;
        inline constexpr uintptr_t GripPos = 0x494;
        inline constexpr uintptr_t GripRight = 0x470;
        inline constexpr uintptr_t GripUp = 0x47C;
        inline constexpr uintptr_t ManualActivationOnly = 0x4A2;
        inline constexpr uintptr_t RequiresHandle = 0x4A3;
        inline constexpr uintptr_t ToolTip = 0x450;
    }

    namespace Highlight {
        inline constexpr uintptr_t DepthMode = 0xF8;
        inline constexpr uintptr_t FillColor = 0xE0;
        inline constexpr uintptr_t FillTransparency = 0xFC;
        inline constexpr uintptr_t OutlineColor = 0xEC;
        inline constexpr uintptr_t OutlineTransparency = 0xF0;
    }

    namespace ProximityPrompt {
        inline constexpr uintptr_t ActionText = 0xD0;
        inline constexpr uintptr_t Enabled = 0x156;
        inline constexpr uintptr_t HoldDuration = 0x140;
        inline constexpr uintptr_t KeyboardKeyCode = 0x144;
        inline constexpr uintptr_t MaxActivationDistance = 0x148;
        inline constexpr uintptr_t ObjectText = 0xF0;
        inline constexpr uintptr_t RequiresLineOfSight = 0x157;
    }

    namespace ByteCode {
        inline constexpr uintptr_t Pointer = 0x10;
        inline constexpr uintptr_t Size = 0x20;
    }

    namespace ModuleScript {
        inline constexpr uintptr_t Bytecode = 0x150;
        inline constexpr uintptr_t Hash = 0x160;
    }

    namespace LocalScript {
        inline constexpr uintptr_t Bytecode = 0x1A8;
        inline constexpr uintptr_t Hash = 0x1B8;
    }

    namespace Terrain {
        inline constexpr uintptr_t GrassLength = 0x1F8;
        inline constexpr uintptr_t MaterialColors = 0x280;
        inline constexpr uintptr_t WaterColor = 0x1E8;
        inline constexpr uintptr_t WaterReflectance = 0x200;
        inline constexpr uintptr_t WaterTransparency = 0x204;
        inline constexpr uintptr_t WaterWaveSize = 0x208;
        inline constexpr uintptr_t WaterWaveSpeed = 0x20C;
    }

    namespace MaterialColors {
        inline constexpr uintptr_t Asphalt = 0x10;
        inline constexpr uintptr_t Basalt = 0xD;
        inline constexpr uintptr_t Brick = 0x5;
        inline constexpr uintptr_t Cobblestone = 0x11;
        inline constexpr uintptr_t Concrete = 0x4;
        inline constexpr uintptr_t CrackedLava = 0xF;
        inline constexpr uintptr_t Glacier = 0x9;
        inline constexpr uintptr_t Grass = 0x2;
        inline constexpr uintptr_t Ground = 0xE;
        inline constexpr uintptr_t Ice = 0x12;
        inline constexpr uintptr_t LeafyGrass = 0x13;
        inline constexpr uintptr_t Limestone = 0x15;
        inline constexpr uintptr_t Mud = 0xC;
        inline constexpr uintptr_t Pavement = 0x16;
        inline constexpr uintptr_t Rock = 0x8;
        inline constexpr uintptr_t Salt = 0x14;
        inline constexpr uintptr_t Sand = 0x6;
        inline constexpr uintptr_t Sandstone = 0xB;
        inline constexpr uintptr_t Slate = 0x3;
        inline constexpr uintptr_t Snow = 0xA;
        inline constexpr uintptr_t WoodPlanks = 0x7;
    }

    namespace Camera {
        inline constexpr uintptr_t FieldOfView = 0x160;
        inline constexpr uintptr_t Position = 0x11C;
        inline constexpr uintptr_t Rotation = 0xF8;
    }
}
