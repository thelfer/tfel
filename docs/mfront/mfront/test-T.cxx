/*
 * \file   pleiades/SiCCreep/test-T.ple
 * \brief  imposed displacement test case
 * celaeno --scheme=BehaviourTesting test-T.ple
 * \author Helfer Thomas
 * \date   06 / 12 / 07
 */

Loadings{'Tx', 'Ty',
         'Tz'} Import 'SiCCreep.ple' LoadingVariable ExternalTemperature
    LoadingVariable flux MechanicalLoading flux LoadingDescription Table
        Time = {0., 3600. in 100}
               /* Name of the different loading variables */
               Time T ExternalTemperature flux 0 -
               100.e6 293.15 0. 3600. - 100.e6 293.15 0. EndOfLoadingDescription
