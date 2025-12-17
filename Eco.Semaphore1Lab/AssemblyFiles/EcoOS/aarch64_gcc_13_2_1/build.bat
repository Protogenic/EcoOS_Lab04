call make clean -f Makefile TARGET=0 DEBUG=0
call make -f Makefile TARGET=0 DEBUG=0
call make clean -f Makefile TARGET=0 DEBUG=1
call make -f Makefile TARGET=0 DEBUG=1
call make clean -f Makefile TARGET=1 DEBUG=0
call make -f Makefile TARGET=1 DEBUG=0
call make clean -f Makefile TARGET=1 DEBUG=1
call make -f Makefile TARGET=1 DEBUG=1

rem xcopy /y ..\..\..\BuildFiles\EcoOS\arm64-v8a\StaticRelease\lib0741985B8FD0476C867CAE177CD26E7C.a %ECO_FRAMEWORK%\Eco.Semaphore1Lab\BuildFiles\EcoOS\arm64-v8a\StaticRelease\
rem xcopy /y ..\..\..\BuildFiles\EcoOS\arm64-v8a\DynamicRelease\0741985B8FD0476C867CAE177CD26E7C.so %ECO_FRAMEWORK%\Eco.Semaphore1Lab\BuildFiles\EcoOS\arm64-v8a\DynamicRelease\
