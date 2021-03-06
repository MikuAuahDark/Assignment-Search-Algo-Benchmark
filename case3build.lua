local vertex = {}

local function contains(t, v)
	for i, u in ipairs(t) do
		if u == v then
			return i
		end
	end

	return nil
end

local data = {}

for line in io.lines("case_3.txt") do
	local a, b, c = line:match("(%d+)%s+(%d+)%s+(%d+)")
	a, b, c = assert(tonumber(a)), assert(tonumber(b)), assert(tonumber(c))
	data[#data + 1] = {a, b, c}

	if not contains(vertex, a) then
		vertex[#vertex + 1] = a
	end

	if not contains(vertex, b) then
		vertex[#vertex + 1] = b
	end
end

local function sortedValues(t)
	return t[1] * #t + t[2]
end

table.sort(vertex)
table.sort(data, function(a, b)
	return sortedValues(a) < sortedValues(b)
end)

-- write

io.write("static Vertex numberVertexes[] = {\n")
for i = 1, #vertex do
	io.write("\t\"", vertex[i], "\",\n")
end
io.write("};\nconstexpr size_t numberVertexSize = sizeof(numberVertexes) / sizeof(Vertex);\n\n")

io.write("static Edge numberEdges[] = {\n")
for _, e in ipairs(data) do
	io.write("\tEdge(numberVertexes[", e[1], "], numberVertexes[", e[2], "], ", e[3], "),\n")
end
io.write("};\nconstexpr size_t numberEdgeSize = sizeof(numberEdges) / sizeof(Edge);\n")
