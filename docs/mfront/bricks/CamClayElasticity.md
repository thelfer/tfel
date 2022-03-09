The `CamClayElasticity` brick is build upon the `CamClay` stress potential.


~~~~{.bash}
$ mfront --help-stress-potential=CamClay
~~~~

## Example

~~~~{.cpp}
@Brick CamClayElasticity{
  unloading_reloading_slope : 0.01,
  initial_void_ratio : 0.5,
  poisson_ratio : 0.3,
  pressure_threshold : 1
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
