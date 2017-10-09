#pragma fuse lc(ccs, ccs, cp)
extern void CompDebugAssert(bool );
template < typename Type1, typename Type2 >
class B {
  public :
  Type1 b1;
  Type2 b2;
};
template < typename Type1, typename Type2 >
class C {
  public :
  int c1;
  B < Type1, Type2 > c2 [ 10 ] [ 10 ];
  int c3;
};

void baz1()
{
  class C< char  , double  > x;
  x . C< char ,double > ::c2[0][1] . B< char ,double > ::b1 = 1;
  x . C< char ,double > ::c2[1][2] . B< char ,double > ::b2 = 2.0;
  CompDebugAssert(x . C< char ,double > ::c2[0][1] . B< char ,double > ::b1 == 1);
  CompDebugAssert(x . C< char ,double > ::c2[1][2] . B< char ,double > ::b2 == 2.0);
  CompDebugAssert(x . C< char ,double > ::c2[x . C< char ,double > ::c2[0][1] . B< char ,double > ::b1][(int )x . C< char ,double > ::c2[1][2] . B< char ,double > ::b2] . B< char ,double > ::b2 == 2.0);
}
// Almost identical to baz1(), with a few changes to the types to force another version of the
// above templated classes to be generated. This tests our ability disambiguate the different copies.

void baz2()
{
  class C< char  , double  > x;
  x . C< char ,double > ::c2[0][1] . B< char ,double > ::b1 = 1;
  x . C< char ,double > ::c2[1][2] . B< char ,double > ::b2 = 2.0;
  CompDebugAssert(x . C< char ,double > ::c2[0][1] . B< char ,double > ::b1 == 1);
  CompDebugAssert(x . C< char ,double > ::c2[1][2] . B< char ,double > ::b2 == 2.0);
  CompDebugAssert(x . C< char ,double > ::c2[x . C< char ,double > ::c2[0][1] . B< char ,double > ::b1][(int )x . C< char ,double > ::c2[1][2] . B< char ,double > ::b2] . B< char ,double > ::b2 == 2.0);
}
