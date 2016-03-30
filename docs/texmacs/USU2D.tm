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
      <with|color|red|(<with|math-font-family|rm|%i>1) >
    <|unfolded-io>
      U:matrix([U0,U3/sqrt(2),0],[U3/sqrt(2),U1,0],[0,0,U2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o1>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|U0>>|<cell|<frac|<with|math-font-family|rm|U3>|<sqrt|2>>>|<cell|0>>|<row|<cell|<frac|<with|math-font-family|rm|U3>|<sqrt|2>>>|<cell|<with|math-font-family|rm|U1>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|U2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>2) >
    <|unfolded-io>
      S:matrix([S0,S3/sqrt(2),0],[S3/sqrt(2),S1,0],[0,0,S2])
    <|unfolded-io>
      \;

      \ <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o2>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|S0>>|<cell|<frac|<with|math-font-family|rm|S3>|<sqrt|2>>>|<cell|0>>|<row|<cell|<frac|<with|math-font-family|rm|S3>|<sqrt|2>>>|<cell|<with|math-font-family|rm|S1>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|S2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>8) >
    <|unfolded-io>
      s:U.S.U
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o8>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|U0>*<around*|(|<frac|<with|math-font-family|rm|S3>*<with|math-font-family|rm|U3>|2>+<with|math-font-family|rm|S0>*<with|math-font-family|rm|U0>|)>+<frac|<with|math-font-family|rm|U3>*<around*|(|<frac|<with|math-font-family|rm|S1>*<with|math-font-family|rm|U3>|<sqrt|2>>+<frac|<with|math-font-family|rm|S3>*<with|math-font-family|rm|U0>|<sqrt|2>>|)>|<sqrt|2>>>|<cell|<frac|<with|math-font-family|rm|U3>*<around*|(|<frac|<with|math-font-family|rm|S3>*<with|math-font-family|rm|U3>|2>+<with|math-font-family|rm|S1>*<with|math-font-family|rm|U1>|)>|<sqrt|2>>+<with|math-font-family|rm|U0>*<around*|(|<frac|<with|math-font-family|rm|S0>*<with|math-font-family|rm|U3>|<sqrt|2>>+<frac|<with|math-font-family|rm|S3>*<with|math-font-family|rm|U1>|<sqrt|2>>|)>>|<cell|0>>|<row|<cell|<frac|<with|math-font-family|rm|U3>*<around*|(|<frac|<with|math-font-family|rm|S3>*<with|math-font-family|rm|U3>|2>+<with|math-font-family|rm|S0>*<with|math-font-family|rm|U0>|)>|<sqrt|2>>+<with|math-font-family|rm|U1>*<around*|(|<frac|<with|math-font-family|rm|S1>*<with|math-font-family|rm|U3>|<sqrt|2>>+<frac|<with|math-font-family|rm|S3>*<with|math-font-family|rm|U0>|<sqrt|2>>|)>>|<cell|<with|math-font-family|rm|U1>*<around*|(|<frac|<with|math-font-family|rm|S3>*<with|math-font-family|rm|U3>|2>+<with|math-font-family|rm|S1>*<with|math-font-family|rm|U1>|)>+<frac|<with|math-font-family|rm|U3>*<around*|(|<frac|<with|math-font-family|rm|S0>*<with|math-font-family|rm|U3>|<sqrt|2>>+<frac|<with|math-font-family|rm|S3>*<with|math-font-family|rm|U1>|<sqrt|2>>|)>|<sqrt|2>>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|S2>*<with|math-font-family|rm|U2><rsup|2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>12) >
    <|unfolded-io>
      fullratsimp(transpose(matrix([s[1,1],s[2,2],s[3,3],sqrt(2)*s[1,2]])))
    <|unfolded-io>
      \;

      \ <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o12>)
      >><matrix|<tformat|<table|<row|<cell|<frac|<with|math-font-family|rm|S1>*<with|math-font-family|rm|U3><rsup|2>+2*<with|math-font-family|rm|S3>*<with|math-font-family|rm|U0>*<with|math-font-family|rm|U3>+2*<with|math-font-family|rm|S0>*<with|math-font-family|rm|U0><rsup|2>|2>>>|<row|<cell|<frac|<with|math-font-family|rm|S0>*<with|math-font-family|rm|U3><rsup|2>+2*<with|math-font-family|rm|S3>*<with|math-font-family|rm|U1>*<with|math-font-family|rm|U3>+2*<with|math-font-family|rm|S1>*<with|math-font-family|rm|U1><rsup|2>|2>>>|<row|<cell|<with|math-font-family|rm|S2>*<with|math-font-family|rm|U2><rsup|2>>>|<row|<cell|<frac|<with|math-font-family|rm|S3>*<with|math-font-family|rm|U3><rsup|2>+<around*|(|2*<with|math-font-family|rm|S1>*<with|math-font-family|rm|U1>+2*<with|math-font-family|rm|S0>*<with|math-font-family|rm|U0>|)>*<with|math-font-family|rm|U3>+2*<with|math-font-family|rm|S3>*<with|math-font-family|rm|U0>*<with|math-font-family|rm|U1>|2>>>>>>>>
    </unfolded-io>

    <\input>
      <with|color|red|(<with|math-font-family|rm|%i>13) >
    <|input>
      \;
    </input>
  </session>
</body>