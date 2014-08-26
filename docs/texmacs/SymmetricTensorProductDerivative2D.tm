<TeXmacs|1.0.7.15>

<style|<tuple|generic|maxima>>

<\body>
  <\session|maxima|default>
    <\output>
      \;

      Maxima 5.27.0 http://maxima.sourceforge.net

      using Lisp GNU Common Lisp (GCL) GCL 2.6.7 (a.k.a. GCL)

      Distributed under the GNU Public License. See the file COPYING.

      Dedicated to the memory of William Schelter.

      The function bug_report() provides bug reporting information.
    </output>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>68) >
    <|unfolded-io>
      F:matrix([F0,F3/sqrt(2),0],[F3/sqrt(2),F1,0],[0,0,F2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o68>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F0>>|<cell|<frac|<with|math-font-family|rm|F3>|<sqrt|2>>>|<cell|0>>|<row|<cell|<frac|<with|math-font-family|rm|F3>|<sqrt|2>>>|<cell|<with|math-font-family|rm|F1>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|F2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>69) >
    <|unfolded-io>
      G:matrix([G0,G3/sqrt(2),0],[G3/sqrt(2),G1,0],[0,0,G2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o69>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|G0>>|<cell|<frac|<with|math-font-family|rm|G3>|<sqrt|2>>>|<cell|0>>|<row|<cell|<frac|<with|math-font-family|rm|G3>|<sqrt|2>>>|<cell|<with|math-font-family|rm|G1>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|G2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>70) >
    <|unfolded-io>
      prod:F.G
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o70>)
      >><matrix|<tformat|<table|<row|<cell|<frac|<with|math-font-family|rm|F3>*<with|math-font-family|rm|G3>|2>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|G0>>|<cell|<frac|<with|math-font-family|rm|F0>*<with|math-font-family|rm|G3>|<sqrt|2>>+<frac|<with|math-font-family|rm|F3>*<with|math-font-family|rm|G1>|<sqrt|2>>>|<cell|0>>|<row|<cell|<frac|<with|math-font-family|rm|F1>*<with|math-font-family|rm|G3>|<sqrt|2>>+<frac|<with|math-font-family|rm|F3>*<with|math-font-family|rm|G0>|<sqrt|2>>>|<cell|<frac|<with|math-font-family|rm|F3>*<with|math-font-family|rm|G3>|2>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|G1>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|F2>*<with|math-font-family|rm|G2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>71) >
    <|unfolded-io>
      pv:transpose(matrix([prod[1,1],prod[2,2],prod[3,3],fullratsimp(sqrt(2)*prod[1,2])]))
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o71>)
      >><matrix|<tformat|<table|<row|<cell|<frac|<with|math-font-family|rm|F3>*<with|math-font-family|rm|G3>|2>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|G0>>>|<row|<cell|<frac|<with|math-font-family|rm|F3>*<with|math-font-family|rm|G3>|2>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|G1>>>|<row|<cell|<with|math-font-family|rm|F2>*<with|math-font-family|rm|G2>>>|<row|<cell|<with|math-font-family|rm|F0>*<with|math-font-family|rm|G3>+<with|math-font-family|rm|F3>*<with|math-font-family|rm|G1>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>72) >
    <|unfolded-io>
      J1:addcol(diff(pv,F0),diff(pv,F1),diff(pv,F2),diff(pv,F3))
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o72>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|G0>>|<cell|0>|<cell|0>|<cell|<frac|<with|math-font-family|rm|G3>|2>>>|<row|<cell|0>|<cell|<with|math-font-family|rm|G1>>|<cell|0>|<cell|<frac|<with|math-font-family|rm|G3>|2>>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|G2>>|<cell|0>>|<row|<cell|<with|math-font-family|rm|G3>>|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|G1>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>73) >
    <|unfolded-io>
      J2:addcol(diff(pv,G0),diff(pv,G1),diff(pv,G2),diff(pv,G3))
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o73>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F0>>|<cell|0>|<cell|0>|<cell|<frac|<with|math-font-family|rm|F3>|2>>>|<row|<cell|0>|<cell|<with|math-font-family|rm|F1>>|<cell|0>|<cell|<frac|<with|math-font-family|rm|F3>|2>>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|F2>>|<cell|0>>|<row|<cell|0>|<cell|<with|math-font-family|rm|F3>>|<cell|0>|<cell|<with|math-font-family|rm|F0>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>74) >
    <|unfolded-io>
      C:matrix([C00,C01,C02,C03],[C10,C11,C12,C13],[C20,C21,C22,C23],[C30,C31,C32,C33])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o74>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|C00>>|<cell|<with|math-font-family|rm|C01>>|<cell|<with|math-font-family|rm|C02>>|<cell|<with|math-font-family|rm|C03>>>|<row|<cell|<with|math-font-family|rm|C10>>|<cell|<with|math-font-family|rm|C11>>|<cell|<with|math-font-family|rm|C12>>|<cell|<with|math-font-family|rm|C13>>>|<row|<cell|<with|math-font-family|rm|C20>>|<cell|<with|math-font-family|rm|C21>>|<cell|<with|math-font-family|rm|C22>>|<cell|<with|math-font-family|rm|C23>>>|<row|<cell|<with|math-font-family|rm|C30>>|<cell|<with|math-font-family|rm|C31>>|<cell|<with|math-font-family|rm|C32>>|<cell|<with|math-font-family|rm|C33>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>75) >
    <|unfolded-io>
      J1.C
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o75>)
      >><matrix|<tformat|<table|<row|<cell|<frac|<with|math-font-family|rm|C30>*<with|math-font-family|rm|G3>|2>+<with|math-font-family|rm|C00>*<with|math-font-family|rm|G0>>|<cell|<frac|<with|math-font-family|rm|C31>*<with|math-font-family|rm|G3>|2>+<with|math-font-family|rm|C01>*<with|math-font-family|rm|G0>>|<cell|<frac|<with|math-font-family|rm|C32>*<with|math-font-family|rm|G3>|2>+<with|math-font-family|rm|C02>*<with|math-font-family|rm|G0>>|<cell|<frac|<with|math-font-family|rm|C33>*<with|math-font-family|rm|G3>|2>+<with|math-font-family|rm|C03>*<with|math-font-family|rm|G0>>>|<row|<cell|<frac|<with|math-font-family|rm|C30>*<with|math-font-family|rm|G3>|2>+<with|math-font-family|rm|C10>*<with|math-font-family|rm|G1>>|<cell|<frac|<with|math-font-family|rm|C31>*<with|math-font-family|rm|G3>|2>+<with|math-font-family|rm|C11>*<with|math-font-family|rm|G1>>|<cell|<frac|<with|math-font-family|rm|C32>*<with|math-font-family|rm|G3>|2>+<with|math-font-family|rm|C12>*<with|math-font-family|rm|G1>>|<cell|<frac|<with|math-font-family|rm|C33>*<with|math-font-family|rm|G3>|2>+<with|math-font-family|rm|C13>*<with|math-font-family|rm|G1>>>|<row|<cell|<with|math-font-family|rm|C20>*<with|math-font-family|rm|G2>>|<cell|<with|math-font-family|rm|C21>*<with|math-font-family|rm|G2>>|<cell|<with|math-font-family|rm|C22>*<with|math-font-family|rm|G2>>|<cell|<with|math-font-family|rm|C23>*<with|math-font-family|rm|G2>>>|<row|<cell|<with|math-font-family|rm|C00>*<with|math-font-family|rm|G3>+<with|math-font-family|rm|C30>*<with|math-font-family|rm|G1>>|<cell|<with|math-font-family|rm|C01>*<with|math-font-family|rm|G3>+<with|math-font-family|rm|C31>*<with|math-font-family|rm|G1>>|<cell|<with|math-font-family|rm|C02>*<with|math-font-family|rm|G3>+<with|math-font-family|rm|C32>*<with|math-font-family|rm|G1>>|<cell|<with|math-font-family|rm|C03>*<with|math-font-family|rm|G3>+<with|math-font-family|rm|C33>*<with|math-font-family|rm|G1>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>76) >
    <|unfolded-io>
      J2.C
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o76>)
      >><matrix|<tformat|<table|<row|<cell|<frac|<with|math-font-family|rm|C30>*<with|math-font-family|rm|F3>|2>+<with|math-font-family|rm|C00>*<with|math-font-family|rm|F0>>|<cell|<frac|<with|math-font-family|rm|C31>*<with|math-font-family|rm|F3>|2>+<with|math-font-family|rm|C01>*<with|math-font-family|rm|F0>>|<cell|<frac|<with|math-font-family|rm|C32>*<with|math-font-family|rm|F3>|2>+<with|math-font-family|rm|C02>*<with|math-font-family|rm|F0>>|<cell|<frac|<with|math-font-family|rm|C33>*<with|math-font-family|rm|F3>|2>+<with|math-font-family|rm|C03>*<with|math-font-family|rm|F0>>>|<row|<cell|<frac|<with|math-font-family|rm|C30>*<with|math-font-family|rm|F3>|2>+<with|math-font-family|rm|C10>*<with|math-font-family|rm|F1>>|<cell|<frac|<with|math-font-family|rm|C31>*<with|math-font-family|rm|F3>|2>+<with|math-font-family|rm|C11>*<with|math-font-family|rm|F1>>|<cell|<frac|<with|math-font-family|rm|C32>*<with|math-font-family|rm|F3>|2>+<with|math-font-family|rm|C12>*<with|math-font-family|rm|F1>>|<cell|<frac|<with|math-font-family|rm|C33>*<with|math-font-family|rm|F3>|2>+<with|math-font-family|rm|C13>*<with|math-font-family|rm|F1>>>|<row|<cell|<with|math-font-family|rm|C20>*<with|math-font-family|rm|F2>>|<cell|<with|math-font-family|rm|C21>*<with|math-font-family|rm|F2>>|<cell|<with|math-font-family|rm|C22>*<with|math-font-family|rm|F2>>|<cell|<with|math-font-family|rm|C23>*<with|math-font-family|rm|F2>>>|<row|<cell|<with|math-font-family|rm|C10>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|C30>*<with|math-font-family|rm|F0>>|<cell|<with|math-font-family|rm|C11>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|C31>*<with|math-font-family|rm|F0>>|<cell|<with|math-font-family|rm|C12>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|C32>*<with|math-font-family|rm|F0>>|<cell|<with|math-font-family|rm|C13>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|C33>*<with|math-font-family|rm|F0>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>77) >
    <|unfolded-io>
      C:matrix([C00,C01,C02,C03],[C10,C11,C12,C13],[C20,C21,C22,C23],[C30,C31,C32,C33])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o77>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|C00>>|<cell|<with|math-font-family|rm|C01>>|<cell|<with|math-font-family|rm|C02>>|<cell|<with|math-font-family|rm|C03>>>|<row|<cell|<with|math-font-family|rm|C10>>|<cell|<with|math-font-family|rm|C11>>|<cell|<with|math-font-family|rm|C12>>|<cell|<with|math-font-family|rm|C13>>>|<row|<cell|<with|math-font-family|rm|C20>>|<cell|<with|math-font-family|rm|C21>>|<cell|<with|math-font-family|rm|C22>>|<cell|<with|math-font-family|rm|C23>>>|<row|<cell|<with|math-font-family|rm|C30>>|<cell|<with|math-font-family|rm|C31>>|<cell|<with|math-font-family|rm|C32>>|<cell|<with|math-font-family|rm|C33>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>78) >
    <|unfolded-io>
      J1.C
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o78>)
      >><matrix|<tformat|<table|<row|<cell|<frac|<with|math-font-family|rm|C30>*<with|math-font-family|rm|G3>|2>+<with|math-font-family|rm|C00>*<with|math-font-family|rm|G0>>|<cell|<frac|<with|math-font-family|rm|C31>*<with|math-font-family|rm|G3>|2>+<with|math-font-family|rm|C01>*<with|math-font-family|rm|G0>>|<cell|<frac|<with|math-font-family|rm|C32>*<with|math-font-family|rm|G3>|2>+<with|math-font-family|rm|C02>*<with|math-font-family|rm|G0>>|<cell|<frac|<with|math-font-family|rm|C33>*<with|math-font-family|rm|G3>|2>+<with|math-font-family|rm|C03>*<with|math-font-family|rm|G0>>>|<row|<cell|<frac|<with|math-font-family|rm|C30>*<with|math-font-family|rm|G3>|2>+<with|math-font-family|rm|C10>*<with|math-font-family|rm|G1>>|<cell|<frac|<with|math-font-family|rm|C31>*<with|math-font-family|rm|G3>|2>+<with|math-font-family|rm|C11>*<with|math-font-family|rm|G1>>|<cell|<frac|<with|math-font-family|rm|C32>*<with|math-font-family|rm|G3>|2>+<with|math-font-family|rm|C12>*<with|math-font-family|rm|G1>>|<cell|<frac|<with|math-font-family|rm|C33>*<with|math-font-family|rm|G3>|2>+<with|math-font-family|rm|C13>*<with|math-font-family|rm|G1>>>|<row|<cell|<with|math-font-family|rm|C20>*<with|math-font-family|rm|G2>>|<cell|<with|math-font-family|rm|C21>*<with|math-font-family|rm|G2>>|<cell|<with|math-font-family|rm|C22>*<with|math-font-family|rm|G2>>|<cell|<with|math-font-family|rm|C23>*<with|math-font-family|rm|G2>>>|<row|<cell|<with|math-font-family|rm|C00>*<with|math-font-family|rm|G3>+<with|math-font-family|rm|C30>*<with|math-font-family|rm|G1>>|<cell|<with|math-font-family|rm|C01>*<with|math-font-family|rm|G3>+<with|math-font-family|rm|C31>*<with|math-font-family|rm|G1>>|<cell|<with|math-font-family|rm|C02>*<with|math-font-family|rm|G3>+<with|math-font-family|rm|C32>*<with|math-font-family|rm|G1>>|<cell|<with|math-font-family|rm|C03>*<with|math-font-family|rm|G3>+<with|math-font-family|rm|C33>*<with|math-font-family|rm|G1>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>79) >
    <|unfolded-io>
      J2.C
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o79>)
      >><matrix|<tformat|<table|<row|<cell|<frac|<with|math-font-family|rm|C30>*<with|math-font-family|rm|F3>|2>+<with|math-font-family|rm|C00>*<with|math-font-family|rm|F0>>|<cell|<frac|<with|math-font-family|rm|C31>*<with|math-font-family|rm|F3>|2>+<with|math-font-family|rm|C01>*<with|math-font-family|rm|F0>>|<cell|<frac|<with|math-font-family|rm|C32>*<with|math-font-family|rm|F3>|2>+<with|math-font-family|rm|C02>*<with|math-font-family|rm|F0>>|<cell|<frac|<with|math-font-family|rm|C33>*<with|math-font-family|rm|F3>|2>+<with|math-font-family|rm|C03>*<with|math-font-family|rm|F0>>>|<row|<cell|<frac|<with|math-font-family|rm|C30>*<with|math-font-family|rm|F3>|2>+<with|math-font-family|rm|C10>*<with|math-font-family|rm|F1>>|<cell|<frac|<with|math-font-family|rm|C31>*<with|math-font-family|rm|F3>|2>+<with|math-font-family|rm|C11>*<with|math-font-family|rm|F1>>|<cell|<frac|<with|math-font-family|rm|C32>*<with|math-font-family|rm|F3>|2>+<with|math-font-family|rm|C12>*<with|math-font-family|rm|F1>>|<cell|<frac|<with|math-font-family|rm|C33>*<with|math-font-family|rm|F3>|2>+<with|math-font-family|rm|C13>*<with|math-font-family|rm|F1>>>|<row|<cell|<with|math-font-family|rm|C20>*<with|math-font-family|rm|F2>>|<cell|<with|math-font-family|rm|C21>*<with|math-font-family|rm|F2>>|<cell|<with|math-font-family|rm|C22>*<with|math-font-family|rm|F2>>|<cell|<with|math-font-family|rm|C23>*<with|math-font-family|rm|F2>>>|<row|<cell|<with|math-font-family|rm|C10>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|C30>*<with|math-font-family|rm|F0>>|<cell|<with|math-font-family|rm|C11>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|C31>*<with|math-font-family|rm|F0>>|<cell|<with|math-font-family|rm|C12>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|C32>*<with|math-font-family|rm|F0>>|<cell|<with|math-font-family|rm|C13>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|C33>*<with|math-font-family|rm|F0>>>>>>>>
    </unfolded-io>

    <\input>
      <with|color|red|(<with|math-font-family|rm|%i>80) >
    <|input>
      \;
    </input>
  </session>
</body>

<\initial>
  <\collection>
    <associate|language|french>
    <associate|page-type|a2>
  </collection>
</initial>