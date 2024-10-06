// This file contains the classes that represent the abstract syntax tree.
// You can change this as you like.

sealed trait Node

case class Program(funDecls: List[NodeFunctionDecl], main: Node) extends Node

case class NodeFunctionDecl(name: String, fun: NodeFunction) extends Node

case class NodeFunction(params: List[String], body: Node) extends Node

case class NodeCond(cond: Node, positive: Node, negative: Node) extends Node

case class NodeCall(fun: Node, params: List[Node]) extends Node

case class NodeVariable(name: String) extends Node

case class NodeValue(value: ExpValue) extends Node

case class NodePlus(lhs: Node, rhs: Node) extends Node

case class NodeMinus(lhs: Node, rhs: Node) extends Node

// TODO add nodes for all missing syntax elements   

case class NodeList(elements: List[Node]) extends Node

case class NodeMult(lhs: Node, rhs: Node) extends Node

case class NodeDivide(lhs: Node, rhs: Node) extends Node

case class NodeEqualCmp(lhs: Node, rhs: Node) extends Node

case class NodeUnequalCmp(lhs: Node, rhs: Node) extends Node

case class NodePipe(lhs: Node, rhs: Node) extends Node

case class NodeBindingOp(lhs: String, rhs: Node) extends Node

case class NodeListComp(expr: Node, bindings: List[NodeBindingOp]) extends Node

case class NodeConcatenator(expressions: List[Node], vars: List[String], fct: Node) extends Node

case class NodeLetExpression(bindings: List[NodeBindingOp], body: Node) extends Node
