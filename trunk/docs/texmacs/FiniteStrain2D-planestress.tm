<TeXmacs|1.0.7.15>

<style|<tuple|generic|maxima>>

<\body>
  Compute deformation tensors from the\ 

  <\session|maxima|default>
    <\output>
      Maxima 5.27.0 http://maxima.sourceforge.net

      using Lisp GNU Common Lisp (GCL) GCL 2.6.7 (a.k.a. GCL)

      Distributed under the GNU Public License. See the file COPYING.

      Dedicated to the memory of William Schelter.

      The function bug_report() provides bug reporting information.
    </output>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>37) >
    <|unfolded-io>
      F:matrix([F0,F3,0],[F4,F1,0],[0,0,F2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o37>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F0>>|<cell|<with|math-font-family|rm|F3>>|<cell|0>>|<row|<cell|<with|math-font-family|rm|F4>>|<cell|<with|math-font-family|rm|F1>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|F2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>38) >
    <|unfolded-io>
      transpose(F).F /* right cauchy green tensor */
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o38>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F4><rsup|2>+<with|math-font-family|rm|F0><rsup|2>>|<cell|<with|math-font-family|rm|F1>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|F3>>|<cell|0>>|<row|<cell|<with|math-font-family|rm|F1>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|F3>>|<cell|<with|math-font-family|rm|F3><rsup|2>+<with|math-font-family|rm|F1><rsup|2>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|F2><rsup|2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>39) >
    <|unfolded-io>
      F.transpose(F) /* left cauchy green tensor */
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o39>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F3><rsup|2>+<with|math-font-family|rm|F0><rsup|2>>|<cell|<with|math-font-family|rm|F0>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|F3>>|<cell|0>>|<row|<cell|<with|math-font-family|rm|F0>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|F3>>|<cell|<with|math-font-family|rm|F4><rsup|2>+<with|math-font-family|rm|F1><rsup|2>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|F2><rsup|2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>40) >
    <|unfolded-io>
      p:matrix([p0,p3/sqrt(2),0],[p3/sqrt(2),p1,0],[0,0,p2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o40>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|p0>>|<cell|<frac|<with|math-font-family|rm|p3>|<sqrt|2>>>|<cell|0>>|<row|<cell|<frac|<with|math-font-family|rm|p3>|<sqrt|2>>>|<cell|<with|math-font-family|rm|p1>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|p2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>41) >
    <|unfolded-io>
      s:F.p.transpose(F)
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o41>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F0>*<around*|(|<frac|<with|math-font-family|rm|p3>*<with|math-font-family|rm|F3>|<sqrt|2>>+<with|math-font-family|rm|p0>*<with|math-font-family|rm|F0>|)>+<with|math-font-family|rm|F3>*<around*|(|<with|math-font-family|rm|p1>*<with|math-font-family|rm|F3>+<frac|<with|math-font-family|rm|p3>*<with|math-font-family|rm|F0>|<sqrt|2>>|)>>|<cell|<with|math-font-family|rm|F3>*<around*|(|<frac|<with|math-font-family|rm|p3>*<with|math-font-family|rm|F4>|<sqrt|2>>+<with|math-font-family|rm|p1>*<with|math-font-family|rm|F1>|)>+<with|math-font-family|rm|F0>*<around*|(|<with|math-font-family|rm|p0>*<with|math-font-family|rm|F4>+<frac|<with|math-font-family|rm|p3>*<with|math-font-family|rm|F1>|<sqrt|2>>|)>>|<cell|0>>|<row|<cell|<around*|(|<frac|<with|math-font-family|rm|p3>*<with|math-font-family|rm|F3>|<sqrt|2>>+<with|math-font-family|rm|p0>*<with|math-font-family|rm|F0>|)>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|F1>*<around*|(|<with|math-font-family|rm|p1>*<with|math-font-family|rm|F3>+<frac|<with|math-font-family|rm|p3>*<with|math-font-family|rm|F0>|<sqrt|2>>|)>>|<cell|<with|math-font-family|rm|F1>*<around*|(|<frac|<with|math-font-family|rm|p3>*<with|math-font-family|rm|F4>|<sqrt|2>>+<with|math-font-family|rm|p1>*<with|math-font-family|rm|F1>|)>+<with|math-font-family|rm|F4>*<around*|(|<with|math-font-family|rm|p0>*<with|math-font-family|rm|F4>+<frac|<with|math-font-family|rm|p3>*<with|math-font-family|rm|F1>|<sqrt|2>>|)>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|p2>*<with|math-font-family|rm|F2><rsup|2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>42) >
    <|unfolded-io>
      fullratsimp(s[1,1])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o42>)
      >><with|math-font-family|rm|p1>*<with|math-font-family|rm|F3><rsup|2>+<sqrt|2>*<with|math-font-family|rm|p3>*<with|math-font-family|rm|F0>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|p0>*<with|math-font-family|rm|F0><rsup|2>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>43) >
    <|unfolded-io>
      fullratsimp(s[2,2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o43>)
      >><with|math-font-family|rm|p0>*<with|math-font-family|rm|F4><rsup|2>+<sqrt|2>*<with|math-font-family|rm|p3>*<with|math-font-family|rm|F1>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|p1>*<with|math-font-family|rm|F1><rsup|2>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>44) >
    <|unfolded-io>
      fullratsimp(s[3,3])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o44>)
      >><with|math-font-family|rm|p2>*<with|math-font-family|rm|F2><rsup|2>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>45) >
    <|unfolded-io>
      fullratsimp(s[1,2]*sqrt(2))
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o45>)
      >><around*|(|<with|math-font-family|rm|p3>*<with|math-font-family|rm|F3>+<sqrt|2>*<with|math-font-family|rm|p0>*<with|math-font-family|rm|F0>|)>*<with|math-font-family|rm|F4>+<sqrt|2>*<with|math-font-family|rm|p1>*<with|math-font-family|rm|F1>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F0>*<with|math-font-family|rm|F1>>>
    </unfolded-io>

    <\input>
      <with|color|red|(<with|math-font-family|rm|%i>46) >
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