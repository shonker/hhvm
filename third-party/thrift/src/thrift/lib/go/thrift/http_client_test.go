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
	"net/http"
	"testing"
)

func TestHTTPClient(t *testing.T) {
	l := HTTPClientSetupForTest(t)
	if l != nil {
		defer l.Close()
	}
	trans, err := newHTTPPostClient("http://" + l.Addr().String())
	if err != nil {
		l.Close()
		t.Fatalf("Unable to connect to %s: %s", l.Addr().String(), err)
	}
	TransportTest(t, trans, trans)
}

func TestHTTPClientHeaders(t *testing.T) {
	l := HTTPClientSetupForTest(t)
	if l != nil {
		defer l.Close()
	}
	trans, err := newHTTPPostClient("http://" + l.Addr().String())
	if err != nil {
		l.Close()
		t.Fatalf("Unable to connect to %s: %s", l.Addr().String(), err)
	}
	transportHTTPClientTest(t, trans, trans)
}

func TestHTTPCustomClient(t *testing.T) {
	l := HTTPClientSetupForTest(t)
	if l != nil {
		defer l.Close()
	}
	httpClient, err := newHTTPPostClient("http://" + l.Addr().String())
	if err != nil {
		l.Close()
		t.Fatalf("Unable to connect to %s: %s", l.Addr().String(), err)
	}
	httpTransport := &customHTTPTransport{}
	httpClient.client.Transport = httpTransport
	transportHTTPClientTest(t, httpClient, httpClient)

	if !httpTransport.hit {
		t.Fatalf("Custom client was not used")
	}
}

type customHTTPTransport struct {
	hit bool
}

func (c *customHTTPTransport) RoundTrip(req *http.Request) (*http.Response, error) {
	c.hit = true
	return http.DefaultTransport.RoundTrip(req)
}
