// These classes represent the data types used by the EXP interpreter.

import Utils._

// Don't change these!
sealed trait ExpValue

case class ExpInteger(i: Int) extends ExpValue

case class ExpList(list: List[ExpValue]) extends ExpValue

case class ExpBoolean(b: Boolean) extends ExpValue

case class ExpString(str: String) extends ExpValue

// You can modify these as you wish.
case class ExpFunction(e: Environment, fun: NodeFunction) extends ExpValue

case class ExpBuiltin(fun: List[ExpValue] => ExpValue) extends ExpValue

// case class ExpBinding(key: String, value: ExpValue) extends ExpValue