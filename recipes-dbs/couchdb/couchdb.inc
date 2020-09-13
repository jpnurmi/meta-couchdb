DESCRIPTION = "A document-oriented NoSQL database"
HOMEPAGE = "https://couchdb.apache.org/"
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://LICENSE;md5=1d6953f3b9af3e202ed075fc3019b801"

SRC_URI = " \
    https://downloads.apache.org/couchdb/source/${PV}/apache-couchdb-${PV}.tar.gz \
    file://couchdb.service \
    file://default.ini \
    file://local.ini \
    file://vm.args \
"

inherit autotools-brokensep erlang systemd

# couchdb supports: 1.8.5, 60, 68
SPIDERMONKEY_VERSION = "60"

CONFIGUREOPTS = "--with-curl --spidermonkey-version ${SPIDERMONKEY_VERSION} --disable-docs"
EXTRA_OECONF_remove = "--disable-static"

S = "${WORKDIR}/apache-couchdb-${PV}"

SYSTEMD_SERVICE_${PN} = "couchdb.service"

do_configure_append() {
    sed -i "s:-I/usr/include/mozjs-${SPIDERMONKEY_VERSION}:-I${STAGING_INCDIR}/mozjs-${SPIDERMONKEY_VERSION}:g" ${B}/src/couch/rebar.config.script
    sed -i "s:-L/usr/local/lib:-L${STAGING_LIBDIR}:g" ${B}/src/couch/rebar.config.script
}

do_install_append() {
    install -d ${D}${systemd_system_unitdir}
    install -m 0644 ${WORKDIR}/couchdb.service ${D}${systemd_system_unitdir}

    install -d ${D}/opt/couchdb
    cp -r ${B}/rel/couchdb/bin ${D}/opt/couchdb
    cp -r ${B}/rel/couchdb/etc ${D}/opt/couchdb

    install -d ${D}/opt/couchdb/etc
    install -d ${D}/opt/couchdb/etc/default.d
    install -d ${D}/opt/couchdb/etc/local.d
    install -m 0644 ${WORKDIR}/default.ini ${D}/opt/couchdb/etc
    install -m 0644 ${WORKDIR}/local.ini ${D}/opt/couchdb/etc
    install -m 0644 ${WORKDIR}/vm.args ${D}/opt/couchdb/etc

    install -d ${D}/var/lib/couchdb
    install -d ${D}/var/volatile/log/couchdb
}

FILES_${PN} = " \
    ${systemd_system_unitdir} \
    /opt/couchdb \
    /var/lib/couchdb \
    /var/volatile/log/couchdb \
"

DEPENDS += " \
    curl \
    erlang \
    erlang-native \
    icu \
    mozjs \
    rebar-native \
"

INSANE_SKIP_${PN} = "ldflags"
