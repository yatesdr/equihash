# mining
The starting of work on a basic equihash solver, largely incomplete and mostly for reference.

Progress:
So far, the Working Reference folder contains the fastest compiling blake2b implementation I've been able to cobble together.   It hashes about 1m hashes in 0.35 seconds, and splits and sorts them in about 0.1 seconds into an array (I7-6700k / single thread).   It's hacked together from the blake2b.net code, most of the focus has been on removing the unused parts and consolidating the source into working .c and .h files.  Memory management is a mess right now and needs to be cleaned up.  The current speed is way too slow to be useful for much of anything, but it will improve with time.   The goal of this project is to create a simple program that can be a framework for future optimizations, that produces valid solutions for Equihash PoW algorithms.

The blake2 code was adapted from the reference blake2b files from blake2.net, and oddly runs faster than the "optimized" version in their implementation.   I attribute that to gcc out-optimizing the hand coded instructions (?) but am not really sure why to be honest.   The SSE3 folder has the (compiling) optimized code from blake2.net adapted, which has many SSE and other instructions included, but appears slightly slower than gcc -O2 does on the non-SSE specific code.   Neither one has been tested with any test vectors as of yet, and they could be producing garbage output instead of proper hashes due to byte order and initialization vectors.

Goals:
1) Produce a working single-threaded miner with a small code footprint and document it well.
2) Adapt to a multi-threaded miner
3) Adapt to CUDA-capable miner

Next steps:
1) Confirm the blake2 is really working properly with a set of test vectors for equihash (will use Zcash solutions).
2) Finish the Equihash solver portion of the code and general clean-up.
3) Validate each step against a known solution to verify byte and bit ordering all around.
5) Once it's working, start profiling and optimizing for CPU solving.
6) Move to CUDA and decide how to split the work up for parallelism.

Anyway, the blake2 compiles and seems to work, but needs to be verified that it is in fact working as intended and creating the proper hashes for Zcash.

This folder is a personal working folder, and isn't really intended for sharing as it offers nothing of substance over any of the working reference miners.  It's more of a proof of concept and understanding of the method than anything of substance at the moment.  Over the years I hope to continue the work and multi-thread it, and make it cuda capable with the goal of running it on a rig full of GTX1070's.   It may never happen, so don't hold your breath.

The code here is not released under any license or free for general use, and copyright is retained by the author.  The code from Blake2.net is considered CCO Licensed, and does not require release when used in whole or in part in a new implementation.  It is being adapted as necessary and with time if a working solver is created, a licensed release will be considered.
