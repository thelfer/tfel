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
      s:matrix([s0,s3/sqrt(2),s4/sqrt(2)],[s3/sqrt(2),s1,s5/sqrt(2)],[s4/sqrt(2),s5/sqrt(2),s2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o2>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|s0>>|<cell|<frac|<with|math-font-family|rm|s3>|<sqrt|2>>>|<cell|<frac|<with|math-font-family|rm|s4>|<sqrt|2>>>>|<row|<cell|<frac|<with|math-font-family|rm|s3>|<sqrt|2>>>|<cell|<with|math-font-family|rm|s1>>|<cell|<frac|<with|math-font-family|rm|s5>|<sqrt|2>>>>|<row|<cell|<frac|<with|math-font-family|rm|s4>|<sqrt|2>>>|<cell|<frac|<with|math-font-family|rm|s5>|<sqrt|2>>>|<cell|<with|math-font-family|rm|s2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>4) >
    <|unfolded-io>
      t:matrix([t0,t3/sqrt(2),t4/sqrt(2)],[t3/sqrt(2),t1,t5/sqrt(2)],[t4/sqrt(2),t5/sqrt(2),t2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o4>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|t0>>|<cell|<frac|<with|math-font-family|rm|t3>|<sqrt|2>>>|<cell|<frac|<with|math-font-family|rm|t4>|<sqrt|2>>>>|<row|<cell|<frac|<with|math-font-family|rm|t3>|<sqrt|2>>>|<cell|<with|math-font-family|rm|t1>>|<cell|<frac|<with|math-font-family|rm|t5>|<sqrt|2>>>>|<row|<cell|<frac|<with|math-font-family|rm|t4>|<sqrt|2>>>|<cell|<frac|<with|math-font-family|rm|t5>|<sqrt|2>>>|<cell|<with|math-font-family|rm|t2>>>>>>>>
    </unfolded-io>

    <\folded-io>
      <with|color|red|(<with|math-font-family|rm|%i>6) >
    <|folded-io>
      p:s.t
    <|folded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o6>)
      >><matrix|<tformat|<table|<row|<cell|<frac|<with|math-font-family|rm|s4>*<with|math-font-family|rm|t4>|2>+<frac|<with|math-font-family|rm|s3>*<with|math-font-family|rm|t3>|2>+<with|math-font-family|rm|s0>*<with|math-font-family|rm|t0>>|<cell|<frac|<with|math-font-family|rm|s4>*<with|math-font-family|rm|t5>|2>+<frac|<with|math-font-family|rm|s0>*<with|math-font-family|rm|t3>|<sqrt|2>>+<frac|<with|math-font-family|rm|s3>*<with|math-font-family|rm|t1>|<sqrt|2>>>|<cell|<frac|<with|math-font-family|rm|s3>*<with|math-font-family|rm|t5>|2>+<frac|<with|math-font-family|rm|s0>*<with|math-font-family|rm|t4>|<sqrt|2>>+<frac|<with|math-font-family|rm|s4>*<with|math-font-family|rm|t2>|<sqrt|2>>>>|<row|<cell|<frac|<with|math-font-family|rm|s5>*<with|math-font-family|rm|t4>|2>+<frac|<with|math-font-family|rm|s1>*<with|math-font-family|rm|t3>|<sqrt|2>>+<frac|<with|math-font-family|rm|s3>*<with|math-font-family|rm|t0>|<sqrt|2>>>|<cell|<frac|<with|math-font-family|rm|s5>*<with|math-font-family|rm|t5>|2>+<frac|<with|math-font-family|rm|s3>*<with|math-font-family|rm|t3>|2>+<with|math-font-family|rm|s1>*<with|math-font-family|rm|t1>>|<cell|<frac|<with|math-font-family|rm|s1>*<with|math-font-family|rm|t5>|<sqrt|2>>+<frac|<with|math-font-family|rm|s3>*<with|math-font-family|rm|t4>|2>+<frac|<with|math-font-family|rm|s5>*<with|math-font-family|rm|t2>|<sqrt|2>>>>|<row|<cell|<frac|<with|math-font-family|rm|s2>*<with|math-font-family|rm|t4>|<sqrt|2>>+<frac|<with|math-font-family|rm|s5>*<with|math-font-family|rm|t3>|2>+<frac|<with|math-font-family|rm|s4>*<with|math-font-family|rm|t0>|<sqrt|2>>>|<cell|<frac|<with|math-font-family|rm|s2>*<with|math-font-family|rm|t5>|<sqrt|2>>+<frac|<with|math-font-family|rm|s4>*<with|math-font-family|rm|t3>|2>+<frac|<with|math-font-family|rm|s5>*<with|math-font-family|rm|t1>|<sqrt|2>>>|<cell|<frac|<with|math-font-family|rm|s5>*<with|math-font-family|rm|t5>|2>+<frac|<with|math-font-family|rm|s4>*<with|math-font-family|rm|t4>|2>+<with|math-font-family|rm|s2>*<with|math-font-family|rm|t2>>>>>>>>
    </folded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>8) >
    <|unfolded-io>
      fullratsimp(p[1,1])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o8>)
      >><frac|<with|math-font-family|rm|s4>*<with|math-font-family|rm|t4>+<with|math-font-family|rm|s3>*<with|math-font-family|rm|t3>+2*<with|math-font-family|rm|s0>*<with|math-font-family|rm|t0>|2>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>9) >
    <|unfolded-io>
      fullratsimp(p[2,2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o9>)
      >><frac|<with|math-font-family|rm|s5>*<with|math-font-family|rm|t5>+<with|math-font-family|rm|s3>*<with|math-font-family|rm|t3>+2*<with|math-font-family|rm|s1>*<with|math-font-family|rm|t1>|2>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>10) >
    <|unfolded-io>
      fullratsimp(p[3,3])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o10>)
      >><frac|<with|math-font-family|rm|s5>*<with|math-font-family|rm|t5>+<with|math-font-family|rm|s4>*<with|math-font-family|rm|t4>+2*<with|math-font-family|rm|s2>*<with|math-font-family|rm|t2>|2>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>11) >
    <|unfolded-io>
      fullratsimp(p[1,2]*sqrt(2))
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o11>)
      >><frac|<sqrt|2>*<with|math-font-family|rm|s4>*<with|math-font-family|rm|t5>+2*<with|math-font-family|rm|s0>*<with|math-font-family|rm|t3>+2*<with|math-font-family|rm|s3>*<with|math-font-family|rm|t1>|2>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>12) >
    <|unfolded-io>
      fullratsimp(p[1,3]*sqrt(2))
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o12>)
      >><frac|<sqrt|2>*<with|math-font-family|rm|s3>*<with|math-font-family|rm|t5>+2*<with|math-font-family|rm|s0>*<with|math-font-family|rm|t4>+2*<with|math-font-family|rm|s4>*<with|math-font-family|rm|t2>|2>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>13) >
    <|unfolded-io>
      fullratsimp(p[2,3]*sqrt(2))
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o13>)
      >><frac|2*<with|math-font-family|rm|s1>*<with|math-font-family|rm|t5>+<sqrt|2>*<with|math-font-family|rm|s3>*<with|math-font-family|rm|t4>+2*<with|math-font-family|rm|s5>*<with|math-font-family|rm|t2>|2>>>
    </unfolded-io>

    <\input>
      <with|color|red|(<with|math-font-family|rm|%i>14) >
    <|input>
      +
    </input>
  </session>
</body>

<\initial>
  <\collection>
    <associate|language|french>
    <associate|page-type|a3>
  </collection>
</initial>