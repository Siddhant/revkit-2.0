include(ExternalProject)

find_package( PythonInterp 2 REQUIRED )

externalProject_add(z3ep
  DOWNLOAD_DIR .
  URL "http://www.informatik.uni-bremen.de/revkit/files/z3-4.3.1-unstable.tar.gz"
  CONFIGURE_COMMAND ${PYTHON_EXECUTABLE} scripts/mk_make.py --prefix=${CMAKE_SOURCE_DIR}/ext
  BUILD_COMMAND make -j5 -C build
  BUILD_IN_SOURCE 1
  INSTALL_COMMAND sed "/usr/d" -i build/Makefile COMMAND make install -C build
)

add_ext_library("z3")
