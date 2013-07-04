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
      <with|color|red|(<with|math-font-family|rm|%i>4) <with|color|black|>>
    <|unfolded-io>
      r:matrix([a,b,c],[d,e,f],[g,h,i])
    <|unfolded-io>
      <with|mode|math|math-display|true|<with|mode|text|font-family|tt|color|red|(<with|math-font-family|rm|%o4>)
      <with|color|black|>><left|(><tabular*|<tformat|<table|<row|<cell|a>|<cell|b>|<cell|c>>|<row|<cell|d>|<cell|e>|<cell|f>>|<row|<cell|g>|<cell|h>|<cell|i>>>>><right|)>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>5) <with|color|black|>>
    <|unfolded-io>
      transpose(r)
    <|unfolded-io>
      <with|mode|math|math-display|true|<with|mode|text|font-family|tt|color|red|(<with|math-font-family|rm|%o5>)
      <with|color|black|>><left|(><tabular*|<tformat|<table|<row|<cell|a>|<cell|d>|<cell|g>>|<row|<cell|b>|<cell|e>|<cell|h>>|<row|<cell|c>|<cell|f>|<cell|i>>>>><right|)>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>6) <with|color|black|>>
    <|unfolded-io>
      m:matrix([s_0,s_3,s_5],[s_4,s_1,s_7],[s_6,s_8,s_2])
    <|unfolded-io>
      <with|mode|math|math-display|true|<with|mode|text|font-family|tt|color|red|(<with|math-font-family|rm|%o6>)
      <with|color|black|>><left|(><tabular*|<tformat|<table|<row|<cell|<with|math-font-family|rm|s_0>>|<cell|<with|math-font-family|rm|s_3>>|<cell|<with|math-font-family|rm|s_5>>>|<row|<cell|<with|math-font-family|rm|s_4>>|<cell|<with|math-font-family|rm|s_1>>|<cell|<with|math-font-family|rm|s_7>>>|<row|<cell|<with|math-font-family|rm|s_6>>|<cell|<with|math-font-family|rm|s_8>>|<cell|<with|math-font-family|rm|s_2>>>>>><right|)>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>13) <with|color|black|>>
    <|unfolded-io>
      ns=fullratsimp(transpose(r).m.r)
    <|unfolded-io>
      <with|mode|math|math-display|true|<with|mode|text|font-family|tt|color|red|(<with|math-font-family|rm|%o13>)
      <with|color|black|>><left|(><tabular*|<tformat|<table|<row|<cell|d*g*<with|math-font-family|rm|s_8>+d*g*<with|math-font-family|rm|s_7>+a*g*<with|math-font-family|rm|s_6>+a*g*<with|math-font-family|rm|s_5>+a*d*<with|math-font-family|rm|s_4>+a*d*<with|math-font-family|rm|s_3>+g<rsup|2>*<with|math-font-family|rm|s_2>+d<rsup|2>*<with|math-font-family|rm|s_1>+a<rsup|2>*<with|math-font-family|rm|s_0>>|<cell|e*g*<with|math-font-family|rm|s_8>+d*h*<with|math-font-family|rm|s_7>+b*g*<with|math-font-family|rm|s_6>+a*h*<with|math-font-family|rm|s_5>+b*d*<with|math-font-family|rm|s_4>+a*e*<with|math-font-family|rm|s_3>+g*h*<with|math-font-family|rm|s_2>+d*e*<with|math-font-family|rm|s_1>+a*b*<with|math-font-family|rm|s_0>>|<cell|f*g*<with|math-font-family|rm|s_8>+d*i*<with|math-font-family|rm|s_7>+c*g*<with|math-font-family|rm|s_6>+a*i*<with|math-font-family|rm|s_5>+c*d*<with|math-font-family|rm|s_4>+a*f*<with|math-font-family|rm|s_3>+g*i*<with|math-font-family|rm|s_2>+d*f*<with|math-font-family|rm|s_1>+a*c*<with|math-font-family|rm|s_0>>>|<row|<cell|d*h*<with|math-font-family|rm|s_8>+e*g*<with|math-font-family|rm|s_7>+a*h*<with|math-font-family|rm|s_6>+b*g*<with|math-font-family|rm|s_5>+a*e*<with|math-font-family|rm|s_4>+b*d*<with|math-font-family|rm|s_3>+g*h*<with|math-font-family|rm|s_2>+d*e*<with|math-font-family|rm|s_1>+a*b*<with|math-font-family|rm|s_0>>|<cell|e*h*<with|math-font-family|rm|s_8>+e*h*<with|math-font-family|rm|s_7>+b*h*<with|math-font-family|rm|s_6>+b*h*<with|math-font-family|rm|s_5>+b*e*<with|math-font-family|rm|s_4>+b*e*<with|math-font-family|rm|s_3>+h<rsup|2>*<with|math-font-family|rm|s_2>+e<rsup|2>*<with|math-font-family|rm|s_1>+b<rsup|2>*<with|math-font-family|rm|s_0>>|<cell|f*h*<with|math-font-family|rm|s_8>+e*i*<with|math-font-family|rm|s_7>+c*h*<with|math-font-family|rm|s_6>+b*i*<with|math-font-family|rm|s_5>+c*e*<with|math-font-family|rm|s_4>+b*f*<with|math-font-family|rm|s_3>+h*i*<with|math-font-family|rm|s_2>+e*f*<with|math-font-family|rm|s_1>+b*c*<with|math-font-family|rm|s_0>>>|<row|<cell|d*i*<with|math-font-family|rm|s_8>+f*g*<with|math-font-family|rm|s_7>+a*i*<with|math-font-family|rm|s_6>+c*g*<with|math-font-family|rm|s_5>+a*f*<with|math-font-family|rm|s_4>+c*d*<with|math-font-family|rm|s_3>+g*i*<with|math-font-family|rm|s_2>+d*f*<with|math-font-family|rm|s_1>+a*c*<with|math-font-family|rm|s_0>>|<cell|e*i*<with|math-font-family|rm|s_8>+f*h*<with|math-font-family|rm|s_7>+b*i*<with|math-font-family|rm|s_6>+c*h*<with|math-font-family|rm|s_5>+b*f*<with|math-font-family|rm|s_4>+c*e*<with|math-font-family|rm|s_3>+h*i*<with|math-font-family|rm|s_2>+e*f*<with|math-font-family|rm|s_1>+b*c*<with|math-font-family|rm|s_0>>|<cell|f*i*<with|math-font-family|rm|s_8>+f*i*<with|math-font-family|rm|s_7>+c*i*<with|math-font-family|rm|s_6>+c*i*<with|math-font-family|rm|s_5>+c*f*<with|math-font-family|rm|s_4>+c*f*<with|math-font-family|rm|s_3>+i<rsup|2>*<with|math-font-family|rm|s_2>+f<rsup|2>*<with|math-font-family|rm|s_1>+c<rsup|2>*<with|math-font-family|rm|s_0>>>>>><right|)>=<left|(><tabular*|<tformat|<table|<row|<cell|d*g*<with|math-font-family|rm|s_8>+d*g*<with|math-font-family|rm|s_7>+a*g*<with|math-font-family|rm|s_6>+a*g*<with|math-font-family|rm|s_5>+a*d*<with|math-font-family|rm|s_4>+a*d*<with|math-font-family|rm|s_3>+g<rsup|2>*<with|math-font-family|rm|s_2>+d<rsup|2>*<with|math-font-family|rm|s_1>+a<rsup|2>*<with|math-font-family|rm|s_0>>|<cell|e*g*<with|math-font-family|rm|s_8>+d*h*<with|math-font-family|rm|s_7>+b*g*<with|math-font-family|rm|s_6>+a*h*<with|math-font-family|rm|s_5>+b*d*<with|math-font-family|rm|s_4>+a*e*<with|math-font-family|rm|s_3>+g*h*<with|math-font-family|rm|s_2>+d*e*<with|math-font-family|rm|s_1>+a*b*<with|math-font-family|rm|s_0>>|<cell|f*g*<with|math-font-family|rm|s_8>+d*i*<with|math-font-family|rm|s_7>+c*g*<with|math-font-family|rm|s_6>+a*i*<with|math-font-family|rm|s_5>+c*d*<with|math-font-family|rm|s_4>+a*f*<with|math-font-family|rm|s_3>+g*i*<with|math-font-family|rm|s_2>+d*f*<with|math-font-family|rm|s_1>+a*c*<with|math-font-family|rm|s_0>>>|<row|<cell|d*h*<with|math-font-family|rm|s_8>+e*g*<with|math-font-family|rm|s_7>+a*h*<with|math-font-family|rm|s_6>+b*g*<with|math-font-family|rm|s_5>+a*e*<with|math-font-family|rm|s_4>+b*d*<with|math-font-family|rm|s_3>+g*h*<with|math-font-family|rm|s_2>+d*e*<with|math-font-family|rm|s_1>+a*b*<with|math-font-family|rm|s_0>>|<cell|e*h*<with|math-font-family|rm|s_8>+e*h*<with|math-font-family|rm|s_7>+b*h*<with|math-font-family|rm|s_6>+b*h*<with|math-font-family|rm|s_5>+b*e*<with|math-font-family|rm|s_4>+b*e*<with|math-font-family|rm|s_3>+h<rsup|2>*<with|math-font-family|rm|s_2>+e<rsup|2>*<with|math-font-family|rm|s_1>+b<rsup|2>*<with|math-font-family|rm|s_0>>|<cell|f*h*<with|math-font-family|rm|s_8>+e*i*<with|math-font-family|rm|s_7>+c*h*<with|math-font-family|rm|s_6>+b*i*<with|math-font-family|rm|s_5>+c*e*<with|math-font-family|rm|s_4>+b*f*<with|math-font-family|rm|s_3>+h*i*<with|math-font-family|rm|s_2>+e*f*<with|math-font-family|rm|s_1>+b*c*<with|math-font-family|rm|s_0>>>|<row|<cell|d*i*<with|math-font-family|rm|s_8>+f*g*<with|math-font-family|rm|s_7>+a*i*<with|math-font-family|rm|s_6>+c*g*<with|math-font-family|rm|s_5>+a*f*<with|math-font-family|rm|s_4>+c*d*<with|math-font-family|rm|s_3>+g*i*<with|math-font-family|rm|s_2>+d*f*<with|math-font-family|rm|s_1>+a*c*<with|math-font-family|rm|s_0>>|<cell|e*i*<with|math-font-family|rm|s_8>+f*h*<with|math-font-family|rm|s_7>+b*i*<with|math-font-family|rm|s_6>+c*h*<with|math-font-family|rm|s_5>+b*f*<with|math-font-family|rm|s_4>+c*e*<with|math-font-family|rm|s_3>+h*i*<with|math-font-family|rm|s_2>+e*f*<with|math-font-family|rm|s_1>+b*c*<with|math-font-family|rm|s_0>>|<cell|f*i*<with|math-font-family|rm|s_8>+f*i*<with|math-font-family|rm|s_7>+c*i*<with|math-font-family|rm|s_6>+c*i*<with|math-font-family|rm|s_5>+c*f*<with|math-font-family|rm|s_4>+c*f*<with|math-font-family|rm|s_3>+i<rsup|2>*<with|math-font-family|rm|s_2>+f<rsup|2>*<with|math-font-family|rm|s_1>+c<rsup|2>*<with|math-font-family|rm|s_0>>>>>><right|)>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>15) <with|color|black|>>
    <|unfolded-io>
      ns[1,1]
    <|unfolded-io>
      <with|mode|math|math-display|true|<with|mode|text|font-family|tt|color|red|(<with|math-font-family|rm|%o16>)
      <with|color|black|>>d*g*<with|math-font-family|rm|s_8>+d*g*<with|math-font-family|rm|s_7>+a*g*<with|math-font-family|rm|s_6>+a*g*<with|math-font-family|rm|s_5>+a*d*<with|math-font-family|rm|s_4>+a*d*<with|math-font-family|rm|s_3>+g<rsup|2>*<with|math-font-family|rm|s_2>+d<rsup|2>*<with|math-font-family|rm|s_1>+a<rsup|2>*<with|math-font-family|rm|s_0>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>17) <with|color|black|>>
    <|unfolded-io>
      ns[2,2]
    <|unfolded-io>
      <with|mode|math|math-display|true|<with|mode|text|font-family|tt|color|red|(<with|math-font-family|rm|%o17>)
      <with|color|black|>>e*h*<with|math-font-family|rm|s_8>+e*h*<with|math-font-family|rm|s_7>+b*h*<with|math-font-family|rm|s_6>+b*h*<with|math-font-family|rm|s_5>+b*e*<with|math-font-family|rm|s_4>+b*e*<with|math-font-family|rm|s_3>+h<rsup|2>*<with|math-font-family|rm|s_2>+e<rsup|2>*<with|math-font-family|rm|s_1>+b<rsup|2>*<with|math-font-family|rm|s_0>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>18) <with|color|black|>>
    <|unfolded-io>
      ns[3,3]
    <|unfolded-io>
      <with|mode|math|math-display|true|<with|mode|text|font-family|tt|color|red|(<with|math-font-family|rm|%o18>)
      <with|color|black|>>f*i*<with|math-font-family|rm|s_8>+f*i*<with|math-font-family|rm|s_7>+c*i*<with|math-font-family|rm|s_6>+c*i*<with|math-font-family|rm|s_5>+c*f*<with|math-font-family|rm|s_4>+c*f*<with|math-font-family|rm|s_3>+i<rsup|2>*<with|math-font-family|rm|s_2>+f<rsup|2>*<with|math-font-family|rm|s_1>+c<rsup|2>*<with|math-font-family|rm|s_0>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>19) <with|color|black|>>
    <|unfolded-io>
      ns[1,2]
    <|unfolded-io>
      <with|mode|math|math-display|true|<with|mode|text|font-family|tt|color|red|(<with|math-font-family|rm|%o19>)
      <with|color|black|>>e*g*<with|math-font-family|rm|s_8>+d*h*<with|math-font-family|rm|s_7>+b*g*<with|math-font-family|rm|s_6>+a*h*<with|math-font-family|rm|s_5>+b*d*<with|math-font-family|rm|s_4>+a*e*<with|math-font-family|rm|s_3>+g*h*<with|math-font-family|rm|s_2>+d*e*<with|math-font-family|rm|s_1>+a*b*<with|math-font-family|rm|s_0>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>20) <with|color|black|>>
    <|unfolded-io>
      ns[2,1]
    <|unfolded-io>
      <with|mode|math|math-display|true|<with|mode|text|font-family|tt|color|red|(<with|math-font-family|rm|%o20>)
      <with|color|black|>>d*h*<with|math-font-family|rm|s_8>+e*g*<with|math-font-family|rm|s_7>+a*h*<with|math-font-family|rm|s_6>+b*g*<with|math-font-family|rm|s_5>+a*e*<with|math-font-family|rm|s_4>+b*d*<with|math-font-family|rm|s_3>+g*h*<with|math-font-family|rm|s_2>+d*e*<with|math-font-family|rm|s_1>+a*b*<with|math-font-family|rm|s_0>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>21) <with|color|black|>>
    <|unfolded-io>
      ns[1,3]
    <|unfolded-io>
      <with|mode|math|math-display|true|<with|mode|text|font-family|tt|color|red|(<with|math-font-family|rm|%o21>)
      <with|color|black|>>f*g*<with|math-font-family|rm|s_8>+d*i*<with|math-font-family|rm|s_7>+c*g*<with|math-font-family|rm|s_6>+a*i*<with|math-font-family|rm|s_5>+c*d*<with|math-font-family|rm|s_4>+a*f*<with|math-font-family|rm|s_3>+g*i*<with|math-font-family|rm|s_2>+d*f*<with|math-font-family|rm|s_1>+a*c*<with|math-font-family|rm|s_0>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>22) <with|color|black|>>
    <|unfolded-io>
      ns[3,1]
    <|unfolded-io>
      <with|mode|math|math-display|true|<with|mode|text|font-family|tt|color|red|(<with|math-font-family|rm|%o22>)
      <with|color|black|>>d*i*<with|math-font-family|rm|s_8>+f*g*<with|math-font-family|rm|s_7>+a*i*<with|math-font-family|rm|s_6>+c*g*<with|math-font-family|rm|s_5>+a*f*<with|math-font-family|rm|s_4>+c*d*<with|math-font-family|rm|s_3>+g*i*<with|math-font-family|rm|s_2>+d*f*<with|math-font-family|rm|s_1>+a*c*<with|math-font-family|rm|s_0>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>23) <with|color|black|>>
    <|unfolded-io>
      ns[2,3]
    <|unfolded-io>
      <with|mode|math|math-display|true|<with|mode|text|font-family|tt|color|red|(<with|math-font-family|rm|%o23>)
      <with|color|black|>>f*h*<with|math-font-family|rm|s_8>+e*i*<with|math-font-family|rm|s_7>+c*h*<with|math-font-family|rm|s_6>+b*i*<with|math-font-family|rm|s_5>+c*e*<with|math-font-family|rm|s_4>+b*f*<with|math-font-family|rm|s_3>+h*i*<with|math-font-family|rm|s_2>+e*f*<with|math-font-family|rm|s_1>+b*c*<with|math-font-family|rm|s_0>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>24) <with|color|black|>>
    <|unfolded-io>
      ns[3,2]
    <|unfolded-io>
      <with|mode|math|math-display|true|<with|mode|text|font-family|tt|color|red|(<with|math-font-family|rm|%o24>)
      <with|color|black|>>e*i*<with|math-font-family|rm|s_8>+f*h*<with|math-font-family|rm|s_7>+b*i*<with|math-font-family|rm|s_6>+c*h*<with|math-font-family|rm|s_5>+b*f*<with|math-font-family|rm|s_4>+c*e*<with|math-font-family|rm|s_3>+h*i*<with|math-font-family|rm|s_2>+e*f*<with|math-font-family|rm|s_1>+b*c*<with|math-font-family|rm|s_0>>
    </unfolded-io>

    <\input>
      <with|color|red|(<with|math-font-family|rm|%i>25) <with|color|black|>>
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