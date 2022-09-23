.PHONY: all clean update flash menuconfig config

# BUILD_TYPE := RELEASE
BUILD_TYPE := DEBUG

all: config
	cmake --build build

config:
	cmake -G Ninja -S . -B build -DCMAKE_TOOLCHAIN_FILE=tool.cmake -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)

clean:
	rm -r build

update: config
	cmake --build build --target update

flash: config
	cmake --build build --target flash

menuconfig: config
	ccmake . build