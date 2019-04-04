<TeXmacs|1.0.7.4>

<style|<tuple|generic|maxima>>

<\body>
  <\session|maxima|default>
    <\output>
      \;

      Maxima 5.21.1 http://maxima.sourceforge.net

      using Lisp GNU Common Lisp (GCL) GCL 2.6.7 (a.k.a. GCL)

      Distributed under the GNU Public License. See the file COPYING.

      Dedicated to the memory of William Schelter.

      The function bug_report() provides bug reporting information.
    </output>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>1) <with|color|black|>>
    <|unfolded-io>
      r:matrix([a,b,c],[d,e,f],[g,h,i])
    <|unfolded-io>
      <with|mode|math|math-display|true|<with|mode|text|font-family|tt|color|red|(<with|math-font-family|rm|%o1>)
      <with|color|black|>><left|(><tabular*|<tformat|<table|<row|<cell|a>|<cell|b>|<cell|c>>|<row|<cell|d>|<cell|e>|<cell|f>>|<row|<cell|g>|<cell|h>|<cell|i>>>>><right|)>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>2) <with|color|black|>>
    <|unfolded-io>
      transpose(r)
    <|unfolded-io>
      <with|mode|math|math-display|true|<with|mode|text|font-family|tt|color|red|(<with|math-font-family|rm|%o2>)
      <with|color|black|>><left|(><tabular*|<tformat|<table|<row|<cell|a>|<cell|d>|<cell|g>>|<row|<cell|b>|<cell|e>|<cell|h>>|<row|<cell|c>|<cell|f>|<cell|i>>>>><right|)>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>3) <with|color|black|>>
    <|unfolded-io>
      m:matrix([s_0,s_3/sqrt(2),s_4/sqrt(2)],[s_3/sqrt(2),s_1,s_5/sqrt(2)],[s_4/sqrt(2),s_5/sqrt(2),s_2])
    <|unfolded-io>
      <with|mode|math|math-display|true|<with|mode|text|font-family|tt|color|red|(<with|math-font-family|rm|%o3>)
      <with|color|black|>><left|(><tabular*|<tformat|<table|<row|<cell|<with|math-font-family|rm|s_0>>|<cell|<frac|<with|math-font-family|rm|s_3>|<sqrt|2>>>|<cell|<frac|<with|math-font-family|rm|s_4>|<sqrt|2>>>>|<row|<cell|<frac|<with|math-font-family|rm|s_3>|<sqrt|2>>>|<cell|<with|math-font-family|rm|s_1>>|<cell|<frac|<with|math-font-family|rm|s_5>|<sqrt|2>>>>|<row|<cell|<frac|<with|math-font-family|rm|s_4>|<sqrt|2>>>|<cell|<frac|<with|math-font-family|rm|s_5>|<sqrt|2>>>|<cell|<with|math-font-family|rm|s_2>>>>>><right|)>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>6) <with|color|black|>>
    <|unfolded-io>
      fullratsimp(transpose(r).m.r)
    <|unfolded-io>
      <with|mode|math|math-display|true|<with|mode|text|font-family|tt|color|red|(<with|math-font-family|rm|%o6>)
      <with|color|black|>><left|(><tabular*|<tformat|<table|<row|<cell|<sqrt|2>*d*g*<with|math-font-family|rm|s_5>+<sqrt|2>*a*g*<with|math-font-family|rm|s_4>+<sqrt|2>*a*d*<with|math-font-family|rm|s_3>+g<rsup|2>*<with|math-font-family|rm|s_2>+d<rsup|2>*<with|math-font-family|rm|s_1>+a<rsup|2>*<with|math-font-family|rm|s_0>>|<cell|<frac|<left|(><sqrt|2>*d*h+<sqrt|2>*e*g<right|)>*<with|math-font-family|rm|s_5>+<left|(><sqrt|2>*a*h+<sqrt|2>*b*g<right|)>*<with|math-font-family|rm|s_4>+<left|(><sqrt|2>*a*e+<sqrt|2>*b*d<right|)>*<with|math-font-family|rm|s_3>+2*g*h*<with|math-font-family|rm|s_2>+2*d*e*<with|math-font-family|rm|s_1>+2*a*b*<with|math-font-family|rm|s_0>|2>>|<cell|<frac|<left|(><sqrt|2>*d*i+<sqrt|2>*f*g<right|)>*<with|math-font-family|rm|s_5>+<left|(><sqrt|2>*a*i+<sqrt|2>*c*g<right|)>*<with|math-font-family|rm|s_4>+<left|(><sqrt|2>*a*f+<sqrt|2>*c*d<right|)>*<with|math-font-family|rm|s_3>+2*g*i*<with|math-font-family|rm|s_2>+2*d*f*<with|math-font-family|rm|s_1>+2*a*c*<with|math-font-family|rm|s_0>|2>>>|<row|<cell|<frac|<left|(><sqrt|2>*d*h+<sqrt|2>*e*g<right|)>*<with|math-font-family|rm|s_5>+<left|(><sqrt|2>*a*h+<sqrt|2>*b*g<right|)>*<with|math-font-family|rm|s_4>+<left|(><sqrt|2>*a*e+<sqrt|2>*b*d<right|)>*<with|math-font-family|rm|s_3>+2*g*h*<with|math-font-family|rm|s_2>+2*d*e*<with|math-font-family|rm|s_1>+2*a*b*<with|math-font-family|rm|s_0>|2>>|<cell|<sqrt|2>*e*h*<with|math-font-family|rm|s_5>+<sqrt|2>*b*h*<with|math-font-family|rm|s_4>+<sqrt|2>*b*e*<with|math-font-family|rm|s_3>+h<rsup|2>*<with|math-font-family|rm|s_2>+e<rsup|2>*<with|math-font-family|rm|s_1>+b<rsup|2>*<with|math-font-family|rm|s_0>>|<cell|<frac|<left|(><sqrt|2>*e*i+<sqrt|2>*f*h<right|)>*<with|math-font-family|rm|s_5>+<left|(><sqrt|2>*b*i+<sqrt|2>*c*h<right|)>*<with|math-font-family|rm|s_4>+<left|(><sqrt|2>*b*f+<sqrt|2>*c*e<right|)>*<with|math-font-family|rm|s_3>+2*h*i*<with|math-font-family|rm|s_2>+2*e*f*<with|math-font-family|rm|s_1>+2*b*c*<with|math-font-family|rm|s_0>|2>>>|<row|<cell|<frac|<left|(><sqrt|2>*d*i+<sqrt|2>*f*g<right|)>*<with|math-font-family|rm|s_5>+<left|(><sqrt|2>*a*i+<sqrt|2>*c*g<right|)>*<with|math-font-family|rm|s_4>+<left|(><sqrt|2>*a*f+<sqrt|2>*c*d<right|)>*<with|math-font-family|rm|s_3>+2*g*i*<with|math-font-family|rm|s_2>+2*d*f*<with|math-font-family|rm|s_1>+2*a*c*<with|math-font-family|rm|s_0>|2>>|<cell|<frac|<left|(><sqrt|2>*e*i+<sqrt|2>*f*h<right|)>*<with|math-font-family|rm|s_5>+<left|(><sqrt|2>*b*i+<sqrt|2>*c*h<right|)>*<with|math-font-family|rm|s_4>+<left|(><sqrt|2>*b*f+<sqrt|2>*c*e<right|)>*<with|math-font-family|rm|s_3>+2*h*i*<with|math-font-family|rm|s_2>+2*e*f*<with|math-font-family|rm|s_1>+2*b*c*<with|math-font-family|rm|s_0>|2>>|<cell|<sqrt|2>*f*i*<with|math-font-family|rm|s_5>+<sqrt|2>*c*i*<with|math-font-family|rm|s_4>+<sqrt|2>*c*f*<with|math-font-family|rm|s_3>+i<rsup|2>*<with|math-font-family|rm|s_2>+f<rsup|2>*<with|math-font-family|rm|s_1>+c<rsup|2>*<with|math-font-family|rm|s_0>>>>>><right|)>>
    </unfolded-io>

    <\input>
      <with|color|red|(<with|math-font-family|rm|%i>7) <with|color|black|>>
    <|input>
      \;
    </input>
  </session>
</body>

<\initial>
  <\collection>
    <associate|language|french>
  </collection>
</initial>