level = {
	--[[
	-- Table to define the list of assets
	]]
	assets = {
		[0] = { type = "texture", id = "tilemap-texture", file = "./assets/tilemaps/jungle.png" },
		{ type = "texture", id = "chopper-texture", file = "./assets/images/chopper-spritesheet.png" },
		{ type = "texture", id = "takeoff-texture", file = "./assets/images/takeoff-base.png" },
		{ type = "texture", id = "landing-texture", file = "./assets/images/landing-base.png" },
		{ type = "texture", id = "tank-texture",    file = "./assets/images/tank-tiger-right.png" },
		{ type = "texture", id = "truck-texture",   file = "./assets/images/truck-ford-right.png" },
		{ type = "texture", id = "radar-texture",   file = "./assets/images/radar.png" },
		{ type = "texture", id = "bullet-texture",  file = "./assets/images/bullet.png" },
		{ type = "font",    id = "pico-font-5",     file = "./assets/fonts/pico8.ttf",               font_size = 5 },
		{ type = "font",    id = "pico-font-10",    file = "./assets/fonts/pico8.ttf",               font_size = 10 },
		{ type = "font",    id = "charriot-font",   file = "./assets/fonts/charriot.ttf",            font_size = 24 },
	},

	--[[
	-- Table to define the map config variables
	]]
	tilemap = {
		map_file = "./assets/tilemaps/jungle.map",
		texture_asset_id = "tilemap-texture",
		num_rows = 20,
		num_cols = 25,
		tile_size = 32,
		scale = 3.0,
	},

	--[[
	-- Table to define entities and their components
	]]
	entities = {
		[0] = {
			-- Player
			tag = "player",
			components = {
				transform = {
					position = { x = 240, y = 108 },
					scale = { x = 1.0, y = 1.0 },
					rotation = 0.0, -- deg
				},
				rigid_body = {
					velocity = { x = 0.0, y = 0.0 },
				},
				sprite = {
					texture_asset_id = "chopper-texture",
					width = 32,
					height = 32,
					z_index = 5,
				},
				animation = {
					num_frames = 2,
					speed_rate = 10, -- fps
				},
				boxcollider = {
					offset = { x = 0, y = 5 },
					width = 32,
					height = 25,
				},
				health = {
					health_pct = 100,
				},
				projectile_emitter = {
					projectile_velocity = { x = 100, y = 100 },
					projectile_duration = 10, -- secs
					repeat_freq = 0, -- secs
					hit_pct_damage = 10,
					friendly = true,
				},
				keyboard_controller = {
					up_vel = { x = 0, y = -50 },
					right_vel = { x = 50, y = 0 },
					down_vel = { x = 0, y = 50 },
					left_vel = { x = -50, y = 0 },
				},
				camera_follow = {
					follow = true,
				},
			},
		},
		{
			-- Tank
			group = "enemies",
			components = {
				transform = {
					position = { x = 800, y = 150 },
					scale = { x = 1.0, y = 1.0 },
					rotation = 0.0, -- deg
				},
				rigid_body = {
					velocity = { x = 5.0, y = 0.0 },
				},
				sprite = {
					texture_asset_id = "tank-texture",
					width = 32,
					height = 32,
					z_index = 2,
				},
				boxcollider = {
					offset = { x = 0, y = 10 },
					width = 25,
					height = 15,
				},
				health = {
					health_pct = 100,
				},
				projectile_emitter = {
					projectile_velocity = { x = 100, y = 0 },
					projectile_duration = 10, -- secs
					repeat_freq = 3, -- secs
					hit_pct_damage = 20,
					friendly = false,
				},
			},
		},
	},
}
