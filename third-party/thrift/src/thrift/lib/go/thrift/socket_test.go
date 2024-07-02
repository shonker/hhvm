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
	"net"
	"runtime"
	"testing"
)

func TestNewSocket(t *testing.T) {
	if runtime.GOOS == "windows" {
		t.Skip("skipping on windows")
	}
	address := "localhost6:1234"
	if runtime.GOOS == "darwin" {
		address = "localhost:1234"
	}
	addr, err := resolveAddr(address)
	if err != nil {
		t.Error(err)
	}
	if addr.String() != "[::1]:1234" {
		t.Errorf("wrong address: %s", addr)
	}
}

func TestNewSocketUnix(t *testing.T) {
	path := t.TempDir() + "/test.sock"

	l, err := net.Listen("unix", path)
	if err != nil {
		t.Fatalf("Cannot listen on unix socket %q: %v", path, err)
	}
	defer l.Close()

	conn, err := net.Dial("unix", path)
	if err != nil {
		t.Fatalf("Cannot dial unix socket %q: %v", path, err)
	}

	s, err := NewSocket(SocketConn(conn))
	if err != nil {
		t.Fatalf("Cannot create new socket: %v", err)
	}
	defer s.Close()
}
