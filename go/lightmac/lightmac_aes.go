package lightmac

import (
    "crypto/aes"
    "crypto/sha256"
)

func NewAES(ctrLength, tagLength int, key []byte) (*LightMAC, error) {
    k1, err := deriveKey(16, key, "k1")
    if err != nil {
        return nil, err
    }

    k2, err := deriveKey(16, key, "k2")
    if err != nil {
        return nil, err
    }

    return NewAESWithTwoKeys(ctrLength, tagLength, k1, k2)
}

func NewAESWithTwoKeys(ctrLength, tagLength int, k1, k2 []byte) (*LightMAC, error) {
    if ctrLength > 4 {
        return nil, CounterLengthError(ctrLength)
    }

    if tagLength > aes.BlockSize {
        return nil, TagLengthError(tagLength)
    }

    b1, err := aes.NewCipher(k1)
    if err != nil {
        return nil, err
    }

    b2, err := aes.NewCipher(k2)
    if err != nil {
        return nil, err
    }

    lm := LightMAC{ctrLength, tagLength, b1, b2}
    return &lm, nil
}

func deriveKey(keyLength int, key []byte, identifier string) ([]byte, error) {
    if keyLength > sha256.Size {
        return nil, KeyLengthError(keyLength)
    }

    derivedKey := sha256.Sum256(append(key, []byte(identifier)...))
    return derivedKey[0:keyLength], nil
}
