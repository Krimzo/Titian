package math

import java.io.Serializable

class Triangle : Serializable {
    var a: Vertex = Vertex()
    var b: Vertex = Vertex()
    var c: Vertex = Vertex()

    constructor()

    constructor(A: Vertex, B: Vertex, C: Vertex) {
        a = A
        b = B
        c = C
    }

    constructor(triangle: Triangle) {
        a = Vertex(triangle.a)
        b = Vertex(triangle.b)
        c = Vertex(triangle.c)
    }
}