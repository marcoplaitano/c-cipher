# C-CIPHER

A program to encrypt/decrypt a file using the **AES-CTR-256** cipher.

It can be used to store passwords (or any other kind of sensible information) 
locally, in a private way.  
The user only needs one password to both encrypt and decrypt the information.

- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

## Details on Security

The algorithm is implemented in C, using the [OpenSSL]'s Encryption [API].

The **Encryption Key** has to be 256-bits long; it is generated by computing
`SHA256(p)`, with `p` being the user's password.<sup>[1](#f1)</sup>  
The same key is used for both encryption and decryption.

The _Initialization Vector IV_ is a string of `16` bytes.  
The last (**LSB**)<sup>[2](#f2)</sup> `8` bytes contain a counter which will be
incremented at every new encrypted block.  
The first (**MSB**) `8` bytes contain the "_nonce_": a random sequence of bits.  
When encrypting, the _nonce_ is filled with random bytes; these will be
prepended to the ciphertext in order to read them during the decryption
procedure.

The _block length n_, is an important parameter for determining the concrete
security of the cipher.  
Being the **IV** a uniform string of _n-bits_, it is expected to repeat after
having performed "only" `2`<sup>`n/2`</sup> encryptions.<sup>[3](#f3)</sup>  
In our case `n` is `128` bits; this means that only `2`<sup>`64`</sup> blocks -
or `2*10`<sup>`5`</sup> **Petabytes** - can be safely encrypted without ever
changing the `IV`.  
This is more than safe for a regular text file.

### Tampering

It must be noted that this mode of encryption provides no security against data
tampering.  
If the ciphertext were to be modified even by just one bit, the *integrity* of
the information would be lost.

The scope of this project however is to provide a trivial example of encryption,
to be used for simpler "protection" against *adversaries* who know little to
nothing about cryptography.

<br>

<sup id="f1">**1**</sup> The usage of a **PBKDF** (Password-Based Key
Derivation Function) would further improve security.  
<sup id="f2">**2**</sup> In **big-endian** order.  
<sup id="f3">**3**</sup> Similar concept to '[The Birthday Problem]'.

- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

## Installation

Take a look at the [dependencies] listed below.

To download and compile the program:

```sh
git clone https://github.com/marcoplaitano/c-cipher
cd c-cipher
make
```

The executable file is `ccipher`.  
To install it globally:

```sh
make install
```

_**Note:** this will simply copy the executable in `~/.local/bin`. Make sure the
directory exists and is in `$PATH`._

- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

## Usage

```sh
ccipher [in_file] [out_file] [mode] [password]
```

The program needs the following 4 command line arguments, in order:

+ `in_file`  
    Input file on which the cipher will be applied
+ `out_file`  
    Output file storing the result
+ `mode`  
    A string: either **encrypt**, **e**, **decrypt** or **d**
+ `password`  
    A string chosen by the user

_**Note:** using this approach means that anyone with access to the computer
can learn the password since it is stored in the shell's history.
A workaround might be requesting the 4 arguments **inside** the program, at
runtime._

### Example

The first time you run the program, the `in_file` should be the plain text file
containing the private information.  
The output file will store the encrypted version of it.  
It is at this point that you must choose the **password** to use. It can contain
any ASCII character and can be of any (reasonable) length.

```sh
ccipher input.txt private.dat e Password123
```

The following times, the plaintext version can be recreated with the command:

```sh
ccipher private.dat output.txt d Password123
```

Notice how the two paths have been swapped, the mode is now **d**ecrypt and the
password remains the same.

- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

## Dependencies

+ gcc
+ make
+ libssl3

- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

## Author

Marco Plaitano

- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

## License

Distributed under the [MIT] license.


<!-- Links -->

[OpenSSL]:
https://www.openssl.org/
"Main Website"

[API]:
https://www.openssl.org/docs/man3.0/man3/EVP_aes_256_ctr.html
"Online Documentation"

[The Birthday Problem]:
https://en.wikipedia.org/wiki/Birthday_problem
"Wikipedia Article"

[dependencies]:
#dependencies
"Anchor to header"

[MIT]:
LICENSE
"Repository file"
