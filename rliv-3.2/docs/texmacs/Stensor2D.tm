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
      <with|color|red|(<with|math-font-family|rm|%i>14) >
    <|unfolded-io>
      s:matrix([s0,s3/sqrt(2),0],[s3/sqrt(2),s1,0],[0,0,s2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o14>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|s0>>|<cell|<frac|<with|math-font-family|rm|s3>|<sqrt|2>>>|<cell|0>>|<row|<cell|<frac|<with|math-font-family|rm|s3>|<sqrt|2>>>|<cell|<with|math-font-family|rm|s1>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|s2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>15) >
    <|unfolded-io>
      t:matrix([t0,t3/sqrt(2),0],[t3/sqrt(2),t1,0],[0,0,t2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o15>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|t0>>|<cell|<frac|<with|math-font-family|rm|t3>|<sqrt|2>>>|<cell|0>>|<row|<cell|<frac|<with|math-font-family|rm|t3>|<sqrt|2>>>|<cell|<with|math-font-family|rm|t1>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|t2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>16) >
    <|unfolded-io>
      p:s.t
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o16>)
      >><matrix|<tformat|<table|<row|<cell|<frac|<with|math-font-family|rm|s3>*<with|math-font-family|rm|t3>|2>+<with|math-font-family|rm|s0>*<with|math-font-family|rm|t0>>|<cell|<frac|<with|math-font-family|rm|s0>*<with|math-font-family|rm|t3>|<sqrt|2>>+<frac|<with|math-font-family|rm|s3>*<with|math-font-family|rm|t1>|<sqrt|2>>>|<cell|0>>|<row|<cell|<frac|<with|math-font-family|rm|s1>*<with|math-font-family|rm|t3>|<sqrt|2>>+<frac|<with|math-font-family|rm|s3>*<with|math-font-family|rm|t0>|<sqrt|2>>>|<cell|<frac|<with|math-font-family|rm|s3>*<with|math-font-family|rm|t3>|2>+<with|math-font-family|rm|s1>*<with|math-font-family|rm|t1>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|s2>*<with|math-font-family|rm|t2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>17) >
    <|unfolded-io>
      fullratsimp(p[1,1])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o17>)
      >><frac|<with|math-font-family|rm|s3>*<with|math-font-family|rm|t3>+2*<with|math-font-family|rm|s0>*<with|math-font-family|rm|t0>|2>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>18) >
    <|unfolded-io>
      fullratsimp(p[2,2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o18>)
      >><frac|<with|math-font-family|rm|s3>*<with|math-font-family|rm|t3>+2*<with|math-font-family|rm|s1>*<with|math-font-family|rm|t1>|2>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>19) >
    <|unfolded-io>
      fullratsimp(p[3,3])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o19>)
      >><with|math-font-family|rm|s2>*<with|math-font-family|rm|t2>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>20) >
    <|unfolded-io>
      fullratsimp(p[1,2]*sqrt(2))
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o20>)
      >><with|math-font-family|rm|s0>*<with|math-font-family|rm|t3>+<with|math-font-family|rm|s3>*<with|math-font-family|rm|t1>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>21) >
    <|unfolded-io>
      fullratsimp(p[1,3]*sqrt(2))
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o21>)
      >>0>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>22) >
    <|unfolded-io>
      fullratsimp(p[2,3]*sqrt(2))
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o22>)
      >>0>>
    </unfolded-io>

    <\input>
      <with|color|red|(<with|math-font-family|rm|%i>14) >
    <|input>
      \;
    </input>
  </session>
</body>

<\initial>
  <\collection>
    <associate|language|french>
    <associate|page-type|a3>
  </collection>
</initial>