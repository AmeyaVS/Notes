--[[A multi-line
    long comment
]]

-- types and values
type(nil)  --> nil
type(true)  --> boolean
type(10.4 * 3)  --> number
type("Hello world")  --> string
type(io.stdin)  --> userdata
type(print)  --> function
type(type)  --> function
type({})  --> table
type(type(X))  --> string

-- Booleans
--> 4 and 5  --> 5
--> nil and 13  --> nil
--> false and 13  --> false
--> 0 or 5  --> 0
--> false or "hi"  --> "hi"
--> nil or false  --> false

-- Useful idiom:
-- if not x then x = v end
-- is equivalent to below statement
-- x = x or v

--> not nil  --> true
--> not false  --> true
--> not 0  --> false
--> not not 1  --> true
--> not not nil  --> false
