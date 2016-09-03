deltask do_configure
deltask do_compile
deltask do_package
deltask do_package_write_rpm
deltask do_package_write_ipk
deltask do_package_write_deb
deltask do_install
deltask do_populate_sysroot
deltask do_populate_lic
deltask do_package_qa
deltask do_packagedata
deltask do_rm_work

PN = "${EXOTIC_TARGET_PREFIX}newlib-source-${PV}"
WORKDIR = "${TMPDIR}/work-shared/${EXOTIC_TARGET_PREFIX}newlib-${PV}-${PR}"
SSTATE_SWSPEC = "sstate:${EXOTIC_TARGET_PREFIX}newlib::${PV}:${PR}::${SSTATE_VERSION}:"

STAMP = "${STAMPS_DIR}/work-shared/${EXOTIC_TARGET_PREFIX}newlib-${PV}-${PR}"
STAMPCLEAN = "${STAMPS_DIR}/work-shared/${EXOTIC_TARGET_PREFIX}newlib-${PV}-*"

INHIBIT_DEFAULT_DEPS = "1"
DEPENDS = ""
PACKAGES = ""

