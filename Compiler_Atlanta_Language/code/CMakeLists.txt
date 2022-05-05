#..............................................................
# CMakeList.txt : CMake project for CMakeProject1, include source and define
# project specific logic here.
#..............................................................

cmake_minimum_required (VERSION 3.8)

project ("CompilerAssignment")

# Add source to this project's executable.
add_executable (CompilerAssignment
	"Compilers.h"	"Compilers.c"
	"Buffer.h"		"Buffer.c"		"MainBuffer.c"		"Scanner.h"		"Scanner.c"      "Parser.h"      "Parser.c"       "MainParser.c"		"MainScanner.c"
)

# TO_DO: Add tests and install targets if needed.
