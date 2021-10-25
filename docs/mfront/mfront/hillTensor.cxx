st2tost2<N, real> H;
if ((getModellingHypothesis() == ModellingHypothesis::PLANESTRESS) ||
    (getModellingHypothesis() == ModellingHypothesis::PLANESTRAIN) ||
    (getModellingHypothesis() == ModellingHypothesis::GENERALISEDPLANESTRAIN)) {
  H = hillTensor<N, real>(Hzz, Hrr, Htt, Hrt, Hrz, Htz);
} else {
  H = hillTensor<N, real>(Htt, Hrr, Hzz, Hrz, Hrt, Htz);
}
