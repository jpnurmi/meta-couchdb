DESCRIPTION = "A document-oriented NoSQL database"
HOMEPAGE = "https://couchdb.apache.org/"
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://LICENSE;md5=1d6953f3b9af3e202ed075fc3019b801"

SRC_URI = "https://downloads.apache.org/couchdb/source/${PV}/apache-couchdb-${PV}.tar.gz"

inherit autotools-brokensep

# couchdb supports: 1.8.5, 60, 68
SPIDERMONKEY_VERSION = "60"

CONFIGUREOPTS = "--with-curl --spidermonkey-version ${SPIDERMONKEY_VERSION} --disable-docs"
EXTRA_OECONF_remove = "--disable-static"

S = "${WORKDIR}/apache-couchdb-${PV}"

do_configure_append() {
    sed -i "s:-I/usr/include/mozjs-${SPIDERMONKEY_VERSION}:-I${STAGING_INCDIR}/mozjs-${SPIDERMONKEY_VERSION}:g" ${B}/src/couch/rebar.config.script
    sed -i "s:-L/usr/local/lib:-L${STAGING_LIBDIR}:g" ${B}/src/couch/rebar.config.script
}

DEPENDS += " \
    curl \
    erlang-native \
    icu \
    mozjs \
"
