C++11 in eclipse

http://wiki.eclipse.org/CDT/User/FAQ#CDT_does_not_recognize_C.2B.2B11_features

Use this link to set the Dialect option to c++11
    Since CDT 8.3 there is a new "Dialect" option in the compiler option settings, 
    see: https://wiki.eclipse.org/CDT/User/NewIn83#Toolchains

Use this to get the code completion to work
    Project Properties->C/C++ General->Preprocessor Include Paths, Macros->[Providers] tab->your Built-in Compiler Settings provider (toolchain dependent).
    Click on "Workspace Settings" link which gets you to "Settings" property page, select [Discovery] tab and your provider again. There is "Command to get compiler specs", add "-std=c++0x" in there.