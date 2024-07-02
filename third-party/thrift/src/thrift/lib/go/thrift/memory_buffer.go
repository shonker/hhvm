/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package thrift

import (
	"bytes"
)

// Memory buffer-based implementation of the Transport interface.
type MemoryBuffer struct {
	*bytes.Buffer
	size int
}

func NewMemoryBuffer() *MemoryBuffer {
	return &MemoryBuffer{Buffer: &bytes.Buffer{}, size: 0}
}

func NewMemoryBufferWithData(data []byte) *MemoryBuffer {
	return &MemoryBuffer{Buffer: bytes.NewBuffer(data), size: len(data)}
}

func NewMemoryBufferLen(size int) *MemoryBuffer {
	buf := make([]byte, 0, size)
	return &MemoryBuffer{Buffer: bytes.NewBuffer(buf), size: size}
}

func (p *MemoryBuffer) Close() error {
	p.Buffer.Reset()
	return nil
}

// Flushing a memory buffer is a no-op
func (p *MemoryBuffer) Flush() error {
	return nil
}

func (p *MemoryBuffer) RemainingBytes() uint64 {
	return uint64(p.Buffer.Len())
}
