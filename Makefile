coverage:
	cd cmake-build-debug && \
	rm -f coverage.info && \
	rm -rf report && \
	lcov -t "tests/test_toys_search" -o coverage.info -c -d toys_catalogue_lib/

coverage_server:
	cd cmake-build-debug && \
	rm -f coverage.info && \
	rm -rf report && \
	lcov -t "tests/test_toys_search" -o coverage.info -c -d toys_catalogue_lib/ && \
	genhtml -o report coverage.info && \
	cd report && \
	cp index.html ../.. && \
	python3 -m http.server 8000

check:
	./linters/linter.sh

valgrind:
	./linters/valgrind.sh

sanitizer_address:
	./linters/sanitizer_address.sh
	rm -f a.out

sanitizer_thread:
	./linters/sanitizer_thread.sh
	rm -f a.out

fbinfer:
	rm -rf cmake-build-debug/ && \
	mkdir cmake-build-debug && \
	cd cmake-build-debug && \
	cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 .. && \
	cd .. && \
	infer run --fail-on-issue --compilation-database cmake-build-debug/compile_commands.json

scan-build:
	rm -rf cmake-build-debug/ && \
    mkdir cmake-build-debug && \
    cd cmake-build-debug && \
    cmake .. && \
    scan-build -v cmake --build .
