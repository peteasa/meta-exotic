do_fetch() {
	:
}
do_fetch[noexec] = "1"
deltask do_unpack
deltask do_patch

do_configure[depends] += "${EXOTIC_TARGET_PREFIX}gcc-source-${PV}:do_preconfigure"
do_populate_lic[depends] += "${EXOTIC_TARGET_PREFIX}gcc-source-${PV}:do_unpack"
