function onUpdate(diff)
	print(me:getX() .. ", " .. me:getY());
end

function onShoot(target)

end

function onDamageDealt(dmg, hit)
	local objs = getObjectsInRange(hit:getX(), hit:getY(), 50);
	print(me);
	for k, v in pairs(objs) do
		v:applyDamage(1);
	end
end
