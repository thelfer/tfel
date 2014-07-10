import sys, os, py_compile

if(len(sys.argv)>1):
    sys.stdout.write('Byte-compiling python modules...\n')
    for f in sys.argv[1:]:
        if not os.path.exists(f) or not (len(f) >= 3
                                         and f[-3:] == '.py'):
            continue
        sys.stdout.write(f+'\n')
        sys.stdout.flush()
        py_compile.compile(f,f + 'o')
