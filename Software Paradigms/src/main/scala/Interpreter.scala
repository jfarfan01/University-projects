import scala.language.{existentials, postfixOps}
import Utils._


class Interpreter(reader: () => ExpValue, writer: String => _) {

  private val builtinFunctions: Map[String, List[ExpValue] => ExpValue] =
    Map(
      "eq?" -> {
        case List(a, b) => ExpBoolean(a == b)
        case _ => throw new IllegalArgumentException("Illegal argument(s) for built-in function.")
      },
      "add" -> {
        case List(ExpInteger(a), ExpInteger(b)) => ExpInteger(a + b)
        case List(ExpList(a), ExpInteger(b)) => ExpList(a.map({
                            case ExpInteger(elem) => ExpInteger(elem + b)
                            case _ => throw new IllegalArgumentException("Illegal argument(s) for built-in function.")}))
        case List(ExpInteger(a), ExpList(b)) => ExpList(b.map({
                            case ExpInteger(elem) => ExpInteger(elem + a)
                            case _ => throw new IllegalArgumentException("Illegal argument(s) for built-in function.")}))
        case _ => throw new IllegalArgumentException("Illegal argument(s) for built-in function.")
      },
      "sub" -> {
        case List(ExpInteger(a), ExpInteger(b)) => ExpInteger(a - b)
        case List(ExpList(a), ExpInteger(b)) => ExpList(a.map({
                            case ExpInteger(elem) => ExpInteger(elem - b)
                            case _ => throw new IllegalArgumentException("Illegal argument(s) for built-in function.")}))
        case List(ExpInteger(a), ExpList(b)) => ExpList(b.map({
                            case ExpInteger(elem) => ExpInteger(elem - a)
                            case _ => throw new IllegalArgumentException("Illegal argument(s) for built-in function.")}))
        case _ => throw new IllegalArgumentException("Illegal argument(s) for built-in function.")
      },
      "mult" -> {
        case List(ExpInteger(a), ExpInteger(b)) => ExpInteger(a * b)
        case List(ExpList(a), ExpInteger(b)) => ExpList(a.map({
                            case ExpInteger(elem) => ExpInteger(elem * b)
                            case _ => throw new IllegalArgumentException("Illegal argument(s) for built-in function.")}))
        case List(ExpInteger(a), ExpList(b)) => ExpList(b.map({
                            case ExpInteger(elem) => ExpInteger(elem * a)
                            case _ => throw new IllegalArgumentException("Illegal argument(s) for built-in function.")}))
        case _ => throw new IllegalArgumentException("Illegal argument(s) for built-in function.")
      },
      "lt?" -> {
        case List(ExpInteger(a), ExpInteger(b)) => ExpBoolean(a < b)
        case _ => throw new IllegalArgumentException("Illegal argument(s) for built-in function.")
      },
      "first" -> {
        case List(ExpList(Nil)) => ExpList(Nil)
        case List(ExpList(h :: _)) => h
        case List(_) => ExpList(Nil)
        case _ => throw new IllegalArgumentException("Illegal argument(s) for built-in function.")
      },
      "rest" -> {
        case List(ExpList(Nil)) => ExpList(Nil)
        case List(ExpList(_ :: t)) => ExpList(t)
        case List(_) => ExpList(Nil)
        case _ => throw new IllegalArgumentException("Illegal argument(s) for built-in function.")
      },
      "build" -> {
        case List(x, ExpList(l)) => ExpList(x :: l)
        case Nil => ExpList(Nil)
        case List(_, y) => y
        case _ => throw new IllegalArgumentException("Illegal argument(s) for built-in function.")
      },
      "atom?" -> {
        case List(x) => ExpBoolean(x match { case ExpList(_) => false case _ => true })
        case _ => throw new IllegalArgumentException("Illegal argument(s) for built-in function.")
      },
      "empty?" -> {
        case List(x) => ExpBoolean(x == ExpList(List()))
        case _ => throw new IllegalArgumentException("Illegal argument(s) for built-in function.")
      },
      "and" -> {
        case List(ExpBoolean(a), ExpBoolean(b)) => ExpBoolean(a && b)
        case _ => throw new IllegalArgumentException("Illegal argument(s) for built-in function.")
      },
      "or" -> {
        case List(ExpBoolean(a), ExpBoolean(b)) => ExpBoolean(a || b)
        case _ => throw new IllegalArgumentException("Illegal argument(s) for built-in function.")
      },
      "not" -> {
        case List(ExpBoolean(b)) => ExpBoolean(!b)
        case _ => throw new IllegalArgumentException("Illegal argument(s) for built-in function.")
      },
      "print" -> {
        case List(str) => writer(Utils.format(str)); str
        case _ => throw new IllegalArgumentException("Illegal argument(s) for built-in function.")
      },
      "read" -> {
        case Nil => reader()
        case _ => throw new IllegalArgumentException("Illegal argument(s) for built-in function.")
      }
    )

  def interpret(program: Program): ExpValue = interpret(newEnvironment(), constructFunctionEnvironment(program.funDecls), program.main)

  def interpret(e: Environment, fe: Environment, n: Node): ExpValue = n match {
    case NodeValue(value) => value
    case NodeVariable(name) => getValue(e, fe, name)
    case NodeList(values) => ExpList(values.map(interpret(e, fe, _)))
    case NodeCond(cond, positive, negative) => interpret(e, fe, if (interpret(e, fe, cond) match {
      case ExpBoolean(b) => b
      case _ => throw new IllegalArgumentException("Illegal type for condition.")
    }) positive else negative)
    case NodeCall(name, args) => interpret(e, fe, name) match {
      case ExpBuiltin(fun) => fun(args map (arg => interpret(e, fe, arg)))
      case ExpFunction(locale: Environment, NodeFunction(params, body)) =>
        interpret(locale ++ (params zip (args map (arg => interpret(e, fe, arg)))).toMap, fe, body)
      case v => throw new IllegalArgumentException(s"Expected a function, but got $v")
    }
    case NodePlus(lhs, rhs) => (interpret(e, fe, lhs), interpret(e, fe, rhs)) match {
      case (ExpInteger(x), ExpInteger(y)) => ExpInteger(x + y)
      case (ExpList(x), ExpList(y)) => ExpList(x ++ y)
      case (_, _) => throw new IllegalArgumentException("Illegal types for + op.")
    }
    case NodeMinus(lhs, rhs) => (interpret(e, fe, lhs), interpret(e, fe, rhs)) match {
      case (ExpInteger(x), ExpInteger(y)) => ExpInteger(x - y)
      case (_, _) => throw new IllegalArgumentException("Illegal types for - op.")
    }
    case NodeDivide(lhs, rhs) => (interpret(e, fe, lhs), interpret(e, fe, rhs)) match {
      case (ExpInteger(x), ExpInteger(y)) => ExpInteger(x / y)
      case (_, _) => throw new IllegalArgumentException("Illegal types for / op.")
    }
    case NodeMult(lhs, rhs) => (interpret(e, fe, lhs), interpret(e, fe, rhs)) match {
      case (ExpInteger(x), ExpInteger(y)) => ExpInteger(x * y)
      case (_, _) => throw new IllegalArgumentException("Illegal types for * op.")
    }
    case NodeEqualCmp(lhs, rhs) => (interpret(e, fe, lhs), interpret(e, fe, rhs)) match {
      case (ExpInteger(x), ExpInteger(y)) => ExpBoolean(x == y)
      case (_, _) => throw new IllegalArgumentException("Illegal types for == op.")
    }
    case NodeUnequalCmp(lhs, rhs) => (interpret(e, fe, lhs), interpret(e, fe, rhs)) match {
      case (ExpInteger(x), ExpInteger(y)) => ExpBoolean(x != y)
      case (_, _) => throw new IllegalArgumentException("Illegal types for != op.")
    }
    
    case NodeFunction(params, body) => ExpFunction(e, NodeFunction(params, body))

    case NodeLetExpression(bindings, body) => interpret(updateEnvironment(e, fe, bindings), fe, body)

    case NodePipe(lhs, rhs) => (interpret(e, fe, lhs), interpret(e, fe, rhs)) match {
    case (_) => interpret(e, fe, NodeCall(rhs, List(lhs)))
    case _ => throw new IllegalArgumentException("Illegal types for |> op.") //errros can be handled by NodeCall
    }

    case NodeConcatenator(expr, pipeCont, fun) => interpret(e, fe, NodeCall(fun, expr))

    case NodeListComp(expr, bindings) => 
      val lists = bindings.map{
        case NodeBindingOp(lhs, rhs) => interpret(e, fe, rhs) match {
          case ExpList(l) => l
        }
      }
      val combinations = lists.foldLeft(List(List[ExpValue]()))((acc, l) => acc.flatMap{ combination => l.map{value => value :: combination}})

     val results = combinations.map { combination =>
     val new_bindings = bindings.zip(combination).map { case (NodeBindingOp(lhs, _), value) =>
        NodeBindingOp(lhs, NodeValue(value))
    }
     val new_env = updateEnvironment(e, fe, new_bindings)
     interpret(new_env, fe, expr)
    }
      ExpList(results)


    case n => throw new NotImplementedError(s"Interpretation of node $n not implemented.")
  }

  private def getValue(e: Environment, fe: Environment, name: String): ExpValue = fe get name match {
    case Some(expr) => expr
    case None => e get name match {
      case Some(expr) => expr
      case None => throw new IllegalArgumentException(s"ID $name not found.")
    }
  }

  private def newEnvironment(): Environment = Map()

  private def constructFunctionEnvironment(funDecls: List[NodeFunctionDecl]): Environment = interpretFunctionDecls(interpretBuiltinFcts(), funDecls)

  private def interpretBuiltinFcts(): Environment = builtinFunctions map {
    case (name, fun) => name -> ExpBuiltin(fun)
    case _ => throw new IllegalArgumentException("Illegal function declaration node.")
  }

  private def interpretFunctionDecls(fe: Environment, fcts: List[NodeFunctionDecl]): Environment =
    fcts.foldLeft(fe)((e, fct) => fct match {
      case NodeFunctionDecl(name, fun) => e + (name -> interpret(newEnvironment(), e, fun))
    })

  def updateEnvironment(env: Environment, fe: Environment, bindings: List[NodeBindingOp]): Environment = {
    val new_env = bindings map(binding => (binding.lhs -> binding.rhs))
      new_env.foldLeft(env) { case (acc, (key, value)) =>
        acc + (key -> interpret(acc, fe, value))
      }
  }
}
