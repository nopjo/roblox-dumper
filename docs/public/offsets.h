/*
 * Dumped With: roblox-dumper 2.4
 * Created by: Jonah (jonahw on Discord)
 * Github: https://github.com/nopjo/roblox-dumper
 * Roblox Version: version-bd08027bb04e4045
 * Time Taken: 3991 ms (3.991000 seconds)
 * Total Offsets: 250
 */

#pragma once
#include <cstdint>

// clang-format off
namespace offsets {
    inline constexpr const char* roblox_version = "version-bd08027bb04e4045";

    namespace Atmosphere {
        inline constexpr uintptr_t Color = 0xD0;
        inline constexpr uintptr_t Decay = 0xDC;
        inline constexpr uintptr_t Density = 0xE8;
        inline constexpr uintptr_t Glare = 0xEC;
        inline constexpr uintptr_t Haze = 0xF0;
        inline constexpr uintptr_t Offset = 0xF4;
    }

    namespace BasePart {
        inline constexpr uintptr_t CastShadow = 0xF5;
        inline constexpr uintptr_t Color3 = 0x194;
        inline constexpr uintptr_t Locked = 0xF6;
        inline constexpr uintptr_t Massless = 0xF7;
        inline constexpr uintptr_t Primitive = 0x148;
        inline constexpr uintptr_t Reflectance = 0xEC;
        inline constexpr uintptr_t Shape = 0x1B1;
        inline constexpr uintptr_t Transparency = 0xF0;
    }

    namespace BloomEffect {
        inline constexpr uintptr_t Intensity = 0xD0;
        inline constexpr uintptr_t Size = 0xD4;
        inline constexpr uintptr_t Threshold = 0xD8;
    }

    namespace ByteCode {
        inline constexpr uintptr_t Pointer = 0x10;
        inline constexpr uintptr_t Size = 0x20;
    }

    namespace Camera {
        inline constexpr uintptr_t CFrame = 0xF8;
        inline constexpr uintptr_t FieldOfView = 0x160;
        inline constexpr uintptr_t Position = 0x11C;
        inline constexpr uintptr_t Rotation = 0xF8;
        inline constexpr uintptr_t ViewportInt16 = 0x2AC;
        inline constexpr uintptr_t ViewportSize = 0x2E8;
    }

    namespace CharacterMesh {
        inline constexpr uintptr_t BaseTextureId = 0xD0;
        inline constexpr uintptr_t BodyPart = 0xE8;
        inline constexpr uintptr_t MeshId = 0xD8;
        inline constexpr uintptr_t OverlayTextureId = 0xE0;
    }

    namespace DataModel {
        inline constexpr uintptr_t CreatorId = 0x188;
        inline constexpr uintptr_t GameId = 0x190;
        inline constexpr uintptr_t GameLoaded = 0x5F8;
        inline constexpr uintptr_t JobId = 0x138;
        inline constexpr uintptr_t PlaceId = 0x198;
        inline constexpr uintptr_t ServerIP = 0x5E0;
        inline constexpr uintptr_t Workspace = 0x178;
    }

    namespace FakeDataModel {
        inline constexpr uintptr_t Pointer = 0x7D909F8;
        inline constexpr uintptr_t RealDataModel = 0x1C0;
    }

    namespace GuiBase2D {
        inline constexpr uintptr_t AbsolutePosition = 0x10C;
        inline constexpr uintptr_t AbsoluteRotation = 0x188;
        inline constexpr uintptr_t AbsoluteSize = 0x118;
    }

    namespace GuiObject {
        inline constexpr uintptr_t Active = 0x5AC;
        inline constexpr uintptr_t AnchorPoint = 0x560;
        inline constexpr uintptr_t AutomaticSize = 0x568;
        inline constexpr uintptr_t BackgroundColor3 = 0x548;
        inline constexpr uintptr_t BackgroundTransparency = 0x56C;
        inline constexpr uintptr_t BorderColor3 = 0x554;
        inline constexpr uintptr_t BorderMode = 0x570;
        inline constexpr uintptr_t BorderSizePixel = 0x574;
        inline constexpr uintptr_t ClipsDescendants = 0x5AD;
        inline constexpr uintptr_t GuiState = 0x580;
        inline constexpr uintptr_t Interactable = 0x5AF;
        inline constexpr uintptr_t LayoutOrder = 0x584;
        inline constexpr uintptr_t Position = 0x518;
        inline constexpr uintptr_t Rotation = 0x188;
        inline constexpr uintptr_t Selectable = 0x5B0;
        inline constexpr uintptr_t SelectionOrder = 0x5A0;
        inline constexpr uintptr_t Size = 0x538;
        inline constexpr uintptr_t SizeConstraint = 0x5A4;
        inline constexpr uintptr_t Visible = 0x5B1;
        inline constexpr uintptr_t ZIndex = 0x5A8;
    }

    namespace Humanoid {
        inline constexpr uintptr_t AutoJumpEnabled = 0x1D8;
        inline constexpr uintptr_t AutoRotate = 0x1D9;
        inline constexpr uintptr_t AutomaticScalingEnabled = 0x1DA;
        inline constexpr uintptr_t BreakJointsOnDeath = 0x1DB;
        inline constexpr uintptr_t CameraOffset = 0x140;
        inline constexpr uintptr_t DisplayDistanceType = 0x18C;
        inline constexpr uintptr_t EvaluateStateMachine = 0x1DC;
        inline constexpr uintptr_t Health = 0x194;
        inline constexpr uintptr_t HealthDisplayDistance = 0x198;
        inline constexpr uintptr_t HealthDisplayType = 0x19C;
        inline constexpr uintptr_t HipHeight = 0x1A0;
        inline constexpr uintptr_t JumpHeight = 0x1AC;
        inline constexpr uintptr_t JumpPower = 0x1B0;
        inline constexpr uintptr_t MaxHealth = 0x1B4;
        inline constexpr uintptr_t MaxSlopeAngle = 0x1B8;
        inline constexpr uintptr_t NameDisplayDistance = 0x1BC;
        inline constexpr uintptr_t NameOcclusion = 0x1C0;
        inline constexpr uintptr_t RequiresNeck = 0x1E0;
        inline constexpr uintptr_t RigType = 0x1C8;
        inline constexpr uintptr_t Sit = 0x1E1;
        inline constexpr uintptr_t TargetPoint = 0x164;
        inline constexpr uintptr_t UseJumpPower = 0x1E3;
        inline constexpr uintptr_t WalkSpeed = 0x1D4;
        inline constexpr uintptr_t WalkSpeedCheck = 0x3C0;
        inline constexpr uintptr_t WalkToPoint = 0x17C;
    }

    namespace InputObject {
        inline constexpr uintptr_t MousePosition = 0xEC;
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

    namespace Lighting {
        inline constexpr uintptr_t Ambient = 0xD8;
        inline constexpr uintptr_t Atmosphere = 0x1E8;
        inline constexpr uintptr_t Brightness = 0x120;
        inline constexpr uintptr_t ClockTime = 0x1B8;
        inline constexpr uintptr_t ColorShift_Bottom = 0xE4;
        inline constexpr uintptr_t ColorShift_Top = 0xF0;
        inline constexpr uintptr_t EnvironmentDiffuseScale = 0x124;
        inline constexpr uintptr_t EnvironmentSpecularScale = 0x128;
        inline constexpr uintptr_t GeographicLatitude = 0x190;
        inline constexpr uintptr_t OutdoorAmbient = 0x108;
        inline constexpr uintptr_t Sky = 0x1D8;
    }

    namespace LocalScript {
        inline constexpr uintptr_t Bytecode = 0x1A8;
        inline constexpr uintptr_t Hash = 0xE8;
    }

    namespace MaterialColors {
        inline constexpr uintptr_t Asphalt = 0x30;
        inline constexpr uintptr_t Basalt = 0x27;
        inline constexpr uintptr_t Brick = 0xF;
        inline constexpr uintptr_t Cobblestone = 0x33;
        inline constexpr uintptr_t Concrete = 0xC;
        inline constexpr uintptr_t CrackedLava = 0x2D;
        inline constexpr uintptr_t Glacier = 0x1B;
        inline constexpr uintptr_t Grass = 0x6;
        inline constexpr uintptr_t Ground = 0x2A;
        inline constexpr uintptr_t Ice = 0x36;
        inline constexpr uintptr_t LeafyGrass = 0x39;
        inline constexpr uintptr_t Limestone = 0x3F;
        inline constexpr uintptr_t Mud = 0x24;
        inline constexpr uintptr_t Pavement = 0x42;
        inline constexpr uintptr_t Rock = 0x18;
        inline constexpr uintptr_t Salt = 0x3C;
        inline constexpr uintptr_t Sand = 0x12;
        inline constexpr uintptr_t Sandstone = 0x21;
        inline constexpr uintptr_t Slate = 0x9;
        inline constexpr uintptr_t Snow = 0x1E;
        inline constexpr uintptr_t WoodPlanks = 0x15;
    }

    namespace MeshPart {
        inline constexpr uintptr_t MeshId = 0x2E8;
        inline constexpr uintptr_t TextureId = 0x318;
    }

    namespace ModuleScript {
        inline constexpr uintptr_t Bytecode = 0x150;
        inline constexpr uintptr_t Hash = 0xE8;
    }

    namespace MouseService {
        inline constexpr uintptr_t InputObject = 0x110;
    }

    namespace Player {
        inline constexpr uintptr_t AccountAge = 0x30C;
        inline constexpr uintptr_t Character = 0x380;
        inline constexpr uintptr_t DisplayName = 0x130;
        inline constexpr uintptr_t HealthDisplayDistance = 0x338;
        inline constexpr uintptr_t LocaleId = 0x6B8;
        inline constexpr uintptr_t NameDisplayDistance = 0x344;
        inline constexpr uintptr_t Team = 0x290;
        inline constexpr uintptr_t TeamColor = 0x350;
        inline constexpr uintptr_t UserId = 0x2B8;
    }

    namespace Players {
        inline constexpr uintptr_t LocalPlayer = 0x130;
    }

    namespace Primitive {
        inline constexpr uintptr_t AssemblyAngularVelocity = 0xFC;
        inline constexpr uintptr_t AssemblyLinearVelocity = 0xF0;
        inline constexpr uintptr_t CFrame = 0xC0;
        inline constexpr uintptr_t Material = 0x246;
        inline constexpr uintptr_t Orientation = 0xC0;
        inline constexpr uintptr_t Position = 0xE4;
        inline constexpr uintptr_t PrimitiveFlags = 0x1AE;
        inline constexpr uintptr_t Rotation = 0xC0;
        inline constexpr uintptr_t Size = 0x1B0;
    }

    namespace PrimitiveFlags {
        inline constexpr uintptr_t Anchored = 0x2;
        inline constexpr uintptr_t CanCollide = 0x8;
        inline constexpr uintptr_t CanTouch = 0x10;
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

    namespace RenderView {
        inline constexpr uintptr_t LightingValid = 0x148;
        inline constexpr uintptr_t SkyboxValid = 0x28D;
    }

    namespace Sky {
        inline constexpr uintptr_t MoonAngularSize = 0x25C;
        inline constexpr uintptr_t MoonTextureId = 0xE0;
        inline constexpr uintptr_t SkyboxBk = 0x110;
        inline constexpr uintptr_t SkyboxDn = 0x140;
        inline constexpr uintptr_t SkyboxFt = 0x170;
        inline constexpr uintptr_t SkyboxLf = 0x1A0;
        inline constexpr uintptr_t SkyboxOrientation = 0x250;
        inline constexpr uintptr_t SkyboxRt = 0x1D0;
        inline constexpr uintptr_t SkyboxUp = 0x200;
        inline constexpr uintptr_t StarCount = 0x260;
        inline constexpr uintptr_t SunAngularSize = 0x264;
        inline constexpr uintptr_t SunTextureId = 0x230;
    }

    namespace SpecialMesh {
        inline constexpr uintptr_t MeshId = 0x108;
        inline constexpr uintptr_t Offset = 0xD0;
        inline constexpr uintptr_t Scale = 0xDC;
        inline constexpr uintptr_t TextureId = 0x130;
    }

    namespace Team {
        inline constexpr uintptr_t TeamColor = 0xD0;
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

    namespace TextButton {
        inline constexpr uintptr_t AutoButtonColor = 0xA14;
        inline constexpr uintptr_t ContentText = 0xD28;
        inline constexpr uintptr_t Font = 0x1150;
        inline constexpr uintptr_t LineHeight = 0xD9C;
        inline constexpr uintptr_t LocalizedText = 0xD28;
        inline constexpr uintptr_t MaxVisibleGraphemes = 0x115C;
        inline constexpr uintptr_t Modal = 0xA15;
        inline constexpr uintptr_t RichText = 0xE45;
        inline constexpr uintptr_t Selected = 0xA16;
        inline constexpr uintptr_t Text = 0xD28;
        inline constexpr uintptr_t TextColor3 = 0x1138;
        inline constexpr uintptr_t TextDirection = 0xE48;
        inline constexpr uintptr_t TextScaled = 0xD98;
        inline constexpr uintptr_t TextSize = 0x1164;
        inline constexpr uintptr_t TextStrokeColor3 = 0x1144;
        inline constexpr uintptr_t TextStrokeTransparency = 0x1168;
        inline constexpr uintptr_t TextTransparency = 0x116C;
        inline constexpr uintptr_t TextTruncate = 0x1170;
        inline constexpr uintptr_t TextWrapped = 0xDE8;
        inline constexpr uintptr_t TextXAlignment = 0x1174;
        inline constexpr uintptr_t TextYAlignment = 0xDE4;
    }

    namespace TextLabel {
        inline constexpr uintptr_t ContentText = 0xAA8;
        inline constexpr uintptr_t Font = 0xED0;
        inline constexpr uintptr_t LineHeight = 0xB1C;
        inline constexpr uintptr_t LocalizedText = 0xAA8;
        inline constexpr uintptr_t MaxVisibleGraphemes = 0xEDC;
        inline constexpr uintptr_t RichText = 0xBC5;
        inline constexpr uintptr_t Text = 0xAA8;
        inline constexpr uintptr_t TextColor3 = 0xEB8;
        inline constexpr uintptr_t TextDirection = 0xBC8;
        inline constexpr uintptr_t TextScaled = 0xB18;
        inline constexpr uintptr_t TextSize = 0xEE4;
        inline constexpr uintptr_t TextStrokeColor3 = 0xEC4;
        inline constexpr uintptr_t TextStrokeTransparency = 0xEE8;
        inline constexpr uintptr_t TextTransparency = 0xEEC;
        inline constexpr uintptr_t TextTruncate = 0xEF0;
        inline constexpr uintptr_t TextWrapped = 0xB68;
        inline constexpr uintptr_t TextXAlignment = 0xEF4;
        inline constexpr uintptr_t TextYAlignment = 0xB64;
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
        inline constexpr uintptr_t Tooltip = 0x450;
    }

    namespace Value {
        inline constexpr uintptr_t Value = 0xD0;
    }

    namespace VisualEngine {
        inline constexpr uintptr_t Dimensions = 0x720;
        inline constexpr uintptr_t FakeDataModel = 0x700;
        inline constexpr uintptr_t Pointer = 0x79449E0;
        inline constexpr uintptr_t RenderView = 0x800;
        inline constexpr uintptr_t ViewMatrix = 0x120;
    }

    namespace Workspace {
        inline constexpr uintptr_t CurrentCamera = 0x4A0;
        inline constexpr uintptr_t ReadOnlyGravity = 0xA28;
        inline constexpr uintptr_t World = 0x3D8;
    }

    namespace World {
        inline constexpr uintptr_t Gravity = 0x1D0;
    }

} // namespace offsets
