ifeq ($(OS),Windows_NT)
    # Windows settings
    DETECTED_OS := Windows
    RM := del /Q /F
    FIX_SLASHES = $(subst /,\,$1)
	EXEC_PATH = ./build/Debug/Ainulindale.exe
else
    # Unix settings (Linux and macOS)
    DETECTED_OS := $(shell uname -s)
    RM := rm -f
    FIX_SLASHES = $1
	EXEC_PATH = ./build/Ainulindale
endif

compile:
	cmake -S . -B ./build
	cmake --build ./build

run:
	./$(EXEC_PATH)

run-server:
	./build/AinulinServer