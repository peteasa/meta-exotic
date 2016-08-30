require exotic-gcc/exotic-gcc-configure-common.inc

CXXFLAGS := "${@oe_filter_out('-fvisibility-inlines-hidden', '${CXXFLAGS}', d)}"

EXTRA_OECONF_PATHS = "\
    --with-gxx-include-dir=${includedir}/c++/${BINV} \
    --with-sysroot=${STAGING_DIR_TARGET} \
    --with-build-sysroot=${STAGING_DIR_TARGET} \
"

# removed libssp libgomp libatomic
RUNTIMETARGET = "libstdc++-v3"
#  ?
# libiberty
# libmudflap
# libgfortran needs separate recipe due to libquadmath dependency
# libssp libstdc++-v3 libgomp libatomic

do_configure () {
	export CXX="${CXX} -nostdinc++ -nostdlib++"
	mtarget=${EXOTIC_TARGET_SYS}
	target=${EXOTIC_TARGET_SYS}
	hardlinkdir ${STAGING_INCDIR_NATIVE}/../${EXOTIC_TARGET_SYS}/include/gcc-build-internal-$mtarget ${B}
	for d in libgcc ${RUNTIMETARGET}; do
		echo "Configuring $d"
		rm -rf ${B}/$target/$d/
		mkdir -p ${B}/$target/$d/
		cd ${B}/$target/$d/
		chmod a+x ${S}/$d/configure
		${S}/$d/configure ${CONFIGUREOPTS} ${EXTRA_OECONF}
	done
}

do_compile () {
	target=${EXOTIC_TARGET_SYS}
	for d in libgcc ${RUNTIMETARGET}; do
		cd ${B}/$target/$d/
		oe_runmake MULTIBUILDTOP=${B}/$target/$d/
	done
}

do_install () {
	target=${EXOTIC_TARGET_SYS}
	for d in ${RUNTIMETARGET}; do
		cd ${B}/$target/$d/
		oe_runmake 'DESTDIR=${D}' MULTIBUILDTOP=${B}/$target/$d/ install
	done
	if [ -d ${D}${infodir} ]; then
		rmdir --ignore-fail-on-non-empty -p ${D}${infodir}
	fi
	chown -R root:root ${D}
}

INHIBIT_DEFAULT_DEPS = "1"

DEPENDS = "virtual/${EXOTIC_TARGET_PREFIX}gcc virtual/${EXOTIC_TARGET_PREFIX}g++ ${EXOTIC_TARGET_PREFIX}libgcc "
PROVIDES = "virtual/${EXOTIC_TARGET_PREFIX}compilerlibs ${EXOTIC_TARGET_PREFIX}gcc-runtime"

# Can we use the parallella-hdmi sysroot version for the sdk?  Uncomment if we need sdk version
# BBCLASSEXTEND = "nativesdk"

# Ensure that static libraries are built
DISABLE_STATIC = ""

PACKAGES = "\
    ${PN}-dbg \
    ${PN}-libstdc++ \
    ${PN}-libstdc++-dev \
    ${PN}-libstdc++-staticdev \
"
# The base package doesn't exist, so we clear the recommends.
RRECOMMENDS_${PN}-dbg = ""

# include python debugging scripts
FILES_${PN}-dbg += "\
    ${libdir}/libstdc++.*-gdb.py \
    ${datadir}/gcc-${BINV}/python/libstdcxx \
"

FILES_${PN}-libstdc++ = "${libdir}/libstdc++.so.*"
FILES_${PN}-libstdc++-dev = "\
    ${includedir}/c++/ \
    ${libdir}/libstdc++.so \
    ${libdir}/libstdc++.la \
    ${libdir}/libsupc++.la \
"
FILES_${PN}-libstdc++-staticdev = "\
    ${libdir}/libstdc++.a \
    ${libdir}/libsupc++.a \
"

# FILES_${PN}-libstdc++-precompile-dev = "${includedir}/c++/${EXOTIC_TARGET_SYS}/bits/*.gch"

do_package_write_ipk[depends] += "virtual/${MLPREFIX}libc:do_packagedata"
do_package_write_deb[depends] += "virtual/${MLPREFIX}libc:do_packagedata"
do_package_write_rpm[depends] += "virtual/${MLPREFIX}libc:do_packagedata"

## Following causes a problem with the SDK creation!  HOST_OS is here fixed
## so SDK build does not fully work
## look at package_rpm.bbclass -
## need to override pkgarch = d.expand('${PACKAGE_ARCH_EXTEND}-poky-linux-gnueabi')
## at the moment it looks like pkgarch = d.expand('${PACKAGE_ARCH_EXTEND}-e-os')
HOST_OS="${TARGET_VENDOR_GVARIABLE}-${TARGET_OS_GVARIABLE}"
