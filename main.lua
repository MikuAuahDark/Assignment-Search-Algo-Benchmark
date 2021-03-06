-- stars.csv comes from Pioneer Spacesim 02_local_stars.lua and 03_bright_stars.lua
local Vertex = {}
local Edge = {}
local Adjacent = {}

local function dist2(x1, y1, x2, y2)
	return (x1 - x2) ^ 2 + (y1 - y2) ^2
end

local function compareSet(u, v)
	if #u == #v then
		for i = 1, #v do
			local found = false

			for j = 1, #v do
				if u[i] == v[j] then
					found = true
					break
				end
			end

			if not found then
				return false
			end
		end

		return true
	end

	return false
end

local function contains(set, v)
	for i = 1, #set do
		if set[i] == v then
			return true
		end
	end

	return false
end

function love.load(arg)
	love.event.quit()

	local MAX = 4
	local MAXCOORD = MAX - MAX/128

	local seed = tonumber(arg[1] or math.random(0, 2147483647))
	local rng = love.math.newRandomGenerator(seed)
	local first = true

	-- parse data
	print("Parsing vertices")
	for line in love.filesystem.lines("stars.csv") do
		if first then
			first = false
		else
			local name, x, y, z = line:match("([^;]+);([^;]+);([^;]+);([^;]+)")
			x, y, z = tonumber(x), tonumber(y), tonumber(z)

			if name and x and y and z and math.abs(x) < MAXCOORD and math.abs(y) < MAXCOORD and math.abs(z) < MAXCOORD then
				Vertex[#Vertex + 1] = {name, x * 4096/MAX, y * 4096/MAX, z * 20}
			end
		end
	end

	-- remove vertex which are too close to each other
	print("Filtering vertices")
	for i = #Vertex, 1, -1 do
		local v = Vertex[i]
		local d = math.huge

		for j, u in ipairs(Vertex) do
			if i ~= j then
				local nd = dist2(u[2], u[3], v[2], v[3])
				if nd < d then
					d = nd
				end
			end
		end

		if d < 96*96 then
			table.remove(Vertex, i)
		end
	end
	print(#Vertex)

	-- Use Vertex as reverse index lookup too
	for i, v in ipairs(Vertex) do
		Vertex[v] = i
	end

	-- add edges to every vertex with distance as the cost
	do
		local tempVertex = {Vertex[rng:random(1, #Vertex)]}
		local tempAdjacent = {}

		print("Loading temporary edges")
		for i, v in ipairs(Vertex) do
			for j = 1, #Vertex do
				if j ~= i then
					local u = Vertex[j]
					local d = dist2(v[2], v[3], u[2], u[3])
					local e = {u, math.max(math.sqrt(dist2(v[2], v[3], u[2], u[3])) + (v[4] + u[4]) * (rng:random() / 2 + 0.5) * 2, 1)}
					tempAdjacent[v] = tempAdjacent[v] or {}
					table.insert(tempAdjacent[v], e)
				end
			end
		end

		-- Do Prim's algorithm to find MST
		print("Running Prim's algorithm")
		while compareSet(tempVertex, Vertex) == false do
			local u, v
			local c = math.huge

			for i, u2 in ipairs(tempVertex) do
				for j, e2 in ipairs(tempAdjacent[u2]) do
					if contains(tempVertex, e2[1]) == false and e2[2] < c then
						c = e2[2]
						u = u2
						v = e2[1]
					end
				end
			end

			assert(u and v and c < math.huge)
			local e = {u, v, c}
			tempVertex[#tempVertex + 1] = v
			Edge[#Edge + 1] = e
			Adjacent[u] = Adjacent[u] or {}
			table.insert(Adjacent[u], e)
		end
	end

	-- rewrite cost
	for _, e in ipairs(Edge) do
		local u, v = e[1], e[2]
		e[3] = math.floor(math.sqrt(dist2(v[2], v[3], u[2], u[3])) * 10)
	end

	-- plot
	local canvas = love.graphics.newCanvas(8192, 8192)
	local font1 = love.graphics.newFont(24)
	local font2 = love.graphics.newFont(18)
	local font1HeightH = font1:getHeight() / 2
	local font2HeightH = font2:getHeight() / 2
	local lineWidth = love.graphics.getLineWidth()

	love.graphics.push("all")

	love.graphics.setCanvas(canvas)
	love.graphics.clear(1,1,1)
	love.graphics.setColor(0,0,0)
	love.graphics.rectangle("line", 0,0,8192,8192)
	love.graphics.translate(4096, 4096)

	love.graphics.setLineWidth(4)
	print("Drawing edges", #Edge)
	-- draw edges
	for _, v in ipairs(Edge) do
		love.graphics.line(v[1][2], v[1][3], v[2][2], v[2][3])
	end
	
	love.graphics.setColor(217/255, 98/255, 74/255)
	print("Drawing vertex", #Vertex)
	-- draw vertex
	for _, v in ipairs(Vertex) do
		love.graphics.circle("fill", v[2], v[3], 12, 360)
	end
	for _, v in ipairs(Vertex) do
		love.graphics.circle("line", v[2], v[3], 12, 360)
	end

	-- draw sector name
	print("Drawing sector label")
	for _, v in ipairs(Vertex) do
		local w = font1:getWidth(v[1]) / 2
		love.graphics.setColor(1,1,1)
		love.graphics.print(v[1], font1, v[2] - 1, v[3] - 1, 0, 1, 1, w, font1HeightH)
		love.graphics.print(v[1], font1, v[2], v[3] - 1, 0, 1, 1, w, font1HeightH)
		love.graphics.print(v[1], font1, v[2] + 1, v[3] - 1, 0, 1, 1, w, font1HeightH)
		love.graphics.print(v[1], font1, v[2] - 1, v[3], 0, 1, 1, w, font1HeightH)
		love.graphics.print(v[1], font1, v[2] + 1, v[3], 0, 1, 1, w, font1HeightH)
		love.graphics.print(v[1], font1, v[2] - 1, v[3] + 1, 0, 1, 1, w, font1HeightH)
		love.graphics.print(v[1], font1, v[2], v[3] + 1, 0, 1, 1, w, font1HeightH)
		love.graphics.print(v[1], font1, v[2] + 1, v[3] + 1, 0, 1, 1, w, font1HeightH)
		love.graphics.setColor(0,0,0)
		love.graphics.print(v[1], font1, v[2], v[3], 0, 1, 1, w, font1HeightH)
	end

	-- draw cost
	print("Drawing (cost * 10) rounded down")
	for _, v in ipairs(Edge) do
		local s = tostring(v[3])
		local w = font2:getWidth(s) / 2
		local x, y = (v[1][2] + v[2][2]) / 2, (v[1][3] + v[2][3]) / 2
		love.graphics.setColor(0,0,0)
		love.graphics.print(s, font2, x - 1, y - 1, 0, 1, 1, w, font2HeightH)
		love.graphics.print(s, font2, x, y - 1, 0, 1, 1, w, font2HeightH)
		love.graphics.print(s, font2, x + 1, y - 1, 0, 1, 1, w, font2HeightH)
		love.graphics.print(s, font2, x - 1, y, 0, 1, 1, w, font2HeightH)
		love.graphics.print(s, font2, x + 1, y, 0, 1, 1, w, font2HeightH)
		love.graphics.print(s, font2, x - 1, y + 1, 0, 1, 1, w, font2HeightH)
		love.graphics.print(s, font2, x, y + 1, 0, 1, 1, w, font2HeightH)
		love.graphics.print(s, font2, x + 1, y + 1, 0, 1, 1, w, font2HeightH)
		love.graphics.setColor(1,1,1)
		love.graphics.print(s, font2, x, y, 0, 1, 1, w, font2HeightH)
	end

	love.graphics.pop()
	print("Writing")
	local imageData = canvas:newImageData()
	local result = io.open("test_graph_stars.png", "wb")
	result:write(imageData:encode("png"):getString())
	result:close()

	-- Write vertex and edges data
	local f = io.open("test_graph_stars.hpp", "w")
	f:setvbuf("full", 4096)
	f:write("// autogenerated by main.lua\n\n")
	f:write("static Vertex starVertexes[] = {\n")
	for i, v in ipairs(Vertex) do
		f:write("\t\"", v[1], "\", // ", i - 1, "\n")
	end
	f:write("};\nconstexpr size_t starVertexSize = sizeof(starVertexes) / sizeof(Vertex);\n\n")
	f:write("static Edge starEdgeFull[] = {\n")
	for i, v in ipairs(Edge) do
		local ui = Vertex[v[1]]
		local vi = Vertex[v[2]]

		f:write("\tEdge(starVertexes[", ui - 1, "], starVertexes[", vi - 1, "], ", v[3], "),\n")
	end
	f:write("};\nconstexpr size_t starEdgeSize = sizeof(starEdgeFull) / sizeof(Edge);\n")
	f:close()
end
