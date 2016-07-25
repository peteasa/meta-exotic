inherit cross-canadian-exotic

SUMMARY = "GNU cc and gcc C compilers (cross-canadian for ${EXOTIC_TARGET_ARCH} target)"
PN = "${EXOTIC_TARGET_PREFIX}gcc-cross-canadian-${TRANSLATED_TARGET_ARCH}"

#
DEPENDS = "virtual/${EXOTIC_TARGET_PREFIX}gcc virtual/${HOST_PREFIX}binutils-crosssdk virtual/nativesdk-${HOST_PREFIX}libc-for-gcc nativesdk-gettext"

GCCMULTILIB = "--enable-multilib"

require exotic-gcc/exotic-gcc-configure-common.inc

# Ensure that the original ${TARGET_SYS} is used to calculate the sysroot
# allowing us to share the same sysroot.
# TODO fix up the correct path for the c++ include dir...
EXTRA_OECONF = "\
    ${GCCMULTILIB} \
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
    --with-cross-host=${TARGET_SYS_GVARIABLE} \
"

EXTRA_OECONF_PATHS = "\
    --with-gxx-include-dir=${SDKPATH}/sysroots/${TUNE_PKGARCH}${TARGET_VENDOR}-${TARGET_OS}${target_includedir}/c++/${BINV} \
    --with-build-time-tools=${STAGING_DIR_NATIVE}${prefix_native}/${EXOTIC_TARGET_SYS}/bin \
    --with-sysroot=${SDKPATH}/sysroots/${TUNE_PKGARCH}${TARGET_VENDOR_GVARIABLE}-${TARGET_OS_GVARIABLE} \
    --with-build-sysroot=${STAGING_DIR_TARGET} \
"

# Override things set in cross-canadian
STAGING_BINDIR_TOOLCHAIN = "${STAGING_DIR_NATIVE}${bindir_native}/${SDK_ARCH}${SDK_VENDOR}-${SDK_OS}:${STAGING_DIR_NATIVE}${bindir_native}/${EXOTIC_TARGET_SYS}"
STAGING_BINDIR_CROSS = "${STAGING_DIR_NATIVE}${bindir_native}/${EXOTIC_TARGET_SYS}"

#base_prefix = "${SDKPATHNATIVE}"
#prefix = "${SDKPATHNATIVE}${prefix_nativesdk}"
#exec_prefix = "${SDKPATHNATIVE}${prefix_nativesdk}"
bindir = "${exec_prefix}/bin/${EXOTIC_TARGET_SYS}"
sbindir = "${bindir}"
base_bindir = "${bindir}"
base_sbindir = "${bindir}"
libdir = "${exec_prefix}/lib/${EXOTIC_TARGET_SYS}"
libexecdir = "${exec_prefix}/lib/${EXOTIC_TARGET_SYS}"

# We have to point gcc at a sysroot but we don't need to rebuild if this changes
# e.g. we switch between different machines with different tunes.
EXTRA_OECONF_PATHS[vardepsexclude] = "TUNE_PKGARCH"
TARGET_ARCH[vardepsexclude] = "TUNE_ARCH"

#
# gcc-cross looks and finds these in ${exec_prefix} but we're not so lucky
# for the sdk. Hardcoding the paths ensures the build doesn't go canadian or worse.
#
export AR_FOR_TARGET = "${EXOTIC_TARGET_PREFIX}ar"
export AS_FOR_TARGET = "${EXOTIC_TARGET_PREFIX}as"
export DLLTOOL_FOR_TARGET = "${EXOTIC_TARGET_PREFIX}dlltool"
export CC_FOR_TARGET = "${EXOTIC_TARGET_PREFIX}gcc"
export CXX_FOR_TARGET = "${EXOTIC_TARGET_PREFIX}g++"
export GCC_FOR_TARGET = "${EXOTIC_TARGET_PREFIX}gcc"
export LD_FOR_TARGET = "${EXOTIC_TARGET_PREFIX}ld"
export LIPO_FOR_TARGET = "${EXOTIC_TARGET_PREFIX}lipo"
export NM_FOR_TARGET = "${EXOTIC_TARGET_PREFIX}nm"
export OBJDUMP_FOR_TARGET = "${EXOTIC_TARGET_PREFIX}objdump"
export RANLIB_FOR_TARGET = "${EXOTIC_TARGET_PREFIX}ranlib"
export STRIP_FOR_TARGET = "${EXOTIC_TARGET_PREFIX}strip"
export WINDRES_FOR_TARGET = "${EXOTIC_TARGET_PREFIX}windres"

#
# We need to override this and make sure the compiler can find staging
#
export ARCH_FLAGS_FOR_TARGET = "--sysroot=${STAGING_DIR_TARGET}"

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

do_compile () {
	oe_runmake all-host configure-target-libgcc
}

INHIBIT_PACKAGE_STRIP = "1"

# Having anything auto depending on gcc-cross-sdk is a really bad idea...
EXCLUDE_FROM_SHLIBS = "1"

PACKAGES = "${PN} ${PN}-doc"

FILES_${PN} = "\
    ${exec_prefix}/bin/${EXOTIC_TARGET_SYS}/* \
    ${libexecdir}/gcc/${EXOTIC_TARGET_SYS}/${BINV}/* \
    ${gcclibdir}/${EXOTIC_TARGET_SYS}/${BINV}/*.o \
    ${gcclibdir}/${EXOTIC_TARGET_SYS}/${BINV}/specs \
    ${gcclibdir}/${EXOTIC_TARGET_SYS}/${BINV}/lib* \
    ${gcclibdir}/${EXOTIC_TARGET_SYS}/${BINV}/include \
    ${gcclibdir}/${EXOTIC_TARGET_SYS}/${BINV}/include-fixed \
    ${gcclibdir}/${EXOTIC_TARGET_SYS}/${BINV}/plugin/include/ \
    ${gcclibdir}/${EXOTIC_TARGET_SYS}/${BINV}/plugin/gtype.* \
    ${includedir}/c++/${BINV} \
    ${prefix}/${EXOTIC_TARGET_SYS}/bin/* \
    ${prefix}/${EXOTIC_TARGET_SYS}/lib/* \
    ${prefix}/${EXOTIC_TARGET_SYS}/usr/include/* \
"
INSANE_SKIP_${PN} += "dev-so"

# Skip the architecture qa check
# this allows EXOTIC_TARGET_ARCH code to be packaged alongside arm code
INSANE_SKIP_${PN} += "arch"

FILES_${PN}-doc = "\
    ${infodir} \
    ${mandir} \
    ${gcclibdir}/${EXOTIC_TARGET_SYS}/${BINV}/include/README \
"

EXEEXT = ""

# Compute how to get from libexecdir to bindir in python (easier than shell)
BINRELPATH = "${@os.path.relpath(d.expand("${bindir}"), d.expand("${libexecdir}/gcc/${EXOTIC_TARGET_SYS}/${BINV}"))}"

do_install () {
	( cd ${B}/${EXOTIC_TARGET_SYS}/libgcc; oe_runmake 'DESTDIR=${D}' install-unwind_h )
	oe_runmake 'DESTDIR=${D}' install-host

	# Cleanup some of the ${libdir}{,exec}/gcc stuff ...
	# rm -r ${D}${libdir}/gcc/${EXOTIC_TARGET_SYS}/${BINV}/install-tools
	# rm -r ${D}${libexecdir}/gcc/${EXOTIC_TARGET_SYS}/${BINV}/install-tools
	# rm -rf ${D}${libdir}/gcc/${EXOTIC_TARGET_SYS}/${BINV}/finclude

	# We care about g++ not c++
	rm -f ${D}${bindir}/*c++

	# We don't care about the gcc-<version> copies
	rm -f ${D}${bindir}/*gcc-?.?*
	rm -rf ${D}${prefix}/${EXOTIC_TARGET_SYS}

	# We use libiberty from binutils
	rm -rf ${D}${prefix}/lib/lib

	# Cleanup empty directories which are not shipped
	# we use rmdir instead of 'rm -f' to ensure the non empty directories are not deleted
	# ${D}${libdir}/../lib only seems to appear with SDKMACHINE=i686
	local empty_dirs="${D}${libdir}/../lib ${D}${prefix}/${EXOTIC_TARGET_SYS}/lib ${D}${prefix}/${EXOTIC_TARGET_SYS} ${D}${includedir}"
	for i in $empty_dirs; do
		[ -d $i ] && rmdir --ignore-fail-on-non-empty $i
	done

	# Insert symlinks into libexec so when tools without a prefix are searched for, the correct ones are
	# found.
	dest=${D}${libexecdir}/gcc/${EXOTIC_TARGET_SYS}/${BINV}/
	install -d $dest
	suffix=${EXEEXT}
	for t in ar as ld nm objcopy objdump ranlib strip g77 gcc cpp gfortran; do
		if [ "$t" = "g77" -o "$t" = "gfortran" ] && [ ! -e ${D}${bindir}/${EXOTIC_TARGET_PREFIX}$t$suffix ]; then
			continue
		fi

		ln -sf ${BINRELPATH}/${EXOTIC_TARGET_PREFIX}$t$suffix $dest$t$suffix
	done

	# libquadmath headers need to  be available in the gcc libexec dir
	install -d ${D}${libdir}/gcc/${EXOTIC_TARGET_SYS}/${BINV}/include/
	cp ${S}/libquadmath/quadmath.h ${D}${libdir}/gcc/${EXOTIC_TARGET_SYS}/${BINV}/include/
	cp ${S}/libquadmath/quadmath_weak.h ${D}${libdir}/gcc/${EXOTIC_TARGET_SYS}/${BINV}/include/

	chown -R root:root ${D}

	cross_canadian_bindirlinks
}

ELFUTILS = "nativesdk-elfutils"
DEPENDS += "nativesdk-gmp nativesdk-mpfr nativesdk-libmpc ${ELFUTILS} nativesdk-zlib"
RDEPENDS_${PN} += "nativesdk-mpfr nativesdk-libmpc ${ELFUTILS}"

SYSTEMHEADERS = "/usr/include"
SYSTEMLIBS = "${target_base_libdir}/"
SYSTEMLIBS1 = "${target_libdir}/"


EXTRA_OECONF += "--enable-poison-system-directories"

EXTRA_OECONF += "\
    --with-mpfr=${STAGING_DIR_HOST}${layout_exec_prefix} \
    --with-mpc=${STAGING_DIR_HOST}${layout_exec_prefix} \
"

# gcc 4.7 needs -isystem
export ARCH_FLAGS_FOR_TARGET = "--sysroot=${STAGING_DIR_TARGET} -isystem=${target_includedir}"
