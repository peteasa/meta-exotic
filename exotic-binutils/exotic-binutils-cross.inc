inherit cross-exotic
PROVIDES = "virtual/${EXOTIC_TARGET_PREFIX}binutils"

PN = "epiphany-elf-binutils-cross-${EXOTIC_TARGET_ARCH}"
BPN = "epiphany-elf-binutils"

INHIBIT_DEFAULT_DEPS = "1"
INHIBIT_AUTOTOOLS_DEPS = "1"

EXTRA_OECONF += "--with-sysroot=${STAGING_DIR_TARGET} \
                --disable-install-libbfd \
                --enable-poison-system-directories \
                "

do_install_append () {

    # TODO why does the cross compiler create /lib/lib folder?
    rm -rf ${D}${prefix}/lib/lib/libiberty.a
}

# Now add the /usr/${EXOTIC_TARGET_SYS} directory to the sysroot staging folder list
SYSROOT_PREPROCESS_FUNCS += "epiphany_elf_binutils_sysroot_preprocess"
epiphany_elf_binutils_sysroot_preprocess () {
	sysroot_stage_dir ${D}${prefix}/${EXOTIC_TARGET_SYS} ${STAGING_DIR_NATIVE}${prefix_native}/${EXOTIC_TARGET_SYS}
}

# Now ensure that the /usr/${EXOTIC_TARGET_SYS} directory is correctly cleaned up
CLEANFUNCS += "epiphany_elf_binutils_sstate_clean"
epiphany_elf_binutils_sstate_clean () {
	rm -rf ${STAGING_DIR_NATIVE}${prefix_native}/${EXOTIC_TARGET_SYS}
}
