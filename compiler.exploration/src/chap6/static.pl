#!/usr/bin/perl


$y=101 ;

sub f () { 

	my $y ;   # Lexically scoped variable

	$y = 10 ; # This refers to the 'y' declared above

	return g(); # g continues to access the global 'y'


}

sub g () { 
	return $y ;
}


print "y at the start = $y \n" ;

print "y when g is called indirectly=",f(),"\n" ;
print "y when g is called directly=",g(),"\n" ;


