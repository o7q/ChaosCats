CXX = g++
CXXFLAGS = -std=c++17 -Wall -O0

SOURCES = $(wildcard *.cpp */*.cpp */*/*.cpp */*/*/*.cpp)

SFML_ROOT = C:\CppLibs\SFML\SFML-2.6.1_static
SFML_INCLUDE = -I "$(SFML_ROOT)/include"
SFML_LIBRARY = -L "$(SFML_ROOT)/lib"

ASIO_ROOT = C:\CppLibs\Asio\asio-1.30.2
ASIO_INCLUDE = -I "$(ASIO_ROOT)/include"

INCLUDES = $(SFML_INCLUDE) $(ASIO_INCLUDE) -include "chaoscats/net/framework/net.hpp"
LIBRARIES = $(SFML_LIBRARY)

WINDOWS_STD_LINK = -static-libstdc++ \
				   -static-libgcc
WINDOWS_SFML_LINK = -DSFML_STATIC \
					-lsfml-window-s \
					-lsfml-graphics-s \
					-lsfml-audio-s \
					-lsfml-system-s \
					-lopengl32 \
					-lwinmm \
					-lgdi32 \
					-lfreetype \
					-lopenal32 \
					-lflac \
					-lvorbisenc \
					-lvorbisfile \
					-lvorbis \
					-logg
WINDOWS_NET_LINK = -lwsock32 \
				   -lws2_32

EXECUTABLE = ChaosCats

# Windows-specific settings
compile-windows:
	$(CXX) $(CXXFLAGS) $(SOURCES) $(INCLUDES) $(LIBRARIES) -o $(EXECUTABLE) \
	-static \
	$(WINDOWS_STD_LINK) \
	$(WINDOWS_SFML_LINK) \
	$(WINDOWS_NET_LINK)

compile-windows-debug:
	$(CXX) -g $(CXXFLAGS) $(SOURCES) $(INCLUDES) $(LIBRARIES) -o $(EXECUTABLE) \
	-static \
	$(WINDOWS_STD_LINK) \
	$(WINDOWS_SFML_LINK) \
	$(WINDOWS_NET_LINK)

# Precompile net headers
precompile-net-windows:
	$(CXX) $(CXXFLAGS) -H -x c++-header -o "chaoscats/net/framework/net.hpp.gch" "chaoscats/net/framework/net.hpp" $(ASIO_INCLUDE)