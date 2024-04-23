function ModCS.Game.Act()
	-- Swimming Script
	if (ModCS.Game.GetMode()) == 3 then
		if ModCS.Player.TouchWater() then
			if ModCS.Key.Jump() then
				ModCS.Player.ym = ModCS.Player.ym - 1280
			end
			
			if ModCS.Key.Shoot() then
				ModCS.AVA.FMOD.PlayEvent("event:/Autumn/AVA/Test")
			end
		end
	end
	
	if ModCS.AVA.GetVar(1) == 10 then
		print("Hello World")
	end
	
	if ModCS.Key.Map() then
		ModCS.AVA.SetVar(1, ModCS.AVA.GetVar(1) + 1)
		print(ModCS.AVA.GetVar(1))
	end
end