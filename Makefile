BUILDDIR=./build
WORKDIR=$(shell pwd)
BIN_NAME=learn-opengl
TARGET=$(BUILDDIR)/bin/$(BIN_NAME)

CMAKE_FLAGS := ""

CLANG_FMT_FILES += src/*.cpp
CLANG_FMT_FILES += include/*.hpp
CLANG_FMT_FILES += include/*.h

.PHONY: build init clean debug run test memcheck

build: init
	cmake -B $(BUILDDIR) $(CMAKE_FLAGS); \
	cd $(BUILDDIR); \
	make; \
	cd $(WORKDIR)

init:
	mkdir -p $(BUILDDIR)

clean:
	rm -rf $(BUILDDIR)

debug:
	gdb $(TARGET)

fmt-check:
	clang-format -n --Werror --style=file --fallback-style="LLVM" $(CLANG_FMT_FILES)

fmt:
	clang-format -i --Werror --style=file --fallback-style="LLVM" $(CLANG_FMT_FILES)

run:
	$(TARGET)

test:
	cmake -B $(BUILDDIR) $(CMAKE_TEST_FLAGS);
	cd $(BUILDDIR); 
	make;
	cd $(WORKDIR);
	$(TARGET)

memcheck:
	valgrind \
		--tool=memcheck                 \
		--leak-check=full               \
		--show-reachable=yes            \
		--num-callers=20                \
		--suppressions=./supfilters.log \
		--gen-suppressions=no           \
		$(TARGET)
