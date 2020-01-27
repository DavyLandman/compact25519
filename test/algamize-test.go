package main 

// #include "../dist/compact25519.h"
// #include "../dist/compact25519.c"
import "C"

import (
	"fmt"
	"log"
	"crypto/rand"
	"bytes"
	//"crypto/ed25519"
	"golang.org/x/crypto/curve25519"
)

func bytePointer(ar []byte) *C.uint8_t {
	return (*C.uint8_t)(&ar[0])
}

func x25519FullCompact() {
	fmt.Print("testing compact_X25519: ")
	var seed1, seed2 [C.X25519_KEY_SIZE]byte
	rand.Read(seed1[:])
	rand.Read(seed2[:])


	var sec1, pub1, sec2, pub2 [C.X25519_KEY_SIZE]byte
	C.compact_x25519_keygen(bytePointer(sec1[:]), bytePointer(pub1[:]), bytePointer(seed1[:]))
	C.compact_x25519_keygen(bytePointer(sec2[:]), bytePointer(pub2[:]), bytePointer(seed2[:]))

	var shared1, shared2 [C.X25519_SHARED_SIZE]byte

	C.compact_x25519_shared(bytePointer(shared1[:]), bytePointer(sec1[:]), bytePointer(pub2[:]))
	C.compact_x25519_shared(bytePointer(shared2[:]), bytePointer(sec2[:]), bytePointer(pub1[:]))

	if bytes.Equal(shared1[:], shared2[:]) {
		fmt.Println("OK")
	} else {
		fmt.Println("** FAIL **")
	}
}

func x25519SharedKey(theirPublic, ourPrivate []byte) []byte {
	result, err := curve25519.X25519(ourPrivate, theirPublic)
	if err != nil {
		log.Fatalf("Failure to calculate common secret: %v", err)
	}
	return result
}

func x25519CalcPublic(privateKey []byte) []byte {
	result, err := curve25519.X25519(privateKey, curve25519.Basepoint)
	if err != nil {
		log.Fatalf("Unexpected basepoint failure: %v", err)
	}
	return result
}

func x25519Interopt() {
	fmt.Print("testing X25519 interop: ")
	var seed1 [C.X25519_KEY_SIZE]byte
	rand.Read(seed1[:])

	var sec1, pub1 [C.X25519_KEY_SIZE]byte
	C.compact_x25519_keygen(bytePointer(sec1[:]), bytePointer(pub1[:]), bytePointer(seed1[:]))

	sec2 := make([]byte, 32)
	rand.Read(sec2)
	pub2 := x25519CalcPublic(sec2)

	var shared1 [C.X25519_SHARED_SIZE]byte
	C.compact_x25519_shared(bytePointer(shared1[:]), bytePointer(sec1[:]), bytePointer(pub2))

	shared2 := x25519SharedKey(pub1[:], sec2)
	if bytes.Equal(shared1[:], shared2) {
		fmt.Println("OK")
	} else {
		fmt.Println("** FAIL **")
	}
}

func main() {
	x25519FullCompact()
	x25519Interopt()
}