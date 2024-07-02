__StarlarkEnum__ = record(name=str, value=int)
__EmptyEnum_enum__ = record(
)

EmptyEnum = __EmptyEnum_enum__(
)

__MyEnum_enum__ = record(
    ONE = __StarlarkEnum__,
    TWO = __StarlarkEnum__,
)

MyEnum = __MyEnum_enum__(
    ONE = __StarlarkEnum__(name="ONE", value=1),
    TWO = __StarlarkEnum__(name="TWO", value=2),
)

__MyBigEnum_enum__ = record(
    UNKNOWN = __StarlarkEnum__,
    ONE = __StarlarkEnum__,
    TWO = __StarlarkEnum__,
    THREE = __StarlarkEnum__,
    FOUR = __StarlarkEnum__,
    FIVE = __StarlarkEnum__,
    SIX = __StarlarkEnum__,
    SEVEN = __StarlarkEnum__,
    EIGHT = __StarlarkEnum__,
    NINE = __StarlarkEnum__,
    TEN = __StarlarkEnum__,
    ELEVEN = __StarlarkEnum__,
    TWELVE = __StarlarkEnum__,
    THIRTEEN = __StarlarkEnum__,
    FOURTEEN = __StarlarkEnum__,
    FIFTEEN = __StarlarkEnum__,
    SIXTEEN = __StarlarkEnum__,
    SEVENTEEN = __StarlarkEnum__,
    EIGHTEEN = __StarlarkEnum__,
    NINETEEN = __StarlarkEnum__,
)

MyBigEnum = __MyBigEnum_enum__(
    UNKNOWN = __StarlarkEnum__(name="UNKNOWN", value=0),
    ONE = __StarlarkEnum__(name="ONE", value=1),
    TWO = __StarlarkEnum__(name="TWO", value=2),
    THREE = __StarlarkEnum__(name="THREE", value=3),
    FOUR = __StarlarkEnum__(name="FOUR", value=4),
    FIVE = __StarlarkEnum__(name="FIVE", value=5),
    SIX = __StarlarkEnum__(name="SIX", value=6),
    SEVEN = __StarlarkEnum__(name="SEVEN", value=7),
    EIGHT = __StarlarkEnum__(name="EIGHT", value=8),
    NINE = __StarlarkEnum__(name="NINE", value=9),
    TEN = __StarlarkEnum__(name="TEN", value=10),
    ELEVEN = __StarlarkEnum__(name="ELEVEN", value=11),
    TWELVE = __StarlarkEnum__(name="TWELVE", value=12),
    THIRTEEN = __StarlarkEnum__(name="THIRTEEN", value=13),
    FOURTEEN = __StarlarkEnum__(name="FOURTEEN", value=14),
    FIFTEEN = __StarlarkEnum__(name="FIFTEEN", value=15),
    SIXTEEN = __StarlarkEnum__(name="SIXTEEN", value=16),
    SEVENTEEN = __StarlarkEnum__(name="SEVENTEEN", value=17),
    EIGHTEEN = __StarlarkEnum__(name="EIGHTEEN", value=18),
    NINETEEN = __StarlarkEnum__(name="NINETEEN", value=19),
)

kOne = MyEnum.ONE

enumNames = {
  MyEnum.ONE: "one",
  MyEnum.TWO: "two",
}

