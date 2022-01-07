.SILENT:
.IGNORE:

# Source
PROGRAM_SRC=src\win32_sgr.cpp

TEST_SRC=src\test.cpp

# Objects
OBJ=

# Headers
HDR=

# Defines
DDEFINES=/DSAN_DEVELOPER_BUILD

# Compiler Debug Flags
DCFLAGS=/EHa /MDd /Zi /Od /FC /MDd /MP /diagnostics:column /nologo

# Compiler Release Flags
RCFLAGS=

# c4189 initialized but not referenced warning
WINIT=/wd4189
WARNINGS=/W4 /wd4201 $(WINIT)

BUILD_DIR=build
DEBUG_DIR=debug
RELEASE_DIR=release

DEBUG_TARGET_PATH=$(BUILD_DIR)\$(DEBUG_DIR)
RELEASE_TARGET_PATH=$(BUILD_DIR)\$(RELEASE_DIR)

SYS_LIB=User32.lib gdi32.lib winmm.lib comdlg32.lib

LIB_SRC_DIR=
LIB_SRC=

PROGRAM=win32_sgr.exe
PROGRAM_DEPENDECIES=$(SYS_LIB)

DEBUG_PROGRAM=d$(PROGRAM)


all: $(DEBUG_PROGRAM)

$(DEBUG_PROGRAM): $(SRC)
	if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
	if not exist $(BUILD_DIR)\$(DEBUG_DIR) mkdir $(BUILD_DIR)\$(DEBUG_DIR)
	$(CC) $(PROGRAM_SRC) $(DCFLAGS) $(WARNINGS) $(INCLUDE_DIR) $(SYS_LIB) $(DDEFINES) /Fo$(DEBUG_TARGET_PATH)\ /Fe$(DEBUG_TARGET_PATH)\$(PROGRAM)

clean:
	del build /s /q

tags: $(SRC) $(LIB_SRC)
	ctags -R --kinds-C=+l --c++-kinds=+p --fields=+iaS --extras=+q --language-force=C++ --sort=true .\src

test_path:
	echo $(DEBUG_TARGET_PATH)\*.pdb

test: $(TEST_SRC)
	$(CC) $(TEST_SRC) $(DCFLAGS) $(WARNINGS) $(INCLUDE_DIR) $(SYS_LIB) $(DDEFINES) /Fo$(DEBUG_TARGET_PATH)\ /Fe$(DEBUG_TARGET_PATH)\test.exe



