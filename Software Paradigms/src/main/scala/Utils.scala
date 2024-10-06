object Utils {
  type Environment = collection.immutable.Map[String, ExpValue]
  // type Environment = MutableEnvironment

  def format(value: ExpValue): String = value match {
    // Don't change the format for existing data types!
    // We need it for testing!
    case ExpBoolean(true) => "True"
    case ExpBoolean(false) => "False"
    case ExpInteger(i) => i.toString
    case ExpList(list) => list.map(format).mkString("[", ", ", "]")
    case ExpString(str) => str
    // case ExpBinding(key, value) => key.toString() + "<-" + value.toString()
    case value => value.toString
  }
}
