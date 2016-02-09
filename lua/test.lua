foo = 4
bar = {}
bar[3] = "Hi"
bar["key"] = "there"

function onUpdate(diff)
	print("update " .. diff .. "ms")
end
