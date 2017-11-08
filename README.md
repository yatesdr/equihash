# mining
The starting of work on a basic equihash solver, largely incomplete and mostly for reference.

Progress:
So far, the Working Reference folder contains the fastest compiling blake2b implementation I've been able to cobble together.   It hashes about 1m hashes in 0.35 seconds, and sorts them in about 0.1 seconds into an array.   It's hacked together, and memory management is a mess right now and needs to be cleaned up since I was mostly focused on getting it working and to compile so far.

The blake2 code was adapted from the reference blake2b files from blake2.net, and oddly runs faster than the "optimized" version in their system with all the SSE stuff.   I attribute that to gcc out-optimizing the hand coded instructions (?) but am not really sure why to be honest.   The SSE3 folder has the (compiling) optimized code from blake2.net adapted for this.   Neither one has been tested with any test vectors as of yet, and they could be producing garbage output instead of proper hashes.

Anyway, the blake2 compiles and seems to work, but needs to be verified that it is in fact working as intended and creating the proper hashes for Zcash.

This folder is a personal working folder, and isn't really intended for sharing as it offers nothing of substance over any reference miner.  Over the years I hope to continue the work and multi-thread it, and make it cuda capable with the goal of running it on a rig full of GTX1070's.   It may never happen, but it's a learning tool so don't hold your breath.
