# Me Malloc
I mostly followed steps in [this video](https://www.youtube.com/watch?v=sZ8GJ1TiMdk&t=1489s&ab_channel=TsodingDaily) by tsoding. I think I'll just go on my own to expand on his tutorial by having a dynamically sized heap ;3.

## Description
Basically, it just takes chunks out of a byte array and keeps track of chunks in free/alloced lists. Whenever heap_alloc is called, it merges conitgious free chunks together and tries to reuse a free chunk of required size.
