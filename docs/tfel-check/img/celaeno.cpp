/*!
 * \file   celaeno.check
 * \brief  This is the test file
 *         used to check non regression
 * \author Helfer Thomas
 * \date   01 Feb 2008
 */

// Meshing
MeshCommand '../../..//CELAENO/src/celaenoMesh --scheme=Unicell2 ../../../../CELAENO/tests/test-isotrope/celaeno.mesh'

// Compute
Command '../../..//CELAENO/src/runcelaeno --scheme=Unicell ../../../../CELAENO/tests/test-isotrope/celaeno.ple'

// Tests
TestType Absolute

Test '../../../../CELAENO/tests/test-isotrope/curves/gas_release.ref' 'curves/gas_release.txt' 2 3 4 5 6 7 8
