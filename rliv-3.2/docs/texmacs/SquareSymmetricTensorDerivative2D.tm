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
      <with|color|red|(<with|math-font-family|rm|%i>2) >
    <|unfolded-io>
      F:matrix([F0,F3/sqrt(2),0],[F3/sqrt(2),F1,0],[0,0,F2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o2>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F0>>|<cell|<frac|<with|math-font-family|rm|F3>|<sqrt|2>>>|<cell|0>>|<row|<cell|<frac|<with|math-font-family|rm|F3>|<sqrt|2>>>|<cell|<with|math-font-family|rm|F1>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|F2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>3) >
    <|unfolded-io>
      prod:F.F
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o3>)
      >><matrix|<tformat|<table|<row|<cell|<frac|<with|math-font-family|rm|F3><rsup|2>|2>+<with|math-font-family|rm|F0><rsup|2>>|<cell|<frac|<with|math-font-family|rm|F1>*<with|math-font-family|rm|F3>|<sqrt|2>>+<frac|<with|math-font-family|rm|F0>*<with|math-font-family|rm|F3>|<sqrt|2>>>|<cell|0>>|<row|<cell|<frac|<with|math-font-family|rm|F1>*<with|math-font-family|rm|F3>|<sqrt|2>>+<frac|<with|math-font-family|rm|F0>*<with|math-font-family|rm|F3>|<sqrt|2>>>|<cell|<frac|<with|math-font-family|rm|F3><rsup|2>|2>+<with|math-font-family|rm|F1><rsup|2>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|F2><rsup|2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>7) >
    <|unfolded-io>
      pv:fullratsimp(transpose(matrix([prod[1,1],prod[2,2],prod[3,3],sqrt(2)*prod[1,2]])))
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o7>)
      >><matrix|<tformat|<table|<row|<cell|<frac|<with|math-font-family|rm|F3><rsup|2>+2*<with|math-font-family|rm|F0><rsup|2>|2>>>|<row|<cell|<frac|<with|math-font-family|rm|F3><rsup|2>+2*<with|math-font-family|rm|F1><rsup|2>|2>>>|<row|<cell|<with|math-font-family|rm|F2><rsup|2>>>|<row|<cell|<around*|(|<with|math-font-family|rm|F1>+<with|math-font-family|rm|F0>|)>*<with|math-font-family|rm|F3>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>8) >
    <|unfolded-io>
      J1:addcol(diff(pv,F0),diff(pv,F1),diff(pv,F2),diff(pv,F3))
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o8>)
      >><matrix|<tformat|<table|<row|<cell|2*<with|math-font-family|rm|F0>>|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|F3>>>|<row|<cell|0>|<cell|2*<with|math-font-family|rm|F1>>|<cell|0>|<cell|<with|math-font-family|rm|F3>>>|<row|<cell|0>|<cell|0>|<cell|2*<with|math-font-family|rm|F2>>|<cell|0>>|<row|<cell|<with|math-font-family|rm|F3>>|<cell|<with|math-font-family|rm|F3>>|<cell|0>|<cell|<with|math-font-family|rm|F1>+<with|math-font-family|rm|F0>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>9) >
    <|unfolded-io>
      C:matrix([C00,C01,C02,C03],[C10,C11,C12,C13],[C20,C21,C22,C23],[C30,C31,C32,C33])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o9>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|C00>>|<cell|<with|math-font-family|rm|C01>>|<cell|<with|math-font-family|rm|C02>>|<cell|<with|math-font-family|rm|C03>>>|<row|<cell|<with|math-font-family|rm|C10>>|<cell|<with|math-font-family|rm|C11>>|<cell|<with|math-font-family|rm|C12>>|<cell|<with|math-font-family|rm|C13>>>|<row|<cell|<with|math-font-family|rm|C20>>|<cell|<with|math-font-family|rm|C21>>|<cell|<with|math-font-family|rm|C22>>|<cell|<with|math-font-family|rm|C23>>>|<row|<cell|<with|math-font-family|rm|C30>>|<cell|<with|math-font-family|rm|C31>>|<cell|<with|math-font-family|rm|C32>>|<cell|<with|math-font-family|rm|C33>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>11) >
    <|unfolded-io>
      J1.C
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o11>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|C30>*<with|math-font-family|rm|F3>+2*<with|math-font-family|rm|C00>*<with|math-font-family|rm|F0>>|<cell|<with|math-font-family|rm|C31>*<with|math-font-family|rm|F3>+2*<with|math-font-family|rm|C01>*<with|math-font-family|rm|F0>>|<cell|<with|math-font-family|rm|C32>*<with|math-font-family|rm|F3>+2*<with|math-font-family|rm|C02>*<with|math-font-family|rm|F0>>|<cell|<with|math-font-family|rm|C33>*<with|math-font-family|rm|F3>+2*<with|math-font-family|rm|C03>*<with|math-font-family|rm|F0>>>|<row|<cell|<with|math-font-family|rm|C30>*<with|math-font-family|rm|F3>+2*<with|math-font-family|rm|C10>*<with|math-font-family|rm|F1>>|<cell|<with|math-font-family|rm|C31>*<with|math-font-family|rm|F3>+2*<with|math-font-family|rm|C11>*<with|math-font-family|rm|F1>>|<cell|<with|math-font-family|rm|C32>*<with|math-font-family|rm|F3>+2*<with|math-font-family|rm|C12>*<with|math-font-family|rm|F1>>|<cell|<with|math-font-family|rm|C33>*<with|math-font-family|rm|F3>+2*<with|math-font-family|rm|C13>*<with|math-font-family|rm|F1>>>|<row|<cell|2*<with|math-font-family|rm|C20>*<with|math-font-family|rm|F2>>|<cell|2*<with|math-font-family|rm|C21>*<with|math-font-family|rm|F2>>|<cell|2*<with|math-font-family|rm|C22>*<with|math-font-family|rm|F2>>|<cell|2*<with|math-font-family|rm|C23>*<with|math-font-family|rm|F2>>>|<row|<cell|<with|math-font-family|rm|C10>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|C00>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|C30>*<around*|(|<with|math-font-family|rm|F1>+<with|math-font-family|rm|F0>|)>>|<cell|<with|math-font-family|rm|C11>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|C01>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|C31>*<around*|(|<with|math-font-family|rm|F1>+<with|math-font-family|rm|F0>|)>>|<cell|<with|math-font-family|rm|C12>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|C02>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|C32>*<around*|(|<with|math-font-family|rm|F1>+<with|math-font-family|rm|F0>|)>>|<cell|<with|math-font-family|rm|C13>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|C03>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|C33>*<around*|(|<with|math-font-family|rm|F1>+<with|math-font-family|rm|F0>|)>>>>>>>>
    </unfolded-io>

    <\input>
      <with|color|red|(<with|math-font-family|rm|%i>17) >
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