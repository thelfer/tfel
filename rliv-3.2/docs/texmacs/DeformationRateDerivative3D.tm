<TeXmacs|1.99.2>

<style|<tuple|old-generic|maxima|french>>

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
      iF:matrix([iF0,iF3/sqrt(2),0],[iF4/sqrt(2),iF1,0],[0,0,iF2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o10>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|iF0>>|<cell|<frac|<with|math-font-family|rm|iF3>|<sqrt|2>>>|<cell|0>>|<row|<cell|<frac|<with|math-font-family|rm|iF4>|<sqrt|2>>>|<cell|<with|math-font-family|rm|iF1>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|iF2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>11) >
    <|unfolded-io>
      dF:matrix([dF0,dF3/sqrt(2),0],[dF4/sqrt(2),dF1,0],[0,0,dF2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o11>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|dF0>>|<cell|<frac|<with|math-font-family|rm|dF3>|<sqrt|2>>>|<cell|0>>|<row|<cell|<frac|<with|math-font-family|rm|dF4>|<sqrt|2>>>|<cell|<with|math-font-family|rm|dF1>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|dF2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>12) >
    <|unfolded-io>
      prod:fullratsimp((dF.iF+transpose(iF).transpose(dF))/2)
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o12>)
      >><matrix|<tformat|<table|<row|<cell|<frac|<with|math-font-family|rm|dF3>*<with|math-font-family|rm|iF4>+2*<with|math-font-family|rm|dF0>*<with|math-font-family|rm|iF0>|2>>|<cell|<frac|<sqrt|2>*<with|math-font-family|rm|dF1>*<with|math-font-family|rm|iF4>+<sqrt|2>*<with|math-font-family|rm|dF0>*<with|math-font-family|rm|iF3>+<sqrt|2>*<with|math-font-family|rm|dF3>*<with|math-font-family|rm|iF1>+<sqrt|2>*<with|math-font-family|rm|dF4>*<with|math-font-family|rm|iF0>|4>>|<cell|0>>|<row|<cell|<frac|<sqrt|2>*<with|math-font-family|rm|dF1>*<with|math-font-family|rm|iF4>+<sqrt|2>*<with|math-font-family|rm|dF0>*<with|math-font-family|rm|iF3>+<sqrt|2>*<with|math-font-family|rm|dF3>*<with|math-font-family|rm|iF1>+<sqrt|2>*<with|math-font-family|rm|dF4>*<with|math-font-family|rm|iF0>|4>>|<cell|<frac|<with|math-font-family|rm|dF4>*<with|math-font-family|rm|iF3>+2*<with|math-font-family|rm|dF1>*<with|math-font-family|rm|iF1>|2>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|dF2>*<with|math-font-family|rm|iF2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>13) >
    <|unfolded-io>
      pv:fullratsimp(transpose(matrix([prod[1,1],prod[2,2],prod[3,3],sqrt(2)*prod[1,2]])))
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o13>)
      >><matrix|<tformat|<table|<row|<cell|<frac|<with|math-font-family|rm|dF3>*<with|math-font-family|rm|iF4>+2*<with|math-font-family|rm|dF0>*<with|math-font-family|rm|iF0>|2>>>|<row|<cell|<frac|<with|math-font-family|rm|dF4>*<with|math-font-family|rm|iF3>+2*<with|math-font-family|rm|dF1>*<with|math-font-family|rm|iF1>|2>>>|<row|<cell|<with|math-font-family|rm|dF2>*<with|math-font-family|rm|iF2>>>|<row|<cell|<frac|<with|math-font-family|rm|dF1>*<with|math-font-family|rm|iF4>+<with|math-font-family|rm|dF0>*<with|math-font-family|rm|iF3>+<with|math-font-family|rm|dF3>*<with|math-font-family|rm|iF1>+<with|math-font-family|rm|dF4>*<with|math-font-family|rm|iF0>|2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>15) >
    <|unfolded-io>
      J1:fullratsimp(addcol(diff(pv,dF0),diff(pv,dF1),diff(pv,dF2),diff(pv,dF3),diff(pv,dF4)))
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o15>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|iF0>>|<cell|0>|<cell|0>|<cell|<frac|<with|math-font-family|rm|iF4>|2>>|<cell|0>>|<row|<cell|0>|<cell|<with|math-font-family|rm|iF1>>|<cell|0>|<cell|0>|<cell|<frac|<with|math-font-family|rm|iF3>|2>>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|iF2>>|<cell|0>|<cell|0>>|<row|<cell|<frac|<with|math-font-family|rm|iF3>|2>>|<cell|<frac|<with|math-font-family|rm|iF4>|2>>|<cell|0>|<cell|<frac|<with|math-font-family|rm|iF1>|2>>|<cell|<frac|<with|math-font-family|rm|iF0>|2>>>>>>>>
    </unfolded-io>

    <\input>
      <with|color|red|(<with|math-font-family|rm|%i>16) >
    <|input>
      \;
    </input>

    \;
  </session>
</body>

<\initial>
  <\collection>
    <associate|page-type|a2>
  </collection>
</initial>