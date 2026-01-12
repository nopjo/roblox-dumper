--!optimize 2
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

local promptPart = Instance.new("Part")
promptPart.Name = "ProximityPromptTest"
promptPart.Anchored = true

local prompt = Instance.new("ProximityPrompt")
prompt.Name = "TestPrompt"
prompt.ActionText = "TestAction123"
prompt.ObjectText = "TestObject456"
prompt.MaxActivationDistance = 12.5
prompt.HoldDuration = 2.56
prompt.Enabled = true
prompt.RequiresLineOfSight = false
prompt.KeyboardKeyCode = Enum.KeyCode.E
prompt.Parent = promptPart

promptPart.Parent = ReplicatedStorage

local plasticFloor = Instance.new("Part")
plasticFloor.Name = "PlasticFloor"
plasticFloor.Material = Enum.Material.Plastic
plasticFloor.Size = Vector3.new(30, 3, 30)
plasticFloor.Position = Vector3.new(0, -1.5, 0)
plasticFloor.Anchored = true
plasticFloor.CanCollide = true
plasticFloor.Parent = Workspace

local woodFloor = Instance.new("Part")
woodFloor.Name = "WoodFloor"
woodFloor.Material = Enum.Material.Wood
woodFloor.Size = Vector3.new(30, 3, 30)
woodFloor.Position = Vector3.new(60, -1.5, 0) 
woodFloor.Anchored = true
woodFloor.CanCollide = true
woodFloor.Parent = Workspace

local clientGuiInfo = {
	abs_pos_x = 0,
	abs_pos_y = 0,
	abs_size_x = 0,
	abs_size_y = 0,
	ready = false
}

local guiInfoRemote = Instance.new("RemoteEvent")
guiInfoRemote.Name = "GuiInfoRemote"
guiInfoRemote.Parent = ReplicatedStorage

guiInfoRemote.OnServerEvent:Connect(function(player, info)
	clientGuiInfo.abs_pos_x = info.abs_pos_x
	clientGuiInfo.abs_pos_y = info.abs_pos_y
	clientGuiInfo.abs_size_x = info.abs_size_x
	clientGuiInfo.abs_size_y = info.abs_size_y
	clientGuiInfo.ready = true
	print("Received GUI info from client:", info.abs_pos_x, info.abs_pos_y, info.abs_size_x, info.abs_size_y)
end)

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
		
	elseif action == "set_npc_auto_rotate" then
		local npc = Workspace:FindFirstChild("npc")
		if npc then
			local hum = npc:FindFirstChild("Humanoid")
			if hum then
				run(function() hum.AutoRotate = data.value end)
			else
				submitResult(commandId, "failed", {error = "Humanoid missing"})
			end
		else
			submitResult(commandId, "failed", {error = "NPC missing"})
		end

	elseif action == "set_npc_auto_jump_enabled" then
		local npc = Workspace:FindFirstChild("npc")
		if npc then
			local hum = npc:FindFirstChild("Humanoid")
			if hum then
				run(function() hum.AutoJumpEnabled = data.value end)
			else
				submitResult(commandId, "failed", {error = "Humanoid missing"})
			end
		else
			submitResult(commandId, "failed", {error = "NPC missing"})
		end

	elseif action == "set_npc_break_joints_on_death" then
		local npc = Workspace:FindFirstChild("npc")
		if npc then
			local hum = npc:FindFirstChild("Humanoid")
			if hum then
				run(function() hum.BreakJointsOnDeath = data.value end)
			else
				submitResult(commandId, "failed", {error = "Humanoid missing"})
			end
		else
			submitResult(commandId, "failed", {error = "NPC missing"})
		end

	elseif action == "set_npc_requires_neck" then
		local npc = Workspace:FindFirstChild("npc")
		if npc then
			local hum = npc:FindFirstChild("Humanoid")
			if hum then
				run(function() hum.RequiresNeck = data.value end)
			else
				submitResult(commandId, "failed", {error = "Humanoid missing"})
			end
		else
			submitResult(commandId, "failed", {error = "NPC missing"})
		end

	elseif action == "set_npc_use_jump_power" then
		local npc = Workspace:FindFirstChild("npc")
		if npc then
			local hum = npc:FindFirstChild("Humanoid")
			if hum then
				run(function() hum.UseJumpPower = data.value end)
			else
				submitResult(commandId, "failed", {error = "Humanoid missing"})
			end
		else
			submitResult(commandId, "failed", {error = "NPC missing"})
		end

	elseif action == "set_npc_continuous_jump" then
		local npc = Workspace:FindFirstChild("npc")
		if npc then
			local hum = npc:FindFirstChild("Humanoid")
			if hum then
				run(function()
					if data.enabled then
						local endTime = tick() + (data.duration or 3)
						while tick() < endTime do
							hum.Jump = true
							task.wait(0.05)
						end
					end
				end)
			else
				submitResult(commandId, "failed", {error = "Humanoid missing"})
			end
		else
			submitResult(commandId, "failed", {error = "NPC missing"})
		end
		
	elseif action == "set_npc_move_direction" then
		local npc = Workspace:FindFirstChild("npc")
		if npc then
			local hum = npc:FindFirstChild("Humanoid")
			if hum then
				run(function()
					hum:Move(Vector3.new(data.x, data.y, data.z))
				end)
			else
				submitResult(commandId, "failed", {error = "Humanoid missing"})
			end
		else
			submitResult(commandId, "failed", {error = "NPC missing"})
		end

	elseif action == "move_npc_to_floor" then
		local npc = Workspace:FindFirstChild("npc")
		local floor = Workspace:FindFirstChild(data.floor_name)
		if npc and floor then
			local hrp = npc:FindFirstChild("HumanoidRootPart")
			if hrp then
				run(function()
					hrp.CFrame = CFrame.new(floor.Position + Vector3.new(0, 5, 0))
					task.wait(0.3)
				end)
			else
				submitResult(commandId, "failed", {error = "HumanoidRootPart missing"})
			end
		else
			submitResult(commandId, "failed", {error = "NPC or floor '" .. tostring(data.floor_name) .. "' missing"})
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
		
	elseif action == "set_frame_visible" then
		local frame, err = getScreenGuiFrame()
		if frame then
			run(function() frame.Visible = data.value end)
		else
			submitResult(commandId, "failed", {error = err})
		end
		
	elseif action == "get_client_gui_info" then
		if clientGuiInfo.ready then
			submitResult(commandId, "completed", {
				abs_pos_x = clientGuiInfo.abs_pos_x,
				abs_pos_y = clientGuiInfo.abs_pos_y,
				abs_size_x = clientGuiInfo.abs_size_x,
				abs_size_y = clientGuiInfo.abs_size_y
			})
		else
			submitResult(commandId, "failed", {error = "Client GUI info not ready"})
		end

	elseif action == "request_client_gui_info" then
		guiInfoRemote:FireAllClients("request", data.frame_name or "hello")
		run(function()
			task.wait(0.5)
		end)
		
	elseif action == "set_part_cast_shadow" then
		local part = Workspace:FindFirstChild(data.part_name)
		if part then
			run(function() part.CastShadow = data.value end)
		else
			submitResult(commandId, "failed", {error = "Part not found"})
		end

	elseif action == "set_part_locked" then
		local part = Workspace:FindFirstChild(data.part_name)
		if part then
			run(function() part.Locked = data.value end)
		else
			submitResult(commandId, "failed", {error = "Part not found"})
		end

	elseif action == "set_part_massless" then
		local part = Workspace:FindFirstChild(data.part_name)
		if part then
			run(function() part.Massless = data.value end)
		else
			submitResult(commandId, "failed", {error = "Part not found"})
		end

	elseif action == "set_tool_can_be_dropped" then
		local tool = ReplicatedStorage:FindFirstChild("Tool1")
		if tool then
			run(function() tool.CanBeDropped = data.value end)
		else
			submitResult(commandId, "failed", {error = "Tool1 missing"})
		end
	elseif action == "set_tool_enabled" then
		local tool = ReplicatedStorage:FindFirstChild("Tool1")
		if tool then
			run(function() tool.Enabled = data.value end)
		else
			submitResult(commandId, "failed", {error = "Tool1 missing"})
		end
	elseif action == "set_tool_manual_activation" then
		local tool = ReplicatedStorage:FindFirstChild("Tool1")
		if tool then
			run(function() tool.ManualActivationOnly = data.value end)
		else
			submitResult(commandId, "failed", {error = "Tool1 missing"})
		end
	elseif action == "set_tool_requires_handle" then
		local tool = ReplicatedStorage:FindFirstChild("Tool1")
		if tool then
			run(function() tool.RequiresHandle = data.value end)
		else
			submitResult(commandId, "failed", {error = "Tool1 missing"})
		end
	elseif action == "set_tool_grip_pos" then
		local tool = ReplicatedStorage:FindFirstChild("Tool1")
		if tool then
			run(function() tool.GripPos = Vector3.new(data.x, data.y, data.z) end)
		else
			submitResult(commandId, "failed", {error = "Tool1 missing"})
		end
	elseif action == "set_highlight_depth_mode" then
		local highlight = ReplicatedStorage:FindFirstChildOfClass("Highlight")
		if highlight then
			run(function()
				if data.value == 0 then
					highlight.DepthMode = Enum.HighlightDepthMode.AlwaysOnTop
				elseif data.value == 1 then
					highlight.DepthMode = Enum.HighlightDepthMode.Occluded
				end
			end)
		else
			submitResult(commandId, "failed", {error = "Highlight missing"})
		end

	elseif action == "set_proximity_prompt_enabled" then
		local promptPart = ReplicatedStorage:FindFirstChild("ProximityPromptTest")
		if promptPart then
			local prompt = promptPart:FindFirstChild("TestPrompt")
			if prompt then
				run(function() prompt.Enabled = data.value end)
			else
				submitResult(commandId, "failed", {error = "TestPrompt missing"})
			end
		else
			submitResult(commandId, "failed", {error = "ProximityPromptTest missing"})
		end


	elseif action == "set_proximity_prompt_requires_line_of_sight" then
		local promptPart = ReplicatedStorage:FindFirstChild("ProximityPromptTest")
		if promptPart then
			local prompt = promptPart:FindFirstChild("TestPrompt")
			if prompt then
				run(function() prompt.RequiresLineOfSight = data.value end)
			else
				submitResult(commandId, "failed", {error = "TestPrompt missing"})
			end
		else
			submitResult(commandId, "failed", {error = "ProximityPromptTest missing"})
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
