// pair.i SWIG interface
%module pair
%{
#include "pair.h"
%}

%include "pair.h"

// Instantiate some templates
%template(pairii) my_random::pair<int, int>;
%template(pairdi) my_random::pair<double, int>;