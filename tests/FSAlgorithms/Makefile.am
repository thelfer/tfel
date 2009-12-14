## Makefile.am -- Process this file with automake to produce a Makefile.in file.
INCLUDES = -I$(top_srcdir)/include

if WITH_TESTS

testdir = $(pkgdatadir)/tests/FSAlgorithms
test_PROGRAMS = accumulate        \
		equal             \
		for_each          \
		generate          \
		inner_product     \
		iota              \
		test_FSalgorithm  \
		test_FSalgorithm2 \
		transform          

accumulate_SOURCES        = accumulate.cxx
equal_SOURCES             = equal.cxx
for_each_SOURCES          = for_each.cxx
generate_SOURCES          = generate.cxx
inner_product_SOURCES     = inner_product.cxx
iota_SOURCES              = iota.cxx
test_FSalgorithm_SOURCES  = test_FSalgorithm.cxx
test_FSalgorithm2_SOURCES = test_FSalgorithm2.cxx
transform_SOURCES         = transform.cxx

TESTS=$(test_PROGRAMS)

endif
