
width = 200
height = 300

function f (x, y)
	return (x^2 * math.sin(y))/(1 - x)
end

--以传统方式注册函数
function test1()
	a = array.new(1000)
	print(a) --> userdata: 0x8064d48
	print(array.size(a)) --> 1000
	for i=1,1000 do
		array.set(a, i, 1/i)
	end

	print(array.get(a, 10))
end

--以面向对象方式注册
function test2()
	a = array.new(1000)
	print(a:size()) --> 1000
	a:set(10, 3.4)
	print(a:get(10)) --> 3.4
end

--以传统方式注册函数时 使用传统模式访问数组元素
function test3()
	local metaarray = getmetatable(array.new(1))
	metaarray.__index = array.get
	metaarray.__newindex = array.set
	a = array.new(1000)
	a[10] = 3.4 -- setarray
	print(a[10]) -- getarray --> 3.4
end

function test4()
	for fname in dir(".") do print(fname) end
end

test4()
