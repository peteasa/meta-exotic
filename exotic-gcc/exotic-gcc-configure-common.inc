require exotic-gcc/exotic-gcc-shared-source.inc

EXTRA_OECONF_BASE ?= ""
EXTRA_OECONF_PATHS ?= ""
EXTRA_OECONF_INITIAL ?= ""

# Ensure that static libraries are built
DISABLE_STATIC = ""

# (following is overridden in
#    exotic-cross-canadian.inc
#    exotic-gcc-cross-initial.inc)

EXTRA_OECONF = "\
    --with-gnu-ld \
    --disable-shared \
    --enable-multilib \
    --enable-c99 \
    --disable-gdbtk \
    --disable-werror \
    --enable-fast-install=N/A \
    --without-included-gettext \
    --enable-languages=c,c++ \
    --enable-linker-build-id \
    --disable-libmudflap \
    --disable-libitm \
    --enable-plugin \
    ${EXTRA_OECONF_BASE} \
    --program-prefix=${EXOTIC_TARGET_PREFIX} \
    ${EXTRA_OECONF_PATHS} \
"

export ac_cv_path_SED = 'sed'
export gcc_cv_collect2_libs = 'none required'
# We need to set gcc_cv_collect2_libs else there is cross-compilation badness
# in the config.log files (which might not get generated until do_compile
# hence being missed by the insane do_configure check).

EXTRA_OECONF_GCC_FLOAT ??= ""
CPPFLAGS = ""

SYSTEMHEADERS = "${target_includedir}"
SYSTEMLIBS = "${target_base_libdir}/"
SYSTEMLIBS1 = "${target_libdir}/"

do_configure_prepend () {
	# teach gcc to find correct target includedir when checking libc ssp support
	mkdir -p ${B}/gcc
	echo "NATIVE_SYSTEM_HEADER_DIR = ${SYSTEMHEADERS}" > ${B}/gcc/t-oe
	cat ${S}/gcc/defaults.h | grep -v "\#endif.*GCC_DEFAULTS_H" > ${B}/gcc/defaults.h.new
	cat >>${B}/gcc/defaults.h.new <<_EOF
#define NATIVE_SYSTEM_HEADER_DIR "${SYSTEMHEADERS}"
#define STANDARD_STARTFILE_PREFIX_1 "${SYSTEMLIBS}"
#define STANDARD_STARTFILE_PREFIX_2 "${SYSTEMLIBS1}"
#define SYSTEMLIBS_DIR "${SYSTEMLIBS}"
#endif /* ! GCC_DEFAULTS_H */
_EOF
	mv ${B}/gcc/defaults.h.new ${B}/gcc/defaults.h
}

do_configure () {
	export CC_FOR_BUILD="${BUILD_CC}"
	export CXX_FOR_BUILD="${BUILD_CXX}"
	export CFLAGS_FOR_BUILD="${BUILD_CFLAGS}"
	export CPPFLAGS_FOR_BUILD="${BUILD_CPPFLAGS}"
	export CXXFLAGS_FOR_BUILD="${BUILD_CXXFLAGS}"
	export LDFLAGS_FOR_BUILD="${BUILD_LDFLAGS}"
	export CFLAGS_FOR_TARGET="${EXOTIC_TARGET_CFLAGS}"
	export CPPFLAGS_FOR_TARGET="${EXOTIC_TARGET_CPPFLAGS}"
	export CXXFLAGS_FOR_TARGET="${EXOTIC_TARGET_CXXFLAGS}"
	export LDFLAGS_FOR_TARGET="${EXOTIC_TARGET_LDFLAGS}"


	oe_runconf
}
