dofile("lib1.lua")  -- load our library
n = norm(3.4, 1.0)
o = twice(n)  --> 7.0880180586677
print(o)

-- running application from cli with library loaded:
-- lua -i -llib1 -e "x = 10"
