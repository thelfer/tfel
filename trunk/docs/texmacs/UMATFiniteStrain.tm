<TeXmacs|1.0.7.15>

<style|<tuple|generic|maxima>>

<\body>
  <\session|maxima|default>
    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>2) >
    <|unfolded-io>
      F:matrix([F0,F3,F6],[F1,F4,F7],[F2,F5,F8])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o2>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F0>>|<cell|<with|math-font-family|rm|F3>>|<cell|<with|math-font-family|rm|F6>>>|<row|<cell|<with|math-font-family|rm|F1>>|<cell|<with|math-font-family|rm|F4>>|<cell|<with|math-font-family|rm|F7>>>|<row|<cell|<with|math-font-family|rm|F2>>|<cell|<with|math-font-family|rm|F5>>|<cell|<with|math-font-family|rm|F8>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>3) >
    <|unfolded-io>
      0.5*(transpose(F).F-ident(3))
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o3>)
      >><matrix|<tformat|<table|<row|<cell|0.5*<around*|(|<with|math-font-family|rm|F2><rsup|2>+<with|math-font-family|rm|F1><rsup|2>+<with|math-font-family|rm|F0><rsup|2>-1|)>>|<cell|0.5*<around*|(|<with|math-font-family|rm|F2>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|F3>|)>>|<cell|0.5*<around*|(|<with|math-font-family|rm|F2>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|F6>|)>>>|<row|<cell|0.5*<around*|(|<with|math-font-family|rm|F2>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|F3>|)>>|<cell|0.5*<around*|(|<with|math-font-family|rm|F5><rsup|2>+<with|math-font-family|rm|F4><rsup|2>+<with|math-font-family|rm|F3><rsup|2>-1|)>>|<cell|0.5*<around*|(|<with|math-font-family|rm|F5>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|F4>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|F3>*<with|math-font-family|rm|F6>|)>>>|<row|<cell|0.5*<around*|(|<with|math-font-family|rm|F2>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|F1>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|F0>*<with|math-font-family|rm|F6>|)>>|<cell|0.5*<around*|(|<with|math-font-family|rm|F5>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|F4>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|F3>*<with|math-font-family|rm|F6>|)>>|<cell|0.5*<around*|(|<with|math-font-family|rm|F8><rsup|2>+<with|math-font-family|rm|F7><rsup|2>+<with|math-font-family|rm|F6><rsup|2>-1|)>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>4) >
    <|unfolded-io>
      J:determinant(F)
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o4>)
      >><with|math-font-family|rm|F0>*<around*|(|<with|math-font-family|rm|F4>*<with|math-font-family|rm|F8>-<with|math-font-family|rm|F5>*<with|math-font-family|rm|F7>|)>-<with|math-font-family|rm|F3>*<around*|(|<with|math-font-family|rm|F1>*<with|math-font-family|rm|F8>-<with|math-font-family|rm|F2>*<with|math-font-family|rm|F7>|)>+<around*|(|<with|math-font-family|rm|F1>*<with|math-font-family|rm|F5>-<with|math-font-family|rm|F2>*<with|math-font-family|rm|F4>|)>*<with|math-font-family|rm|F6>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>5) >
    <|unfolded-io>
      pk:matrix([p0,p3,p4],[p3,p1,p5],[p4,p5,p2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o5>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|p0>>|<cell|<with|math-font-family|rm|p3>>|<cell|<with|math-font-family|rm|p4>>>|<row|<cell|<with|math-font-family|rm|p3>>|<cell|<with|math-font-family|rm|p1>>|<cell|<with|math-font-family|rm|p5>>>|<row|<cell|<with|math-font-family|rm|p4>>|<cell|<with|math-font-family|rm|p5>>|<cell|<with|math-font-family|rm|p2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>6) >
    <|unfolded-io>
      detF_times_s:F.pk.transpose(F)
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o6>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F3>*<around*|(|<with|math-font-family|rm|p5>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|p1>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F0>|)>+<with|math-font-family|rm|F0>*<around*|(|<with|math-font-family|rm|p4>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|p0>*<with|math-font-family|rm|F0>|)>+<with|math-font-family|rm|F6>*<around*|(|<with|math-font-family|rm|p2>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|p5>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|p4>*<with|math-font-family|rm|F0>|)>>|<cell|<with|math-font-family|rm|F3>*<around*|(|<with|math-font-family|rm|p5>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|p1>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F1>|)>+<with|math-font-family|rm|F0>*<around*|(|<with|math-font-family|rm|p4>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|p0>*<with|math-font-family|rm|F1>|)>+<with|math-font-family|rm|F6>*<around*|(|<with|math-font-family|rm|p2>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|p5>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|p4>*<with|math-font-family|rm|F1>|)>>|<cell|<with|math-font-family|rm|F3>*<around*|(|<with|math-font-family|rm|p5>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|p1>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F2>|)>+<with|math-font-family|rm|F0>*<around*|(|<with|math-font-family|rm|p4>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|p0>*<with|math-font-family|rm|F2>|)>+<with|math-font-family|rm|F6>*<around*|(|<with|math-font-family|rm|p2>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|p5>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|p4>*<with|math-font-family|rm|F2>|)>>>|<row|<cell|<around*|(|<with|math-font-family|rm|p2>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|p5>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|p4>*<with|math-font-family|rm|F0>|)>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|F4>*<around*|(|<with|math-font-family|rm|p5>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|p1>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F0>|)>+<with|math-font-family|rm|F1>*<around*|(|<with|math-font-family|rm|p4>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|p0>*<with|math-font-family|rm|F0>|)>>|<cell|<with|math-font-family|rm|F4>*<around*|(|<with|math-font-family|rm|p5>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|p1>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F1>|)>+<with|math-font-family|rm|F1>*<around*|(|<with|math-font-family|rm|p4>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|p0>*<with|math-font-family|rm|F1>|)>+<with|math-font-family|rm|F7>*<around*|(|<with|math-font-family|rm|p2>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|p5>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|p4>*<with|math-font-family|rm|F1>|)>>|<cell|<with|math-font-family|rm|F4>*<around*|(|<with|math-font-family|rm|p5>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|p1>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F2>|)>+<with|math-font-family|rm|F1>*<around*|(|<with|math-font-family|rm|p4>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|p0>*<with|math-font-family|rm|F2>|)>+<with|math-font-family|rm|F7>*<around*|(|<with|math-font-family|rm|p2>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|p5>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|p4>*<with|math-font-family|rm|F2>|)>>>|<row|<cell|<around*|(|<with|math-font-family|rm|p2>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|p5>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|p4>*<with|math-font-family|rm|F0>|)>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|F5>*<around*|(|<with|math-font-family|rm|p5>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|p1>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F0>|)>+<with|math-font-family|rm|F2>*<around*|(|<with|math-font-family|rm|p4>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|p0>*<with|math-font-family|rm|F0>|)>>|<cell|<around*|(|<with|math-font-family|rm|p2>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|p5>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|p4>*<with|math-font-family|rm|F1>|)>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|F5>*<around*|(|<with|math-font-family|rm|p5>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|p1>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F1>|)>+<with|math-font-family|rm|F2>*<around*|(|<with|math-font-family|rm|p4>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|p0>*<with|math-font-family|rm|F1>|)>>|<cell|<with|math-font-family|rm|F5>*<around*|(|<with|math-font-family|rm|p5>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|p1>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F2>|)>+<with|math-font-family|rm|F2>*<around*|(|<with|math-font-family|rm|p4>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|p0>*<with|math-font-family|rm|F2>|)>+<with|math-font-family|rm|F8>*<around*|(|<with|math-font-family|rm|p2>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|p5>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|p4>*<with|math-font-family|rm|F2>|)>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>7) >
    <|unfolded-io>
      detF_times_s[1,1]
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o7>)
      >><with|math-font-family|rm|F3>*<around*|(|<with|math-font-family|rm|p5>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|p1>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F0>|)>+<with|math-font-family|rm|F0>*<around*|(|<with|math-font-family|rm|p4>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|p0>*<with|math-font-family|rm|F0>|)>+<with|math-font-family|rm|F6>*<around*|(|<with|math-font-family|rm|p2>*<with|math-font-family|rm|F6>+<with|math-font-family|rm|p5>*<with|math-font-family|rm|F3>+<with|math-font-family|rm|p4>*<with|math-font-family|rm|F0>|)>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>8) >
    <|unfolded-io>
      detF_times_s[2,2]
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o8>)
      >><with|math-font-family|rm|F4>*<around*|(|<with|math-font-family|rm|p5>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|p1>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F1>|)>+<with|math-font-family|rm|F1>*<around*|(|<with|math-font-family|rm|p4>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|p0>*<with|math-font-family|rm|F1>|)>+<with|math-font-family|rm|F7>*<around*|(|<with|math-font-family|rm|p2>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|p5>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|p4>*<with|math-font-family|rm|F1>|)>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>9) >
    <|unfolded-io>
      detF_times_s[3,3]
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o9>)
      >><with|math-font-family|rm|F5>*<around*|(|<with|math-font-family|rm|p5>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|p1>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F2>|)>+<with|math-font-family|rm|F2>*<around*|(|<with|math-font-family|rm|p4>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|p0>*<with|math-font-family|rm|F2>|)>+<with|math-font-family|rm|F8>*<around*|(|<with|math-font-family|rm|p2>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|p5>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|p4>*<with|math-font-family|rm|F2>|)>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>10) >
    <|unfolded-io>
      detF_times_s[1,2]
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o10>)
      >><with|math-font-family|rm|F3>*<around*|(|<with|math-font-family|rm|p5>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|p1>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F1>|)>+<with|math-font-family|rm|F0>*<around*|(|<with|math-font-family|rm|p4>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|p0>*<with|math-font-family|rm|F1>|)>+<with|math-font-family|rm|F6>*<around*|(|<with|math-font-family|rm|p2>*<with|math-font-family|rm|F7>+<with|math-font-family|rm|p5>*<with|math-font-family|rm|F4>+<with|math-font-family|rm|p4>*<with|math-font-family|rm|F1>|)>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>11) >
    <|unfolded-io>
      detF_times_s[1,3]
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o11>)
      >><with|math-font-family|rm|F3>*<around*|(|<with|math-font-family|rm|p5>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|p1>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F2>|)>+<with|math-font-family|rm|F0>*<around*|(|<with|math-font-family|rm|p4>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|p0>*<with|math-font-family|rm|F2>|)>+<with|math-font-family|rm|F6>*<around*|(|<with|math-font-family|rm|p2>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|p5>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|p4>*<with|math-font-family|rm|F2>|)>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>12) >
    <|unfolded-io>
      detF_times_s[2,3]
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o12>)
      >><with|math-font-family|rm|F4>*<around*|(|<with|math-font-family|rm|p5>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|p1>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F2>|)>+<with|math-font-family|rm|F1>*<around*|(|<with|math-font-family|rm|p4>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|p3>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|p0>*<with|math-font-family|rm|F2>|)>+<with|math-font-family|rm|F7>*<around*|(|<with|math-font-family|rm|p2>*<with|math-font-family|rm|F8>+<with|math-font-family|rm|p5>*<with|math-font-family|rm|F5>+<with|math-font-family|rm|p4>*<with|math-font-family|rm|F2>|)>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>13) >
    <|unfolded-io>
      iF:fullratsimp(J*invert(F))
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o13>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F4>*<with|math-font-family|rm|F8>-<with|math-font-family|rm|F5>*<with|math-font-family|rm|F7>>|<cell|<with|math-font-family|rm|F5>*<with|math-font-family|rm|F6>-<with|math-font-family|rm|F3>*<with|math-font-family|rm|F8>>|<cell|<with|math-font-family|rm|F3>*<with|math-font-family|rm|F7>-<with|math-font-family|rm|F4>*<with|math-font-family|rm|F6>>>|<row|<cell|<with|math-font-family|rm|F2>*<with|math-font-family|rm|F7>-<with|math-font-family|rm|F1>*<with|math-font-family|rm|F8>>|<cell|<with|math-font-family|rm|F0>*<with|math-font-family|rm|F8>-<with|math-font-family|rm|F2>*<with|math-font-family|rm|F6>>|<cell|<with|math-font-family|rm|F1>*<with|math-font-family|rm|F6>-<with|math-font-family|rm|F0>*<with|math-font-family|rm|F7>>>|<row|<cell|<with|math-font-family|rm|F1>*<with|math-font-family|rm|F5>-<with|math-font-family|rm|F2>*<with|math-font-family|rm|F4>>|<cell|<with|math-font-family|rm|F2>*<with|math-font-family|rm|F3>-<with|math-font-family|rm|F0>*<with|math-font-family|rm|F5>>|<cell|<with|math-font-family|rm|F0>*<with|math-font-family|rm|F4>-<with|math-font-family|rm|F1>*<with|math-font-family|rm|F3>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>24) >
    <|unfolded-io>
      iF[1,1]
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o24>)
      >><with|math-font-family|rm|F4>*<with|math-font-family|rm|F8>-<with|math-font-family|rm|F5>*<with|math-font-family|rm|F7>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>25) >
    <|unfolded-io>
      iF[2,1]
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o25>)
      >><with|math-font-family|rm|F2>*<with|math-font-family|rm|F7>-<with|math-font-family|rm|F1>*<with|math-font-family|rm|F8>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>26) >
    <|unfolded-io>
      iF[3,1]
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o26>)
      >><with|math-font-family|rm|F1>*<with|math-font-family|rm|F5>-<with|math-font-family|rm|F2>*<with|math-font-family|rm|F4>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>27) >
    <|unfolded-io>
      iF[1,2]
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o27>)
      >><with|math-font-family|rm|F5>*<with|math-font-family|rm|F6>-<with|math-font-family|rm|F3>*<with|math-font-family|rm|F8>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>28) >
    <|unfolded-io>
      iF[2,2]
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o28>)
      >><with|math-font-family|rm|F0>*<with|math-font-family|rm|F8>-<with|math-font-family|rm|F2>*<with|math-font-family|rm|F6>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>29) >
    <|unfolded-io>
      iF[3,2]
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o29>)
      >><with|math-font-family|rm|F2>*<with|math-font-family|rm|F3>-<with|math-font-family|rm|F0>*<with|math-font-family|rm|F5>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>30) >
    <|unfolded-io>
      iF[1,3]
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o30>)
      >><with|math-font-family|rm|F3>*<with|math-font-family|rm|F7>-<with|math-font-family|rm|F4>*<with|math-font-family|rm|F6>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>31) >
    <|unfolded-io>
      iF[2,3]
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o31>)
      >><with|math-font-family|rm|F1>*<with|math-font-family|rm|F6>-<with|math-font-family|rm|F0>*<with|math-font-family|rm|F7>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>32) >
    <|unfolded-io>
      iF[3,3]
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o32>)
      >><with|math-font-family|rm|F0>*<with|math-font-family|rm|F4>-<with|math-font-family|rm|F1>*<with|math-font-family|rm|F3>>>
    </unfolded-io>

    <\folded-io>
      <with|color|red|(<with|math-font-family|rm|%i>33) >
    <|folded-io>
      \;
    <|folded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o23>)
      >><with|math-font-family|rm|F0>*<with|math-font-family|rm|F8>-<with|math-font-family|rm|F2>*<with|math-font-family|rm|F6>>>
    </folded-io>

    <\input>
      <with|color|red|(<with|math-font-family|rm|%i>24) >
    <|input>
      \;
    </input>
  </session>
</body>