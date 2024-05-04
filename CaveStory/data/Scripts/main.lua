function ModCS.Game.Act()
	ModCS.Player.Equip(32)
	-- Swimming Script
	if (ModCS.Game.GetMode()) == 3 then
		ModCS.Collectable.Set(0, ModCS.Collectable.Get(0) + 1)
	
		if ModCS.Player.TouchWater() then
			if ModCS.Key.Jump() then
				ModCS.Player.ym = ModCS.Player.ym - 1280
			end
		end
	end
	
	if ModCS.Key.Arms() then
		ModCS.AVA.KillPlayer()
	end
	
	if ModCS.AVA.LoadingSave() == true then
		print("Loading Savefile")
	end
	
	-- Walljump!
	if ModCS.AVA.OnWall() then
		if ModCS.Key.Jump() then
			ModCS.Sound.Play(15)
			
			if ModCS.Player.direct == 0 then
				ModCS.Player.xm = 1000
				ModCS.Player.ym = -832
			else
				ModCS.Player.xm = -1000
				ModCS.Player.ym = -832
			end
		end
	end

	ModCS.Physics.Set(0, 3000)
end
local value = 3
ModCS.Npc.Act[70] = function(npc)
	-- define the rect for our entity
    local rect = {
        {96, 48, 112, 64},
        {112, 48, 128, 64},
        {128, 48, 144, 64},
        {144, 48, 160, 64}
    }
	
	value = ModCS.CPXE.Get(npc, 0)

	-- increase npc.ani_wait every frame
    npc.ani_wait = npc.ani_wait + 1
	
	-- if wait is above 3, then we set wait back to 0 and increase rect frame by 1
    if npc.ani_wait > value then
        npc.ani_wait = 0
        npc.ani_no = npc.ani_no + 1
    end

	-- if our *frame* is above 3, then we reset back to rect 0
    if npc.ani_no > 3 then
        npc.ani_no = 0
    end
	
	-- set the rect to the npc --> the [1], [2], [3], and [4] part should just always be at the end.
	npc:SetRect(rect[npc.ani_no + 1][1], rect[npc.ani_no + 1][2], rect[npc.ani_no + 1][3], rect[npc.ani_no + 1][4])
end