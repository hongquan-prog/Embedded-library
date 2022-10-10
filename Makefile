.PHONY: all clean flash menuconfig config

MODULES := MetaOS SGP30 SHT3x Modbus

BUILD_TYPE := RELEASE
# BUILD_TYPE := DEBUG

all: config
	cmake --build build

config:
	cmake -G Ninja -S . -B build -DCMAKE_TOOLCHAIN_FILE=tool.cmake -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) -DCMD_TARGET=${MAKECMDGOALS}

clean:
	rm -r build

flash: config
	cmake --build build --target flash

menuconfig: config
	ccmake . build

${MODULES}: config
	cmake --build build