function ModCS.Game.Act()
	-- Swimming Script
	if (ModCS.Game.GetMode()) == 3 then
		ModCS.Collectable.Set(0, ModCS.Collectable.Get(0) + 1)
	
		if ModCS.Player.TouchWater() then
			if ModCS.Key.Jump() then
				ModCS.Player.ym = ModCS.Player.ym - 1280
			end
		end
	end
	
	if ModCS.AVA.LoadingSave() == true then
		print("Loading Savefile")
	end
end

function ModCS.Tsc.Command.CN1()
    local newSheet = ModCS.Tsc.GetString()
	ModCS.Surface.LoadBitmap(21, newSheet)
end

function ModCS.Tsc.Command.CN2()
    local newSheet = ModCS.Tsc.GetString()
	ModCS.Surface.LoadBitmap(22, newSheet)
end