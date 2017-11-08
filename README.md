# mining
The starting of work on a basic equihash solver, largely incomplete and mostly for reference.

Progress:
So far, the Working Reference folder contains the fastest compiling blake2b implementation I've been able to cobble together.   It hashes about 1m hashes in 0.35 seconds, and splits and sorts them in about 0.1 seconds into an array.   It's hacked together from the blake2b.net code, most of the focus has been on removing the unnecessary parts and consolidating the source into a working .c and .h file.  Memory management is a mess right now and needs to be cleaned up since I was mostly focused on getting it working and to compile so far.  The current speed is way too slow to be useful for much of anything, but it may come out to about 1 solution / second which would be a great accomplishment for me.

The blake2 code was adapted from the reference blake2b files from blake2.net, and oddly runs faster than the "optimized" version in their system with all the SSE stuff.   I attribute that to gcc out-optimizing the hand coded instructions (?) but am not really sure why to be honest.   The SSE3 folder has the (compiling) optimized code from blake2.net adapted for this.   Neither one has been tested with any test vectors as of yet, and they could be producing garbage output instead of proper hashes due to byte order and initialization vectors.

Next steps:
1) Confirm the blake2 is really working properly with known test vectors for equihash.
2) Refactor the blake2 to a parallel implementation for cpu
2a) Add thread support for this code all around
3) Finish the actual Equihash solver portion of the code
4) Generate a set of valid Equihash test data, and find a known solution to verify byte orders and such.
5) Once it's working, start profiling and optimizing for CPU
6) Move to CUDA and decide how to split the work up for parallelism.

Anyway, the blake2 compiles and seems to work, but needs to be verified that it is in fact working as intended and creating the proper hashes for Zcash.

This folder is a personal working folder, and isn't really intended for sharing as it offers nothing of substance over any of the working reference miners.  It's more of a proof of concept and understanding of the method than anything of substance at the moment.  Over the years I hope to continue the work and multi-thread it, and make it cuda capable with the goal of running it on a rig full of GTX1070's.   It may never happen, so don't hold your breath.
