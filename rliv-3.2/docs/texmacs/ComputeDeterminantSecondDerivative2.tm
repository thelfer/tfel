<TeXmacs|1.99.2>

<style|<tuple|generic|french|maxima>>

<\body>
  <\session|maxima|default>
    <\output>
      \;

      Maxima 5.34.1 http://maxima.sourceforge.net

      using Lisp GNU Common Lisp (GCL) GCL 2.6.12 (a.k.a. GCL)

      Distributed under the GNU Public License. See the file COPYING.

      Dedicated to the memory of William Schelter.

      The function bug_report() provides bug reporting information.
    </output>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>21) >
    <|unfolded-io>
      F:matrix([F0,F3,F5],[F4,F1,F7],[F6,F8,F2])
    <|unfolded-io>
      \;

      \ <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o21>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F0>>|<cell|<with|math-font-family|rm|F3>>|<cell|<with|math-font-family|rm|F5>>>|<row|<cell|<with|math-font-family|rm|F4>>|<cell|<with|math-font-family|rm|F1>>|<cell|<with|math-font-family|rm|F7>>>|<row|<cell|<with|math-font-family|rm|F6>>|<cell|<with|math-font-family|rm|F8>>|<cell|<with|math-font-family|rm|F2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>22) >
    <|unfolded-io>
      iFm:fullratsimp(determinant(F)*invert(F))
    <|unfolded-io>
      \;

      \ <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o22>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F1>*<with|math-font-family|rm|F2>-<with|math-font-family|rm|F7>*<with|math-font-family|rm|F8>>|<cell|<with|math-font-family|rm|F5>*<with|math-font-family|rm|F8>-<with|math-font-family|rm|F2>*<with|math-font-family|rm|F3>>|<cell|<with|math-font-family|rm|F3>*<with|math-font-family|rm|F7>-<with|math-font-family|rm|F1>*<with|math-font-family|rm|F5>>>|<row|<cell|<with|math-font-family|rm|F6>*<with|math-font-family|rm|F7>-<with|math-font-family|rm|F2>*<with|math-font-family|rm|F4>>|<cell|<with|math-font-family|rm|F0>*<with|math-font-family|rm|F2>-<with|math-font-family|rm|F5>*<with|math-font-family|rm|F6>>|<cell|<with|math-font-family|rm|F4>*<with|math-font-family|rm|F5>-<with|math-font-family|rm|F0>*<with|math-font-family|rm|F7>>>|<row|<cell|<with|math-font-family|rm|F4>*<with|math-font-family|rm|F8>-<with|math-font-family|rm|F1>*<with|math-font-family|rm|F6>>|<cell|<with|math-font-family|rm|F3>*<with|math-font-family|rm|F6>-<with|math-font-family|rm|F0>*<with|math-font-family|rm|F8>>|<cell|<with|math-font-family|rm|F0>*<with|math-font-family|rm|F1>-<with|math-font-family|rm|F3>*<with|math-font-family|rm|F4>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>23) >
    <|unfolded-io>
      iF:transpose(matrix([iFm[1,1],iFm[2,2],iFm[3,3],iFm[1,2],iFm[2,1],iFm[1,3],iFm[3,1],iFm[2,3],iFm[3,2]]))
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o23>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F1>*<with|math-font-family|rm|F2>-<with|math-font-family|rm|F7>*<with|math-font-family|rm|F8>>>|<row|<cell|<with|math-font-family|rm|F0>*<with|math-font-family|rm|F2>-<with|math-font-family|rm|F5>*<with|math-font-family|rm|F6>>>|<row|<cell|<with|math-font-family|rm|F0>*<with|math-font-family|rm|F1>-<with|math-font-family|rm|F3>*<with|math-font-family|rm|F4>>>|<row|<cell|<with|math-font-family|rm|F5>*<with|math-font-family|rm|F8>-<with|math-font-family|rm|F2>*<with|math-font-family|rm|F3>>>|<row|<cell|<with|math-font-family|rm|F6>*<with|math-font-family|rm|F7>-<with|math-font-family|rm|F2>*<with|math-font-family|rm|F4>>>|<row|<cell|<with|math-font-family|rm|F3>*<with|math-font-family|rm|F7>-<with|math-font-family|rm|F1>*<with|math-font-family|rm|F5>>>|<row|<cell|<with|math-font-family|rm|F4>*<with|math-font-family|rm|F8>-<with|math-font-family|rm|F1>*<with|math-font-family|rm|F6>>>|<row|<cell|<with|math-font-family|rm|F4>*<with|math-font-family|rm|F5>-<with|math-font-family|rm|F0>*<with|math-font-family|rm|F7>>>|<row|<cell|<with|math-font-family|rm|F3>*<with|math-font-family|rm|F6>-<with|math-font-family|rm|F0>*<with|math-font-family|rm|F8>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>24) >
    <|unfolded-io>
      diF:addcol(diff(iF,F0),diff(iF,F1),diff(iF,F2),diff(iF,F3),diff(iF,F4),diff(iF,F5),diff(iF,F6),diff(iF,F7),diff(iF,F8))
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o24>)
      >><matrix|<tformat|<table|<row|<cell|0>|<cell|<with|math-font-family|rm|F2>>|<cell|<with|math-font-family|rm|F1>>|<cell|0>|<cell|0>|<cell|0>|<cell|0>|<cell|-<with|math-font-family|rm|F8>>|<cell|-<with|math-font-family|rm|F7>>>|<row|<cell|<with|math-font-family|rm|F2>>|<cell|0>|<cell|<with|math-font-family|rm|F0>>|<cell|0>|<cell|0>|<cell|-<with|math-font-family|rm|F6>>|<cell|-<with|math-font-family|rm|F5>>|<cell|0>|<cell|0>>|<row|<cell|<with|math-font-family|rm|F1>>|<cell|<with|math-font-family|rm|F0>>|<cell|0>|<cell|-<with|math-font-family|rm|F4>>|<cell|-<with|math-font-family|rm|F3>>|<cell|0>|<cell|0>|<cell|0>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|-<with|math-font-family|rm|F3>>|<cell|-<with|math-font-family|rm|F2>>|<cell|0>|<cell|<with|math-font-family|rm|F8>>|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|F5>>>|<row|<cell|0>|<cell|0>|<cell|-<with|math-font-family|rm|F4>>|<cell|0>|<cell|-<with|math-font-family|rm|F2>>|<cell|0>|<cell|<with|math-font-family|rm|F7>>|<cell|<with|math-font-family|rm|F6>>|<cell|0>>|<row|<cell|0>|<cell|-<with|math-font-family|rm|F5>>|<cell|0>|<cell|<with|math-font-family|rm|F7>>|<cell|0>|<cell|-<with|math-font-family|rm|F1>>|<cell|0>|<cell|<with|math-font-family|rm|F3>>|<cell|0>>|<row|<cell|0>|<cell|-<with|math-font-family|rm|F6>>|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|F8>>|<cell|0>|<cell|-<with|math-font-family|rm|F1>>|<cell|0>|<cell|<with|math-font-family|rm|F4>>>|<row|<cell|-<with|math-font-family|rm|F7>>|<cell|0>|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|F5>>|<cell|<with|math-font-family|rm|F4>>|<cell|0>|<cell|-<with|math-font-family|rm|F0>>|<cell|0>>|<row|<cell|-<with|math-font-family|rm|F8>>|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|F6>>|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|F3>>|<cell|0>|<cell|-<with|math-font-family|rm|F0>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>6) >
    <|unfolded-io>
      F:matrix([F0,F3,0],[F4,F1,0],[0,0,F2])
    <|unfolded-io>
      \;

      \ <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o6>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F0>>|<cell|<with|math-font-family|rm|F3>>|<cell|0>>|<row|<cell|<with|math-font-family|rm|F4>>|<cell|<with|math-font-family|rm|F1>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|F2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>7) >
    <|unfolded-io>
      iFm:fullratsimp(determinant(F)*invert(F))
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o7>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F1>*<with|math-font-family|rm|F2>>|<cell|-<with|math-font-family|rm|F2>*<with|math-font-family|rm|F3>>|<cell|0>>|<row|<cell|-<with|math-font-family|rm|F2>*<with|math-font-family|rm|F4>>|<cell|<with|math-font-family|rm|F0>*<with|math-font-family|rm|F2>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|F0>*<with|math-font-family|rm|F1>-<with|math-font-family|rm|F3>*<with|math-font-family|rm|F4>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>8) >
    <|unfolded-io>
      iF:transpose(matrix([iFm[1,1],iFm[2,2],iFm[3,3],iFm[1,2],iFm[2,1]]))
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o8>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F1>*<with|math-font-family|rm|F2>>>|<row|<cell|<with|math-font-family|rm|F0>*<with|math-font-family|rm|F2>>>|<row|<cell|<with|math-font-family|rm|F0>*<with|math-font-family|rm|F1>-<with|math-font-family|rm|F3>*<with|math-font-family|rm|F4>>>|<row|<cell|-<with|math-font-family|rm|F2>*<with|math-font-family|rm|F3>>>|<row|<cell|-<with|math-font-family|rm|F2>*<with|math-font-family|rm|F4>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>11) >
    <|unfolded-io>
      diF:addcol(diff(iF,F0),diff(iF,F1),diff(iF,F2),diff(iF,F3),diff(iF,F4))
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o11>)
      >><matrix|<tformat|<table|<row|<cell|0>|<cell|<with|math-font-family|rm|F2>>|<cell|<with|math-font-family|rm|F1>>|<cell|0>|<cell|0>>|<row|<cell|<with|math-font-family|rm|F2>>|<cell|0>|<cell|<with|math-font-family|rm|F0>>|<cell|0>|<cell|0>>|<row|<cell|<with|math-font-family|rm|F1>>|<cell|<with|math-font-family|rm|F0>>|<cell|0>|<cell|-<with|math-font-family|rm|F4>>|<cell|-<with|math-font-family|rm|F3>>>|<row|<cell|0>|<cell|0>|<cell|-<with|math-font-family|rm|F3>>|<cell|-<with|math-font-family|rm|F2>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|-<with|math-font-family|rm|F4>>|<cell|0>|<cell|-<with|math-font-family|rm|F2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>12) >
    <|unfolded-io>
      F:matrix([F0,0,0],[0,F1,0],[0,0,F2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o12>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F0>>|<cell|0>|<cell|0>>|<row|<cell|0>|<cell|<with|math-font-family|rm|F1>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|F2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>13) >
    <|unfolded-io>
      iFm:fullratsimp(determinant(F)*invert(F))
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o13>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F1>*<with|math-font-family|rm|F2>>|<cell|0>|<cell|0>>|<row|<cell|0>|<cell|<with|math-font-family|rm|F0>*<with|math-font-family|rm|F2>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|F0>*<with|math-font-family|rm|F1>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>14) >
    <|unfolded-io>
      iF:transpose(matrix([iFm[1,1],iFm[2,2],iFm[3,3]]))
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o14>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F1>*<with|math-font-family|rm|F2>>>|<row|<cell|<with|math-font-family|rm|F0>*<with|math-font-family|rm|F2>>>|<row|<cell|<with|math-font-family|rm|F0>*<with|math-font-family|rm|F1>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>15) >
    <|unfolded-io>
      diF:addcol(diff(iF,F0),diff(iF,F1),diff(iF,F2))
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o15>)
      >><matrix|<tformat|<table|<row|<cell|0>|<cell|<with|math-font-family|rm|F2>>|<cell|<with|math-font-family|rm|F1>>>|<row|<cell|<with|math-font-family|rm|F2>>|<cell|0>|<cell|<with|math-font-family|rm|F0>>>|<row|<cell|<with|math-font-family|rm|F1>>|<cell|<with|math-font-family|rm|F0>>|<cell|0>>>>>>>
    </unfolded-io>

    <\input>
      <with|color|red|(<with|math-font-family|rm|%i>16) >
    <|input>
      \;
    </input>
  </session>
</body>