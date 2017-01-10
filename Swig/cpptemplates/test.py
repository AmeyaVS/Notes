#!/usr/bin/python

import pair as p

a = p.pairii(3, 4)
print "a= " + str(a.first) + " " + str(a.second)

a.second = 16
print "a= " + str(a.first) + " " + str(a.second)

b = p.pairdi(3.14159, 8)
print "b= " + str(b.first) + " " + str(b.second)

c = a
print "c= " + str(c.first) + " " + str(c.second)
c.second = 10
print "c= " + str(c.first) + " " + str(c.second)
print "a= " + str(a.first) + " " + str(a.second)