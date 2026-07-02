/*
 * Dumped With: roblox-dumper 2.8
 * Created by: Jonah (jonahw on Discord)
 * Github: https://github.com/nopjo/roblox-dumper
 * Roblox Version: version-5cf2272675e145f5
 * Time Taken: 3527 ms (3.527000 seconds)
 * Total Offsets: 278
 */

#pragma once
#include <cstdint>

// clang-format off
namespace offsets {
    inline constexpr const char* roblox_version = "version-5cf2272675e145f5";

    namespace Atmosphere {
        inline constexpr uintptr_t Color = 0xB8;
        inline constexpr uintptr_t Decay = 0xC4;
        inline constexpr uintptr_t Density = 0xD0;
        inline constexpr uintptr_t Glare = 0xD4;
        inline constexpr uintptr_t Haze = 0xD8;
        inline constexpr uintptr_t Offset = 0xDC;
    }

    namespace Attribute {
        inline constexpr uintptr_t Key = 0x0;
        inline constexpr uintptr_t Size = 0x58;
        inline constexpr uintptr_t Value = 0x18;
    }

    namespace BasePart {
        inline constexpr uintptr_t CastShadow = 0xD5;
        inline constexpr uintptr_t Color3 = 0x148;
        inline constexpr uintptr_t Locked = 0xD6;
        inline constexpr uintptr_t Massless = 0xD7;
        inline constexpr uintptr_t Primitive = 0x128;
        inline constexpr uintptr_t Reflectance = 0xCC;
        inline constexpr uintptr_t Shape = 0x159;
        inline constexpr uintptr_t Transparency = 0xD0;
    }

    namespace BloomEffect {
        inline constexpr uintptr_t Intensity = 0xB8;
        inline constexpr uintptr_t Size = 0xBC;
        inline constexpr uintptr_t Threshold = 0xC0;
    }

    namespace ByteCode {
        inline constexpr uintptr_t Pointer = 0x10;
        inline constexpr uintptr_t Size = 0x20;
    }

    namespace Camera {
        inline constexpr uintptr_t CFrame = 0xD8;
        inline constexpr uintptr_t FieldOfView = 0x140;
        inline constexpr uintptr_t Position = 0xFC;
        inline constexpr uintptr_t Rotation = 0xD8;
        inline constexpr uintptr_t ViewportInt16 = 0x28C;
        inline constexpr uintptr_t ViewportSize = 0x2C8;
    }

    namespace CharacterMesh {
        inline constexpr uintptr_t BaseTextureId = 0xC8;
        inline constexpr uintptr_t BodyPart = 0x148;
        inline constexpr uintptr_t MeshId = 0xF8;
        inline constexpr uintptr_t OverlayTextureId = 0x128;
    }

    namespace DataModel {
        inline constexpr uintptr_t CreatorId = 0x180;
        inline constexpr uintptr_t GameId = 0x188;
        inline constexpr uintptr_t GameLoaded = 0x658;
        inline constexpr uintptr_t JobId = 0x120;
        inline constexpr uintptr_t PlaceId = 0x190;
        inline constexpr uintptr_t ServerIP = 0x640;
        inline constexpr uintptr_t Workspace = 0x160;
    }

    namespace FakeDataModel {
        inline constexpr uintptr_t Pointer = 0x7C3D2E8;
        inline constexpr uintptr_t RealDataModel = 0x1D0;
    }

    namespace GuiBase2D {
        inline constexpr uintptr_t AbsolutePosition = 0xF4;
        inline constexpr uintptr_t AbsoluteRotation = 0x178;
        inline constexpr uintptr_t AbsoluteSize = 0x100;
    }

    namespace GuiObject {
        inline constexpr uintptr_t Active = 0x5A8;
        inline constexpr uintptr_t AnchorPoint = 0x558;
        inline constexpr uintptr_t AutomaticSize = 0x560;
        inline constexpr uintptr_t BackgroundColor3 = 0x540;
        inline constexpr uintptr_t BackgroundTransparency = 0x564;
        inline constexpr uintptr_t BorderColor3 = 0x54C;
        inline constexpr uintptr_t BorderMode = 0x568;
        inline constexpr uintptr_t BorderSizePixel = 0x56C;
        inline constexpr uintptr_t ClipsDescendants = 0x5A9;
        inline constexpr uintptr_t GuiState = 0x578;
        inline constexpr uintptr_t Interactable = 0x5AB;
        inline constexpr uintptr_t LayoutOrder = 0x580;
        inline constexpr uintptr_t Position = 0x510;
        inline constexpr uintptr_t Rotation = 0x178;
        inline constexpr uintptr_t Selectable = 0x5AC;
        inline constexpr uintptr_t SelectionOrder = 0x59C;
        inline constexpr uintptr_t Size = 0x530;
        inline constexpr uintptr_t SizeConstraint = 0x5A0;
        inline constexpr uintptr_t Visible = 0x5AD;
        inline constexpr uintptr_t ZIndex = 0x5A4;
    }

    namespace Humanoid {
        inline constexpr uintptr_t AutoJumpEnabled = 0x1D4;
        inline constexpr uintptr_t AutoRotate = 0x1D5;
        inline constexpr uintptr_t AutomaticScalingEnabled = 0x1D6;
        inline constexpr uintptr_t BreakJointsOnDeath = 0x1D7;
        inline constexpr uintptr_t CameraOffset = 0x128;
        inline constexpr uintptr_t DisplayDistanceType = 0x180;
        inline constexpr uintptr_t EvaluateStateMachine = 0x1D8;
        inline constexpr uintptr_t Health = 0x188;
        inline constexpr uintptr_t HealthDisplayDistance = 0x18C;
        inline constexpr uintptr_t HealthDisplayType = 0x190;
        inline constexpr uintptr_t HipHeight = 0x194;
        inline constexpr uintptr_t JumpHeight = 0x1A0;
        inline constexpr uintptr_t JumpPower = 0x1A4;
        inline constexpr uintptr_t MaxHealth = 0x1A8;
        inline constexpr uintptr_t MaxSlopeAngle = 0x1AC;
        inline constexpr uintptr_t NameDisplayDistance = 0x1B0;
        inline constexpr uintptr_t NameOcclusion = 0x1B4;
        inline constexpr uintptr_t RequiresNeck = 0x1DD;
        inline constexpr uintptr_t RigType = 0x1C0;
        inline constexpr uintptr_t SeatPart = 0x108;
        inline constexpr uintptr_t Sit = 0x1DE;
        inline constexpr uintptr_t TargetPoint = 0x14C;
        inline constexpr uintptr_t UseJumpPower = 0x1E0;
        inline constexpr uintptr_t WalkSpeed = 0x1D0;
        inline constexpr uintptr_t WalkSpeedCheck = 0x3BC;
        inline constexpr uintptr_t WalkToPoint = 0x164;
    }

    namespace InputObject {
        inline constexpr uintptr_t MousePosition = 0xD4;
    }

    namespace Instance {
        inline constexpr uintptr_t ChildrenEnd = 0x8;
        inline constexpr uintptr_t ChildrenStart = 0x70;
        inline constexpr uintptr_t ClassDescriptor = 0x18;
        inline constexpr uintptr_t ClassName = 0x8;
        inline constexpr uintptr_t ComponentMap = 0x38;
        inline constexpr uintptr_t Name = 0x98;
        inline constexpr uintptr_t Parent = 0x68;
    }

    namespace Lighting {
        inline constexpr uintptr_t Ambient = 0xC8;
        inline constexpr uintptr_t Atmosphere = 0x1D8;
        inline constexpr uintptr_t Brightness = 0x110;
        inline constexpr uintptr_t ClockTime = 0x1A8;
        inline constexpr uintptr_t ColorShift_Bottom = 0xD4;
        inline constexpr uintptr_t ColorShift_Top = 0xE0;
        inline constexpr uintptr_t EnvironmentDiffuseScale = 0x114;
        inline constexpr uintptr_t EnvironmentSpecularScale = 0x118;
        inline constexpr uintptr_t ExposureCompensation = 0x11C;
        inline constexpr uintptr_t FogColor = 0xEC;
        inline constexpr uintptr_t FogEnd = 0x124;
        inline constexpr uintptr_t FogStart = 0x128;
        inline constexpr uintptr_t OutdoorAmbient = 0xF8;
        inline constexpr uintptr_t ShadowSoftness = 0x130;
        inline constexpr uintptr_t Sky = 0x1C8;
    }

    namespace LightingParameters {
        inline constexpr uintptr_t GeographicLatitude = 0x180;
        inline constexpr uintptr_t LightColor = 0x14C;
        inline constexpr uintptr_t LightDirection = 0x158;
        inline constexpr uintptr_t SkyAmbient = 0x140;
        inline constexpr uintptr_t SkyAmbient2 = 0x184;
        inline constexpr uintptr_t Source = 0x164;
        inline constexpr uintptr_t TrueMoonPosition = 0x174;
        inline constexpr uintptr_t TrueSunPosition = 0x168;
    }

    namespace LocalScript {
        inline constexpr uintptr_t Bytecode = 0x190;
        inline constexpr uintptr_t Hash = 0x98;
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
        inline constexpr uintptr_t MeshId = 0x290;
        inline constexpr uintptr_t TextureId = 0x2C0;
    }

    namespace ModuleScript {
        inline constexpr uintptr_t Bytecode = 0x138;
        inline constexpr uintptr_t Hash = 0x98;
    }

    namespace MouseService {
        inline constexpr uintptr_t InputObject = 0x100;
    }

    namespace Player {
        inline constexpr uintptr_t AccountAge = 0x34C;
        inline constexpr uintptr_t Character = 0x298;
        inline constexpr uintptr_t DisplayName = 0x138;
        inline constexpr uintptr_t HealthDisplayDistance = 0x380;
        inline constexpr uintptr_t LocaleId = 0x728;
        inline constexpr uintptr_t NameDisplayDistance = 0x390;
        inline constexpr uintptr_t Team = 0x2C8;
        inline constexpr uintptr_t TeamColor = 0x39C;
        inline constexpr uintptr_t UserId = 0x2F0;
    }

    namespace Players {
        inline constexpr uintptr_t LocalPlayer = 0x130;
    }

    namespace Primitive {
        inline constexpr uintptr_t AssemblyAngularVelocity = 0x104;
        inline constexpr uintptr_t AssemblyLinearVelocity = 0xF8;
        inline constexpr uintptr_t CFrame = 0xC8;
        inline constexpr uintptr_t Material = 0x23E;
        inline constexpr uintptr_t Orientation = 0xC8;
        inline constexpr uintptr_t Position = 0xEC;
        inline constexpr uintptr_t PrimitiveFlags = 0x1B6;
        inline constexpr uintptr_t Rotation = 0xC8;
        inline constexpr uintptr_t Size = 0x1B8;
    }

    namespace PrimitiveFlags {
        inline constexpr uintptr_t Anchored = 0x2;
        inline constexpr uintptr_t CanCollide = 0x8;
        inline constexpr uintptr_t CanQuery = 0x20;
        inline constexpr uintptr_t CanTouch = 0x10;
    }

    namespace ProximityPrompt {
        inline constexpr uintptr_t ActionText = 0xB0;
        inline constexpr uintptr_t Enabled = 0x136;
        inline constexpr uintptr_t HoldDuration = 0x120;
        inline constexpr uintptr_t KeyboardKeyCode = 0x124;
        inline constexpr uintptr_t MaxActivationDistance = 0x128;
        inline constexpr uintptr_t ObjectText = 0xD0;
        inline constexpr uintptr_t RequiresLineOfSight = 0x137;
    }

    namespace RenderView {
        inline constexpr uintptr_t LightingValid = 0x228;
        inline constexpr uintptr_t SkyboxValid = 0x28D;
    }

    namespace Seat {
        inline constexpr uintptr_t Occupant = 0x1B0;
    }

    namespace Sky {
        inline constexpr uintptr_t MoonAngularSize = 0x244;
        inline constexpr uintptr_t MoonTextureId = 0xC8;
        inline constexpr uintptr_t SkyboxBk = 0xF8;
        inline constexpr uintptr_t SkyboxDn = 0x128;
        inline constexpr uintptr_t SkyboxFt = 0x158;
        inline constexpr uintptr_t SkyboxLf = 0x188;
        inline constexpr uintptr_t SkyboxOrientation = 0x238;
        inline constexpr uintptr_t SkyboxRt = 0x1B8;
        inline constexpr uintptr_t SkyboxUp = 0x1E8;
        inline constexpr uintptr_t StarCount = 0x248;
        inline constexpr uintptr_t SunAngularSize = 0x24C;
        inline constexpr uintptr_t SunTextureId = 0x218;
    }

    namespace SpecialMesh {
        inline constexpr uintptr_t MeshId = 0xF8;
        inline constexpr uintptr_t Offset = 0xB8;
        inline constexpr uintptr_t Scale = 0xC4;
        inline constexpr uintptr_t TextureId = 0x128;
    }

    namespace TaskScheduler {
        inline constexpr uintptr_t JobEnd = 0xD0;
        inline constexpr uintptr_t JobName = 0x1E0;
        inline constexpr uintptr_t JobStart = 0xC8;
        inline constexpr uintptr_t MaxFps = 0xB0;
        inline constexpr uintptr_t Pointer = 0x81CC868;
    }

    namespace Team {
        inline constexpr uintptr_t TeamColor = 0xB8;
    }

    namespace Terrain {
        inline constexpr uintptr_t GrassLength = 0x188;
        inline constexpr uintptr_t MaterialColors = 0x438;
        inline constexpr uintptr_t WaterColor = 0x178;
        inline constexpr uintptr_t WaterReflectance = 0x190;
        inline constexpr uintptr_t WaterTransparency = 0x194;
        inline constexpr uintptr_t WaterWaveSize = 0x198;
        inline constexpr uintptr_t WaterWaveSpeed = 0x19C;
    }

    namespace TextButton {
        inline constexpr uintptr_t AutoButtonColor = 0x9C4;
        inline constexpr uintptr_t ContentText = 0xDD0;
        inline constexpr uintptr_t Font = 0x10E8;
        inline constexpr uintptr_t LineHeight = 0xEC8;
        inline constexpr uintptr_t LocalizedText = 0xDD0;
        inline constexpr uintptr_t MaxVisibleGraphemes = 0x10F4;
        inline constexpr uintptr_t Modal = 0x9C5;
        inline constexpr uintptr_t RichText = 0xFC6;
        inline constexpr uintptr_t Selected = 0x9C6;
        inline constexpr uintptr_t Text = 0xDD0;
        inline constexpr uintptr_t TextColor3 = 0x10D0;
        inline constexpr uintptr_t TextDirection = 0xF68;
        inline constexpr uintptr_t TextScaled = 0xFBE;
        inline constexpr uintptr_t TextSize = 0x10FC;
        inline constexpr uintptr_t TextStrokeColor3 = 0x10DC;
        inline constexpr uintptr_t TextStrokeTransparency = 0x1100;
        inline constexpr uintptr_t TextTransparency = 0x1104;
        inline constexpr uintptr_t TextTruncate = 0x1108;
        inline constexpr uintptr_t TextWrapped = 0xFC0;
        inline constexpr uintptr_t TextXAlignment = 0x110C;
        inline constexpr uintptr_t TextYAlignment = 0xF10;
    }

    namespace TextLabel {
        inline constexpr uintptr_t ContentText = 0xB50;
        inline constexpr uintptr_t Font = 0xE68;
        inline constexpr uintptr_t LineHeight = 0xC48;
        inline constexpr uintptr_t LocalizedText = 0xB50;
        inline constexpr uintptr_t MaxVisibleGraphemes = 0xE74;
        inline constexpr uintptr_t RichText = 0xD46;
        inline constexpr uintptr_t Text = 0xB50;
        inline constexpr uintptr_t TextColor3 = 0xE50;
        inline constexpr uintptr_t TextDirection = 0xCE8;
        inline constexpr uintptr_t TextScaled = 0xB39;
        inline constexpr uintptr_t TextSize = 0xE7C;
        inline constexpr uintptr_t TextStrokeColor3 = 0xE5C;
        inline constexpr uintptr_t TextStrokeTransparency = 0xE80;
        inline constexpr uintptr_t TextTransparency = 0xE84;
        inline constexpr uintptr_t TextTruncate = 0xE88;
        inline constexpr uintptr_t TextWrapped = 0xD40;
        inline constexpr uintptr_t TextXAlignment = 0xE8C;
        inline constexpr uintptr_t TextYAlignment = 0xC90;
    }

    namespace Tool {
        inline constexpr uintptr_t CanBeDropped = 0x4B8;
        inline constexpr uintptr_t Enabled = 0x4B9;
        inline constexpr uintptr_t Grip = 0x488;
        inline constexpr uintptr_t GripForward = 0x4A0;
        inline constexpr uintptr_t GripPos = 0x4AC;
        inline constexpr uintptr_t GripRight = 0x488;
        inline constexpr uintptr_t GripUp = 0x494;
        inline constexpr uintptr_t ManualActivationOnly = 0x4BA;
        inline constexpr uintptr_t RequiresHandle = 0x4BB;
        inline constexpr uintptr_t Tooltip = 0x468;
    }

    namespace Value {
        inline constexpr uintptr_t Value = 0xB8;
    }

    namespace VehicleSeat {
        inline constexpr uintptr_t MaxSpeed = 0x1C8;
        inline constexpr uintptr_t Occupant = 0x1A8;
        inline constexpr uintptr_t SteerFloat = 0x1D0;
        inline constexpr uintptr_t ThrottleFloat = 0x1D8;
        inline constexpr uintptr_t Torque = 0x1DC;
        inline constexpr uintptr_t TurnSpeed = 0x1E0;
    }

    namespace VisualEngine {
        inline constexpr uintptr_t Dimensions = 0xAB0;
        inline constexpr uintptr_t FakeDataModel = 0xA90;
        inline constexpr uintptr_t Pointer = 0x835A548;
        inline constexpr uintptr_t RenderView = 0xBB8;
        inline constexpr uintptr_t ViewMatrix = 0x150;
    }

    namespace Workspace {
        inline constexpr uintptr_t CurrentCamera = 0x4A0;
        inline constexpr uintptr_t ReadOnlyGravity = 0x9B0;
        inline constexpr uintptr_t World = 0x3F8;
    }

    namespace World {
        inline constexpr uintptr_t Gravity = 0x210;
        inline constexpr uintptr_t Primitives = 0x288;
        inline constexpr uintptr_t WorldSteps = 0x680;
    }

} // namespace offsets
