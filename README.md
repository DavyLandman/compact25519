# compact25519: A compact portable X25519 + Ed25519 implementation

This library is a compact and portable (c99) implementation of two popular
operations on the Curve25519: 

    - X25519: Diffie-Hellman Key Exchange 
    - Ed25519: EdDSA signature system 

There are quite some advantages over older RSA based algorithms:

    - compact key size (32 bytes for X25519 and 64bytes for Ed25519)
    - Less opportunities to introduce side-channels
    - Faster than other popular alternatives at the same 128bit security level

compact25519 targets smaller embedded devices, binary size is modest and all
operations are on a byte level, no assumption of fast int32/int64 operations.

Since there is no single C package management system, compact25519 is released as
a single pair of c & h file. Deployment options:

    - Download [release from Github](https://github.com/DavyLandman/compact25519/releases) 
        and copy into your project directory
    - Use PlatformIO and take a dependency on compact25519

## License and origins
The implementation of the X25519 and Ed25519 is extracted from 
[Daniel Beer's c25519](https://www.dlbeer.co.nz/oss/c25519.html) version 2017-10-05.


## Smaller binaries

By default the library contains both X25519 and Ed25519. They share quite some 
code. compact25519 has several defines so that you can disable either one of the
operations to reduce binary size.

| Define Flag | Description |
|----|----|
| `COMPACT_DISABLE_ED25519` | Disable Ed25519 feature and the related code, shaves of 32% |
| `COMPACT_DISABLE_X25519` | Disable X25519 feature and the related code, shaves of 25% |
| `COMPACT_DISABLE_X25519_DERIVE` | Disable custom derive secret feature, 
    extra useful combined with `COMPACT_DISABLE_ED25519`, in itself only saves 3%, 
    combined with disabling Ed25519: 45%|