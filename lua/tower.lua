function onUpdate(diff)
	--print(me:getX() .. ", " .. me:getY());
end

function onShoot(target)
	print("Shooting at (" .. target:getX() .. ", " .. target:getY() .. ")");
end

function onDamageDealt(dmg, hit)
	me:setDamage(me:getDamage() + (me:getDamage() * 0.1));
	print("Damage: " .. me:getDamage());
end
