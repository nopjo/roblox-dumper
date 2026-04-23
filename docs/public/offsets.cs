/*
 * Dumped With: roblox-dumper 2.7
 * Created by: Jonah (jonahw on Discord)
 * Github: https://github.com/nopjo/roblox-dumper
 * Roblox Version: version-2e6461290a3541f5
 * Time Taken: 3398 ms (3.398000 seconds)
 * Total Offsets: 280
 */

using System;

namespace RobloxOffsets
{
    public static class Metadata
    {
        public const string RobloxVersion = "version-2e6461290a3541f5";
    }

    public static class Atmosphere
    {
        public const ulong Color = 0xD0;
        public const ulong Decay = 0xDC;
        public const ulong Density = 0xE8;
        public const ulong Glare = 0xEC;
        public const ulong Haze = 0xF0;
        public const ulong Offset = 0xF4;
    }

    public static class BasePart
    {
        public const ulong CastShadow = 0xF5;
        public const ulong Color3 = 0x194;
        public const ulong Locked = 0xF6;
        public const ulong Massless = 0xF7;
        public const ulong Primitive = 0x148;
        public const ulong Reflectance = 0xEC;
        public const ulong Shape = 0x1B1;
        public const ulong Transparency = 0xF0;
    }

    public static class BloomEffect
    {
        public const ulong Intensity = 0xD0;
        public const ulong Size = 0xD4;
        public const ulong Threshold = 0xD8;
    }

    public static class ByteCode
    {
        public const ulong Pointer = 0x10;
        public const ulong Size = 0x20;
    }

    public static class Camera
    {
        public const ulong CFrame = 0xF8;
        public const ulong FieldOfView = 0x160;
        public const ulong Position = 0x11C;
        public const ulong Rotation = 0xF8;
        public const ulong ViewportInt16 = 0x2AC;
        public const ulong ViewportSize = 0x2E8;
    }

    public static class CharacterMesh
    {
        public const ulong BaseTextureId = 0xE0;
        public const ulong BodyPart = 0x160;
        public const ulong MeshId = 0x110;
        public const ulong OverlayTextureId = 0x140;
    }

    public static class DataModel
    {
        public const ulong CreatorId = 0x188;
        public const ulong GameId = 0x190;
        public const ulong GameLoaded = 0x5F8;
        public const ulong JobId = 0x138;
        public const ulong PlaceId = 0x198;
        public const ulong ServerIP = 0x5E0;
        public const ulong Workspace = 0x178;
    }

    public static class FakeDataModel
    {
        public const ulong Pointer = 0x7B990F8;
        public const ulong RealDataModel = 0x1D0;
    }

    public static class GuiBase2D
    {
        public const ulong AbsolutePosition = 0x10C;
        public const ulong AbsoluteRotation = 0x188;
        public const ulong AbsoluteSize = 0x118;
    }

    public static class GuiObject
    {
        public const ulong Active = 0x5B0;
        public const ulong AnchorPoint = 0x560;
        public const ulong AutomaticSize = 0x568;
        public const ulong BackgroundColor3 = 0x548;
        public const ulong BackgroundTransparency = 0x56C;
        public const ulong BorderColor3 = 0x554;
        public const ulong BorderMode = 0x570;
        public const ulong BorderSizePixel = 0x574;
        public const ulong ClipsDescendants = 0x5B1;
        public const ulong GuiState = 0x580;
        public const ulong Interactable = 0x5B3;
        public const ulong LayoutOrder = 0x588;
        public const ulong Position = 0x518;
        public const ulong Rotation = 0x188;
        public const ulong Selectable = 0x5B4;
        public const ulong SelectionOrder = 0x5A4;
        public const ulong Size = 0x538;
        public const ulong SizeConstraint = 0x5A8;
        public const ulong Visible = 0x5B5;
        public const ulong ZIndex = 0x5AC;
    }

    public static class Humanoid
    {
        public const ulong AutoJumpEnabled = 0x1D8;
        public const ulong AutoRotate = 0x1D9;
        public const ulong AutomaticScalingEnabled = 0x1DA;
        public const ulong BreakJointsOnDeath = 0x1DB;
        public const ulong CameraOffset = 0x140;
        public const ulong DisplayDistanceType = 0x18C;
        public const ulong EvaluateStateMachine = 0x1DC;
        public const ulong Health = 0x194;
        public const ulong HealthDisplayDistance = 0x198;
        public const ulong HealthDisplayType = 0x19C;
        public const ulong HipHeight = 0x1A0;
        public const ulong JumpHeight = 0x1AC;
        public const ulong JumpPower = 0x1B0;
        public const ulong MaxHealth = 0x1B4;
        public const ulong MaxSlopeAngle = 0x1B8;
        public const ulong NameDisplayDistance = 0x1BC;
        public const ulong NameOcclusion = 0x1C0;
        public const ulong RequiresNeck = 0x1E0;
        public const ulong RigType = 0x1C8;
        public const ulong SeatPart = 0x120;
        public const ulong Sit = 0x1E1;
        public const ulong TargetPoint = 0x164;
        public const ulong UseJumpPower = 0x1E3;
        public const ulong WalkSpeed = 0x1D4;
        public const ulong WalkSpeedCheck = 0x3BC;
        public const ulong WalkToPoint = 0x17C;
    }

    public static class InputObject
    {
        public const ulong MousePosition = 0xEC;
    }

    public static class Instance
    {
        public const ulong AttributeContainer = 0x48;
        public const ulong AttributeList = 0x18;
        public const ulong AttributeToNext = 0x58;
        public const ulong AttributeToValue = 0x18;
        public const ulong ChildrenEnd = 0x8;
        public const ulong ChildrenStart = 0x78;
        public const ulong ClassDescriptor = 0x18;
        public const ulong ClassName = 0x8;
        public const ulong Name = 0xB0;
        public const ulong Parent = 0x70;
    }

    public static class Lighting
    {
        public const ulong Ambient = 0xD8;
        public const ulong Atmosphere = 0x1E8;
        public const ulong Brightness = 0x120;
        public const ulong ClockTime = 0x1B8;
        public const ulong ColorShift_Bottom = 0xE4;
        public const ulong ColorShift_Top = 0xF0;
        public const ulong EnvironmentDiffuseScale = 0x124;
        public const ulong EnvironmentSpecularScale = 0x128;
        public const ulong ExposureCompensation = 0x12C;
        public const ulong FogColor = 0xFC;
        public const ulong FogEnd = 0x134;
        public const ulong FogStart = 0x138;
        public const ulong OutdoorAmbient = 0x108;
        public const ulong ShadowSoftness = 0x140;
        public const ulong Sky = 0x1D8;
    }

    public static class LightingParameters
    {
        public const ulong GeographicLatitude = 0x190;
        public const ulong LightColor = 0x15C;
        public const ulong LightDirection = 0x168;
        public const ulong SkyAmbient = 0x150;
        public const ulong SkyAmbient2 = 0x194;
        public const ulong Source = 0x174;
        public const ulong TrueMoonPosition = 0x184;
        public const ulong TrueSunPosition = 0x178;
    }

    public static class LocalScript
    {
        public const ulong Bytecode = 0x1A8;
        public const ulong Hash = 0x1B8;
    }

    public static class MaterialColors
    {
        public const ulong Asphalt = 0x30;
        public const ulong Basalt = 0x27;
        public const ulong Brick = 0xF;
        public const ulong Cobblestone = 0x33;
        public const ulong Concrete = 0xC;
        public const ulong CrackedLava = 0x2D;
        public const ulong Glacier = 0x1B;
        public const ulong Grass = 0x6;
        public const ulong Ground = 0x2A;
        public const ulong Ice = 0x36;
        public const ulong LeafyGrass = 0x39;
        public const ulong Limestone = 0x3F;
        public const ulong Mud = 0x24;
        public const ulong Pavement = 0x42;
        public const ulong Rock = 0x18;
        public const ulong Salt = 0x3C;
        public const ulong Sand = 0x12;
        public const ulong Sandstone = 0x21;
        public const ulong Slate = 0x9;
        public const ulong Snow = 0x1E;
        public const ulong WoodPlanks = 0x15;
    }

    public static class MeshPart
    {
        public const ulong MeshId = 0x2F8;
        public const ulong TextureId = 0x328;
    }

    public static class ModuleScript
    {
        public const ulong Bytecode = 0x150;
        public const ulong Hash = 0xE8;
        public const ulong IsRobloxScript = 0x180;
    }

    public static class MouseService
    {
        public const ulong InputObject = 0x110;
    }

    public static class Player
    {
        public const ulong AccountAge = 0x31C;
        public const ulong Character = 0x398;
        public const ulong DisplayName = 0x130;
        public const ulong HealthDisplayDistance = 0x348;
        public const ulong LocaleId = 0x6D0;
        public const ulong NameDisplayDistance = 0x358;
        public const ulong Team = 0x2A0;
        public const ulong TeamColor = 0x364;
        public const ulong UserId = 0x2C8;
    }

    public static class Players
    {
        public const ulong LocalPlayer = 0x130;
    }

    public static class Primitive
    {
        public const ulong AssemblyAngularVelocity = 0xFC;
        public const ulong AssemblyLinearVelocity = 0xF0;
        public const ulong CFrame = 0xC0;
        public const ulong Material = 0x22E;
        public const ulong Orientation = 0xC0;
        public const ulong Position = 0xE4;
        public const ulong PrimitiveFlags = 0x1AE;
        public const ulong Rotation = 0xC0;
        public const ulong Size = 0x1B0;
    }

    public static class PrimitiveFlags
    {
        public const ulong Anchored = 0x2;
        public const ulong CanCollide = 0x8;
        public const ulong CanQuery = 0x20;
        public const ulong CanTouch = 0x10;
    }

    public static class ProximityPrompt
    {
        public const ulong ActionText = 0xD0;
        public const ulong Enabled = 0x156;
        public const ulong HoldDuration = 0x140;
        public const ulong KeyboardKeyCode = 0x144;
        public const ulong MaxActivationDistance = 0x148;
        public const ulong ObjectText = 0xF0;
        public const ulong RequiresLineOfSight = 0x157;
    }

    public static class RenderView
    {
        public const ulong LightingValid = 0x148;
        public const ulong SkyboxValid = 0x28D;
    }

    public static class ScriptContext
    {
        public const ulong RequireBypass = 0x824;
    }

    public static class Seat
    {
        public const ulong Occupant = 0x220;
    }

    public static class Sky
    {
        public const ulong MoonAngularSize = 0x25C;
        public const ulong MoonTextureId = 0xE0;
        public const ulong SkyboxBk = 0x110;
        public const ulong SkyboxDn = 0x140;
        public const ulong SkyboxFt = 0x170;
        public const ulong SkyboxLf = 0x1A0;
        public const ulong SkyboxOrientation = 0x250;
        public const ulong SkyboxRt = 0x1D0;
        public const ulong SkyboxUp = 0x200;
        public const ulong StarCount = 0x260;
        public const ulong SunAngularSize = 0x264;
        public const ulong SunTextureId = 0x230;
    }

    public static class SpecialMesh
    {
        public const ulong MeshId = 0x108;
        public const ulong Offset = 0xD0;
        public const ulong Scale = 0xDC;
        public const ulong TextureId = 0x130;
    }

    public static class TaskScheduler
    {
        public const ulong JobEnd = 0xD0;
        public const ulong JobName = 0x18;
        public const ulong JobStart = 0xC8;
        public const ulong MaxFps = 0xB0;
        public const ulong Pointer = 0x7C73D70;
    }

    public static class Team
    {
        public const ulong TeamColor = 0xD0;
    }

    public static class Terrain
    {
        public const ulong GrassLength = 0x1F8;
        public const ulong MaterialColors = 0x290;
        public const ulong WaterColor = 0x1E8;
        public const ulong WaterReflectance = 0x200;
        public const ulong WaterTransparency = 0x204;
        public const ulong WaterWaveSize = 0x208;
        public const ulong WaterWaveSpeed = 0x20C;
    }

    public static class TextButton
    {
        public const ulong AutoButtonColor = 0x9CC;
        public const ulong ContentText = 0xCE0;
        public const ulong Font = 0x1108;
        public const ulong LineHeight = 0xD54;
        public const ulong LocalizedText = 0xCE0;
        public const ulong MaxVisibleGraphemes = 0x1114;
        public const ulong Modal = 0x9CD;
        public const ulong RichText = 0xDFD;
        public const ulong Selected = 0x9CE;
        public const ulong Text = 0xCE0;
        public const ulong TextColor3 = 0x10F0;
        public const ulong TextDirection = 0xE00;
        public const ulong TextScaled = 0xD50;
        public const ulong TextSize = 0x111C;
        public const ulong TextStrokeColor3 = 0x10FC;
        public const ulong TextStrokeTransparency = 0x1120;
        public const ulong TextTransparency = 0x1124;
        public const ulong TextTruncate = 0x1128;
        public const ulong TextWrapped = 0xDA0;
        public const ulong TextXAlignment = 0x112C;
        public const ulong TextYAlignment = 0xD9C;
    }

    public static class TextLabel
    {
        public const ulong ContentText = 0xA60;
        public const ulong Font = 0xE88;
        public const ulong LineHeight = 0xAD4;
        public const ulong LocalizedText = 0xA60;
        public const ulong MaxVisibleGraphemes = 0xE94;
        public const ulong RichText = 0xB7D;
        public const ulong Text = 0xA60;
        public const ulong TextColor3 = 0xE70;
        public const ulong TextDirection = 0xB80;
        public const ulong TextScaled = 0xAD0;
        public const ulong TextSize = 0xE9C;
        public const ulong TextStrokeColor3 = 0xE7C;
        public const ulong TextStrokeTransparency = 0xEA0;
        public const ulong TextTransparency = 0xEA4;
        public const ulong TextTruncate = 0xEA8;
        public const ulong TextWrapped = 0xB20;
        public const ulong TextXAlignment = 0xEAC;
        public const ulong TextYAlignment = 0xB1C;
    }

    public static class Tool
    {
        public const ulong CanBeDropped = 0x4C8;
        public const ulong Enabled = 0x4C9;
        public const ulong Grip = 0x498;
        public const ulong GripForward = 0x4B0;
        public const ulong GripPos = 0x4BC;
        public const ulong GripRight = 0x498;
        public const ulong GripUp = 0x4A4;
        public const ulong ManualActivationOnly = 0x4CA;
        public const ulong RequiresHandle = 0x4CB;
        public const ulong Tooltip = 0x478;
    }

    public static class Value
    {
        public const ulong Value = 0xD0;
    }

    public static class VehicleSeat
    {
        public const ulong MaxSpeed = 0x238;
        public const ulong Occupant = 0x218;
        public const ulong SteerFloat = 0x240;
        public const ulong ThrottleFloat = 0x248;
        public const ulong Torque = 0x24C;
        public const ulong TurnSpeed = 0x250;
    }

    public static class VisualEngine
    {
        public const ulong Dimensions = 0xA60;
        public const ulong FakeDataModel = 0xA40;
        public const ulong Pointer = 0x7746018;
        public const ulong RenderView = 0xB40;
        public const ulong ViewMatrix = 0x130;
    }

    public static class Workspace
    {
        public const ulong CurrentCamera = 0x490;
        public const ulong ReadOnlyGravity = 0x9C0;
        public const ulong World = 0x408;
    }

    public static class World
    {
        public const ulong Gravity = 0x1D8;
        public const ulong Primitives = 0x248;
        public const ulong WorldSteps = 0x668;
    }

} // namespace RobloxOffsets
