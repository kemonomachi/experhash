ExPerHash
=========

Perceptual hashing for images. Implemented as a C++ program connected to 
Elixir via a port.


## Installation

Requires [Magick++](http://www.imagemagick.org/Magick++/), the ImageMagick
C++ API. Check your package manager or the [ImageMagick downloads page](http://www.imagemagick.org/script/binary-releases.php).

For use in a mix project, add ExPerHash to the `mix.exs` dependencies:

```elixir
def deps do
  [{:experhash, github: "kemonomachi/experhash"}]
end
```

Download by using:

```sh-session
$ mix deps.get
```

Run `mix compile` to build both the Elixir and the C++ code.


## Usage

```elixir
iex> {:ok, srv} = ExPerHash.start_link
{:ok, #PID<0.91.0>}

iex> {:ok, hash1} = ExPerHash.dd_hash_128 srv, "some/image.png"
{:ok, <<140, 173, 167, 143, 157, 141, 14, 39, 77, 9, 3, 135, 23, 49, 25, 89>>}

iex> {:ok, hash2} = ExPerHash.dd_hash_128 srv, "some/other/image.png"
{:ok, <<140, 173, 167, 143, 157, 141, 14, 39, 77, 9, 0, 135, 23, 49, 25, 89>>}

iex> ExPerHash.hamming_distance srv, hash1, hash2
{:ok, 2}
```

Three hash functions are available: aHash, dHash and ddHash.

[aHash](http://www.hackerfactor.com/blog/index.php?/archives/432-Looks-Like-It.html)
by dr. Neal Krawitz creates a hash based on the average of the low frequencies
of an image.

[dHash](http://www.hackerfactor.com/blog/index.php?/archives/529-Kind-of-Like-That.html)
by dr. Neal Krawitz and [David Oftedal](http://01101001.net/programming.php)
tracks gradients instead. Hash bits are set in order from left to right
(most significant to least significant).

ddHash is a double dHash, one row-wise and one column-wise.

All hash functions return `{:ok, hash}` on success, where hash is a binary with
four possible sizes. For aHash and dHash, the sizes are 64, 256, 1024, and 4096
bits. For ddHash, the sizes are 128, 512, 2048, and 8192 bits. Each function
name ends with the hash size, like `a_hash_64` and `dd_hash_128`.

On error, all functions return `{:error, {error_type, reason}`.

The `hamming_distance/3` function compares two hashes and return the number of
bits that differ. Low distance means similar images.


## License

Copyright © 2015-2018 Ookami Kenrou \<ookamikenrou@gmail.com\>

This work is free. You can redistribute it and/or modify it under the terms of
the Do What The Fuck You Want To Public License, Version 2, as published by
Sam Hocevar. See the LICENSE file or the [WTFPL homepage](http://www.wtfpl.net)
for more details.

