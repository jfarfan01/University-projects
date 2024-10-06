CC            := clang
CXX           := clang++
CCFLAGS       := -Wall -Wextra -pedantic -std=c17 -g
CXXFLAGS      := -Wall -Wextra -pedantic -std=c++17 -g -c -o
ASSIGNMENT    := a1

BUILDDIR      := build
SOURCES       := $(wildcard *.cpp)
OBJECTS       := $(patsubst %,$(BUILDDIR)/%,${SOURCES:.cpp=.o})

.DEFAULT_GOAL := bin
.PHONY: reset clean bin lib all run test help

prepare:
	mkdir -p $(BUILDDIR)

$(BUILDDIR)/%.o: %.cpp
	@echo "[\033[36mINFO\033[0m] Compiling object:" $<
	$(CXX) $(CXXFLAGS) $@ $< -MMD -MF ./$@.d

$(ASSIGNMENT) : $(OBJECTS)
	@echo "[\033[36mINFO\033[0m] Linking objects:" $@
	$(CXX) -o $@ $^

clean:						## cleans up project folder
	@echo "[\033[36mINFO\033[0m] Cleaning up folder..."
	rm -f $(ASSIGNMENT)
	rm -f testreport.html
	rm -rf ./$(BUILDDIR)
	rm -rf result.json
	rm -rf result.html
	rm -rf ./valgrind_logs

reset:			## resets the config files
	@echo "[\033[36mINFO\033[0m] Resetting config files..."
	rm -rf ./config
	mkdir ./config
	rm -rf ./messages
	mkdir ./messages
	find ./tests -type f -name "config*.txt" -exec cp -r -t ./config {} +

bin: prepare $(ASSIGNMENT)	## compiles project to executable binary
	@echo "[\033[36mINFO\033[0m] Compiling binary..."
	chmod +x $(ASSIGNMENT)

all: clean reset bin				## all of the above

run: all					## runs the project with default config
	@echo "[\033[36mINFO\033[0m] Executing binary..."
	./$(ASSIGNMENT) ./config.txt

test: all					## runs public testcases on the project
	@echo "[\033[36mINFO\033[0m] Executing testrunner..."
	chmod +x testrunner
	rm -rf ./messages
	./testrunner -c test.toml

help:						## prints the help text
	@echo "Usage: make \033[36m<TARGET>\033[0m"
	@echo "Available targets:"
	@awk -F':.*?##' '/^[a-zA-Z_-]+:.*?##.*$$/{printf "  \033[36m%-10s\033[0m%s\n", $$1, $$2}' $(MAKEFILE_LIST)

-include $(wildcard *.d)

