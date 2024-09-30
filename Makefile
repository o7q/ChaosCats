CXX = g++
CXXFLAGS = -std=c++17 -Wall -O0

rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))
SOURCES := $(call rwildcard,./,*.cpp)

OBJECTS = $(SOURCES:.cpp=.o)

SFML_ROOT_STATIC = C:\CppLibs\SFML\SFML-2.6.1_static
SFML_INCLUDE_STATIC = -I "$(SFML_ROOT_STATIC)/include"
SFML_LIBRARY_STATIC = -L "$(SFML_ROOT_STATIC)/lib"

SFML_ROOT_SHARED = C:\CppLibs\SFML\SFML-2.6.1_shared
SFML_INCLUDE_SHARED = -I "$(SFML_ROOT_SHARED)/include"
SFML_LIBRARY_SHARED = -L "$(SFML_ROOT_SHARED)/lib"

ASIO_ROOT = C:\CppLibs\Asio\asio-1.30.2
ASIO_INCLUDE = -I "$(ASIO_ROOT)/include"

WINDOWS_STD_LINK_STATIC = -static-libstdc++ \
	-static-libgcc
WINDOWS_SFML_LINK_STATIC = -lsfml-window-s \
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
WINDOWS_SFML_LINK_SHARED = -lsfml-window \
	-lsfml-graphics \
	-lsfml-audio \
	-lsfml-system \
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

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(INCLUDES) -DSFML_STATIC

# Windows-specific settings
compile-windows-static: INCLUDES = $(SFML_INCLUDE_STATIC) $(ASIO_INCLUDE) -include "chaoscats/net/framework/net.hpp"
compile-windows-static: LIBRARIES = $(SFML_LIBRARY_STATIC)
compile-windows-static: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) $(INCLUDES) $(LIBRARIES) -o $(EXECUTABLE)  \
	-static \
	$(WINDOWS_STD_LINK_STATIC) \
	$(WINDOWS_SFML_LINK_STATIC) \
	$(WINDOWS_NET_LINK)

compile-windows-shared: INCLUDES = $(SFML_INCLUDE_SHARED) $(ASIO_INCLUDE) -include "chaoscats/net/framework/net.hpp"
compile-windows-shared: LIBRARIES = $(SFML_LIBRARY_SHARED)
compile-windows-shared: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) $(INCLUDES) $(LIBRARIES) -o $(EXECUTABLE)  \
	$(WINDOWS_SFML_LINK_SHARED) \
	$(WINDOWS_NET_LINK)

precompile-net-windows:
	$(CXX) $(CXXFLAGS) -H -x c++-header -o "chaoscats/net/framework/net.hpp.gch" "chaoscats/net/framework/net.hpp" $(ASIO_INCLUDE)