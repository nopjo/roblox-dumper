//================================================================
// Dumped By Jonah's Roblox Dumper | Discord: jonahw
// Github Link: https://github.com/nopjo/roblox-dumper
// Dumped at: 2025-12-18 12:57:18
// Roblox Version: version-eaebe135b3ec446b
// Total offsets dumped: 143
//================================================================

#pragma once
#include <cstdint>

namespace offsets {
    namespace VisualEngine {
        inline constexpr uintptr_t FakeToRealDataModel = 0x1C0;
        inline constexpr uintptr_t Pointer = 0x7A684F0;
        inline constexpr uintptr_t RenderView = 0x800;
        inline constexpr uintptr_t ToFakeDataModel = 0x700;
        inline constexpr uintptr_t ViewMatrix = 0x180;
        inline constexpr uintptr_t WindowDimensions = 0x720;
    }

    namespace RenderView {
        inline constexpr uintptr_t InvalidateLighting = 0x148;
    }

    namespace DataModel {
        inline constexpr uintptr_t ClientReplicator = 0x3E0;
        inline constexpr uintptr_t CreatorId = 0x188;
        inline constexpr uintptr_t GameId = 0x190;
        inline constexpr uintptr_t PlaceId = 0x198;
        inline constexpr uintptr_t RunService = 0x398;
        inline constexpr uintptr_t UserInputService = 0x3A8;
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
        inline constexpr uintptr_t Camera = 0x450;
        inline constexpr uintptr_t Gravity = 0x9B0;
    }

    namespace Players {
        inline constexpr uintptr_t LocalPlayer = 0x130;
    }

    namespace Player {
        inline constexpr uintptr_t DisplayName = 0x130;
        inline constexpr uintptr_t ModelInstance = 0x360;
        inline constexpr uintptr_t Team = 0x270;
        inline constexpr uintptr_t UserId = 0x298;
    }

    namespace Team {
        inline constexpr uintptr_t Color = 0xD0;
    }

    namespace Part {
        inline constexpr uintptr_t AssemblyAngularVelocity = 0xFC;
        inline constexpr uintptr_t AssemblyLinearVelocity = 0xF0;
        inline constexpr uintptr_t Color3 = 0x194;
        inline constexpr uintptr_t Material = 0x226;
        inline constexpr uintptr_t Position = 0xE4;
        inline constexpr uintptr_t Primitive = 0x148;
        inline constexpr uintptr_t PrimitiveFlags = 0x1AE;
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
        inline constexpr uintptr_t Health = 0x194;
        inline constexpr uintptr_t HealthDisplayDistance = 0x198;
        inline constexpr uintptr_t HipHeight = 0x1A0;
        inline constexpr uintptr_t IsWalking = 0x956;
        inline constexpr uintptr_t JumpPower = 0x1B0;
        inline constexpr uintptr_t MaxHealth = 0x1B4;
        inline constexpr uintptr_t MaxSlopeOffset = 0x1B8;
        inline constexpr uintptr_t NameDisplayDistance = 0x1BC;
        inline constexpr uintptr_t RigType = 0x1C8;
        inline constexpr uintptr_t WalkSpeed = 0x1D4;
        inline constexpr uintptr_t WalkSpeedCheck = 0x3C0;
        inline constexpr uintptr_t WalkToPoint = 0x17C;
    }

    namespace Mesh {
        inline constexpr uintptr_t MeshId = 0x2E0;
        inline constexpr uintptr_t TextureId = 0x310;
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
        inline constexpr uintptr_t InputObject = 0x100;
        inline constexpr uintptr_t Position = 0xEC;
    }

    namespace UIGradient {
        inline constexpr uintptr_t Color = 0x134;
        inline constexpr uintptr_t Offset = 0x158;
        inline constexpr uintptr_t Rotation = 0x160;
        inline constexpr uintptr_t Transparency = 0xF4;
    }

    namespace GuiObject {
        inline constexpr uintptr_t AnchorPoint = 0x568;
        inline constexpr uintptr_t BackgroundColor3 = 0x550;
        inline constexpr uintptr_t BackgroundTransparency = 0x574;
        inline constexpr uintptr_t BorderColor3 = 0x55C;
        inline constexpr uintptr_t BorderSizePixel = 0x57C;
        inline constexpr uintptr_t LayoutOrder = 0x58C;
        inline constexpr uintptr_t Position = 0x520;
        inline constexpr uintptr_t Rotation = 0x188;
        inline constexpr uintptr_t Size = 0x540;
        inline constexpr uintptr_t ZIndex = 0x5B0;
    }

    namespace TextLabel {
        inline constexpr uintptr_t ContentText = 0xAE0;
        inline constexpr uintptr_t LineHeight = 0xB54;
        inline constexpr uintptr_t MaxVisibleGraphemes = 0xF14;
        inline constexpr uintptr_t Text = 0xE40;
        inline constexpr uintptr_t TextBounds = 0xD18;
        inline constexpr uintptr_t TextColor3 = 0xEF0;
        inline constexpr uintptr_t TextSize = 0xD1C;
        inline constexpr uintptr_t TextStrokeColor3 = 0xEFC;
        inline constexpr uintptr_t TextStrokeTransparency = 0xF20;
        inline constexpr uintptr_t TextTransparency = 0xF24;
    }

    namespace Tool {
        inline constexpr uintptr_t CanBeDropped = 0x490;
        inline constexpr uintptr_t Enabled = 0x491;
        inline constexpr uintptr_t Grip = 0x460;
        inline constexpr uintptr_t GripForward = 0x478;
        inline constexpr uintptr_t GripPos = 0x484;
        inline constexpr uintptr_t GripRight = 0x460;
        inline constexpr uintptr_t GripUp = 0x46C;
        inline constexpr uintptr_t ManualActivationOnly = 0x492;
        inline constexpr uintptr_t RequiresHandle = 0x493;
        inline constexpr uintptr_t ToolTip = 0x440;
    }

    namespace Camera {
        inline constexpr uintptr_t FieldOfView = 0x160;
        inline constexpr uintptr_t Position = 0x11C;
        inline constexpr uintptr_t Rotation = 0xF8;
    }
}
