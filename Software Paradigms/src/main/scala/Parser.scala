import scala.util.parsing.combinator._

object ExpParser extends RegexParsers {

  def parse(code: String): ParseResult[Program] = parseAll(program, code)

  def program: Parser[Program] = rep(globFun <~ ";") ~ expr ^^ {
    case ls ~ expr => Program(ls, expr)
  }

  private def globFun: Parser[NodeFunctionDecl] = ("$" ~> id) ~ ("(" ~> repsep(id, ",") <~ ")") ~ ("=>" ~> expr) ^^ {
    case name ~ params ~ body => NodeFunctionDecl(name, NodeFunction(params, body))
  }

  // exprLet | 
  private def expr: Parser[Node] = exprLet | exprPipe
 
  private def exprPipe: Parser[Node] = chainl1(exprCMP, //pipes are the last operations to be performed
      "|"~repsep(variable, ",") ~">" ^^^ ((lhs: Node, rhs: Node) => NodePipe(lhs, rhs))
    )

  private def exprCMP: Parser[Node] = chainl1(exprPM, 
      "==" ^^^ ((lhs: Node, rhs: Node) => NodeEqualCmp(lhs, rhs))
    | "!=" ^^^ ((lhs: Node, rhs: Node) => NodeUnequalCmp(lhs, rhs))
    )

  private def exprPM: Parser[Node] = chainl1(exprMD, 
      "+" ^^^ ((lhs: Node, rhs: Node) => NodePlus(lhs, rhs))
    | "-" ^^^ ((lhs: Node, rhs: Node) => NodeMinus(lhs, rhs))
    )

  private def exprMD: Parser[Node] = chainl1(expr0, //multiplication and division must be done first
      "*" ^^^ ((lhs: Node, rhs: Node) => NodeMult(lhs, rhs))
    | "/" ^^^ ((lhs: Node, rhs: Node) => NodeDivide(lhs, rhs))
    )
  
  private def expr0: Parser[Node] = ((constant | list | cond | variable |fun | paren| listComp| concatenator) ~ rep(call)) ^^ { //order may matter here
    case e ~ Nil => e
    case e ~ l => l.foldLeft(e)((e, args) => NodeCall(e, args))
  }

  private def int: Parser[Node] = """(0)|([1-9][0-9]*)|(-[1-9][0-9]*)""".r ^^ (value => NodeValue(ExpInteger(value.toInt)))

  private def constant: Parser[Node] = (int | "True" | "False" | "Nil") ^^ {
    case n: Node => n
    case "True" => NodeValue(ExpBoolean(true))
    case "False" => NodeValue(ExpBoolean(false))
    case "Nil" => NodeValue(ExpList(Nil))
  }

  private def cond: Parser[Node] = ("if" ~> expr) ~ ("then" ~> expr) ~ ("else" ~> expr) ^^ {
    case cond ~ positive ~ negative => NodeCond(cond, positive, negative)
  }

  private def variable: Parser[Node] = id ^^ (id => NodeVariable(id))

  private def id: Parser[String] = """[a-z][A-Za-z0-9?_]*""".r

  private def paren: Parser[Node] = "(" ~> expr <~ ")"

  private def call: Parser[List[Node]] = "(" ~> repsep(expr, ",") <~ ")"

  // TODO add all missing grammar rules

  private def fun: Parser[NodeFunction] = ("("~> repsep(id, ",") <~")")  ~ ("=>" ~> expr)^^{
    case params ~ expr => NodeFunction(params, expr)
  }

  private def list: Parser[NodeList] = "["~> repsep(expr, ",")  <~"]" ^^ {
    case n => NodeList(n)
  }
  
  private def binding: Parser[NodeBindingOp] = id ~ "<-" ~ expr ^^{
    case lhs ~ "<-" ~ rhs => NodeBindingOp(lhs, rhs)
  }

  private def exprLet: Parser[Node] = "let" ~> rep(binding) ~ ("in" ~> expr) ^^ {
    case bindings ~ body => NodeLetExpression(bindings, body)
  }

  def listComp: Parser[NodeListComp] = "[" ~ expr ~ "|" ~ repsep(binding, ",") ~ "]" ^^ {
    case _ ~ expr ~ _ ~ bindings ~ _ => NodeListComp(expr, bindings)
  }

  private def concatenator: Parser[NodeConcatenator] = "(" ~ repsep(expr, ";") ~ ")" ~ "|" ~ repsep(id, ",") ~ ">" ~ expr ^^ {
    case p1 ~ expressions ~ p2 ~ pi1 ~ ids ~ pi2 ~ fct => NodeConcatenator(expressions, ids, fct)
  }

}
