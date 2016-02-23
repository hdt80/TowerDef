function onUpdate(diff)
	--print(me:getX() .. ", " .. me:getY());
end

function onShoot(target)
	print("Shooting at (" .. target:getX() .. ", " .. target:getY() .. ")");
end

function onDamageDealt(dmg, hit)
	print("Hit for " .. dmg .. "dmg")
end
