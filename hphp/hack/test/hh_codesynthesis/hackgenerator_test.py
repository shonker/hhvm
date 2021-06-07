#!/usr/bin/env python3
# pyre-strict
# Copyright (c) Facebook, Inc. and its affiliates.
#
# This source code is licensed under the MIT license found in the
# LICENSE file in the "hack" directory of this source tree.

import unittest

from hphp.hack.src.hh_codesynthesis.hackGenerator import (
    _HackInterfaceGenerator,
    _HackClassGenerator,
    HackCodeGenerator,
)


class _HackInterfaceGeneratorTest(unittest.TestCase):
    def setUp(self) -> None:
        self.obj = _HackInterfaceGenerator("I0")

    def test_single_interface(self) -> None:
        self.assertEqual("interface I0  {}", str(self.obj))

    def test_multiple_extends_interface(self) -> None:
        self.obj.add_extend("I1")
        self.assertEqual("interface I0 extends I1 {}", str(self.obj))
        self.obj.add_extend("I2")
        self.assertEqual("interface I0 extends I1,I2 {}", str(self.obj))


class _HackClassGeneratorTest(unittest.TestCase):
    def setUp(self) -> None:
        self.obj = _HackClassGenerator("C0")

    def test_single_class(self) -> None:
        self.assertEqual("class C0   {}", str(self.obj))

    def test_multiple_implements_interface(self) -> None:
        self.obj.add_implement("I1")
        self.assertEqual("class C0  implements I1 {}", str(self.obj))
        self.obj.add_implement("I2")
        self.assertEqual("class C0  implements I1,I2 {}", str(self.obj))

    def test_single_extend_class_multiple_implements_interface(self) -> None:
        self.obj.add_implement("I1")
        self.assertEqual("class C0  implements I1 {}", str(self.obj))
        self.obj.set_extend("C1")
        self.assertEqual("class C0 extends C1 implements I1 {}", str(self.obj))
        self.obj.add_implement("I2")
        self.assertEqual("class C0 extends C1 implements I1,I2 {}", str(self.obj))
        # invoke set_extend again, will overwrite the previous "C1"
        self.obj.set_extend("C2")
        self.assertEqual("class C0 extends C2 implements I1,I2 {}", str(self.obj))


class HackCodeGeneratorTest(unittest.TestCase):
    def setUp(self) -> None:
        self.obj = HackCodeGenerator()

    def test_single_class(self) -> None:
        exp = """\
<?hh
class C0   {}

"""
        self.obj._add_class("C0")
        self.assertEqual(exp, str(self.obj))

    def test_single_interface(self) -> None:
        exp = """\
<?hh

interface I0  {}
"""
        self.obj._add_interface("I0")
        self.assertEqual(exp, str(self.obj))

    def test_mix_single_class_and_interface(self) -> None:
        exp = """\
<?hh
class C0   {}
interface I0  {}
"""
        self.obj._add_class("C0")
        self.obj._add_interface("I0")
        self.assertEqual(exp, str(self.obj))

    def test_class_extends_class_implements_interface(self) -> None:
        exp = """\
<?hh
class C0   {}
class C1 extends C0 implements I1,I2 {}
interface I0  {}
interface I1 extends I0 {}
interface I2  {}
"""
        self.obj._add_class("C0")
        self.obj._add_class("C1")
        self.obj._add_interface("I0")
        self.obj._add_interface("I1")
        self.obj._add_interface("I2")
        self.obj._add_extend("C1", "C0")
        self.obj._add_extend("I1", "I0")
        self.obj._add_implement("C1", "I1")
        self.obj._add_implement("C1", "I2")
        self.assertEqual(exp, str(self.obj))
