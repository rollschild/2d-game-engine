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
		{ type = "texture", id = "su27-texture",    file = "./assets/images/su27-spritesheet.png" },
		{ type = "texture", id = "f22-texture",     file = "./assets/images/f22-spritesheet.png" },
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
		{
			-- SU-27 fighter jet
			group = "enemies",
			components = {
				transform = {
					position = { x = 317, y = 985 },
					scale = { x = 1.0, y = 1.0 },
					rotation = 0.0, -- degrees
				},
				rigid_body = {
					velocity = { x = 0.0, y = -50.0 },
				},
				sprite = {
					texture_asset_id = "su27-texture",
					width = 32,
					height = 32,
					z_index = 5,
				},
				animation = {
					num_frames = 2,
					speed_rate = 10, -- fps
				},
				boxcollider = {
					width = 32,
					height = 32,
				},
				health = {
					health_pct = 100,
				},
				projectile_emitter = {
					projectile_velocity = { x = 0, y = -100 },
					projectile_duration = 5, -- seconds
					repeat_freq = 1, -- seconds
					hit_pct_damage = 10,
					friendly = false,
				},
				on_update_script = {
					[0] = function(entity, delta_time, ellapsed_time)
						print("Executing the SU-27 fighter jet Lua script!")

						--[[
                        -- this function makes the fighter jet move up and down the map shooting projectiles
                        local current_position_x, current_position_y = get_position(entity)
                        local current_velocity_x, current_velocity_y = get_velocity(entity)

                        -- if it reaches the top or the bottom of the map
                        if current_position_y < 10  or current_position_y > map_height - 32 then
                            set_velocity(entity, 0, current_velocity_y * -1); -- flip the entity y-velocity
                        else
                            set_velocity(entity, 0, current_velocity_y); -- do not flip y-velocity
                        end

                        -- set the transform rotation to match going up or down
                        if (current_velocity_y < 0) then
                            set_rotation(entity, 0) -- point up
                            set_projectile_velocity(entity, 0, -200) -- shoot projectiles up
                        else
                            set_rotation(entity, 180) -- point down
                            set_projectile_velocity(entity, 0, 200) -- shoot projectiles down
                        end
                        --]]
					end,
				},
			},
		},
		{
			-- F-22 fighter jet
			group = "enemies",
			components = {
				transform = {
					position = { x = 10, y = 10 },
					scale = { x = 1.0, y = 1.0 },
					rotation = 90.0, -- degrees
				},
				rigid_body = {
					velocity = { x = 0.0, y = 0.0 },
				},
				sprite = {
					texture_asset_id = "f22-texture",
					width = 32,
					height = 32,
					z_index = 5,
				},
				animation = {
					num_frames = 2,
					speed_rate = 10, -- fps
				},
				boxcollider = {
					width = 32,
					height = 32,
				},
				health = {
					health_pct = 100,
				},
				projectile_emitter = {
					projectile_velocity = { x = 200, y = 0 },
					projectile_duration = 1, -- seconds
					repeat_freq = 1, -- seconds
					hit_pct_damage = 10,
					friendly = false,
				},
				on_update_script = {
					[0] = function(entity, delta_time, ellapsed_time)
						print("Executing F-22 Lua script!")

						-- change the position of the airplane to follow a sine wave movement
						local new_x = ellapsed_time * 0.09
						local new_y = 200 + (math.sin(ellapsed_time * 0.001) * 50)
						set_position(entity, new_x, new_y) -- set the new position
					end,
				},
			},
		},
	},
}

map_width = level.tilemap.num_cols * level.tilemap.tile_size * level.tilemap.scale
map_height = level.tilemap.num_rows * level.tilemap.tile_size * level.tilemap.scale
