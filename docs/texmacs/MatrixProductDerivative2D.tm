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
      <with|color|red|(<with|math-font-family|rm|%i>15) >
    <|unfolded-io>
      F:matrix([F0,F3,0],[F4,F1,0],[0,0,F2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o15>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F0>>|<cell|<with|math-font-family|rm|F3>>|<cell|0>>|<row|<cell|<with|math-font-family|rm|F4>>|<cell|<with|math-font-family|rm|F1>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|F2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>16) >
    <|unfolded-io>
      G:matrix([G0,G3,0],[G4,G1,0],[0,0,G2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o16>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|G0>>|<cell|<with|math-font-family|rm|G3>>|<cell|0>>|<row|<cell|<with|math-font-family|rm|G4>>|<cell|<with|math-font-family|rm|G1>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|G2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>17) >
    <|unfolded-io>
      prod:F.G
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o17>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F3>*<with|math-font-family|rm|G4>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|G0>>|<cell|<with|math-font-family|rm|F0>*<with|math-font-family|rm|G3>+<with|math-font-family|rm|F3>*<with|math-font-family|rm|G1>>|<cell|0>>|<row|<cell|<with|math-font-family|rm|F1>*<with|math-font-family|rm|G4>+<with|math-font-family|rm|F4>*<with|math-font-family|rm|G0>>|<cell|<with|math-font-family|rm|F4>*<with|math-font-family|rm|G3>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|G1>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|F2>*<with|math-font-family|rm|G2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>18) >
    <|unfolded-io>
      pv:transpose(matrix([prod[1,1],prod[2,2],prod[3,3],prod[1,2],prod[2,1]]))
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o18>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F3>*<with|math-font-family|rm|G4>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|G0>>>|<row|<cell|<with|math-font-family|rm|F4>*<with|math-font-family|rm|G3>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|G1>>>|<row|<cell|<with|math-font-family|rm|F2>*<with|math-font-family|rm|G2>>>|<row|<cell|<with|math-font-family|rm|F0>*<with|math-font-family|rm|G3>+<with|math-font-family|rm|F3>*<with|math-font-family|rm|G1>>>|<row|<cell|<with|math-font-family|rm|F1>*<with|math-font-family|rm|G4>+<with|math-font-family|rm|F4>*<with|math-font-family|rm|G0>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>19) >
    <|unfolded-io>
      J1:addcol(diff(pv,F0),diff(pv,F1),diff(pv,F2),diff(pv,F3),diff(pv,F4))
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o19>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|G0>>|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|G4>>|<cell|0>>|<row|<cell|0>|<cell|<with|math-font-family|rm|G1>>|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|G3>>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|G2>>|<cell|0>|<cell|0>>|<row|<cell|<with|math-font-family|rm|G3>>|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|G1>>|<cell|0>>|<row|<cell|0>|<cell|<with|math-font-family|rm|G4>>|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|G0>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>20) >
    <|unfolded-io>
      J2:addcol(diff(pv,G0),diff(pv,G1),diff(pv,G2),diff(pv,G3),diff(pv,G4))
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o20>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F0>>|<cell|0>|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|F3>>>|<row|<cell|0>|<cell|<with|math-font-family|rm|F1>>|<cell|0>|<cell|<with|math-font-family|rm|F4>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|F2>>|<cell|0>|<cell|0>>|<row|<cell|0>|<cell|<with|math-font-family|rm|F3>>|<cell|0>|<cell|<with|math-font-family|rm|F0>>|<cell|0>>|<row|<cell|<with|math-font-family|rm|F4>>|<cell|0>|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|F1>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>24) >
    <|unfolded-io>
      C:matrix([C01,C01,C02,C03,C04],[C10,C11,C12,C13,C14],[C20,C21,C22,C23,C24],[C30,C31,C32,C33,C34],[C40,C41,C42,C43,C44])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o24>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|C01>>|<cell|<with|math-font-family|rm|C01>>|<cell|<with|math-font-family|rm|C02>>|<cell|<with|math-font-family|rm|C03>>|<cell|<with|math-font-family|rm|C04>>>|<row|<cell|<with|math-font-family|rm|C10>>|<cell|<with|math-font-family|rm|C11>>|<cell|<with|math-font-family|rm|C12>>|<cell|<with|math-font-family|rm|C13>>|<cell|<with|math-font-family|rm|C14>>>|<row|<cell|<with|math-font-family|rm|C20>>|<cell|<with|math-font-family|rm|C21>>|<cell|<with|math-font-family|rm|C22>>|<cell|<with|math-font-family|rm|C23>>|<cell|<with|math-font-family|rm|C24>>>|<row|<cell|<with|math-font-family|rm|C30>>|<cell|<with|math-font-family|rm|C31>>|<cell|<with|math-font-family|rm|C32>>|<cell|<with|math-font-family|rm|C33>>|<cell|<with|math-font-family|rm|C34>>>|<row|<cell|<with|math-font-family|rm|C40>>|<cell|<with|math-font-family|rm|C41>>|<cell|<with|math-font-family|rm|C42>>|<cell|<with|math-font-family|rm|C43>>|<cell|<with|math-font-family|rm|C44>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>25) >
    <|unfolded-io>
      J1.C
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o25>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|C30>*<with|math-font-family|rm|G4>+<with|math-font-family|rm|C01>*<with|math-font-family|rm|G0>>|<cell|<with|math-font-family|rm|C31>*<with|math-font-family|rm|G4>+<with|math-font-family|rm|C01>*<with|math-font-family|rm|G0>>|<cell|<with|math-font-family|rm|C32>*<with|math-font-family|rm|G4>+<with|math-font-family|rm|C02>*<with|math-font-family|rm|G0>>|<cell|<with|math-font-family|rm|C33>*<with|math-font-family|rm|G4>+<with|math-font-family|rm|C03>*<with|math-font-family|rm|G0>>|<cell|<with|math-font-family|rm|C34>*<with|math-font-family|rm|G4>+<with|math-font-family|rm|C04>*<with|math-font-family|rm|G0>>>|<row|<cell|<with|math-font-family|rm|C40>*<with|math-font-family|rm|G3>+<with|math-font-family|rm|C10>*<with|math-font-family|rm|G1>>|<cell|<with|math-font-family|rm|C41>*<with|math-font-family|rm|G3>+<with|math-font-family|rm|C11>*<with|math-font-family|rm|G1>>|<cell|<with|math-font-family|rm|C42>*<with|math-font-family|rm|G3>+<with|math-font-family|rm|C12>*<with|math-font-family|rm|G1>>|<cell|<with|math-font-family|rm|C43>*<with|math-font-family|rm|G3>+<with|math-font-family|rm|C13>*<with|math-font-family|rm|G1>>|<cell|<with|math-font-family|rm|C44>*<with|math-font-family|rm|G3>+<with|math-font-family|rm|C14>*<with|math-font-family|rm|G1>>>|<row|<cell|<with|math-font-family|rm|C20>*<with|math-font-family|rm|G2>>|<cell|<with|math-font-family|rm|C21>*<with|math-font-family|rm|G2>>|<cell|<with|math-font-family|rm|C22>*<with|math-font-family|rm|G2>>|<cell|<with|math-font-family|rm|C23>*<with|math-font-family|rm|G2>>|<cell|<with|math-font-family|rm|C24>*<with|math-font-family|rm|G2>>>|<row|<cell|<with|math-font-family|rm|C01>*<with|math-font-family|rm|G3>+<with|math-font-family|rm|C30>*<with|math-font-family|rm|G1>>|<cell|<with|math-font-family|rm|C01>*<with|math-font-family|rm|G3>+<with|math-font-family|rm|C31>*<with|math-font-family|rm|G1>>|<cell|<with|math-font-family|rm|C02>*<with|math-font-family|rm|G3>+<with|math-font-family|rm|C32>*<with|math-font-family|rm|G1>>|<cell|<with|math-font-family|rm|C03>*<with|math-font-family|rm|G3>+<with|math-font-family|rm|C33>*<with|math-font-family|rm|G1>>|<cell|<with|math-font-family|rm|C04>*<with|math-font-family|rm|G3>+<with|math-font-family|rm|C34>*<with|math-font-family|rm|G1>>>|<row|<cell|<with|math-font-family|rm|C10>*<with|math-font-family|rm|G4>+<with|math-font-family|rm|C40>*<with|math-font-family|rm|G0>>|<cell|<with|math-font-family|rm|C11>*<with|math-font-family|rm|G4>+<with|math-font-family|rm|C41>*<with|math-font-family|rm|G0>>|<cell|<with|math-font-family|rm|C12>*<with|math-font-family|rm|G4>+<with|math-font-family|rm|C42>*<with|math-font-family|rm|G0>>|<cell|<with|math-font-family|rm|C13>*<with|math-font-family|rm|G4>+<with|math-font-family|rm|C43>*<with|math-font-family|rm|G0>>|<cell|<with|math-font-family|rm|C14>*<with|math-font-family|rm|G4>+<with|math-font-family|rm|C44>*<with|math-font-family|rm|G0>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>26) >
    <|unfolded-io>
      J2.C
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o26>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|C40>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|C01>*<with|math-font-family|rm|F0>>|<cell|<with|math-font-family|rm|C41>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|C01>*<with|math-font-family|rm|F0>>|<cell|<with|math-font-family|rm|C42>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|C02>*<with|math-font-family|rm|F0>>|<cell|<with|math-font-family|rm|C43>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|C03>*<with|math-font-family|rm|F0>>|<cell|<with|math-font-family|rm|C44>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|C04>*<with|math-font-family|rm|F0>>>|<row|<cell|<with|math-font-family|rm|C30>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|C10>*<with|math-font-family|rm|F1>>|<cell|<with|math-font-family|rm|C31>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|C11>*<with|math-font-family|rm|F1>>|<cell|<with|math-font-family|rm|C32>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|C12>*<with|math-font-family|rm|F1>>|<cell|<with|math-font-family|rm|C33>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|C13>*<with|math-font-family|rm|F1>>|<cell|<with|math-font-family|rm|C34>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|C14>*<with|math-font-family|rm|F1>>>|<row|<cell|<with|math-font-family|rm|C20>*<with|math-font-family|rm|F2>>|<cell|<with|math-font-family|rm|C21>*<with|math-font-family|rm|F2>>|<cell|<with|math-font-family|rm|C22>*<with|math-font-family|rm|F2>>|<cell|<with|math-font-family|rm|C23>*<with|math-font-family|rm|F2>>|<cell|<with|math-font-family|rm|C24>*<with|math-font-family|rm|F2>>>|<row|<cell|<with|math-font-family|rm|C10>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|C30>*<with|math-font-family|rm|F0>>|<cell|<with|math-font-family|rm|C11>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|C31>*<with|math-font-family|rm|F0>>|<cell|<with|math-font-family|rm|C12>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|C32>*<with|math-font-family|rm|F0>>|<cell|<with|math-font-family|rm|C13>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|C33>*<with|math-font-family|rm|F0>>|<cell|<with|math-font-family|rm|C14>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|C34>*<with|math-font-family|rm|F0>>>|<row|<cell|<with|math-font-family|rm|C01>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|C40>*<with|math-font-family|rm|F1>>|<cell|<with|math-font-family|rm|C01>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|C41>*<with|math-font-family|rm|F1>>|<cell|<with|math-font-family|rm|C02>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|C42>*<with|math-font-family|rm|F1>>|<cell|<with|math-font-family|rm|C03>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|C43>*<with|math-font-family|rm|F1>>|<cell|<with|math-font-family|rm|C04>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|C44>*<with|math-font-family|rm|F1>>>>>>>>
    </unfolded-io>

    <\input>
      <with|color|red|(<with|math-font-family|rm|%i>27) >
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