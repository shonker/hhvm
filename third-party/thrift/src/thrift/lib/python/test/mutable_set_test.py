# Copyright (c) Meta Platforms, Inc. and affiliates.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# pyre-strict

import collections.abc
import unittest

from thrift.python.mutable_containers import MutableSet

from thrift.python.types import typeinfo_i32


class MutableSetTest(unittest.TestCase):
    """
    Some of the tests use a Python `set` for verification. They create a
    `MutableSet` and a Python `set`, apply the same operations to both of
    them, and check if they are equal.
    """

    def test_smoke(self) -> None:
        mutable_set = MutableSet(typeinfo_i32, set())
        self.assertIsInstance(mutable_set, MutableSet)
        self.assertIsInstance(mutable_set, collections.abc.MutableSet)
        self.assertEqual(0, len(mutable_set))
        self.assertFalse(mutable_set)

    def test_init(self) -> None:
        with self.assertRaisesRegex(
            TypeError, r"incorrect type \(expected set, got list\)"
        ):
            # pyre-ignore[6]: Incompatible parameter type
            MutableSet(typeinfo_i32, [])

        with self.assertRaisesRegex(
            TypeError, r"incorrect type \(expected set, got dict\)"
        ):
            # pyre-ignore[6]: Incompatible parameter type
            MutableSet(typeinfo_i32, {})

        with self.assertRaisesRegex(
            TypeError, r"incorrect type \(expected set, got frozenset\)"
        ):
            # pyre-ignore[6]: Incompatible parameter type
            MutableSet(typeinfo_i32, frozenset())

    def test_contains(self) -> None:
        mutable_set = MutableSet._from_iterable(typeinfo_i32, set(), range(10))

        for i in range(10):
            self.assertIn(i, mutable_set)

        self.assertNotIn(10, mutable_set)

    def test_contains_wrong_type(self) -> None:
        mutable_set = MutableSet._from_iterable(typeinfo_i32, set(), range(10))
        with self.assertRaisesRegex(
            TypeError, "is not a <class 'int'>, is actually of type <class 'str'>"
        ):
            if "Not an interger" in mutable_set:
                pass

    def test_contains_i32_overflow(self) -> None:
        mutable_set = MutableSet._from_iterable(typeinfo_i32, set(), range(10))
        with self.assertRaises(OverflowError):
            if (2**31) in mutable_set:
                pass

    def test_iter(self) -> None:
        mutable_set = MutableSet._from_iterable(typeinfo_i32, set(), range(10))
        python_set = set(range(10))

        for i in mutable_set:
            # `remove()` throws `KeyError` if key is absent
            python_set.remove(i)

        self.assertEqual(0, len(python_set))

    def test_iter_next(self) -> None:
        mutable_set = MutableSet._from_iterable(typeinfo_i32, set(), range(10))
        iter1 = iter(mutable_set)
        iter2 = iter(mutable_set)

        for _ in range(10):
            next(iter1)

        with self.assertRaises(StopIteration):
            next(iter1)

        for _ in range(10):
            next(iter2)

        with self.assertRaises(StopIteration):
            next(iter2)

    def test_isdisjoint(self) -> None:
        mutable_set_1 = MutableSet._from_iterable(typeinfo_i32, set(), range(10))
        mutable_set_2 = MutableSet._from_iterable(typeinfo_i32, set(), range(1))
        self.assertFalse(mutable_set_1.isdisjoint(mutable_set_2))

        mutable_set_3 = MutableSet._from_iterable(typeinfo_i32, set(), range(10, 20))
        self.assertTrue(mutable_set_1.isdisjoint(mutable_set_3))

        self.assertFalse(mutable_set_1.isdisjoint(range(3)))
        self.assertTrue(mutable_set_1.isdisjoint(range(10, 11)))
        self.assertTrue(mutable_set_1.isdisjoint([10]))
        self.assertTrue(mutable_set_1.isdisjoint(frozenset([10])))

    def test_eq(self) -> None:
        mutable_set_1 = MutableSet._from_iterable(typeinfo_i32, set(), [1, 2])
        mutable_set_2 = MutableSet._from_iterable(typeinfo_i32, set(), [1, 2, 3])

        self.assertEqual({1, 2}, mutable_set_1)
        self.assertEqual(mutable_set_1, {1, 2})
        self.assertNotEqual({1, 2, 3}, mutable_set_1)
        self.assertNotEqual(mutable_set_1, {1, 2, 3})

        self.assertEqual({1, 2, 3}, mutable_set_2)
        self.assertEqual(mutable_set_2, {1, 2, 3})
        self.assertNotEqual({1, 2}, mutable_set_2)
        self.assertNotEqual(mutable_set_2, {1, 2})

        self.assertNotEqual(mutable_set_1, mutable_set_2)
        self.assertNotEqual(mutable_set_2, mutable_set_1)

    def test_and(self) -> None:
        mutable_set_1 = MutableSet._from_iterable(typeinfo_i32, set(), range(4))
        mutable_set_2 = MutableSet._from_iterable(typeinfo_i32, set(), range(2, 6))

        result_set = mutable_set_1 & mutable_set_2

        self.assertIsInstance(result_set, MutableSet)
        self.assertIsNot(result_set, mutable_set_1)
        self.assertIsNot(result_set, mutable_set_2)
        self.assertEqual(2, len(result_set))
        self.assertEqual({2, 3}, result_set)

        result_set = mutable_set_1 & [1, 2]
        self.assertEqual(2, len(result_set))
        self.assertEqual({1, 2}, result_set)

    def test_or(self) -> None:
        mutable_set_1 = MutableSet._from_iterable(typeinfo_i32, set(), range(4))
        mutable_set_2 = MutableSet._from_iterable(typeinfo_i32, set(), range(2, 6))

        result_set = mutable_set_1 | mutable_set_2

        self.assertIsInstance(result_set, MutableSet)
        self.assertIsNot(result_set, mutable_set_1)
        self.assertTrue(result_set is not mutable_set_2)
        self.assertEqual(6, len(result_set))
        self.assertEqual({0, 1, 2, 3, 4, 5}, result_set)

        result_set = mutable_set_1 | [10, 11, 12, 13]
        self.assertEqual(8, len(result_set))
        self.assertEqual({0, 1, 2, 3, 10, 11, 12, 13}, result_set)

    def test_sub(self) -> None:
        mutable_set_1 = MutableSet._from_iterable(typeinfo_i32, set(), range(4))
        mutable_set_2 = MutableSet._from_iterable(typeinfo_i32, set(), range(2, 7))

        result_set = mutable_set_1 - mutable_set_2
        self.assertIsInstance(result_set, MutableSet)
        self.assertEqual(2, len(result_set))
        self.assertEqual({0, 1}, result_set)

        result_set = mutable_set_2 - mutable_set_1
        self.assertIsInstance(result_set, MutableSet)
        self.assertEqual(3, len(result_set))
        self.assertEqual({4, 5, 6}, result_set)

        result_set = mutable_set_1 - [1, 5, 6]
        self.assertIsInstance(result_set, MutableSet)
        self.assertEqual(3, len(result_set))
        self.assertEqual({0, 2, 3}, result_set)

    def test_xor(self) -> None:
        mutable_set_1 = MutableSet._from_iterable(typeinfo_i32, set(), range(4))
        mutable_set_2 = MutableSet._from_iterable(typeinfo_i32, set(), range(2, 7))

        result_set = mutable_set_1 ^ mutable_set_2
        self.assertIsInstance(result_set, MutableSet)
        self.assertEqual(5, len(result_set))
        self.assertEqual({0, 1, 4, 5, 6}, result_set)

        result_set = mutable_set_1 ^ [1, 2, 3, 4]
        self.assertIsInstance(result_set, MutableSet)
        self.assertEqual(2, len(result_set))
        self.assertEqual({0, 4}, result_set)

    def test_add(self) -> None:
        mutable_set = MutableSet(typeinfo_i32, set())
        python_set = set()

        for i in range(10):
            mutable_set.add(i)
            python_set.add(i)

        self.assertEqual(10, len(mutable_set))
        self.assertEqual(python_set, mutable_set)

    def test_add_wrong_type(self) -> None:
        mutable_set = MutableSet(typeinfo_i32, set())
        with self.assertRaisesRegex(
            TypeError, "is not a <class 'int'>, is actually of type <class 'str'>"
        ):
            mutable_set.add("Not an interger")

    def test_add_i32_overflow(self) -> None:
        mutable_set = MutableSet(typeinfo_i32, set())
        with self.assertRaises(OverflowError):
            mutable_set.add(2**31)

    def test_discard(self) -> None:
        mutable_set = MutableSet._from_iterable(typeinfo_i32, set(), range(10))
        python_set = set(range(10))

        for i in range(1, 10, 2):
            mutable_set.discard(i)
            python_set.discard(i)

        self.assertEqual(5, len(mutable_set))
        self.assertEqual(python_set, mutable_set)

    def test_discard_wrong_type(self) -> None:
        mutable_set = MutableSet._from_iterable(typeinfo_i32, set(), range(10))
        # Discard doesn't raise an error, should it?
        mutable_set.discard("Not an integer")

    def test_discard_i32_overflow(self) -> None:
        mutable_set = MutableSet._from_iterable(typeinfo_i32, set(), range(10))
        # Discard doesn't raise an error, should it?
        mutable_set.discard(2**31)

    def test_remove(self) -> None:
        mutable_set = MutableSet._from_iterable(typeinfo_i32, set(), range(3))

        # `remove()` raises an `KeyError` if key is absent
        with self.assertRaisesRegex(KeyError, "999"):
            mutable_set.remove(999)

        mutable_set.remove(2)
        mutable_set.remove(1)
        mutable_set.remove(0)

        self.assertEqual(0, len(mutable_set))

    def test_remove_wrong_type(self) -> None:
        mutable_set = MutableSet._from_iterable(typeinfo_i32, set(), range(3))
        with self.assertRaisesRegex(
            TypeError, "is not a <class 'int'>, is actually of type <class 'str'>"
        ):
            mutable_set.remove("Not an interger")

    def test_remove_i32_overflow(self) -> None:
        mutable_set = MutableSet._from_iterable(typeinfo_i32, set(), range(3))
        with self.assertRaises(OverflowError):
            mutable_set.remove(2**31)

    def test_pop(self) -> None:
        mutable_set = MutableSet._from_iterable(typeinfo_i32, set(), range(3))

        mutable_set.pop()
        mutable_set.pop()
        mutable_set.pop()

        # Raises `KeyError` if empty
        with self.assertRaises(KeyError):
            mutable_set.pop()

    def test_clear(self) -> None:
        mutable_set = MutableSet._from_iterable(typeinfo_i32, set(), range(10))

        for i in range(10):
            mutable_set.add(i)

        mutable_set.clear()
        self.assertEqual(0, len(mutable_set))
