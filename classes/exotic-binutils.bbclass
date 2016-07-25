require exotic-binutils/exotic-binutils.inc

DEPENDS += "flex bison zlib"

EXTRA_OECONF += "--with-sysroot=/ \
                --enable-shared \
                --disable-install-libiberty \
                "

# remove rpath from binaries that contain an rpath with build machine paths
DEPENDS += "chrpath-replacement-native"
EXTRANATIVEPATH += "chrpath-native"

do_install_append() {
        # TODO why does the cross compiler create /lib/lib folder?
	    rm -rf ${D}${prefix}/lib/lib/libiberty.a
        rm -rf ${D}${prefix}/lib/ldscripts
        # Remove rpath from the offending binaries
        chrpath -d ${D}${bindir}/${EXOTIC_TARGET_PREFIX}ar
        chrpath -d ${D}${bindir}/${EXOTIC_TARGET_PREFIX}ranlib
}
