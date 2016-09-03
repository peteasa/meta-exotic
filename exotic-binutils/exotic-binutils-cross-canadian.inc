inherit cross-canadian-exotic

SUMMARY = "GNU binary utilities (cross-canadian for ${EXOTIC_TARGET_ARCH} target)"
PN = "${EXOTIC_TARGET_PREFIX}binutils-cross-canadian-${TRANSLATED_TARGET_ARCH}"
BPN = "${EXOTIC_TARGET_PREFIX}binutils"

DEPENDS = "${EXOTIC_TARGET_PREFIX}gcc-cross-canadian-${EXOTIC_TARGET_ARCH} nativesdk-${EXOTIC_TARGET_PREFIX}libgcc nativesdk-${EXOTIC_TARGET_PREFIX}gcc-runtime flex-native bison-native virtual/${HOST_PREFIX}gcc-crosssdk virtual/nativesdk-libc nativesdk-zlib nativesdk-gettext nativesdk-flex"

RDEPENDS_${PN} = "${EXOTIC_TARGET_PREFIX}gcc-cross-canadian-${EXOTIC_TARGET_ARCH} \
       nativesdk-${EXOTIC_TARGET_PREFIX}gcc-runtime-dbg \
       nativesdk-${EXOTIC_TARGET_PREFIX}gcc-runtime-libstdc++-dev \
       nativesdk-${EXOTIC_TARGET_PREFIX}gcc-runtime-libstdc++-staticdev \
       nativesdk-${EXOTIC_TARGET_PREFIX}libgcc \
       nativesdk-${EXOTIC_TARGET_PREFIX}libgcc-dev \
       nativesdk-${EXOTIC_TARGET_PREFIX}libgcc-libgcov \
       "

# TODO fix this.. but for now skip the qa check!
INSANE_SKIP_${PN} += "build-deps"

EXTRA_OECONF += "--with-sysroot=${SDKPATH}/sysroots/${TUNE_PKGARCH}${EXOTIC_TARGET_VENDOR}-${EXOTIC_TARGET_OS} \
                "

## TODO work out if we need to override things set in cross-canadian
#STAGING_BINDIR_TOOLCHAIN = "${STAGING_DIR_NATIVE}${bindir_native}/${SDK_ARCH}${SDK_VENDOR}-${SDK_OS}:${STAGING_DIR_NATIVE}${bindir_native}/${EXOTIC_TARGET_SYS}"
#STAGING_BINDIR_CROSS = "${STAGING_DIR_NATIVE}${bindir_native}/${EXOTIC_TARGET_SYS}"
## etc .. see exotic-gcc-cross-canadian.inc

# Ensure that static libraries are built
DISABLE_STATIC = ""

# Override bitbake.conf FILES_${PN} settings as appropriate
FILES_${PN} = " \
    ${bindir}/* \
    ${prefix}/${TARGET_SYS}/bin/* \
    ${libdir}/lib*-*.so \
    ${prefix}/${TARGET_SYS}/lib/ldscripts/* \
    "
FILES_${PN}-dbg += " \
    ${prefix}/${TARGET_SYS}/bin/.debug/* \
    "
FILES_${PN}-dev = "	\
    ${libdir}/libbfd.so \
    ${libdir}/libopcodes.so \
    ${libdir}/*.la \
	"
FILES_${PN}-staticdev = "	\
    ${libdir}/*.a \
	"
RPROVIDES_${PN} += "${PN}-symlinks"

# We have to point binutils at a sysroot but we don't need to rebuild if this changes
# e.g. we switch between different machines with different tunes.
EXTRA_OECONF[vardepsexclude] = "TUNE_PKGARCH"

do_install () {
    autotools_do_install

    # fix directory structure
    mv ${D}${exec_prefix}/lib/lib/libiberty.a ${D}${libdir}
    rm -rf ${D}${exec_prefix}/lib/lib

    cross_canadian_bindirlinks
}

BBCLASSEXTEND = ""
