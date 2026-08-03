# CMake generated Testfile for 
# Source directory: /media/secondary/programming/cpp/hlmrl_proto
# Build directory: /media/secondary/programming/cpp/hlmrl_proto
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test([=[hlmrl_test]=] "quartix_test")
set_tests_properties([=[hlmrl_test]=] PROPERTIES  _BACKTRACE_TRIPLES "/media/secondary/programming/cpp/hlmrl_proto/CMakeLists.txt;116;add_test;/media/secondary/programming/cpp/hlmrl_proto/CMakeLists.txt;0;")
subdirs("_deps/raylib-build")
subdirs("_deps/json-build")
subdirs("_deps/entt-build")
subdirs("_deps/fmt-build")
subdirs("_deps/catch2-build")
