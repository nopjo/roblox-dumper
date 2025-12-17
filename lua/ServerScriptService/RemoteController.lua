local HttpService = game:GetService("HttpService")
local Lighting = game:GetService("Lighting")
local Workspace = game:GetService("Workspace")
local ReplicatedStorage = game:GetService("ReplicatedStorage")
local Teams = game:GetService("Teams")
local Players = game:GetService("Players")

local API_URL = nil

local startEvent = Instance.new("RemoteEvent")
startEvent.Name = "StartPolling"
startEvent.Parent = ReplicatedStorage

local cameraEvent = Instance.new("RemoteEvent")
cameraEvent.Name = "CameraCommand"
cameraEvent.Parent = ReplicatedStorage

local redTeam = Teams:FindFirstChild("Red Team")
if not redTeam then
	redTeam = Instance.new("Team")
	redTeam.Name = "Red Team"
	redTeam.TeamColor = BrickColor.new("Bright red")
	redTeam.Parent = Teams
end

local blueTeam = Teams:FindFirstChild("Blue Team")
if not blueTeam then
	blueTeam = Instance.new("Team")
	blueTeam.Name = "Blue Team"
	blueTeam.TeamColor = BrickColor.new("Bright blue")
	blueTeam.Parent = Teams
end

local player = Players.LocalPlayer
if player then
	player.Team = redTeam
	print("[Teams] Player assigned to Red Team")
end

local function submitResult(commandId, status, result)
	local payload = HttpService:JSONEncode({
		command_id = commandId,
		status = status,
		result = result,
		timestamp = os.date("!%Y-%m-%dT%H:%M:%SZ")
	})
	pcall(function()
		HttpService:PostAsync(API_URL .. "/result", payload, Enum.HttpContentType.ApplicationJson)
	end)
end

local function setColor3(object, property, data)
	object[property] = Color3.fromRGB(data.r, data.g, data.b)
end

local function setUDim2(object, property, data)
	object[property] = UDim2.new(data.x_scale, data.x_offset, data.y_scale, data.y_offset)
end

local function getScreenGuiFrame()
	local screenGui = ReplicatedStorage:FindFirstChildOfClass("ScreenGui")
	if not screenGui then
		return nil, "ScreenGui missing"
	end
	local frame = screenGui:FindFirstChildOfClass("Frame")
	if not frame then
		return nil, "Frame missing"
	end
	return frame
end

local function handleCommand(cmd)
	local action = cmd.action
	local data = cmd.data
	local commandId = cmd.id

	local function run(f)
		local s, e = pcall(f)
		if s then
			print("[Controller] Command", commandId, "completed:", action)
			submitResult(commandId, "completed", {})
		else
			print("[Controller] Command", commandId, "failed:", action, tostring(e))
			submitResult(commandId, "failed", {error = tostring(e)})
		end
	end

	print("[Controller] Executing:", action, "with data:", HttpService:JSONEncode(data))

	if action == "set_camera_fov" or action == "set_camera_position" or action == "set_camera_rotation" then
		cameraEvent:FireAllClients(action, data)
		submitResult(commandId, "completed", {})
		return
	end

	if action == "set_team" then
		run(function()
			local player = Players.LocalPlayer
			if data.team == "red" then
				player.Team = redTeam
			elseif data.team == "blue" then
				player.Team = blueTeam
			end
		end)
		return
	end

	if action == "set_gravity" then
		run(function()
			Workspace.Gravity = data.value
		end)

	elseif action == "set_ambient" then
		run(function() setColor3(Lighting, "Ambient", data) end)
	elseif action == "set_outdoor_ambient" then
		run(function() setColor3(Lighting, "OutdoorAmbient", data) end)
	elseif action == "set_color_shift_top" then
		run(function() setColor3(Lighting, "ColorShift_Top", data) end)
	elseif action == "set_color_shift_bottom" then
		run(function() setColor3(Lighting, "ColorShift_Bottom", data) end)

	elseif action == "set_skybox_orientation" then
		local sky = Lighting:FindFirstChildOfClass("Sky")
		if sky then
			run(function()
				sky.CelestialBodiesShown = true
				sky.SkyboxOrientation = Vector3.new(data.x, data.y, data.z)
			end)
		else
			submitResult(commandId, "failed", {error = "No Sky"})
		end

	elseif action == "set_npc_move_to" then
		local npc = Workspace:FindFirstChild("npc")
		local moveto = Workspace:FindFirstChild("moveto")
		if npc and moveto then
			local hum = npc:FindFirstChild("Humanoid")
			if hum then
				run(function()
					if data.enabled then
						hum:MoveTo(moveto.Position)
					else
						hum:MoveTo(npc.HumanoidRootPart.Position)
					end
				end)
			else
				submitResult(commandId, "failed", {error = "Humanoid missing"})
			end
		else
			submitResult(commandId, "failed", {error = "NPC or moveto missing"})
		end

	elseif action == "set_atmosphere_color" then
		local atmosphere = Lighting:FindFirstChildOfClass("Atmosphere")
		if atmosphere then
			run(function() setColor3(atmosphere, "Color", data) end)
		else
			submitResult(commandId, "failed", {error = "Atmosphere missing"})
		end
	elseif action == "set_atmosphere_decay" then
		local atmosphere = Lighting:FindFirstChildOfClass("Atmosphere")
		if atmosphere then
			run(function() setColor3(atmosphere, "Decay", data) end)
		else
			submitResult(commandId, "failed", {error = "Atmosphere missing"})
		end

	elseif action == "set_color_correction_tint" then
		local cc = Lighting:FindFirstChildOfClass("ColorCorrectionEffect")
		if cc then
			run(function() setColor3(cc, "TintColor", data) end)
		else
			submitResult(commandId, "failed", {error = "ColorCorrectionEffect missing"})
		end

	elseif action == "set_ui_gradient_color" then
		local gradient = ReplicatedStorage:FindFirstChildOfClass("UIGradient")
		if gradient then
			run(function()
				gradient.Color = ColorSequence.new(Color3.fromRGB(data.r, data.g, data.b))
			end)
		else
			submitResult(commandId, "failed", {error = "UIGradient missing"})
		end

	elseif action == "set_frame_background_color" then
		local frame, err = getScreenGuiFrame()
		if frame then
			run(function() setColor3(frame, "BackgroundColor3", data) end)
		else
			submitResult(commandId, "failed", {error = err})
		end
	elseif action == "set_frame_border_color" then
		local frame, err = getScreenGuiFrame()
		if frame then
			run(function() setColor3(frame, "BorderColor3", data) end)
		else
			submitResult(commandId, "failed", {error = err})
		end
	elseif action == "set_frame_position" then
		local frame, err = getScreenGuiFrame()
		if frame then
			run(function() setUDim2(frame, "Position", data) end)
		else
			submitResult(commandId, "failed", {error = err})
		end
	elseif action == "set_frame_size" then
		local frame, err = getScreenGuiFrame()
		if frame then
			run(function() setUDim2(frame, "Size", data) end)
		else
			submitResult(commandId, "failed", {error = err})
		end
	end
end

local function pollCommands()
	local s, response = pcall(function()
		return HttpService:GetAsync(API_URL .. "/poll")
	end)
	if s then
		local data = HttpService:JSONDecode(response)
		for _, cmd in ipairs(data.commands) do
			task.spawn(function()
				handleCommand(cmd)
			end)
		end
	end
end

startEvent.OnServerEvent:Connect(function(player, url)
	API_URL = url
	print("[Controller] Started polling at:", url)
	task.spawn(function()
		while API_URL do
			pollCommands()
			task.wait(0.1)
		end
	end)
end)