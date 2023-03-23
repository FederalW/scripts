if (not getgenv().Tonka or TonkaSilent) then
    return Tonka
end

local Players, Client, Mouse, RS, Camera, r =
	game:GetService("Players"),
	game:GetService("Players").LocalPlayer,
	game:GetService("Players").LocalPlayer:GetMouse(),
	game:GetService("RunService"),
	game.Workspace.CurrentCamera,
	math.random

local Circle = Drawing.new("Circle")
Circle.Color = Color3.new(1,1,1)
Circle.Thickness = 1

local UpdateFOV = function ()
    if (not Circle) then
        return Circle
    end

    if Tonka then
        Circle.Visible = Tonka.FOV["Visible"]
        Circle.Radius = Tonka.FOV.Radius * 3
    else
        return Tonka
    end
    
    Circle.Position = Vector2.new(Mouse.X, Mouse.Y + (game:GetService("GuiService"):GetGuiInset().Y))
    return Circle
end

RS.Heartbeat:Connect(UpdateFOV)

Mouse.KeyDown:Connect(function(TonkaSilentAimEnable)
	if TonkaSilentAimEnable.KeyCode ~= Tonka.TonkaSilent.Keybind then
		return
	elseif TonkaSilentAimEnable.KeyCode == Tonka.TonkaSilent.Keybind then
		Tonka.TonkaSilent.Enabled = not Tonka.TonkaSilent.Enabled
	end
end)



ClosestPlrFromMouse = function()
    local Target, Closest = nil, 1/0
 
    for _ ,v in pairs(Players:GetPlayers()) do
        if (v.Character and v ~= Client and v.Character:FindFirstChild("HumanoidRootPart")) then
            local Position, OnScreen = Camera:WorldToScreenPoint(v.Character.HumanoidRootPart.Position)
            local Distance = (Vector2.new(Position.X, Position.Y) - Vector2.new(Mouse.X, Mouse.Y)).Magnitude

            if (Circle.Radius > Distance and Distance < Closest and OnScreen) then
                Closest = Distance
                Target = v
            end
        end
    end
    return Target
end

local WTS = function (Object)
    local ObjectVector = Camera:WorldToScreenPoint(Object.Position)
    return Vector2.new(ObjectVector.X, ObjectVector.Y)
end

local IsOnScreen = function (Object)
    local IsOnScreen = Camera:WorldToScreenPoint(Object.Position)
    return IsOnScreen
end

local FilterObjs = function (Object)
    if string.find(Object.Name, "Gun") then
        return
    end
    if table.find({"Part", "MeshPart", "BasePart"}, Object.ClassName) then
        return true
    end
end

local GetClosestBodyPart = function (character)
    local ClosestDistance = 1/0
    local BodyPart = nil
    if (character and character:GetChildren()) then
        for _,  x in next, character:GetChildren() do
            if FilterObjs(x) and IsOnScreen(x) then
                local Distance = ((WTS(x) - Vector2.new(Mouse.X, Mouse.Y))/Tonka.TonkaSilent.DistanceDivided).Magnitude
                if (Circle.Radius > Distance and Distance < ClosestDistance) then
                    ClosestDistance = Distance
                    BodyPart = x
                end
            end
        end
    end
    return BodyPart
end


local Prey
local PartToUse = Tonka.TonkaSilent.Part

task.spawn(function ()
    while task.wait() do
        if Prey then
            if Tonka.TonkaSilent.Enabled and Tonka.TonkaSilent.ClosestPart == true then
                PartToUse = tostring(GetClosestBodyPart(Prey.Character))
            end
            if Tonka.Config.UnlockOnDeath == true then
                if Prey.Character.Humanoid.Health < 2 then
                    Prey = nil
                end
            end
            if Tonka.Config.UnlockOnYourDeath == true then
                if Client.Character.Humanoid.Health < 2 then
                    Prey = nil
                end
            end
			if Tonka.Resolver.Enabled[1] == true then
				local playertoresolve = Prey.Character.HumanoidRootPart
				if playertoresolve.Velocity.X > Tonka.Resolver.Enabled[2] or playertoresolve.Velocity.X < Tonka.Resolver.Enabled[3] or playertoresolve.Velocity.Y > Tonka.Resolver.Enabled[2] or playertoresolve.Velocity.Y < Tonka.Resolver.Enabled[3] or playertoresolve.Velocity.Z > Tonka.Resolver.Enabled[2] or playertoresolve.Velocity.Z < Tonka.Resolver.Enabled[3] then
					playertoresolve.Velocity = Vector3.new(0, 0, 0)
				end
			end
			if Tonka.Config.AntiGroundShots then
				pcall(function()
                    local TargetVelv5 = Prey.Character[Tonka.TonkaSilent.Part]
                    TargetVelv5.Velocity = Vector3.new(TargetVelv5.Velocity.X, (TargetVelv5.Velocity.Y * 0.5), TargetVelv5.Velocity.Z)
                    TargetVelv5.AssemblyLinearVelocity = Vector3.new(TargetVelv5.Velocity.X, (TargetVelv5.Velocity.Y * 0.5), TargetVelv5.Velocity.Z)
                end)
			end
			if Tonka.Resolver.DisableOnAnti.Enabled == true and Prey.Character and Prey.Character:FindFirstChild(Sub.AIM.AIM_PART) then
				local playertoresolve = Prey.Character.HumanoidRootPart
				if playertoresolve.Velocity.X > Tonka.Resolver.DisableOnAnti.Max or playertoresolve.Velocity.X < Tonka.Resolver.DisableOnAnti.Min or playertoresolve.Velocity.Y > Tonka.Resolver.DisableOnAnti.Max or playertoresolve.Velocity.Y < Tonka.Resolver.DisableOnAnti.Min or playertoresolve.Velocity.Z > Tonka.Resolver.DisableOnAnti.Max or playertoresolve.Velocity.Z < Tonka.Resolver.DisableOnAnti.Min then
					AimlockTarget = nil
				end
			end
        end
    end
end)

local grmt = getrawmetatable(game)
local backupindex = grmt.__index
setreadonly(grmt, false)

grmt.__index = newcclosure(function(self, v)
    if (Tonka.TonkaSilent.Enabled and Mouse and tostring(v) == "Hit") then

        Prey = ClosestPlrFromMouse()

        if Prey then
            local endpoint = game.Players[tostring(Prey)].Character[PartToUse].CFrame + (
                game.Players[tostring(Prey)].Character[PartToUse].Velocity * Tonka.TonkaSilent.Pred
            )
            return (tostring(v) == "Hit" and endpoint)
        end
    end
    return backupindex(self, v)
end)



local Script = {Functions = {}}
    Script.Functions.getToolName = function(name)
        local split = string.split(string.split(name, "[")[2], "]")[1]
        return split
    end
    Script.Functions.getEquippedWeaponName = function()
        if (Client.Character) and Client.Character:FindFirstChildWhichIsA("Tool") then
           local Tool =  Client.Character:FindFirstChildWhichIsA("Tool")
           if string.find(Tool.Name, "%[") and string.find(Tool.Name, "%]") and not string.find(Tool.Name, "Wallet") and not string.find(Tool.Name, "Phone") then
              return Script.Functions.getToolName(Tool.Name)
           end
        end
        return nil
    end
    RS.RenderStepped:Connect(function()
    if Script.Functions.getEquippedWeaponName() ~= nil then
        local WeaponTonka = Tonka.FOV.GunFOV[Script.Functions.getEquippedWeaponName()]
        if WeaponTonka ~= nil and Tonka.FOV.GunFOV.Enabled == true then
            Tonka.FOV.Radius = WeaponTonka.FOV 
        else
            Tonka.FOV.Radius = Tonka.FOV.Radius
        end
    end
end)
