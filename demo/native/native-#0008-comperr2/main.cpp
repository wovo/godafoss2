#include <array>

struct base { };

template< base name > struct component { };

struct derived : base { };

struct app : component< base() >  { };

app x; 

