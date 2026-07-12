/*
 * Dumped With: roblox-dumper 2.8
 * Created by: Jonah (jonahw on Discord)
 * Github: https://github.com/nopjo/roblox-dumper
 * Roblox Version: version-36a2600cebf1487d
 * Time Taken: 3658 ms (3.658000 seconds)
 * Total Offsets: 278
 */

using System;

namespace RobloxOffsets
{
    public static class Metadata
    {
        public const string RobloxVersion = "version-36a2600cebf1487d";
    }

    public static class Atmosphere
    {
        public const ulong Color = 0xB8;
        public const ulong Decay = 0xC4;
        public const ulong Density = 0xD0;
        public const ulong Glare = 0xD4;
        public const ulong Haze = 0xD8;
        public const ulong Offset = 0xDC;
    }

    public static class Attribute
    {
        public const ulong Key = 0x0;
        public const ulong Size = 0x58;
        public const ulong Value = 0x18;
    }

    public static class BasePart
    {
        public const ulong CastShadow = 0xD5;
        public const ulong Color3 = 0x148;
        public const ulong Locked = 0xD6;
        public const ulong Massless = 0xD7;
        public const ulong Primitive = 0x128;
        public const ulong Reflectance = 0xCC;
        public const ulong Shape = 0x159;
        public const ulong Transparency = 0xD0;
    }

    public static class BloomEffect
    {
        public const ulong Intensity = 0xB8;
        public const ulong Size = 0xBC;
        public const ulong Threshold = 0xC0;
    }

    public static class ByteCode
    {
        public const ulong Pointer = 0x10;
        public const ulong Size = 0x20;
    }

    public static class Camera
    {
        public const ulong CFrame = 0xD8;
        public const ulong FieldOfView = 0x140;
        public const ulong Position = 0xFC;
        public const ulong Rotation = 0xD8;
        public const ulong ViewportInt16 = 0x28C;
        public const ulong ViewportSize = 0x2C8;
    }

    public static class CharacterMesh
    {
        public const ulong BaseTextureId = 0xC8;
        public const ulong BodyPart = 0x148;
        public const ulong MeshId = 0xF8;
        public const ulong OverlayTextureId = 0x128;
    }

    public static class DataModel
    {
        public const ulong CreatorId = 0x180;
        public const ulong GameId = 0x188;
        public const ulong GameLoaded = 0x668;
        public const ulong JobId = 0x120;
        public const ulong PlaceId = 0x190;
        public const ulong ServerIP = 0x650;
        public const ulong Workspace = 0x160;
    }

    public static class FakeDataModel
    {
        public const ulong Pointer = 0x84ABC88;
        public const ulong RealDataModel = 0x1D0;
    }

    public static class GuiBase2D
    {
        public const ulong AbsolutePosition = 0xF4;
        public const ulong AbsoluteRotation = 0x178;
        public const ulong AbsoluteSize = 0x100;
    }

    public static class GuiObject
    {
        public const ulong Active = 0x5A8;
        public const ulong AnchorPoint = 0x558;
        public const ulong AutomaticSize = 0x560;
        public const ulong BackgroundColor3 = 0x540;
        public const ulong BackgroundTransparency = 0x564;
        public const ulong BorderColor3 = 0x54C;
        public const ulong BorderMode = 0x568;
        public const ulong BorderSizePixel = 0x56C;
        public const ulong ClipsDescendants = 0x5A9;
        public const ulong GuiState = 0x578;
        public const ulong Interactable = 0x5AB;
        public const ulong LayoutOrder = 0x580;
        public const ulong Position = 0x510;
        public const ulong Rotation = 0x178;
        public const ulong Selectable = 0x5AC;
        public const ulong SelectionOrder = 0x59C;
        public const ulong Size = 0x530;
        public const ulong SizeConstraint = 0x5A0;
        public const ulong Visible = 0x5AD;
        public const ulong ZIndex = 0x5A4;
    }

    public static class Humanoid
    {
        public const ulong AutoJumpEnabled = 0x1D4;
        public const ulong AutoRotate = 0x1D5;
        public const ulong AutomaticScalingEnabled = 0x1D6;
        public const ulong BreakJointsOnDeath = 0x1D7;
        public const ulong CameraOffset = 0x128;
        public const ulong DisplayDistanceType = 0x180;
        public const ulong EvaluateStateMachine = 0x1D8;
        public const ulong Health = 0x188;
        public const ulong HealthDisplayDistance = 0x18C;
        public const ulong HealthDisplayType = 0x190;
        public const ulong HipHeight = 0x194;
        public const ulong JumpHeight = 0x1A0;
        public const ulong JumpPower = 0x1A4;
        public const ulong MaxHealth = 0x1A8;
        public const ulong MaxSlopeAngle = 0x1AC;
        public const ulong NameDisplayDistance = 0x1B0;
        public const ulong NameOcclusion = 0x1B4;
        public const ulong RequiresNeck = 0x1DD;
        public const ulong RigType = 0x1C0;
        public const ulong SeatPart = 0x108;
        public const ulong Sit = 0x1DE;
        public const ulong TargetPoint = 0x14C;
        public const ulong UseJumpPower = 0x1E0;
        public const ulong WalkSpeed = 0x1D0;
        public const ulong WalkSpeedCheck = 0x3BC;
        public const ulong WalkToPoint = 0x164;
    }

    public static class InputObject
    {
        public const ulong MousePosition = 0xD4;
    }

    public static class Instance
    {
        public const ulong ChildrenEnd = 0x8;
        public const ulong ChildrenStart = 0x70;
        public const ulong ClassDescriptor = 0x18;
        public const ulong ClassName = 0x8;
        public const ulong ComponentMap = 0x38;
        public const ulong Name = 0x98;
        public const ulong Parent = 0x68;
    }

    public static class Lighting
    {
        public const ulong Ambient = 0xC8;
        public const ulong Atmosphere = 0x1D8;
        public const ulong Brightness = 0x110;
        public const ulong ClockTime = 0x1A8;
        public const ulong ColorShift_Bottom = 0xD4;
        public const ulong ColorShift_Top = 0xE0;
        public const ulong EnvironmentDiffuseScale = 0x114;
        public const ulong EnvironmentSpecularScale = 0x118;
        public const ulong ExposureCompensation = 0x11C;
        public const ulong FogColor = 0xEC;
        public const ulong FogEnd = 0x124;
        public const ulong FogStart = 0x128;
        public const ulong OutdoorAmbient = 0xF8;
        public const ulong ShadowSoftness = 0x130;
        public const ulong Sky = 0x1C8;
    }

    public static class LightingParameters
    {
        public const ulong GeographicLatitude = 0x180;
        public const ulong LightColor = 0x14C;
        public const ulong LightDirection = 0x158;
        public const ulong SkyAmbient = 0x140;
        public const ulong SkyAmbient2 = 0x184;
        public const ulong Source = 0x164;
        public const ulong TrueMoonPosition = 0x174;
        public const ulong TrueSunPosition = 0x168;
    }

    public static class LocalScript
    {
        public const ulong Bytecode = 0x190;
        public const ulong Hash = 0xD0;
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
        public const ulong MeshId = 0x290;
        public const ulong TextureId = 0x2C0;
    }

    public static class ModuleScript
    {
        public const ulong Bytecode = 0x138;
        public const ulong Hash = 0x98;
    }

    public static class MouseService
    {
        public const ulong InputObject = 0x100;
    }

    public static class Player
    {
        public const ulong AccountAge = 0x35C;
        public const ulong Character = 0x298;
        public const ulong DisplayName = 0x138;
        public const ulong HealthDisplayDistance = 0x390;
        public const ulong LocaleId = 0x738;
        public const ulong NameDisplayDistance = 0x3A0;
        public const ulong Team = 0x2D8;
        public const ulong TeamColor = 0x3AC;
        public const ulong UserId = 0x300;
    }

    public static class Players
    {
        public const ulong LocalPlayer = 0x130;
    }

    public static class Primitive
    {
        public const ulong AssemblyAngularVelocity = 0x1C;
        public const ulong AssemblyLinearVelocity = 0x10;
        public const ulong CFrame = 0xC8;
        public const ulong Material = 0x23E;
        public const ulong Orientation = 0xC8;
        public const ulong Position = 0xEC;
        public const ulong PrimitiveFlags = 0x1B6;
        public const ulong Rotation = 0xC8;
        public const ulong Size = 0x1B8;
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
        public const ulong ActionText = 0xB0;
        public const ulong Enabled = 0x136;
        public const ulong HoldDuration = 0x120;
        public const ulong KeyboardKeyCode = 0x124;
        public const ulong MaxActivationDistance = 0x128;
        public const ulong ObjectText = 0xD0;
        public const ulong RequiresLineOfSight = 0x137;
    }

    public static class RenderView
    {
        public const ulong LightingValid = 0x228;
        public const ulong SkyboxValid = 0x28D;
    }

    public static class Seat
    {
        public const ulong Occupant = 0x1B0;
    }

    public static class Sky
    {
        public const ulong MoonAngularSize = 0x244;
        public const ulong MoonTextureId = 0xC8;
        public const ulong SkyboxBk = 0xF8;
        public const ulong SkyboxDn = 0x128;
        public const ulong SkyboxFt = 0x158;
        public const ulong SkyboxLf = 0x188;
        public const ulong SkyboxOrientation = 0x238;
        public const ulong SkyboxRt = 0x1B8;
        public const ulong SkyboxUp = 0x1E8;
        public const ulong StarCount = 0x248;
        public const ulong SunAngularSize = 0x24C;
        public const ulong SunTextureId = 0x218;
    }

    public static class SpecialMesh
    {
        public const ulong MeshId = 0xF8;
        public const ulong Offset = 0xB8;
        public const ulong Scale = 0xC4;
        public const ulong TextureId = 0x128;
    }

    public static class TaskScheduler
    {
        public const ulong JobEnd = 0xD0;
        public const ulong JobName = 0x18;
        public const ulong JobStart = 0xC8;
        public const ulong MaxFps = 0xB0;
        public const ulong Pointer = 0x8041EC8;
    }

    public static class Team
    {
        public const ulong TeamColor = 0xB8;
    }

    public static class Terrain
    {
        public const ulong GrassLength = 0x188;
        public const ulong MaterialColors = 0x438;
        public const ulong WaterColor = 0x178;
        public const ulong WaterReflectance = 0x190;
        public const ulong WaterTransparency = 0x194;
        public const ulong WaterWaveSize = 0x198;
        public const ulong WaterWaveSpeed = 0x19C;
    }

    public static class TextButton
    {
        public const ulong AutoButtonColor = 0x9C4;
        public const ulong ContentText = 0xDD0;
        public const ulong Font = 0x10E8;
        public const ulong LineHeight = 0xEC8;
        public const ulong LocalizedText = 0xDD0;
        public const ulong MaxVisibleGraphemes = 0x10F4;
        public const ulong Modal = 0x9C5;
        public const ulong RichText = 0xFC6;
        public const ulong Selected = 0x9C6;
        public const ulong Text = 0xDD0;
        public const ulong TextColor3 = 0x10D0;
        public const ulong TextDirection = 0xF68;
        public const ulong TextScaled = 0xDB9;
        public const ulong TextSize = 0x10FC;
        public const ulong TextStrokeColor3 = 0x10DC;
        public const ulong TextStrokeTransparency = 0x1100;
        public const ulong TextTransparency = 0x1104;
        public const ulong TextTruncate = 0x1108;
        public const ulong TextWrapped = 0xFC0;
        public const ulong TextXAlignment = 0x110C;
        public const ulong TextYAlignment = 0xF10;
    }

    public static class TextLabel
    {
        public const ulong ContentText = 0xB50;
        public const ulong Font = 0xE68;
        public const ulong LineHeight = 0xC48;
        public const ulong LocalizedText = 0xB50;
        public const ulong MaxVisibleGraphemes = 0xE74;
        public const ulong RichText = 0xD46;
        public const ulong Text = 0xB50;
        public const ulong TextColor3 = 0xE50;
        public const ulong TextDirection = 0xCE8;
        public const ulong TextScaled = 0xD3E;
        public const ulong TextSize = 0xE7C;
        public const ulong TextStrokeColor3 = 0xE5C;
        public const ulong TextStrokeTransparency = 0xE80;
        public const ulong TextTransparency = 0xE84;
        public const ulong TextTruncate = 0xE88;
        public const ulong TextWrapped = 0xD40;
        public const ulong TextXAlignment = 0xE8C;
        public const ulong TextYAlignment = 0xC90;
    }

    public static class Tool
    {
        public const ulong CanBeDropped = 0x4B8;
        public const ulong Enabled = 0x4B9;
        public const ulong Grip = 0x488;
        public const ulong GripForward = 0x4A0;
        public const ulong GripPos = 0x4AC;
        public const ulong GripRight = 0x488;
        public const ulong GripUp = 0x494;
        public const ulong ManualActivationOnly = 0x4BA;
        public const ulong RequiresHandle = 0x4BB;
        public const ulong Tooltip = 0x468;
    }

    public static class Value
    {
        public const ulong Value = 0xB8;
    }

    public static class VehicleSeat
    {
        public const ulong MaxSpeed = 0x1C8;
        public const ulong Occupant = 0x1A8;
        public const ulong SteerFloat = 0x1D0;
        public const ulong ThrottleFloat = 0x1D8;
        public const ulong Torque = 0x1DC;
        public const ulong TurnSpeed = 0x1E0;
    }

    public static class VisualEngine
    {
        public const ulong Dimensions = 0xAB0;
        public const ulong FakeDataModel = 0xA90;
        public const ulong Pointer = 0x81D61C8;
        public const ulong RenderView = 0xBB8;
        public const ulong ViewMatrix = 0x150;
    }

    public static class Workspace
    {
        public const ulong CurrentCamera = 0x488;
        public const ulong ReadOnlyGravity = 0x998;
        public const ulong World = 0x3E0;
    }

    public static class World
    {
        public const ulong Gravity = 0x210;
        public const ulong Primitives = 0x288;
        public const ulong WorldSteps = 0x680;
    }

} // namespace RobloxOffsets
