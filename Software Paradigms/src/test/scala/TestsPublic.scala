import scala.collection.mutable._

class TestsPublic extends AbstractTests {
  test("Lists") {
    expectResult("[]", ExpList(List()))
    expectResult("[1, 2]", ExpList(List(ExpInteger(1), ExpInteger(2))))
    expectResult("[[print(1), print(2)], print(3)]", ExpList(List(ExpList(List(ExpInteger(1), ExpInteger(2))), ExpInteger(3))), new Queue(), "123")
  }

  test("Operators") {
    expectResult("2 * 4", ExpInteger(8))
    expectResult("4 + 4 / 2", ExpInteger(6))
    expectResult("10 - 2 * 5", ExpInteger(0))
    expectResult("10 == 2 * 5", ExpBoolean(true))
    expectResult("2 * 3 == 10 - 3", ExpBoolean(false))
    expectResult("4 / 4 != 0 + 2", ExpBoolean(true))

    expectResult("8 / 4 * 2 / 1", ExpInteger(4))
    expectResult("3 + 2 * 3 == 18 / 2", ExpBoolean(true))
    expectResult("[1, 2] + [3, 4]", ExpList(List(ExpInteger(1), ExpInteger(2), ExpInteger(3), ExpInteger(4))))
    expectResult("(4 + 4) / 2", ExpInteger(4))
  }

  test("Let Expression") {
    expectResult(
      """
      let
        a <- 10
        b <- add(a, 42)
      in
        add(a,b)
    """, ExpInteger(62))

    expectResult(
      """
      let
        a <- 4
        b <- 2
      in
        let
          a <- 10
          c <- a + b
        in
          c
    """, ExpInteger(12))

    expectResult(
      """
      let
        a <- 4
        b <- 2
        c <- let
               a <- 10
               c <- a + b
             in
               c
      in
        c
    """, ExpInteger(12))
  }

  test("Anonymous Functions") {
    expectResult(
      """
      ((i) => i + 4)(2)
    """, ExpInteger(6))

    expectResult(
      """
      ((a, b, c) => a + b - c)(3,2,1)
    """, ExpInteger(4))

    expectResult(
      """
      (() => 4 - 2)()
    """, ExpInteger(2))

    expectResult(
      """
      ((a) => build(a, build(a + 1, Nil)))(2)
    """, ExpList(List(ExpInteger(2), ExpInteger(3))))

    expectResult(
      """
      ((x) => first(x))(build(1,build(2,build(3, Nil))))
    """, ExpInteger(1))

     expectResult(
       """
       let
         a <- 1
         b <- 5
         fun <- let
                  a <- 3
                  c <- 10
                in
                  (b) => a + b + c
         foo <- let
                  a <- 4
                in
                  fun(2)
       in
         foo
     """, ExpInteger(15))

     expectResult(
       """
       let
         apply <- (x, f) => f(x)
       in
         apply(4, (x) => x * 2)
     """, ExpInteger(8))

     expectResult(
       """
       let
         mul <- (a) => (b) => a * b
         times2 <- mul(2)
         times4 <- mul(4)
       in
         times2(5) + times4(7)
     """, ExpInteger(38))
   }

   test("List Comprehensions") {
     expectResult(
       """
       [mult(x, 2) | x <- [8, 4, 2]]
     """, ExpList(List(ExpInteger(16), ExpInteger(8), ExpInteger(4))))
  }

  test("Pipe-Operator") {
    expectResult(
      """
      [2,3,4] |> first
    """, ExpInteger(2))

    expectResult(
      """
      [2,3,4,5] |> rest|> rest |> first
    """, ExpInteger(4))

    expectResult(
      """
      [2,3,4] + [5,6] |> rest
    """, ExpList(List(ExpInteger(3), ExpInteger(4), ExpInteger(5), ExpInteger(6))))

    expectResult( //let/in implementation necessary for these tests
      """
      let
        inc <- (a) => a + 1
      in
        [4,2,0] |> first |> inc
    """, ExpInteger(5))

    expectResult(
      """
      let
        fcts <- [(a) => a + 1, (a) => a * a]
      in
        1 + 4 * 2 |> (fcts |> first) |> (fcts |> rest |> first)
    """, ExpInteger(100))
  }

  test("Concatenator") {
    expectResult(
      """
      (2*4 ; True ; 2+4) |> (a, cond, b) => if cond then a else b
    """, ExpInteger(8))
  }

  test("Value Forward Passing (Bonus)") {
    expectResult(
      """
      1+4 |foo> 2*foo
    """, ExpInteger(10))

    expectResult(
      """
      let
        a <- 1
        b <- 4
      in
        (2 ; 5) |a,c> a + b + c
    """, ExpInteger(11))
  }

  test("Multi-Argument List Comprehension (Bonus)") {
    expectResult(
      """
      [add(x, y) | x <- [1, 2, 3], y <- rest([0, 4, 5, 6])]
    """, ExpList(List(ExpInteger(5), ExpInteger(6), ExpInteger(7), ExpInteger(6), ExpInteger(7), ExpInteger(8), ExpInteger(7), ExpInteger(8), ExpInteger(9))))
  }

  test("Misc") {
    expectResult(
      """
      let
        a <- 1
        b <- a
        c <- let
               a <- 2
               c <- a != b
             in
               c
      in
        c
    """, ExpBoolean(true))

    expectResult(
      """
      let
        area1 <- (a,c) => a / 2 + c / 2
        area2 <- (a,c,h) => area1(a,c)*h
      in
        area2(4,6,2)
    """, ExpInteger(10))

    expectResult(
      """
      let
        area      <- (c) => (a,b) => a * b / c
        rectangle <- area(1)
        square    <- (l) => rectangle(l,l)
        triangle  <- area(2)
        rect2     <- (l) => rectangle(2,l)
        tri2      <- (l) => triangle(2,l)
        list      <- [square(2), rect2(4), tri2(3)]
      in
        list
    """, ExpList(List(ExpInteger(4), ExpInteger(8), ExpInteger(3))))
  }
}
