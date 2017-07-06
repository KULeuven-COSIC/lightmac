package lightmac

import (
    "crypto/cipher"
    "crypto/subtle"
    "fmt"
)

// Go does not support const maps
var ctrSize = map[int]uint32 {
    1: 255,
    2: 65535,
    3: 16777215,
    4: 4294967295,
}

type CounterLengthError int
func (e CounterLengthError) Error() string {
    return fmt.Sprintf("lightmac: invalid counter length %d", e)
}
type KeyLengthError int
func (e KeyLengthError) Error() string {
    return fmt.Sprintf("lightmac: invalid key length %d", e)
}
type TagLengthError int
func (e TagLengthError) Error() string {
    return fmt.Sprintf("lightmac: invalid tag length %d", e)
}

type LightMAC struct {
    ctrLength int
    tagLength int
    block1    cipher.Block
    block2    cipher.Block
}
func (lm LightMAC) Tag(message []byte) ([]byte, error) {
    if uint32(len(message)) > ctrSize[lm.ctrLength] {
        return nil, CounterLengthError(lm.ctrLength)
    }

    r := lm.block1.BlockSize() - lm.ctrLength
    if (len(message) - lm.block1.BlockSize()) % r != 0 {
        message = pad(lm.block1.BlockSize(), r, message)
    }

    tag := make([]byte, lm.block1.BlockSize())
    ctr := 1

    remaining := len(message)
    for remaining > lm.block1.BlockSize() {
        ctrBytes := [4]byte{
            uint8(ctr >> 24),
            uint8(ctr >> 16),
            uint8(ctr >> 8),
            uint8(ctr),
        }

        src := append(ctrBytes[4-lm.ctrLength:4], message[(ctr-1)*r:ctr*r]...)
        dst := make([]byte, lm.block1.BlockSize())
        lm.block1.Encrypt(dst, src)

        for i := 0; i < lm.block1.BlockSize(); i++ {
            tag[i] = tag[i] ^ dst[i]
        }

        ctr = ctr + 1
        remaining = remaining - r
    }

    lastBlock := message[(ctr-1)*r:]
    for i := 0; i < lm.block1.BlockSize(); i++ {
        tag[i] = tag[i] ^ lastBlock[i]
    }

    lm.block2.Encrypt(tag, tag)

    return tag[0:lm.tagLength], nil
}
func (lm LightMAC) Verify(tag, message []byte) (bool, error) {
    vt, err := lm.Tag(message)
    if err != nil {
        return false, err
    }

    return len(vt) == len(tag) && subtle.ConstantTimeCompare(vt, tag) == 1, nil
}

func pad(n, r int, message []byte) []byte {
    var padLength int
    if (len(message) < n) {
        padLength = n - len(message)
    } else {
        padLength = r - (len(message) - n) % r
    }

    pad := make([]byte, padLength)
    pad[0] = 0x80

    return append(message, pad...)
}
