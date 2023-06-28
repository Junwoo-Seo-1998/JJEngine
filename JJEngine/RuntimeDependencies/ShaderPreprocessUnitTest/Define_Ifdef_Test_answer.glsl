
#define test

before ifdef

#ifdef test

goodgoodgood
goodgoodgood
goodgoodgood

#endif

after endif


before ifdef fail test

#ifdef fail

Not good
Not good
Not good

#endif

after endif


before ifndef test
#ifndef notest

goodgoodgood
goodgoodgood
goodgoodgood

#endif
after endif



before ifndef fail test
#ifndef test
Not good
Not good
Not good
#endif
after endif