call make clean -f Makefile TARGET=0 DEBUG=0
call make -f Makefile TARGET=0 DEBUG=0
call make clean -f Makefile TARGET=0 DEBUG=1
call make -f Makefile TARGET=0 DEBUG=1
call make clean -f Makefile TARGET=1 DEBUG=0
call make -f Makefile TARGET=1 DEBUG=0
call make clean -f Makefile TARGET=1 DEBUG=1
call make -f Makefile TARGET=1 DEBUG=1

rem xcopy /y ..\..\..\BuildFiles\EcoOS\arm64-v8a\StaticRelease\lib2F48BBCBE4884CC08ECFC45990017215.a %ECO_FRAMEWORK%\Eco.Mutex1Lab\BuildFiles\EcoOS\arm64-v8a\StaticRelease\
rem xcopy /y ..\..\..\BuildFiles\EcoOS\arm64-v8a\DynamicRelease\2F48BBCBE4884CC08ECFC45990017215.so %ECO_FRAMEWORK%\Eco.Mutex1Lab\BuildFiles\EcoOS\arm64-v8a\DynamicRelease\
