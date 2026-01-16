builddir := 'build'
debugdir := 'debug'

fmt:
    find src -type f \( -name "*.cpp" -o -name "*.hpp" \) -print0 | xargs -0 clang-format -i

lint:
	find src -type f -name "*.cpp" -print0 | parallel -j$(nproc) -q0 --no-notice --will-cite --tty --bar clang-tidy --load={{ env_var("TIDYFOX") }}

lint-ci:
	find src -type f -name "*.cpp" -print0 | parallel -j$(nproc) -q0 --no-notice --will-cite --tty clang-tidy --load={{ env_var("TIDYFOX") }}

lint-changed:
	git diff --name-only HEAD | grep "^.*\.cpp\$" |  parallel -j$(nproc) --no-notice --will-cite --tty --bar clang-tidy --load={{ env_var("TIDYFOX") }}

lint-staged:
	git diff --staged --name-only HEAD | grep "^.*\.cpp\$" |  parallel -j$(nproc) --no-notice --will-cite --tty --bar clang-tidy --load={{ env_var("TIDYFOX") }}

configure target='debug' *FLAGS='':
	cmake -GNinja -B {{builddir}} \
		-DCMAKE_BUILD_TYPE={{ if target == "debug" { "Debug" } else { "RelWithDebInfo" } }} \
		-DCMAKE_EXPORT_COMPILE_COMMANDS=ON {{FLAGS}}

	ln -sf {{builddir}}/compile_commands.json compile_commands.json

_configure_if_clean:
	@if ! [ -d {{builddir}} ]; then just configure; fi

build: _configure_if_clean
	cmake --build {{builddir}}

debug: 
  cmake -GNinja -B {{builddir}} \
    -DCMAKE_BUILD_TYPE="Debug" \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
    -DCMAKE_INSTALL_PREFIX={{debugdir}} \
    -DBUILD_QT5="ON" \
    -DBUILD_QT6="OFF"
  cmake --build {{builddir}} 
  cmake --install {{builddir}}

release: (configure "release") build

clean:
	rm -f compile_commands.json
	rm -rf {{builddir}} {{debugdir}}

install *ARGS='':
	cmake --install {{builddir}} {{ARGS}}
