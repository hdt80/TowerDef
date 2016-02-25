function onUpdate(diff)
	--print(me:getX() .. ", " .. me:getY());
end

function onDamageTaken(dmg, who)
	print("I took " .. dmg .. " dmg from " .. tostring(who));
end
