coverage:
	cd cmake-build-debug && \
	rm -f coverage.txt && \
	rm -rf report && \
	lcov -t "libraries" -o coverage.txt -c -d bool_expr_lib/parallel/CMakeFiles/ -d bool_expr_lib/successive/CMakeFiles/ -d polish_notation_lib/int_comparison/CMakeFiles/

coverage_server:
	cd cmake-build-debug && \
	rm -f coverage.txt && \
	rm -rf report && \
	lcov -t "libraries" -o coverage.txt -c -d bool_expr_lib/parallel/CMakeFiles/ -d bool_expr_lib/successive/CMakeFiles/ -d polish_notation_lib/int_comparison/CMakeFiles/ && \
	genhtml -o report coverage.txt && \
	cd report && \
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
	infer run --fail-on-issue --compilation-database cmake-build-debug/compile_commands.json && \
	rm -rf cmake-build-debug

scan-build:
	rm -rf cmake-build-debug/ && \
    mkdir cmake-build-debug && \
    cd cmake-build-debug && \
    cmake .. && \
    scan-build -v cmake --build .
