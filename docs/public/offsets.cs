/*
 * Dumped With: roblox-dumper 2.0
 * Created by: Jonah (jonahw on Discord)
 * Github: https://github.com/nopjo/roblox-dumper
 * Time Taken: 3441 ms (3.441000 seconds)
 * Total Offsets: 204
 */

using System;

namespace RobloxOffsets
{
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
        public const ulong BaseTextureId = 0xD0;
        public const ulong BodyPart = 0xE8;
        public const ulong MeshId = 0xD8;
        public const ulong OverlayTextureId = 0xE0;
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
        public const ulong Pointer = 0x6C14ED8;
        public const ulong RealDataModel = 0x1C0;
    }

    public static class GuiBase2D
    {
        public const ulong AbsolutePosition = 0x10C;
        public const ulong AbsoluteRotation = 0x188;
        public const ulong AbsoluteSize = 0x118;
    }

    public static class GuiObject
    {
        public const ulong Active = 0x5AC;
        public const ulong AnchorPoint = 0x560;
        public const ulong AutomaticSize = 0x568;
        public const ulong BackgroundColor3 = 0x548;
        public const ulong BackgroundTransparency = 0x56C;
        public const ulong BorderColor3 = 0x554;
        public const ulong BorderMode = 0x570;
        public const ulong BorderSizePixel = 0x574;
        public const ulong ClipsDescendants = 0x5AD;
        public const ulong GuiState = 0x580;
        public const ulong Interactable = 0x5AF;
        public const ulong LayoutOrder = 0x584;
        public const ulong Position = 0x518;
        public const ulong Rotation = 0x188;
        public const ulong Selectable = 0x5B0;
        public const ulong SelectionOrder = 0x5A0;
        public const ulong Size = 0x538;
        public const ulong SizeConstraint = 0x5A4;
        public const ulong Visible = 0x5B1;
        public const ulong ZIndex = 0x5A8;
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
        public const ulong Sit = 0x1E1;
        public const ulong TargetPoint = 0x164;
        public const ulong UseJumpPower = 0x1E3;
        public const ulong WalkSpeed = 0x1D4;
        public const ulong WalkSpeedCheck = 0x3C0;
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
        public const ulong ChildrenStart = 0x70;
        public const ulong ClassDescriptor = 0x18;
        public const ulong ClassName = 0x8;
        public const ulong Name = 0xB0;
        public const ulong Parent = 0x68;
    }

    public static class Lighting
    {
        public const ulong Ambient = 0xD8;
        public const ulong Atmosphere = 0x1E8;
        public const ulong Brightness = 0x120;
        public const ulong ClockTime = 0x1B8;
        public const ulong ColorShift_Bottom = 0xE4;
        public const ulong ColorShift_Top = 0xF0;
        public const ulong EnvironmentDefuseScale = 0x124;
        public const ulong EnvironmentSpecularScale = 0x128;
        public const ulong GeographicLatitude = 0x190;
        public const ulong OutdoorAmbient = 0x108;
        public const ulong Sky = 0x1D8;
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
        public const ulong MeshId = 0x2E8;
        public const ulong TextureId = 0x318;
    }

    public static class MouseService
    {
        public const ulong InputObject = 0x110;
    }

    public static class Player
    {
        public const ulong AccountAge = 0x30C;
        public const ulong Character = 0x380;
        public const ulong DisplayName = 0x130;
        public const ulong HealthDisplayDistance = 0x338;
        public const ulong LocaleId = 0x6B8;
        public const ulong NameDisplayDistance = 0x344;
        public const ulong Team = 0x290;
        public const ulong TeamColor = 0x350;
        public const ulong UserId = 0x2B8;
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
        public const ulong Material = 0x246;
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

    public static class Team
    {
        public const ulong TeamColor = 0xD0;
    }

    public static class Terrain
    {
        public const ulong GrassLength = 0x1F8;
        public const ulong MaterialColors = 0x280;
        public const ulong WaterColor = 0x1E8;
        public const ulong WaterReflectance = 0x200;
        public const ulong WaterTransparency = 0x204;
        public const ulong WaterWaveSize = 0x208;
        public const ulong WaterWaveSpeed = 0x20C;
    }

    public static class Tool
    {
        public const ulong CanBeDropped = 0x4A0;
        public const ulong Enabled = 0x4A1;
        public const ulong Grip = 0x470;
        public const ulong GripForward = 0x488;
        public const ulong GripPos = 0x494;
        public const ulong GripRight = 0x470;
        public const ulong GripUp = 0x47C;
        public const ulong ManualActivationOnly = 0x4A2;
        public const ulong RequiresHandle = 0x4A3;
        public const ulong Tooltip = 0x450;
    }

    public static class Value
    {
        public const ulong Value = 0xD0;
    }

    public static class VisualEngine
    {
        public const ulong Dimensions = 0x720;
        public const ulong FakeDataModel = 0x700;
        public const ulong Pointer = 0x7AED020;
        public const ulong RenderView = 0x800;
        public const ulong ViewMatrix = 0x120;
    }

    public static class Workspace
    {
        public const ulong CurrentCamera = 0x460;
        public const ulong ReadOnlyGravity = 0x9C0;
        public const ulong World = 0x3D8;
    }

    public static class World
    {
        public const ulong Gravity = 0x1D0;
    }

} // namespace RobloxOffsets
