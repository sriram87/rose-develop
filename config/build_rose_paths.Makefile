# This file supports the generation of rose_paths.C
# DQ (4/5/2009): Changed the generated file from a C file to a C++ file, so that it
# would be more consistant with the rest of C++ and work better with MSVC.
src/util/rose_paths.C: Makefile
	@@true > src/util/rose_paths.C
	@@echo "#include <string>" >> src/util/rose_paths.C
	@@echo "" >> src/util/rose_paths.C
	@@echo "/* Use the same header file to declare these variables as is used to reference them so that they will be globally available (and not local). */" >> src/util/rose_paths.C
	@@echo "#include \"rose_paths.h\"" >> src/util/rose_paths.C
	@@echo "" >> src/util/rose_paths.C
	@@echo "/* These paths will be absolute or relative depending on how the configure script is called (called with an absolute or relative path). */" >> src/util/rose_paths.C
	@@echo "const std::string ROSE_GFORTRAN_PATH          = \"@GFORTRAN_PATH@\";" >> src/util/rose_paths.C
	@@echo "const std::string ROSE_AUTOMAKE_TOP_SRCDIR    = \"`cd @top_srcdir@; pwd`\";" >> src/util/rose_paths.C
	@@echo "const std::string ROSE_AUTOMAKE_TOP_BUILDDIR  = \"@top_pwd@\";" >> src/util/rose_paths.C
	@@echo "const std::string ROSE_AUTOMAKE_PREFIX        = \"@prefix@\";" >> src/util/rose_paths.C
	@@echo "const std::string ROSE_AUTOMAKE_DATADIR       = \"@datadir@\";" >> src/util/rose_paths.C
	@@echo "const std::string ROSE_AUTOMAKE_BINDIR        = \"@bindir@\";" >> src/util/rose_paths.C
	@@echo "const std::string ROSE_AUTOMAKE_INCLUDEDIR    = \"@includedir@\";" >> src/util/rose_paths.C
	@@echo "const std::string ROSE_AUTOMAKE_INFODIR       = \"@infodir@\";" >> src/util/rose_paths.C
	@@echo "const std::string ROSE_AUTOMAKE_LIBDIR        = \"@libdir@\";" >> src/util/rose_paths.C
	@@echo "const std::string ROSE_AUTOMAKE_LIBEXECDIR    = \"@libexecdir@\";" >> src/util/rose_paths.C
	@@echo "const std::string ROSE_AUTOMAKE_LOCALSTATEDIR = \"@localstatedir@\";" >> src/util/rose_paths.C
	@@echo "const std::string ROSE_AUTOMAKE_MANDIR        = \"@mandir@\";" >> src/util/rose_paths.C
	@@echo "" >> src/util/rose_paths.C
	@@echo "/* This will always be an absolute path, while paths above are dependent on how the configure script is called (called with an absolute or relative path). */" >> src/util/rose_paths.C
	@@echo "const std::string ROSE_AUTOMAKE_ABSOLUTE_PATH_TOP_SRCDIR = \"@absolute_path_srcdir@\";" >> src/util/rose_paths.C
	@@echo "" >> src/util/rose_paths.C
	@@echo "/* Additional interesting data to provide. */" >> src/util/rose_paths.C
	@@echo "const std::string ROSE_CONFIGURE_DATE     = \"@configure_date@\";" >> src/util/rose_paths.C
	@@echo "const std::string ROSE_AUTOMAKE_BUILD_OS  = \"@build_os@\";" >> src/util/rose_paths.C
	@@echo "const std::string ROSE_AUTOMAKE_BUILD_CPU = \"@build_cpu@\";" >> src/util/rose_paths.C
	@@echo "const std::string ROSE_OFP_VERSION_STRING = \"@ROSE_OFP_VERSION_NUMBER@\";" >> src/util/rose_paths.C

	@@echo "" >> src/util/rose_paths.C
#	@@echo "/* Define the location of the Compass tool within ROSE */" >> src/util/rose_paths.C
#	@@echo "const char COMPASS_SOURCE_DIRECTORY = \"@absolute_path_srcdir@/projects/compass\";" >> src/util/rose_paths.C
#	@@echo "" >> src/util/rose_paths.C

#       Numeric form of ROSE version as documented in rose_paths.h. See that documentation before changing this command!
	@@echo "@PACKAGE_VERSION@" |\
	    tr -c 0-9 . |\
	    tr -s . |\
	    awk -F. '{printf "const unsigned long ROSE_NUMERIC_VERSION = %03d%03d%03dul;\n", $$1, $$2, $$3}' |\
	    sed 's/= 0*/= /' >> src/util/rose_paths.C
