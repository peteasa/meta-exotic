DEPENDS = "autoconf-native"

inherit autotools_exotic gettext multilib_header texinfo

# Ensure that static libraries are built
DISABLE_STATIC = ""

FILES_${PN} = " \
	${bindir}/* \
	${prefix}/${EXOTIC_TARGET_SYS}/bin/* \
    ${libdir}/${EXOTIC_TARGET_SYS}/lib*-*.so \
    ${prefix}/${EXOTIC_TARGET_SYS}/lib/ldscripts/* \
    "
FILES_${PN}-dbg += " \
    ${libdir}/${EXOTIC_TARGET_SYS}/.debug/* \
    "
FILES_${PN}-dev = "	\
    ${libdir}/${EXOTIC_TARGET_SYS}/libbfd.so \
    ${libdir}/${EXOTIC_TARGET_SYS}/libopcodes.so \
    ${libdir}/${EXOTIC_TARGET_SYS}/*.la \
	"
FILES_${PN}-staticdev = "	\
    ${libdir}/${EXOTIC_TARGET_SYS}/*.a \
	"

# Skip the architecture qa check
# this allows epiphany code to be packaged alongside arm code
INSANE_SKIP_${PN} += "arch"
INSANE_SKIP_${PN}-dbg += "arch"
INSANE_SKIP_${PN}-dev += "arch"

RPROVIDES_${PN} += " ${PN}-symlinks ${PN}-staticdev "

# Rather than duplicating multiple entries for these, make one
# list and reuse it.

USE_ALTERNATIVES_FOR = " \
	addr2line \
	ar \
	as \
	c++filt \
	elfedit \
	gprof \
	ld \
	ld.bfd \
	nm \
	objcopy \
	objdump \
	ranlib \
	readelf \
	size \
	strings \
	strip \
"

python do_package_prepend() {
    make_alts = d.getVar("USE_ALTERNATIVES_FOR", True) or ""
    prefix = d.getVar("EXOTIC_TARGET_PREFIX", True)
    bindir = d.getVar("bindir", True)
    for alt in make_alts.split():
        d.setVarFlag('ALTERNATIVE_TARGET', alt, bindir + "/" + prefix + alt)
        d.setVarFlag('ALTERNATIVE_LINK_NAME', alt, bindir + "/" + alt)
}

# FILES_${PN}-dbg = "${prefix}/${EXOTIC_TARGET_SYS}/bin/.debug ${prefix}/${libdir}/.debug"

B = "${S}/build.${HOST_SYS}.${EXOTIC_TARGET_SYS}"

EXTRA_OECONF = "--program-prefix=${EXOTIC_TARGET_PREFIX} \
                --disable-werror \
                --enable-plugins \
                ${LDGOLD} \
                ${@bb.utils.contains('DISTRO_FEATURES', 'multiarch', '--enable-64-bit-bfd', '', d)}"

LDGOLD_class-native = ""
LDGOLD_class-crosssdk = ""
LDGOLD ?= "${@bb.utils.contains('DISTRO_FEATURES', 'ld-is-gold', '--enable-gold=default --enable-threads', '', d)}"

# This is necessary due to a bug in the binutils Makefiles
# EXTRA_OEMAKE = "configure-build-libiberty all"

export AR = "${HOST_PREFIX}ar"
export AS = "${HOST_PREFIX}as"
export LD = "${HOST_PREFIX}ld"
export NM = "${HOST_PREFIX}nm"
export RANLIB = "${HOST_PREFIX}ranlib"
export OBJCOPY = "${HOST_PREFIX}objcopy"
export OBJDUMP = "${HOST_PREFIX}objdump"

export AR_FOR_TARGET = "${EXOTIC_TARGET_PREFIX}ar"
export AS_FOR_TARGET = "${EXOTIC_TARGET_PREFIX}as"
export LD_FOR_TARGET = "${EXOTIC_TARGET_PREFIX}ld"
export NM_FOR_TARGET = "${EXOTIC_TARGET_PREFIX}nm"
export RANLIB_FOR_TARGET = "${EXOTIC_TARGET_PREFIX}ranlib"

export CC_FOR_HOST = "${CCACHE}${HOST_PREFIX}gcc ${HOST_CC_ARCH}"
export CXX_FOR_HOST = "${CCACHE}${HOST_PREFIX}gcc ${HOST_CC_ARCH}"

export CC_FOR_BUILD = "${BUILD_CC}"

MULTIARCH := "${@bb.utils.contains("DISTRO_FEATURES", "multiarch", "yes", "no", d)}"
do_configure[vardeps] += "MULTIARCH"
do_configure () {
	(cd ${S}; gnu-configize) || die "Failed to run gnu-configize"
	oe_runconf
#
# must prime config.cache to ensure the build of libiberty
#
	mkdir -p ${B}/build-${BUILD_SYS}
	for i in ${CONFIG_SITE}; do
		cat $i >> ${B}/build-${BUILD_SYS}/config.cache || true
	done
}

do_install () {
	autotools_do_install

	# Fix the /usr/${EXOTIC_TARGET_SYS}/bin/* links
	for l in ${D}${prefix}/${EXOTIC_TARGET_SYS}/bin/*; do
		rm -f $l
		ln -sf `echo ${prefix}/${EXOTIC_TARGET_SYS}/bin \
			| tr -s / \
			| sed -e 's,^/,,' -e 's,[^/]*,..,g'`${bindir}/${EXOTIC_TARGET_PREFIX}`basename $l` $l
	done

	cd ${D}${bindir}

	#for alt in ${USE_ALTERNATIVES_FOR}; do
	#	rm -f ${D}${bindir}/$alt
	#done

	# Remove unwanted stuff
    rm -rf ${D}${prefix}/lib/libiberty.a
    rm -rf ${D}${prefix}/include
    rm -rf ${D}${prefix}/share

	# oe_multilib_header bfd.h
}

inherit update-alternatives

ALTERNATIVE_PRIORITY = "100"

ALTERNATIVE_${PN}_class-target = "${USE_ALTERNATIVES_FOR}"
