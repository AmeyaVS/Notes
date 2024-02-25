#!/usr/bin/perl


$y=101 ;

sub f () { 

	local $y ;   # Dynamically scoped variable

	$y = 10 ;

	return g();


}

sub g () { 
	return $y ;
}


print "y at the start = $y \n" ;

print "y when g is called indirectly=",f(),"\n" ;
print "y when g is called directly=",g(),"\n" ;


