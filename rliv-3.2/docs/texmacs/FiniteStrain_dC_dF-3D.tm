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
      <with|color|red|(<with|math-font-family|rm|%i>10) >
    <|unfolded-io>
      F:matrix([F0,F3,F5],[F4,F1,F7],[F6,F8,F2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o10>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F0>>|<cell|<with|math-font-family|rm|F3>>|<cell|<with|math-font-family|rm|F5>>>|<row|<cell|<with|math-font-family|rm|F4>>|<cell|<with|math-font-family|rm|F1>>|<cell|<with|math-font-family|rm|F7>>>|<row|<cell|<with|math-font-family|rm|F6>>|<cell|<with|math-font-family|rm|F8>>|<cell|<with|math-font-family|rm|F2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>11) >
    <|unfolded-io>
      Cm:transpose(F).F /* right cauchy green tensor */
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o11>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F6><rsup|2>+<with|math-font-family|rm|F4><rsup|2>+<with|math-font-family|rm|F0><rsup|2>>|<cell|<with|math-font-family|rm|F6>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|F3>>|<cell|<with|math-font-family|rm|F4>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|F2>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|F5>>>|<row|<cell|<with|math-font-family|rm|F6>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|F3>>|<cell|<with|math-font-family|rm|F8><rsup|2>+<with|math-font-family|rm|F3><rsup|2>+<with|math-font-family|rm|F1><rsup|2>>|<cell|<with|math-font-family|rm|F2>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|F3>*<with|math-font-family|rm|F5>>>|<row|<cell|<with|math-font-family|rm|F4>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|F2>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|F5>>|<cell|<with|math-font-family|rm|F2>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|F3>*<with|math-font-family|rm|F5>>|<cell|<with|math-font-family|rm|F7><rsup|2>+<with|math-font-family|rm|F5><rsup|2>+<with|math-font-family|rm|F2><rsup|2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>13) >
    <|unfolded-io>
      c:[Cm[1,1],Cm[2,2],Cm[3,3],sqrt(2)*Cm[1,2],sqrt(2)*Cm[1,3],sqrt(2)*Cm[2,3]]
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o13>)
      >><around*|[|<with|math-font-family|rm|F6><rsup|2>+<with|math-font-family|rm|F4><rsup|2>+<with|math-font-family|rm|F0><rsup|2>,<with|math-font-family|rm|F8><rsup|2>+<with|math-font-family|rm|F3><rsup|2>+<with|math-font-family|rm|F1><rsup|2>,<with|math-font-family|rm|F7><rsup|2>+<with|math-font-family|rm|F5><rsup|2>+<with|math-font-family|rm|F2><rsup|2>,<sqrt|2>*<around*|(|<with|math-font-family|rm|F6>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|F3>|)>,<sqrt|2>*<around*|(|<with|math-font-family|rm|F4>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|F2>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|F5>|)>,<sqrt|2>*<around*|(|<with|math-font-family|rm|F2>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|F3>*<with|math-font-family|rm|F5>|)>|]>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>16) >
    <|unfolded-io>
      dcdF:matrix([])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o16>)
      >><matrix|<tformat|<table|<row|<cell|>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>21) >
    <|unfolded-io>
      [diff(c[1],F0),diff(c[2],F0),diff(c[3],F0),diff(c[4],F0),diff(c[5],F0),diff(c[6],F0)]
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o21>)
      >><around*|[|2*<with|math-font-family|rm|F0>,0,0,<sqrt|2>*<with|math-font-family|rm|F3>,<sqrt|2>*<with|math-font-family|rm|F5>,0|]>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>22) >
    <|unfolded-io>
      [diff(c[1],F1),diff(c[2],F1),diff(c[3],F1),diff(c[4],F1),diff(c[5],F1),diff(c[6],F1)]
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o22>)
      >><around*|[|0,2*<with|math-font-family|rm|F1>,0,<sqrt|2>*<with|math-font-family|rm|F4>,0,<sqrt|2>*<with|math-font-family|rm|F7>|]>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>23) >
    <|unfolded-io>
      [diff(c[1],F2),diff(c[2],F2),diff(c[3],F2),diff(c[4],F2),diff(c[5],F2),diff(c[6],F2)]
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o23>)
      >><around*|[|0,0,2*<with|math-font-family|rm|F2>,0,<sqrt|2>*<with|math-font-family|rm|F6>,<sqrt|2>*<with|math-font-family|rm|F8>|]>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>24) >
    <|unfolded-io>
      [diff(c[1],F3),diff(c[2],F3),diff(c[3],F3),diff(c[4],F3),diff(c[5],F3),diff(c[6],F3)]
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o24>)
      >><around*|[|0,2*<with|math-font-family|rm|F3>,0,<sqrt|2>*<with|math-font-family|rm|F0>,0,<sqrt|2>*<with|math-font-family|rm|F5>|]>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>25) >
    <|unfolded-io>
      [diff(c[1],F4),diff(c[2],F4),diff(c[3],F4),diff(c[4],F4),diff(c[5],F4),diff(c[6],F4)]
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o25>)
      >><around*|[|2*<with|math-font-family|rm|F4>,0,0,<sqrt|2>*<with|math-font-family|rm|F1>,<sqrt|2>*<with|math-font-family|rm|F7>,0|]>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>26) >
    <|unfolded-io>
      [diff(c[1],F5),diff(c[2],F5),diff(c[3],F5),diff(c[4],F5),diff(c[5],F5),diff(c[6],F5)]
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o26>)
      >><around*|[|0,0,2*<with|math-font-family|rm|F5>,0,<sqrt|2>*<with|math-font-family|rm|F0>,<sqrt|2>*<with|math-font-family|rm|F3>|]>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>27) >
    <|unfolded-io>
      [diff(c[1],F6),diff(c[2],F6),diff(c[3],F6),diff(c[4],F6),diff(c[5],F6),diff(c[6],F6)]
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o27>)
      >><around*|[|2*<with|math-font-family|rm|F6>,0,0,<sqrt|2>*<with|math-font-family|rm|F8>,<sqrt|2>*<with|math-font-family|rm|F2>,0|]>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>28) >
    <|unfolded-io>
      [diff(c[1],F7),diff(c[2],F7),diff(c[3],F7),diff(c[4],F7),diff(c[5],F7),diff(c[6],F7)]
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o28>)
      >><around*|[|0,0,2*<with|math-font-family|rm|F7>,0,<sqrt|2>*<with|math-font-family|rm|F4>,<sqrt|2>*<with|math-font-family|rm|F1>|]>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>29) >
    <|unfolded-io>
      [diff(c[1],F8),diff(c[2],F8),diff(c[3],F8),diff(c[4],F8),diff(c[5],F8),diff(c[6],F8)]
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o29>)
      >><around*|[|0,2*<with|math-font-family|rm|F8>,0,<sqrt|2>*<with|math-font-family|rm|F6>,0,<sqrt|2>*<with|math-font-family|rm|F2>|]>>>
    </unfolded-io>

    <\input>
      <with|color|red|(<with|math-font-family|rm|%i>30) >
    <|input>
      \;
    </input>
  </session>
</body>

<\initial>
  <\collection>
    <associate|language|french>
    <associate|magnification|0.8>
    <associate|page-medium|automatic>
    <associate|page-screen-height|522752tmpt>
    <associate|page-screen-width|970752tmpt>
    <associate|page-type|a3>
    <associate|sfactor|2>
  </collection>
</initial>