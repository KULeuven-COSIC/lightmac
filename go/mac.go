package mac

import (
    "encoding/hex"
    "fmt"
)

type VerificationError struct {
    Message []byte
    Tag     []byte
}
func (e VerificationError) Error() string {
    return fmt.Sprintf(
        "mac: verification failed ({message: %s, tag: %s})",
        hex.EncodeToString(e.Message),
        hex.EncodeToString(e.Tag),
    )
}

type Algorithm interface {
    Tag(message []byte) ([]byte, error)
    Verify(tag, message []byte) (bool, error)
}
