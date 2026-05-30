# AGENTS.md

## Project purpose

C utility for IPv4/IPv6 address and prefix validation in shell scripts. Used by VyOS as the canonical "is this string a valid IP / network / host / CIDR?" checker invoked from XML validators and conf-mode scripts. Fast and exit-status-driven (intended for `if ipaddrcheck --is-ipv4...; then...`).

## Tech stack

- C, autotools (`configure.ac`, `Makefile.am`, autoconf 2.x).
- Dependencies: `libcidr` (parsing), `libpcre2-8` (regex), `check >= 0.9.4` (test framework).
- Debian packaging under `debian/`. Upstream autotools version `1.1` (`configure.ac`); current Debian package version `1.4` (`debian/changelog`).

## Build / test / run

```
autoreconf -fi          # generate configure
./configure
make
make check              # uses libcheck unit tests under tests/
make install            # installs into AC_PREFIX_DEFAULT (/usr)
# Debian package
dpkg-buildpackage -uc -us -tc -b
```

## Repository layout

- `src/` — C sources (`ipaddrcheck.c`, headers, autotools `Makefile.am`).
- `tests/` — libcheck C unit tests (`check_ipaddrcheck.c`) and shell integration tests (`assert.sh`, `integration_tests.sh`).
- `man/` — manpages.
- `debian/` — Debian packaging.
- `configure.ac`, `Makefile.am`, `INSTALL`, `NEWS`, `ChangeLog`, `AUTHORS`, `COPYING` (GPL-2), `COPYING-LGPL` (LGPL-2.1).

## Cross-repo context

One of the canonical 14 VyOS-image build packages, listed in an internal repository. Built into VyOS images by `vyos-build` and invoked at runtime by `vyos-1x` validators and conf-mode scripts. Sits in the §3.5 native-libraries category alongside `hvinfo`, `udp-broadcast-relay`, etc.

## Conventions

- Commit / PR title format: `component: T12345: description` (Phorge task ID at https://vyos.dev mandatory). Enforced by `vyos/.github` reusable workflows where consumed.
- Branch model: `rolling`, `circinus` (1.5 LTS), `sagitta` (1.4 LTS), `equuleus` (1.3 LTS).
- Maintained by the VyOS team (`maintainers@vyos.net`); license is GPL-2 + LGPL-2.1 dual.

## Notes for future contributors

- Used as a CLI-level validator — exit status 0/1 is the contract. Don't break stdout/stderr conventions or add chatty output.
- `libcidr` is by Matthew Fuller (http://www.over-yonder.net/~fullermd/projects/libcidr) — Debian-packaged.
- Adding a new flag: extend `src/ipaddrcheck.c`'s argument table and add a libcheck test under `tests/`.
