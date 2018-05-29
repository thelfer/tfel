% Access to TFEL sources
% Helfer Thomas
% August 3, 2016

`TFEL` is developed using the
[`Apache Subversion`](https://subversion.apache.org/) open source
version control system held on a `CEA` private server. `CEA`
traditional partners (`EDF` and `Areva`) can request an account on
this server to access the `TFEL` repository).

As we do wish that many people get involved in `TFEL/MFront` usage and
development, the `subversion` repository used by CEA and EDF is alo
synchronized with a public `githbub` repository:

<https://github.com/thelfer/tfel>

All the branches, commit description and history of TFEL are
available. This repository is *read-only*. 

# Access Subversion repository (read/write)

The `TFEL` repository is accessible at:
<https://svn-pleiades.cea.fr/SVN/TFEL>

This server is used for historical and practical reasons.

This server is not meant to receive a wide number of contributors, so
the access is currently restricted. Access can be granted to the
[PLEIADES project manger](mailto:tfel-contact@cea.fr).

We may consider moving to another server if the number of contributors
grows significantly.

## Access through a proxy

If you want to use the `svn` command line through a network using a
proxy, you must edit your `servers` configuration file to indicate
which proxy to use (see the subversion
[FAQ](http://subversion.apache.org/faq.html#proxy) for details).

The files location depends on your operating system. On Linux or Unix
it is located in the directory `~/.subversion`. On Windows it is in
`%APPDATA%\Subversion` (Try `echo %APPDATA%`, note this is a hidden
directory.).

You shall then add the following lines to this file:

~~~~~~~~~~~{#svn-proxy .bash}
[global] 
http-proxy-host = XXXXX 
http-proxy-port = XXXXX
http-proxy-username = XXXXX
http-proxy-password = XXXXX
~~~~~~~~~~~

<!-- Local IspellDict: english -->
