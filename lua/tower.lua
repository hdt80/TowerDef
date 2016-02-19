function onUpdate(diff)
	print("diff" .. diff);
	print(getX());
end

function onShoot(target)
	print("shot shit " .. target)
end

function onDamageDealt(dmg, hit)
	print("Hit for " .. dmg .. "dmg")
end
