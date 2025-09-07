-- This is a lua global variable
some_var = 7 * 6
user_name = "Guangchu"
print("Hello " .. user_name)

if some_var == 42 then
	print("value of var is " .. some_var)
end

-- Lua table
config = {
	title = "C++ 2D Game Engine",
	fullscreen = false,
	resolution = {
		width = 1800,
		height = 1200,
	},
}

entities = {
	tank = {
		transform = {
			position = { x = 300, y = 200 },
			scale = {},
		},
		box_collider = {},
		projectile_emitter = {
			-- angle = rand(0, 360),
		},
	},
}

function factorial(n)
	if n == 1 then
		return 1
	end
	return n * factorial(n - 1)
end

print("3 cube is " .. cube(3))
