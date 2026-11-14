~~~~ {.bash}
$ python /media/th202608/0ed06489-8de7-451c-9cf5-7c6d11bec927/codes/llvm/3.9/install/share/clang/run-clang-tidy.py -checks=*,-google-build-using-namespace,-cppcoreguidelines-pro-bounds-array-to-pointer-decay,-readability-named-parameter,-cppcoreguidelines-pro-bounds-constant-array-index,-cppcoreguidelines-pro-bounds-pointer-arithmetic,-google-runtime-int,-llvm-include-order,-google-runtime-references,-llvmlibc-callee-namespace,-modernize-use-trailing-return-type,-fuchsia-default-arguments-calls,-llvmlibc-implementation-in-namespace,-fuchsia-default-arguments-calls,-hicpp-named-parameter,-llvmlibc-restrict-system-libc-headers,-hicpp-uppercase-literal-suffix,-readability-uppercase-literal-suffix,-
~~~~

~~~~
~/codes/llvm/22.1.3/install/bin/run-clang-tidy -checks='cppcoreguidelines-*',-cppcoreguidelines-macro-usage,-cppcoreguidelines-avoid-magic-numbers,-cppcoreguidelines-pro-bounds-avoid-unchecked-container-access,-cppcoreguidelines-pro-bounds-pointer-arithmetic,-cppcoreguidelines-avoid-const-or-ref-data-members,-cppcoreguidelines-non-private-member-variables-in-classes &> out

~/codes/llvm/22.1.3/install/bin/run-clang-tidy -checks=cppcoreguidelines-noexcept-move-operations,cppcoreguidelines-explicit-virtual-functions,cppcoreguidelines-noexcept-move-operations

~/codes/llvm/22.1.3/install/bin/run-clang-tidy -checks='-*,modernize-*,-modernize-use-trailing-return-typemodernize-avoid-c-arraysys' &> out

~/codes/llvm/22.1.3/install/bin/run-clang-tidy -checks='modernize-*',-modernize-use-trailing-return-type,-modernize-avoid-c-arrays,-modernize-macro-to-enum,-modernize-use-using,'-clang-diag*',-modernize-loop-convert,-modernize-use-ranges &> out
~~~~