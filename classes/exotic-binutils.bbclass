require exotic-binutils/exotic-binutils.inc

DEPENDS += "flex bison zlib"

## TODO install libiberty from either epiphany-elf-gcc or epiphany-elf-binutils!
##      for now --disable-install-libiberty
EXTRA_OECONF += "--with-sysroot=/ \
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

        # Update libdir references in copied .la files
        for i in `find ${D}${prefix}/lib/${EXOTIC_TARGET_SYS} -type f -name *.la`; do
                sed -i -e 's#-L${B}/bfd/../libiberty/pic##g' $i
                sed -i -e 's#-L${B}/opcodes/../libiberty/pic##g' $i
        done
}
