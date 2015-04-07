Chorusmaster is a test server which will be kept compliant with v0.* releases of
the Chant chat protocol.

Quick Start
--------------------
$ make
$ ./server <port>

About Chant
--------------------
Aspirations:
Chant is an experimental chat protocol for secure, connectionless communication.
Wherever possible, Chant assumes that its clients are bad people dead set on
cheating, confusing, compromising and annhilating the server they are
communicating with. A compliant Chant server should be relatively secure by
virtue of its compliance alone.

Reality:
Chant is quite immature. Presently the protocol does provide connectionless
communication, but enforces zero security.

About Chorusmaster:
--------------------
Some of Chorusmaster's server logic is quite hacky and its main protocol is a
work in progress. Much of the backend network infrastructure is quite robust
(servertools, wireio, authenticate), however, and allows for reliable message
packing, unpacking, sending and receiving. It's a rickety house on a solid
foundation.

Chant was previously called Candlechat, and that legacy remains in the source.
